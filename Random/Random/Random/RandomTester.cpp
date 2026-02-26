#include "RandomTester.h"
TestResult RandomTest(TestInput _input)
{
	TestResult result{};
	result.m_isError = false;
	result.m_accuracy = _input.m_accuracy;
	result.m_type = _input.m_type;

	if (_input.m_accuracy <= 1)
	{
		// std::cout << "Accuracy must be greater than 1.\n";
		result.m_isError = true;;
		return result;
	}

	unsigned long long sampleCount = _input.m_accuracy * 1000;
	if (sampleCount / 1000 != _input.m_accuracy)
	{
		// std::cout << "Overflow detected in sampleCount calculation.\n";
		result.m_isError = true;
		return result;
	}

	std::vector<unsigned long long> count(_input.m_accuracy + 1, 0);
	// std::cout << "Collecting " << sampleCount << " samples : ";

	for (unsigned long long i = 0; i < sampleCount; ++i)
	{
		double randomValue = _input.m_randomFunction();
		if (randomValue < 0.0 || randomValue > 1.0) 
		{
			// std::cout << "\nError: Random value " << randomValue << " out of range [0, 1].";
			result.m_isError = true;
			return result;
		}
		unsigned long long bucketIndex = static_cast<unsigned long long>(randomValue * static_cast<double>(_input.m_accuracy));
		count[bucketIndex]++;
	}

	double expected = static_cast<double>(sampleCount) / _input.m_accuracy;
	double chiSquare = 0.0;
	for (unsigned long long i = 0; i < _input.m_accuracy; ++i)
	{
		double diff = static_cast<double>(count[i]) - expected;
		chiSquare += (diff * diff) / expected;
	}

	double threshold = GetCriticalValue(_input.m_accuracy);
	result.m_isReliable = (chiSquare < threshold);
	//  std::cout << "Chi-Square: " << chiSquare << " (Threshold: " << threshold << ")\n";
	return result;
}

double GetCriticalValue(unsigned long long _diff)
{
	double z = 2.326;
	return _diff * std::pow(1.0 - (2.0 / (9.0 * _diff)) + z * std::sqrt(2.0 / (9.0 * _diff)), 3);
}
