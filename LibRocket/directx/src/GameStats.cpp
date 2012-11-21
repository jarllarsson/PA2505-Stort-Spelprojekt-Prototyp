#include "GameStats.h"

int GameStats::m_numIterations = 0;

void GameStats::init()
{
	m_numIterations = 0;
}

void GameStats::playedTheGame()
{
	m_numIterations += 1;
}

int GameStats::getNumIterations()
{
	return m_numIterations;
}