#pragma once

#include "Simulation.h"
#include <random>

class OpinionSimulation : public Simulation
{
public:
	using Simulation::Simulation;

	~OpinionSimulation() override;

	void onStart() override;
	void onStep() override;
	void injectInputHandling() override;
private:
	void injectInfoTextInitialization(std::stringstream& ss) override;
	void injectInfoTextUpdate(int nodeIndex, std::stringstream& ss) override;

private:
	bool pKeyHeld = false;

	sf::Color trueOpinionColor = sf::Color::Cyan;
	sf::Color falseOpinionColor = sf::Color::Red;

	std::mt19937 rngEngine;
	std::uniform_int_distribution<> interactionDistribution{ 0,100 };

	std::vector<Edge*> flashingEdges;
	void flashEdges();
	void unflashEdges();

	unsigned int numOfTrueNodes = 0;
	unsigned int numOfFalseNodes = 0;
	unsigned int recordPeriod = 10;
};