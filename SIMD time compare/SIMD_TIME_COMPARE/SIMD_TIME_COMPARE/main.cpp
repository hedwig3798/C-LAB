#include "TimeChecker.h"

#include <xmmintrin.h>
#include <intrin.h>
#include <iostream>

#define COUNT 100'000'000

/// 내적 연산 with 배열
float* DotArray(int _count, const float* _a, const float* _b)
{
	float* result = new float[_count * 4];
	for (int i = 0; i < _count; i++)
	{
		int j = i * 4;

		result[i] = _a[j + 0] * _b[j + 0]
			+ _a[j + 1] * _b[j + 1]
			+ _a[j + 2] * _b[j + 2]
			+ _a[j + 3] * _b[j + 3];
	}

	return result;
}

/// 내적 연산 with SIMD but slow
float* DotSSEHorizontal(int _count, const float* _a, const float* _b)
{
	float* result = new float[_count * 4];

	for (int i = 0; i < _count; i++)
	{
		int j = i * 4;

		__m128 veca = _mm_load_ps(&_a[j]);
		__m128 vecb = _mm_load_ps(&_b[j]);

		__m128 vec0 = _mm_mul_ps(veca, vecb);

		__m128 vec1 = _mm_hadd_ps(vec0, vec0);

		__m128 vecResult = _mm_hadd_ps(vec1, vec1);

		_mm_store_ss(&result[i], vecResult);
	}

	return result;
}

/// 내적 연산 with SIMD
float* DotSSE(int _count, const float* _a, const float* _b)
{
	float* result = new float[_count * 4];

	for (int i = 0; i < _count; i += 4)
	{
		__m128 vaX = _mm_load_ps(&_a[(i + 0) * 4]);
		__m128 vaY = _mm_load_ps(&_a[(i + 1) * 4]);
		__m128 vaZ = _mm_load_ps(&_a[(i + 2) * 4]);
		__m128 vaW = _mm_load_ps(&_a[(i + 3) * 4]);

		__m128 vbX = _mm_load_ps(&_b[(i + 0) * 4]);
		__m128 vbY = _mm_load_ps(&_b[(i + 1) * 4]);
		__m128 vbZ = _mm_load_ps(&_b[(i + 2) * 4]);
		__m128 vbW = _mm_load_ps(&_b[(i + 3) * 4]);

		_MM_TRANSPOSE4_PS(vaX, vaY, vaZ, vaW);

		_MM_TRANSPOSE4_PS(vbX, vbY, vbZ, vbW);

		__m128 vecResult;
		vecResult = _mm_mul_ps(vaX, vbX);

		vecResult = _mm_add_ps(vecResult, _mm_mul_ps(vaY, vbY));
		vecResult = _mm_add_ps(vecResult, _mm_mul_ps(vaZ, vbZ));
		vecResult = _mm_add_ps(vecResult, _mm_mul_ps(vaW, vbW));

		_mm_store_ps(&result[i], vecResult);
	}

	return result;
}


int main()
{
	TimeChecker tc{ true };

	tc.Start();
	std::cout << "set data" << std::endl;
	float* dataA = new float[COUNT * 4];
	float* dataB = new float[COUNT * 4];
	float* result = nullptr;

	for (int i = 0; i < COUNT * 4; i++)
	{
		dataA[i] = i;
		dataB[i] = i;
	}
	tc.Get();

	std::cout << "Get Time Array" << std::endl;
	tc.Start();
	for (int i = 0; i < 10; i++)
	{
		result = DotArray(COUNT, dataA, dataB);
	}
	tc.Get();
	delete[] result;

	std::cout << "Get Time SSE Slow" << std::endl;
	tc.Start();
	for (int i = 0; i < 10; i++)
	{
		result = DotSSEHorizontal(COUNT, dataA, dataB);
	}
	tc.Get();
	delete[] result;

	std::cout << "Get Time SSE" << std::endl;
	tc.Start();
	for (int i = 0; i < 10; i++)
	{
		result = DotSSE(COUNT, dataA, dataB);
	}
	tc.Get();
	delete[] result;

	delete[] dataA;
	delete[] dataB;
}