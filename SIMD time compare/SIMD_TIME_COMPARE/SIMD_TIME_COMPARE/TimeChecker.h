#pragma once
#include <windows.h>
#include <iostream>

class TimeChecker
{
private:
	LARGE_INTEGER nowTime;
	LARGE_INTEGER startTime;
	LARGE_INTEGER frequency;

	bool isDebug;

public:
	TimeChecker(bool _isDegub = true);
	~TimeChecker();

	void Start();
	long long Get();
};

