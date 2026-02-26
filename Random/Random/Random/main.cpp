#include "RandomTester.h"
#include <random>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

std::queue<TestInput> g_jobQueue;
std::vector<TestResult> g_results;
std::mutex g_queueMutex;
std::mutex g_vectorMutex;
std::condition_variable g_jobCV;
bool g_testFinish = false;

double CustomXorshift()
{
	static thread_local uint32_t xorState = 123456789;
	xorState ^= xorState << 13;
	xorState ^= xorState >> 17;
	xorState ^= xorState << 5;
	return static_cast<double>(xorState) / 4294967296.0;
}

double mtRandom()
{
	static thread_local std::random_device rd;
	static thread_local std::mt19937 gen(rd());
	static thread_local std::uniform_real_distribution<double> dis(0.0, 1.0);

	return dis(gen);
}

void TestThread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(g_queueMutex);
		g_jobCV.wait(lock, []()
			{
				return true == g_testFinish || false == g_jobQueue.empty();
			}
		);

		if (true == g_testFinish
			&& true == g_jobQueue.empty())
		{
			break;
		}

		TestInput input = g_jobQueue.front();
		g_jobQueue.pop();
		lock.unlock();

		TestResult res = RandomTest(input);

		g_vectorMutex.lock();
		g_results.push_back(res);
		g_vectorMutex.unlock();
	}
}

int main()
{
	std::vector<std::thread> threads;
	for (int i = 0; i < 12; ++i)
	{
		threads.emplace_back([&]() {TestThread(); });
	}

	// 1% ~ 0.0001% 까지 체크
	int maxAccuary = 10'000'000;
	int minAccuary = 100;

	for (int i = minAccuary; i <= maxAccuary; i *= 10)
	{
		TestInput randInput;
		randInput.m_type = "rand";
		randInput.m_accuracy = i;
		randInput.m_randomFunction = []() { return static_cast<double>(rand()) / RAND_MAX; };
		g_jobQueue.push(randInput);

		TestInput customInput;
		customInput.m_type = "custom";
		customInput.m_accuracy = i;
		customInput.m_randomFunction = CustomXorshift;
		g_jobQueue.push(customInput);

		TestInput mtInput;
		mtInput.m_type = "mt";
		mtInput.m_accuracy = i;
		mtInput.m_randomFunction = mtRandom;
		g_jobQueue.push(mtInput);

		std::unique_lock<std::mutex> lock(g_queueMutex);
		g_jobCV.notify_one();
		lock.unlock();
	}
	g_testFinish = true;
	g_jobCV.notify_all();

	for(auto& itr : threads)
	{
		itr.join();
	}

	std::vector<TestResult> randResult;
	std::vector<TestResult> customResult;
	std::vector<TestResult> mtResult;

	for(auto& itr : g_results)
	{
		if ("rand" == itr.m_type)
		{
			randResult.push_back(itr);
		}
		else if ("custom" == itr.m_type)
		{
			customResult.push_back(itr);
		}
		else if ("mt" == itr.m_type)
		{
			mtResult.push_back(itr);
		}
	}

	std::sort(randResult.begin(), randResult.end(), TestResult::Compare);
	std::sort(customResult.begin(), customResult.end(), TestResult::Compare);
	std::sort(mtResult.begin(), mtResult.end(), TestResult::Compare);

	bool checkFlag = false;
	std::cout << "rand accuracy : ";
	for(const auto& itr : randResult)
	{
		if (false == itr.m_isReliable)
		{
			std::cout << (1 / static_cast<double>(itr.m_accuracy)) * 1000.0;
			checkFlag = true;
			break;
		}
	}
	if (false == checkFlag)
	{
		std::cout << "over " << (1 / static_cast<double>(maxAccuary)) * 100.0 << "%\n";
	}
	else
	{
		std::cout << "%\n";
	}

	checkFlag = false;
	std::cout << "custom accuracy : ";
	for (const auto& itr : customResult)
	{
		if (false == itr.m_isReliable)
		{
			std::cout << (1.0 / static_cast<double>(itr.m_accuracy)) * 1000.0;
			checkFlag = true;
			break;
		}
	}
	if (false == checkFlag)
	{
		std::cout << "over " << (1.0 / static_cast<double>(maxAccuary)) * 100.0 << "%\n";
	}
	else
	{
		std::cout << "%\n";
	}

	checkFlag = false;
	std::cout << "mt accuracy : ";
	for (const auto& itr : mtResult)
	{
		if (false == itr.m_isReliable)
		{
			std::cout << (1.0 / static_cast<double>(itr.m_accuracy)) * 1000.0;
			checkFlag = true;
			break;
		}
	}
	if (false == checkFlag)
	{
		std::cout << "over " << (1.0 / static_cast<double>(maxAccuary)) * 100.0 << "%\n";
	}
	else
	{
		std::cout << "%\n";
	}

	return 0;
}