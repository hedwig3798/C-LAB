#pragma once
#include <functional>
#include <vector>
#include <iostream>
#include <string>

struct TestResult
{
	std::string m_type;
	unsigned long long m_accuracy;
	bool m_isReliable;
	bool m_isError;

	static bool Compare(const TestResult& _t1, const TestResult& _t2)
	{
		return _t1.m_accuracy > _t2.m_accuracy;
	}
};

struct TestInput
{
	std::function<double()> m_randomFunction;
	std::string m_type;
	unsigned long long m_accuracy;
};

TestResult RandomTest(TestInput _input);

double GetCriticalValue(unsigned long long _diff);


