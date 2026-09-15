#pragma once
#include <vector>
#include <stdexcept>
#include <utility>

template <typename T>
class SwapPopVector
{
private:
	std::vector<T> m_vector;

public:
	SwapPopVector() { m_vector = std::vector<T>(); };
	~SwapPopVector() = default;

	/// 내용이 바뀌지 않은 함수들
	void push_back(const T& _val) { m_vector.push_back(_val); }
	void push_back(T&& _val) { m_vector.push_back(_val); }
	void pop_back() { m_vector.pop_back(); }
	void clear() { m_vector.clear(); }
	void reserve(size_t _val) { m_vector.reserve(_val); }
	size_t size() const { return m_vector.size(); }
	bool empty() const { return m_vector.empty(); }

	T& operator[](size_t _index) { return m_vector[_index]; }
	const T& operator[](size_t _index) const { return m_vector[_index]; }

	auto begin() { return m_vector.begin(); }
	auto cbegin() const { return m_vector.cbegin(); }
	auto end() { return m_vector.end(); }
	auto cend() const { return m_vector.cend(); }

	void erase(typename std::vector<T>::const_iterator _pos) { m_vector.erase(_pos); }
	void erase(typename std::vector<T>::const_iterator _start, typename std::vector<T>::const_iterator _end) { m_vector.erase(_start, _end); }

	/// <summary>
	/// swap-pop delete
	/// </summary>
	/// <param name="_ind">index</param>
	void swap_pop(size_t _ind) 
	{
		// 범위 바깥이면 예외
		if (_ind >= m_vector.size())
		{
			throw std::out_of_range("out of range");
		}

		// 맨 뒤 요소와 바꾼다음 지우기
		if (_ind != m_vector.size() - 1)
		{
			std::swap(m_vector[_ind], m_vector.back());
		}
		m_vector.pop_back();
	}

	/// <summary>
	/// swap-pop delete
	/// </summary>
	/// <param name="_itr">itr</param>
	auto swap_pop(typename std::vector<T>::iterator _itr)
	{
		// 끝은 무시
		if (_itr == m_vector.end())
		{
			return _itr;
		}

		// 맨 뒤 요소와 바꾼 후 삭제
		auto last = std::prev(m_vector.end());
		if (_itr != last)
		{
			std::swap(*_itr, *last);
		}
		m_vector.pop_back();

		return _itr;
	}
};