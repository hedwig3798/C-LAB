#pragma once
#include "ColliderSimulator.h"

class BruteForceSimulator
	: public ColliderSimulator
{

public:
	BruteForceSimulator() = default;
	virtual ~BruteForceSimulator();

	virtual void Simulate(float _dt) override;
};