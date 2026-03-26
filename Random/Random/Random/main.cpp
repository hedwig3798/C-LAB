#include "RandomTester.h"
#include <random>
#include <iomanip>

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

int main()
{
	// 1% ~ 0.0001% 까지 체크
	unsigned long long maxAccuary = 10'000'000;
	unsigned long long minAccuary = 100;
	std::cout << std::setprecision(6);

	for (unsigned long long i = maxAccuary; i >= minAccuary; i /= 10)
	{
		TestInput randInput;
		randInput.m_type = "rand";
		randInput.m_accuracy = i;
		randInput.m_randomFunction = []() { return static_cast<double>(rand()) / RAND_MAX; };
		
		TestResult tr = RandomTest(randInput);
		if (true == tr.m_isReliable)
		{
			std::cout 
				<< "rand accuracy : " 
				<< (1.0 / static_cast<double>(i)) * 100.0
				<< "% (99% trust)\n";

			break;
		}
	}

	for (unsigned long long i = maxAccuary; i >= minAccuary; i /= 10)
	{
		TestInput randInput;
		randInput.m_type = "custom";
		randInput.m_accuracy = i;
		randInput.m_randomFunction = CustomXorshift;

		TestResult tr = RandomTest(randInput);
		if (true == tr.m_isReliable)
		{
			std::cout
				<< "custom accuracy : "
				<< (1.0 / static_cast<double>(i)) * 100.0
				<< "% (99% trust)\n";

			break;
		}
	}

	for (unsigned long long i = maxAccuary; i >= minAccuary; i /= 10)
	{
		TestInput randInput;
		randInput.m_type = "mt";
		randInput.m_accuracy = i;
		randInput.m_randomFunction = mtRandom;

		TestResult tr = RandomTest(randInput);
		if (true == tr.m_isReliable)
		{
			std::cout
				<< "mt accuracy : "
				<< (1.0 / static_cast<double>(i)) * 100.0
				<< "% (99% trust)\n";

			break;
		}
	}

	return 0;
}