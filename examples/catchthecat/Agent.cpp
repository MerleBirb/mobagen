#include "Agent.h"
#include "World.h"

std::map<int, std::map<int, queueEntry>> Agent::BuildWorld(World* w)
{
	auto worldSideOver2 = w->getWorldSideSize() / 2;
	std::map<int, std::map<int, queueEntry>> complexWorld;

	for (int line = -worldSideOver2; line <= worldSideOver2; line++)
	{
		for (int col = -worldSideOver2; col <= worldSideOver2; col++)
		{
            //complexWorld[line][col] = w->getContent(col, line)
		}
	}
}
