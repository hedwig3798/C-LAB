#include "AABBTreeSimulator.h"

AABBTreeSimulator::~AABBTreeSimulator()
{

}

void AABBTreeSimulator::Simulate(float _dt)
{
	// move direction
	for (auto& collider : m_colliders)
	{
		collider.m_x += collider.m_direction[0] * _dt;
		collider.m_y += collider.m_direction[1] * _dt;
	}
}

