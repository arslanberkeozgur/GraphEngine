#pragma once

#include "Simulation.h"
#include <random>

class OpinionSimulation : public Simulation
{
public:
	using Simulation::Simulation;

	void onStart() override;
	void step() override;
	//void handleInputs() override;
private:
	void injectInfoTextInitialization(std::stringstream& ss) override;
	void injectInfoTextUpdate(int nodeIndex, std::stringstream& ss) override;

private:
	std::mt19937 rngEngine;
	std::uniform_int_distribution<> interactionDistribution{ 0,100 };

	std::vector<Edge*> flashingEdges;
	void flashEdges();
	void unflashEdges();
};