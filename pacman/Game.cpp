#include "Game.h"


Game::Game(Settings* settings)
{
	this->settings = settings;
	world = new World();
	stats = new Stats();
	//v �������, ���� ��� �������

	/*player = new Player(world, true,13,23, stats);
	ghost1 = new Ghost(world, true, 13, 11, stats, player, 0);
	ghost2 = new Ghost(world, true, 14, 14, stats, player, 1);
	ghost3 = new Ghost(world, true, 13, 14, stats, player, 2);
	ghost4 = new CyanGhost(world, true, 15, 14, stats, player, ghost1);*/


	actors = new MovableObject * [5]{};	
	actors[0] = new Player(world, true, 13, 23, stats);

	GhostFactory *factory = new RedGhostFactory(world, stats, actors[0], 0);
	actors[1] = factory->createGhost(); 
	delete factory;

	factory = new PinkGhostFactory(world, stats, actors[0], 0);
	actors[2] = factory->createGhost(); 
	delete factory;

	factory = new KlydeGhostFactory(world, stats, actors[0], 0);
	actors[3] = factory->createGhost();	
	delete factory;

	factory = new CyanGhostFactory(world, stats, actors[0], actors[1]);
	actors[4] = factory->createGhost();	
	delete factory;

	scenario = new Scenario(actors[1], actors[2], actors[3], actors[4]);

}

void Game::gameLoop()
{
	const bool gameRunning = true;
	while (gameRunning) {
		levelSplashScreen();
		while (stats->dotsCollected < 246 && !stats->gotHit) {
			const int refreshRate = 1000 / 60;
			update();
			render();
			Sleep(refreshRate);
		}

		if (!stats->gotHit)
		{
			levelFinishedScreen();
			resetAfterLevelUp();
		}
		else {
			if (stats->health <= 0)
			{
				loseSplashScreen();
				break;
			}
			gotHitSplashScreen();
			resetAfterDeath();
		}
		
	}
}


void Game::update()
{
	actors[0]->getControl();
	//player->getControl();
	if (!stats->pause)
	{
		//player->update();

		scenario->update();
		for (int i = 0; i < 5; i++)
		{
			actors[i]->update();
		}

		//actors[0]->update();
		/*ghost1->update();
		ghost2->update();
		ghost3->update();
		ghost4->update();*/

		if (stats->fright)
		{
			stats->fright = false;
		}
	}
}

void Game::render()
{
	wchar_t screenHolder[screen_width * screen_height];
	wchar_t* screen = screenHolder;
	for (int i = 0; i < screen_width * screen_height; i++) screen[i] = L' ';

	//vvv
	WORD screenHolder1[screen_width * screen_height];
	WORD* screen1 = screenHolder1;
	for (int i = 0; i < screen_width * screen_height; i++) screen1[i] = 0;

	//^^^

	screen[0] = 'X'; screen[screen_width * screen_height - 1] = 'X';


	//prepare world (map, coins)
	char* q = world->getMapForRender();
	WORD* e = world->getMapColor();

	/*//prepare @
	q[player->position[0] + (player->position[1] * map_width)] = player->getCurrentSprite();
	e[player->position[0] + (player->position[1] * map_width)] = player->getColor();
	//prepare enemies
	q[ghost1->position[0] + (ghost1->position[1] * map_width)] = ghost1->getCurrentSprite();
	e[ghost1->position[0] + (ghost1->position[1] * map_width)] = ghost1->getColor();
	q[ghost2->position[0] + (ghost2->position[1] * map_width)] = ghost2->getCurrentSprite();
	e[ghost2->position[0] + (ghost2->position[1] * map_width)] = ghost2->getColor();
	q[ghost3->position[0] + (ghost3->position[1] * map_width)] = ghost3->getCurrentSprite();
	e[ghost3->position[0] + (ghost3->position[1] * map_width)] = ghost3->getColor();
	q[ghost4->position[0] + (ghost4->position[1] * map_width)] = ghost4->getCurrentSprite();
	e[ghost4->position[0] + (ghost4->position[1] * map_width)] = ghost4->getColor();
	*/

	for (int i = 0; i < 5; i++)
	{
		q[actors[i]->position[0] + (actors[i]->position[1] * map_width)] = actors[i]->getCurrentSprite();
		e[actors[i]->position[0] + (actors[i]->position[1] * map_width)] = actors[i]->getColor();
	}

	//print map on to screen
	for (int i = 0; i < map_height; i++)
	{
		for (int j = 0; j < map_width; j++)
		{
			const int xOffset = 5;
			const int yOffset = 5;
			screen[(screen_width * (i + yOffset)) + j + xOffset] = q[i * map_width + j];
			screen1[(screen_width * (i + yOffset)) + j + xOffset] = e[i * map_width + j];
		}
	}
	//prepare stats

		//level
	char level[6] = {};
	sprintf_s(level, "%d", stats->level);
	char levelF[14] = { "Level: " };
	for (int i = 0; i < 6; i++) {
		const int xOffset = 34;
		const int yOffset = 5;
		screen[xOffset + i + yOffset * screen_width] = (char)levelF[i];
		screen1[xOffset + i + yOffset * screen_width] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

	}
	for (int i = 0; i < 6; i++){
		const int xOffset = 41;
		const int yOffset = 5;
		screen[xOffset + i + yOffset * screen_width] = (char)level[i];
		screen1[xOffset + i + yOffset * screen_width] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	}

		//score
	char score[10] = {};
	sprintf_s(score, "%d", stats->score);	
	char scoreF[14] = { "Score: " };
	for (int i = 0; i < 6; i++) {
		const int xOffset = 34;
		const int yOffset = 7;
		screen[xOffset + i + yOffset * screen_width] = (char)scoreF[i];
		screen1[xOffset + i + yOffset * screen_width] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	}
	for (int i = 0; i < 10; i++) {
		const int xOffset = 41;
		const int yOffset = 7;
		screen[xOffset + i + yOffset * screen_width] = (char)score[i];
		screen1[xOffset + i + yOffset * screen_width] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	}
	
	//health
	char health[10] = {};
	sprintf_s(health, "%d", stats->health);
	char healthF[14] = { "Health:" };
	for (int i = 0; i < 6; i++) {
		const int xOffset = 34;
		const int yOffset = 9;
		screen[xOffset + i + yOffset * screen_width] = (char)healthF[i];
		screen1[xOffset + i + yOffset * screen_width] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	}
	for (int i = 0; i < 10; i++) {
		const int xOffset = 41;
		const int yOffset = 9;
		screen[xOffset + i + yOffset * screen_width] = (char)health[i];
		screen1[xOffset + i + yOffset * screen_width] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	}


	//buffer
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwBytesWritten = 0;
    WriteConsoleOutputCharacter(h, screen, screen_height * screen_width, { 0,0 }, &dwBytesWritten);

	//vvv
	dwBytesWritten = 0;
	WriteConsoleOutputAttribute(h, screen1, screen_height * screen_width, { 0,0 }, &dwBytesWritten);
	//^^^
	delete q;
	delete e;
}

void Game::resetAfterDeath()
{
	stats->gotHit = false;

		//	|
		//	| rework later
		//	v

	actors[0]->position[0] = 13;
	actors[0]->position[1] = 23;
	actors[1]->position[0] = 13;
	actors[1]->position[1] = 11;
	actors[2]->position[0] = 14;
	actors[2]->position[1] = 14;
	actors[3]->position[0] = 13;
	actors[3]->position[1] = 14;
	actors[4]->position[0] = 15;
	actors[4]->position[1] = 14;
	scenario->reset();
	stats->pause = true;
	actors[0]->moveDirection = MovableObject::movement::stop;
}

void Game::resetAfterLevelUp()
{
	stats->level++;
	stats->dotsCollected = 0;
	world->loadMap();
	actors[0]->position[0] = 13;
	actors[0]->position[1] = 23;
	actors[1]->position[0] = 13;
	actors[1]->position[1] = 11;
	actors[2]->position[0] = 14;
	actors[2]->position[1] = 14;
	actors[3]->position[0] = 13;
	actors[3]->position[1] = 14;
	actors[4]->position[0] = 15;
	actors[4]->position[1] = 14;
	scenario->reset();
	stats->pause = true;
	actors[0]->moveDirection = MovableObject::movement::stop;
}

void Game::levelSplashScreen()
{
	wchar_t rqre[screen_width * screen_height];
	wchar_t* screen = rqre;

	for (int i = 0; i < screen_width * screen_height; i++) screen[i] = L' ';

	WORD screenHolder1[screen_width * screen_height];
	WORD* screen1 = screenHolder1;
	for (int i = 0; i < screen_width * screen_height; i++) screen1[i] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

	char level[6] = {};
	sprintf_s(level, "%d", stats->level);
	char levelF[14] = { "Level: " };
	for (int i = 0; i < 6; i++) screen[14 + i + 18 * screen_width] = (char)levelF[i];
	for (int i = 0; i < 6; i++) screen[21 + i + 18 * screen_width] = (char)level[i];
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacter(h, screen, screen_height * screen_width, { 0,0 }, &dwBytesWritten);

	dwBytesWritten = 0;
	WriteConsoleOutputAttribute(h, screen1, screen_height * screen_width, { 0,0 }, &dwBytesWritten);

	Sleep(3000);
}

void Game::gotHitSplashScreen()
{
	Sleep(1000);
	wchar_t rqre[screen_width * screen_height];
	wchar_t* screen = rqre;
	for (int i = 0; i < screen_width * screen_height; i++) screen[i] = L' ';

	WORD screenHolder1[screen_width * screen_height];
	WORD* screen1 = screenHolder1;
	for (int i = 0; i < screen_width * screen_height; i++) screen1[i] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

	char gotHitF[21] = { "You have been caught" };
	for (int i = 0; i < 21; i++) screen[10 + i + 18 * screen_width] = (char)gotHitF[i];

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacter(h, screen, screen_height * screen_width, { 0,0 }, &dwBytesWritten);

	dwBytesWritten = 0;
	WriteConsoleOutputAttribute(h, screen1, screen_height * screen_width, { 0,0 }, &dwBytesWritten);

	Sleep(3000);
}

void Game::loseSplashScreen()
{
	wchar_t rqre[screen_width * screen_height];
	wchar_t* screen = rqre;
	for (int i = 0; i < screen_width * screen_height; i++) screen[i] = L' ';

	WORD screenHolder1[screen_width * screen_height];
	WORD* screen1 = screenHolder1;
	for (int i = 0; i < screen_width * screen_height; i++) screen1[i] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

	char gameOverF[10] = { "Game over" };
	for (int i = 0; i < 10; i++) screen[10 + i + 18 * screen_width] = (char)gameOverF[i];

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacter(h, screen, screen_height * screen_width, { 0,0 }, &dwBytesWritten);

	dwBytesWritten = 0;
	WriteConsoleOutputAttribute(h, screen1, screen_height * screen_width, { 0,0 }, &dwBytesWritten);

	Sleep(3000);
}

void Game::levelFinishedScreen()
{
	Sleep(500);
	wchar_t rqre[screen_width * screen_height];
	wchar_t* screen = rqre;
	for (int i = 0; i < screen_width * screen_height; i++) screen[i] = L' ';

	WORD screenHolder1[screen_width * screen_height];
	WORD* screen1 = screenHolder1;
	for (int i = 0; i < screen_width * screen_height; i++) screen1[i] = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED;

	char levelF[15] = { "Level finished" };
	for (int i = 0; i < 15; i++) screen[14 + i + 18 * screen_width] = (char)levelF[i];
	
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacter(h, screen, screen_height * screen_width, { 0,0 }, &dwBytesWritten);

	dwBytesWritten = 0;
	WriteConsoleOutputAttribute(h, screen1, screen_height * screen_width, { 0,0 }, &dwBytesWritten);

	Sleep(3000);
}
