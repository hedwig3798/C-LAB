#pragma once
#include "ColliderSimulator.h"

class AABBTreeSimulator
	: public ColliderSimulator
{
public:
	AABBTreeSimulator() = default;
	virtual ~AABBTreeSimulator() override;

	virtual void Simulate(float _dt) override;

};