// FastEraseVector.cpp : 애플리케이션의 진입점을 정의합니다.
//

#include "FastEraseVector.h"
#include "../SwapPopVector/SwapPopVector.hpp"
#include "../SlotVector/SlotVector.hpp"

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <numeric>
#include <algorithm>


int main()
{
	// 테스트 조건
	const size_t NUM_ELEMENTS = 100000;
	const size_t ERASE_COUNT = 50000;

	// 각 벡터
	std::vector<size_t> vec;
	SwapPopVector<size_t> swapVec;
	SlotVector<size_t> slotVect;

	// 할당
	vec.reserve(NUM_ELEMENTS);
	swapVec.reserve(NUM_ELEMENTS);
	slotVect.reserve(NUM_ELEMENTS);
	for (size_t i = 0; i < NUM_ELEMENTS; ++i) 
	{
		vec.push_back(i);
		swapVec.push_back(i);
		slotVect.push_back(i);
	}

	// 랜덤 난수 생성
	std::vector<size_t> eraseIndex;
	eraseIndex.reserve(ERASE_COUNT);
	std::random_device rd;
	std::mt19937 gen(rd());

	for (size_t i = 0; i < ERASE_COUNT; ++i) 
	{
		std::uniform_int_distribution<size_t> dist(0, NUM_ELEMENTS - 1 - i);
		eraseIndex.push_back(dist(gen));
	}

	std::vector<size_t> slotEraseIndex(NUM_ELEMENTS);
	std::iota(slotEraseIndex.begin(), slotEraseIndex.end(), 0);
	std::shuffle(slotEraseIndex.begin(), slotEraseIndex.end(), gen);
	slotEraseIndex.resize(ERASE_COUNT);

	std::cout << "데이터 총 갯수: " << NUM_ELEMENTS << "\n";
	std::cout << "삭제할 갯수: " << ERASE_COUNT << "\n\n";

	// 테스트
	auto start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < ERASE_COUNT; ++i)
	{
		vec.erase(vec.begin() + eraseIndex[i]);
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> stdEraseTime = end - start;

	start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < ERASE_COUNT; ++i)
	{
		swapVec.swap_pop(eraseIndex[i]);
	}
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> swapEraseTime = end - start;

	start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < ERASE_COUNT; ++i)
	{
		slotVect.lazy_erase(slotEraseIndex[i]);
	}
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> slotEraseTime = end - start;

	std::cout << "--- 삭제 연산 성능 ---\n";
	std::cout << "기본 벡터 삭제 시간  : " << stdEraseTime.count() << " ms\n";
	std::cout << "스왑-팝 벡터 삭제 시간: " << swapEraseTime.count() << " ms\n";
	std::cout << "슬롯 벡터 삭제 시간   : " << slotEraseTime.count() << " ms\n\n";

	// 삭제 후 순회
	volatile long long dummy_sum = 0;

	start = std::chrono::high_resolution_clock::now();
	for (size_t val : vec) { dummy_sum += val; }
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> std_iter_time = end - start;

	start = std::chrono::high_resolution_clock::now();
	for (size_t val : swapVec) { dummy_sum += val; }
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> swap_iter_time = end - start;

	start = std::chrono::high_resolution_clock::now();
	for (size_t val : slotVect) { dummy_sum += val; }
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> slot_iter_time = end - start;

	std::cout << "--- 순회 연산 성능 ---\n";
	std::cout << "기본 벡터 순회 시간  : " << std_iter_time.count() << " ms\n";
	std::cout << "스왑-팝 벡터 순회 시간: " << swap_iter_time.count() << " ms\n";
	std::cout << "슬롯 벡터 순회 시간   : " << slot_iter_time.count() << " ms\n";

	return 0;
}
