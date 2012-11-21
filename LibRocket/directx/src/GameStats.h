#ifndef GAMESTATS_H
#define GAMESTATS_H

class GameStats
{
private:
	static int m_numIterations;
public:
	static void init();
	static void playedTheGame();
	static int getNumIterations();
};
#endif //GAMESTATS