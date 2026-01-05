#include <iostream>
#include "TimeChecker.h"

#define ELEMENT_COUNT 20'000

// 4억개 배열
int testArray[ELEMENT_COUNT][ELEMENT_COUNT] = { 0, };

void ColItr()
{
	volatile int sum = 0;
	for (int i = 0; i < ELEMENT_COUNT; i++)
	{
		for (int j = 0; j < ELEMENT_COUNT; j++)
		{
			sum += testArray[i][j];
		}
	}
}

void RowItr()
{
	volatile int sum = 0;
	for (int i = 0; i < ELEMENT_COUNT; i++)
	{
		for (int j = 0; j < ELEMENT_COUNT; j++)
		{
			sum += testArray[j][i];
		}
	}
}

int main()
{
	TimeChecker tc;

	tc.Start();
	ColItr();
	tc.Get();

	tc.Start();
	RowItr();
	tc.Get();
}