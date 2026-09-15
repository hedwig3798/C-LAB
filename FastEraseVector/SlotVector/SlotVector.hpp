#pragma once
#include <vector>
#include <stdexcept>
#include <utility>

/// <summary>
/// 지연 삭제 방식의 slot vecotr
/// </summary>
/// <typeparam name="T">타입</typeparam>
template <typename T>
class SlotVector
{
private:
	/// <summary>
	/// Slot element
	/// </summary>
	struct Slot
	{
		T m_data;
		bool m_isVaild;
		Slot() : m_isVaild(false) {}
		Slot(const T& _val)
			: m_data(_val)
			, m_isVaild(true)
		{
		}
		Slot(T&& _val)
			: m_data(std::move(_val))
			, m_isVaild(true)
		{
		}
	};

	std::vector<Slot> m_vector;
	size_t m_activeCount;

public:
	SlotVector()
		: m_activeCount(0)
	{
	};
	~SlotVector() = default;

	/// std::vector에서 내용이 바뀌지 않은 기본 함수들
	void reserve(size_t _val) { m_vector.reserve(_val); }
	size_t size() const { return m_vector.size(); }
	bool empty() const { return m_vector.empty(); }

	T& operator[](size_t _index) { return m_vector[_index]; }
	const T& operator[](size_t _index) const { return m_vector[_index]; }

	void erase(typename std::vector<T>::const_iterator _pos) { m_vector.erase(_pos); }
	void erase(typename std::vector<T>::const_iterator _start, typename std::vector<T>::const_iterator _end) { m_vector.erase(_start, _end); }

	/// <summary>
	/// Slot vector 전용 이터레이터
	/// </summary>
	class iterator
	{
		friend class SlotVector;
	private:
		typename std::vector<Slot>::iterator m_itr;
		typename std::vector<Slot>::iterator m_end;

		/// <summary>
		/// 유효한 이터레이터가 나올 때 까지 다음 요소로 이동
		/// </summary>
		void AdvanceVaild()
		{
			while (m_itr != m_end && !m_itr->m_isVaild)
			{
				++m_itr;
			}
		}

	public:
		iterator(typename std::vector<Slot>::iterator _itr,
			typename std::vector<Slot>::iterator _end)
			: m_itr(_itr)
			, m_end(_end)
		{
			AdvanceVaild();
		}

		iterator& operator++()
		{
			++m_itr;
			AdvanceVaild();
			return *this;
		}

		bool operator!=(const iterator& _other) const { return m_itr != _other.m_itr; }
		bool operator==(const iterator& _other) const { return m_itr == _other.m_itr; }

		T& operator*() { return m_itr->m_data; }
	};

	/// std::vector에서 내용이 바뀐 기본 함수들
	void push_back(const T& _val)
	{
		m_vector.push_back(Slot(_val));
		++m_activeCount;
	}
	void push_back(T&& _val)
	{
		m_vector.push_back(Slot(std::move(_val)));
		++m_activeCount;
	}
	void pop_back()
	{
		if (false == m_vector.empty())
		{
			if (m_vector.back().is_valid)
			{
				--m_activeCount;
			}
			m_vector.pop_back();
		}
	}
	void clear()
	{
		m_vector.clear();
		m_activeCount = 0;
	}
	iterator begin() { return iterator(m_vector.begin(), m_vector.end()); }
	iterator end() { return iterator(m_vector.end(), m_vector.end()); }

	/// 추가 함수들

	/// <summary>
	/// 지연 삭제
	/// </summary>
	/// <param name="_ind">인덱스</param>
	void lazy_erase(size_t _ind)
	{
		if (_ind >= m_vector.size())
		{
			throw std::out_of_range("out of range");
		}

		if (true == m_vector[_ind].m_isVaild)
		{
			m_vector[_ind].m_isVaild = false;
			--m_activeCount;
		}
	}

	/// <summary>
	/// 지연 삭제 (반복자 버전)
	/// </summary>
	/// <param name="_itr">삭제할 요소를 가리키는 반복자</param>
	/// <returns>삭제된 요소 바로 다음의 유효한 요소를 가리키는 반복자</returns>
	iterator lazy_erase(iterator _itr)
	{
		if (_itr == end())
		{
			return _itr;
		}

		if (_itr.m_itr->m_isVaild)
		{
			_itr.m_itr->m_isVaild = false;
			--m_activeCount;
		}

		iterator next_itr = _itr;
		++next_itr;

		return next_itr;
	}

	/// <summary>
	/// 삭제 연산
	/// </summary>
	void compact()
	{
		if (m_activeCount == m_vector.size())
		{
			return;
		}

		// 벡터 앞쪽부터 유효한 데이터만 가져와서 채우기
		size_t writeInd = 0;
		for (size_t readInd = 0; readInd < m_vector.size(); ++readInd)
		{
			if (true == m_vector[readInd].m_isVaild)
			{
				if (writeInd != readInd)
				{
					m_vector[writeInd] = std::move(m_vector[readInd]);
				}
				++writeInd;
			}
		}

		// 벡터의 사이즈를 맞추기
		m_vector.resize(writeInd);
	}
};