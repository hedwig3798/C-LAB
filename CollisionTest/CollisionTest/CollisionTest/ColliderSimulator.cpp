#include "ColliderSimulator.h"

#include<iostream>
#include<cstdlib>
#include<ctime>

ColliderSimulator::~ColliderSimulator()
{

}

void ColliderSimulator::SetSimulData(const Simulatedata& _data)
{
	m_count = _data.m_count;
	m_radius = _data.m_radius;
	m_poolSize[0] = _data.m_poolSize[0];
	m_poolSize[1] = _data.m_poolSize[1];

	m_colliders.clear();
	m_colliders.resize(m_count);

	srand(static_cast<unsigned int>(time(NULL)));
	for(auto& collider : m_colliders)
	{
		collider.m_x = rand() % m_poolSize[0];
		collider.m_y = rand() % m_poolSize[1];
		collider.m_direction[0] = static_cast<float>(rand()) / RAND_MAX;
		collider.m_direction[0] = collider.m_direction[0] * 2.f - 1.0f;
		collider.m_direction[1] = static_cast<float>(rand()) / RAND_MAX;
		collider.m_direction[1] = collider.m_direction[1] * 2.f - 1.0f;

		Normalize(collider.m_direction);
	}
}

bool ColliderSimulator::IsCollide(const Collider& _a, const Collider& _b)
{
	return Distance(_a, _b) <= m_radius * 2;
}

void ColliderSimulator::CaculateDirection(Collider& _a, Collider& _b)
{
	// normal vector of two collider
	float normal[2];
	Normal(_a, _b, normal);

	// distance
	float dis = Distance(_a, _b);

	// if distance is too small
	if (dis <= 0.0001f)
	{
		normal[0] = 1.0f;
		normal[1] = 0.0f;
		dis = 0.0001f;
	}
	else
	{
		normal[0] /= dis;
		normal[1] /= dis;
	}

	// if overlap now
	float overlap = (m_radius + m_radius) - dis;
	if (overlap > 0.0f)
	{
		_a.m_x += normal[0] * overlap * 0.5f;
		_a.m_y += normal[1] * overlap * 0.5f;

		_b.m_x -= normal[0] * overlap * 0.5f;
		_b.m_y -= normal[1] * overlap * 0.5f;
	}

	// get reflection direction
	float relDirectionX = _a.m_direction[0] - _b.m_direction[0];
	float relDirectionY = _a.m_direction[1] - _b.m_direction[1];

	// separt ball
	float separatingVelocity = (relDirectionX * normal[0]) + (relDirectionY * normal[1]);
	if (separatingVelocity < 0.0f)
	{
		float dot1 = Dot(_a.m_direction, normal);
		_a.m_direction[0] = _a.m_direction[0] - 2.0f * dot1 * normal[0];
		_a.m_direction[1] = _a.m_direction[1] - 2.0f * dot1 * normal[1];
		Normalize(_a.m_direction);

		float dot2 = Dot(_b.m_direction, normal);
		_b.m_direction[0] = _b.m_direction[0] - 2.0f * dot2 * normal[0];
		_b.m_direction[1] = _b.m_direction[1] - 2.0f * dot2 * normal[1];
		Normalize(_b.m_direction);
	}
}

void ColliderSimulator::CalculateEdgeCase(Collider& _a)
{
	if (0 > _a.m_x && 0 > _a.m_direction[0])
	{
		_a.m_direction[0] *= -1;
	}

	if (m_poolSize[0] < _a.m_x && 0 < _a.m_direction[0])
	{
		_a.m_direction[0] *= -1;
	}

	if (0 > _a.m_y && 0 > _a.m_direction[1])
	{
		_a.m_direction[1] *= -1;
	}

	if (m_poolSize[1] < _a.m_y && 0 < _a.m_direction[1])
	{
		_a.m_direction[1] *= -1;
	}
}

