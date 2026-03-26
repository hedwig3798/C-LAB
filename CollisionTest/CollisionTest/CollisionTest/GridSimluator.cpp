#include "GridSimluator.h"

GridSimluator::~GridSimluator()
{

}

void GridSimluator::Simulate(float _dt)
{
	// move direction
	for (auto& collider : m_colliders)
	{
		collider.m_x += collider.m_direction[0] * _dt;
		collider.m_y += collider.m_direction[1] * _dt;
	}

	// 가로 세로 크기 계산
	int col = std::min(MAX_GRID, m_poolSize[0] / (m_radius * 2));
	int row = std::min(MAX_GRID, m_poolSize[1] / (m_radius * 2));

	// 모든 그리드 초기화
	for (int x = 0; x < col; ++x)
	{
		for (int y = 0; y < row; ++y)
		{
			m_grid[x][y].clear();
		}
	}

	// 모든 콜라이더를 본인이 속한 그리드에 넣기
	for(auto& collider : m_colliders)
	{
		int x = static_cast<int>(collider.m_x / (m_radius * 2));
		int y = static_cast<int>(collider.m_y / (m_radius * 2));

		x = std::max(0, std::min(x, col - 1));
		y = std::max(0, std::min(y, row - 1));

		m_grid[x][y].push_back(&collider);
	}

	// 콜라이더 충돌 처리
	for (auto& collider : m_colliders)
	{
		int x = static_cast<int>(collider.m_x / (m_radius * 2));
		int y = static_cast<int>(collider.m_y / (m_radius * 2));

		x = std::max(0, std::min(x, col - 1));
		y = std::max(0, std::min(y, row - 1));

		// 현재 콜라이더가 속한 칸 기준 3x3 칸에 있는 콜라이더 끼리 검사
		for (int i = 0; i < 9; i++)
		{
			int nx = x + m_dx[i];
			int ny = y + m_dy[i];

			if (0 > nx
				|| 0 > ny
				|| col <= nx
				|| row <= ny)
			{
				continue;
			}

			for(auto other : m_grid[nx][ny])
			{
				if (&collider != other && IsCollide(collider, *other))
				{
					CaculateDirection(collider, *other);
				}
			}
		}

		CalculateEdgeCase(collider);
	}
}
