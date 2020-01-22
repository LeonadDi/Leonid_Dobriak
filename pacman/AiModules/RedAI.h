#ifndef REDAI_H
#define REDAI_H
#include "GhostAI.h"
#include "..\World.h"
class RedAI :
	public GhostAI
{
public:
	RedAI(std::shared_ptr<World> world, MovableObject* ghost, MovableObject* target);

	void makeDecision();

	
};

#endif