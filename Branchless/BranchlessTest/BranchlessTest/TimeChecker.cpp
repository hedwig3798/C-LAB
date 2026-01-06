#include "TimeChecker.h"

TimeChecker::TimeChecker(bool _isDegub)
	:isDebug (_isDegub)
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTime);
	nowTime = startTime;
}

TimeChecker::~TimeChecker()
{
}

void TimeChecker::Start()
{
	QueryPerformanceCounter(&nowTime);
	startTime = nowTime;
}

long long TimeChecker::Get()
{
	QueryPerformanceCounter(&nowTime);
	long long time = nowTime.QuadPart - startTime.QuadPart;
	
	if (isDebug)
	{
		std::cout << "Time : " << float(time) / float(frequency.QuadPart) << "s" << std::endl;
	}

	return time;
}

