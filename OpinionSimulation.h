#pragma once

#include "Simulation.h"
#include <random>

class OpinionSimulation : public Simulation
{
public:
	using Simulation::Simulation;

	void start() override;
	//void step() override;
	//void handleInputs() override;
	//void setInfoText() override;
private:
	void injectInfoTextInitialization(std::stringstream& ss) override;
	//void injectInfoTextUpdate(int nodeIndex, std::stringstream& ss) override;

private:
	std::mt19937 rngEngine;
};