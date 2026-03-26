#include "BruteForceSimulator.h"

BruteForceSimulator::~BruteForceSimulator()
{

}

void BruteForceSimulator::Simulate(float _dt)
{
	// brtue force simulater

	// move direction
	for (auto& collider : m_colliders)
	{
		collider.m_x += collider.m_direction[0] * _dt;
		collider.m_y += collider.m_direction[1] * _dt;
	}

	// serch all case
	for (int i = 0; i < m_colliders.size(); ++i)
	{
		for (int j = i + 1; j < m_colliders.size(); ++j)
		{
			// is collide
			if (true == IsCollide(m_colliders[i], m_colliders[j]))
			{
				CaculateDirection(m_colliders[i], m_colliders[j]);
			}
		}

		CalculateEdgeCase(m_colliders[i]);
	}
}
