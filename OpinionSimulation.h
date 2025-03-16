#pragma once

#include "Simulation.h"

class OpinionSimulation : public Simulation
{
public:
	using Simulation::Simulation;

	void start() override;
	void step() override;
	void handleInputs() override;
	void setInfoText() override;
	void initializeInfoText() override;
};