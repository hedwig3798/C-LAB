#pragma once
#include "ColliderSimulator.h"

// 한 그리드의 크기를 최대 8로 고정
constexpr int MAX_GRID = 1024;

class GridSimluator
	: public ColliderSimulator
{
private:
	std::vector<Collider*> m_grid[MAX_GRID][MAX_GRID];

	// 주변 9칸 탐색을 위한 배열
	const int m_dx[9] = { 0, 1, -1, 0, 1, -1, 0, 1, -1 };
	const int m_dy[9] = { 0, 1, -1, 1, -1 ,0, -1, 0, 1 };

public:
	GridSimluator() = default;
	virtual ~GridSimluator() override;

	virtual void Simulate(float _dt) override;
};