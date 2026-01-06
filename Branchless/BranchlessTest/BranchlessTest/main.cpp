#include "TimeChecker.h"
#include <vector>
#include <algorithm>
#include <random>

#define MAX_COUNT 100'000'000
std::vector<int> datas;

void IfLoopSorted()
{
	std::cout << "Loop Sorted Vector\n";
	long long sum = 0;
	std::sort(datas.begin(), datas.end());

	TimeChecker tc;
	tc.Start();
	for (int i = 0; i < MAX_COUNT; i++)
	{
		if (datas[i] > 128)
		{
			sum += datas[i];
		}
	}
	tc.Get();
	std::cout << sum << '\n';

	return;
}

void IfLoop()
{
	long long sum = 0;
	std::cout << "Loop with Branch\n";
	TimeChecker tc;
	tc.Start();
	for (int i = 0; i < MAX_COUNT; i++)
	{
		if (datas[i] > 128)
		{
			sum += datas[i];
		}
	}
	tc.Get();
	std::cout << sum << '\n';


	return;
}

void BranchlessLoop()
{
	long long sum = 0;
	std::cout << "Loop with Branchless\n";
	TimeChecker tc;
	tc.Start();
	for (int i = 0; i < MAX_COUNT; i++)
	{
		int mask = -(datas[i] > 128);
		sum += (mask & datas[i]);
	}
	tc.Get();

	std::cout << sum << '\n';


	return;
}

int main()
{
	// random data setting
	datas.resize(MAX_COUNT);
	std::mt19937 gen(42);
	std::uniform_int_distribution<> dis(0, 255);
	for (int i = 0; i < datas.size() ; i++)
	{
		datas[i] = dis(gen);
	}

	// Branch Code
	IfLoop();
	// Branchless Code
	BranchlessLoop();
	// Branch Code but Sorted vector
	IfLoopSorted();

	return 0;
}