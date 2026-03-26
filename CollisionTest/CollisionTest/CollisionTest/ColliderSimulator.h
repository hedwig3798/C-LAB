#pragma once
#include <vector>
#include <cmath>

struct Collider
{
	float m_x;
	float m_y;
	float m_direction[2];
};

struct Simulatedata
{
	// all simulator data
	int m_count;
	int m_radius;
	int m_poolSize[2];
	int m_simSpeed;
};

class ColliderSimulator
{
protected:
	int m_count;
	int m_radius;
	int m_poolSize[2];

	std::vector<Collider> m_colliders;

public:
	ColliderSimulator() = default;
	virtual ~ColliderSimulator();

	virtual void Simulate(float _dt) abstract;

	void SetSimulData(const Simulatedata& _data);

	const std::vector<Collider>& GetColliderData() { return m_colliders; };

protected:
	bool IsCollide(const Collider& _a, const Collider& _b);
	void CaculateDirection(Collider& _a, Collider& _b);
	void CalculateEdgeCase(Collider& _a);
	float Dot(const float _a[2], const float _b[2])
	{
		return _a[0] * _b[0] + _a[1] * _b[1];
	}

	void Normal(const Collider& _a, const Collider& _b, float _output[])
	{
		_output[0] = _a.m_x - _b.m_x;
		_output[1] = _a.m_y - _b.m_y;
	}

	float Distance(const Collider& _a, const Collider& _b)
	{
		return std::sqrt(static_cast<float>((_a.m_x - _b.m_x) * (_a.m_x - _b.m_x) + (_a.m_y - _b.m_y) * (_a.m_y - _b.m_y)));
	}

	void Normalize(float _a[2])
	{
		float len = sqrt(_a[0] * _a[0] + _a[1] * _a[1]);
		_a[0] /= len;
		_a[1] /= len;
	}
};