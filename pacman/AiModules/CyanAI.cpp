#include "CyanAI.h"

CyanAI::CyanAI(std::shared_ptr<World> world, MovableObject* ghost, MovableObject* target, MovableObject* target2)
	:GhostAI(world, ghost, target)
{
	this->target2 = target2;
}

void CyanAI::makeDecision()
{
	switch (currentAi)
	{
	case AiMode::idle:
		break;
	case AiMode::pursuit:
		pursuit();
		break;
	case AiMode::scatter:
		scatter();
		break;
	case AiMode::fright:
		fright();
		break;
	case AiMode::eaten:
		eaten();
		break;
	default:
		break;
	}
}

void CyanAI::pursuit()
{
	const int MULTIPLIER = -2;
	int x = (target2->position[0] - target->position[0]) * MULTIPLIER;
	targetCoord[0] = target2->position[0] + x;
	int y = (target2->position[1] - target->position[1]) * MULTIPLIER;
	targetCoord[1] = target2->position[0] + y;
	
	navigateToTarget(targetCoord[0], targetCoord[1]);
}
