#include "pathEngine.h"

#include <typeinfo>

namespace jw
{
	deque<int> pathEngine::findPath(int fromId, int toId)
	{
		const float MAX_COST = std::numeric_limits<float>::max();	// POSSIBLE infinity?

		location* to = &targetGraph->nodeAt(toId);
		priority_queue<pathingTuple, deque<pathingTuple>, comparePriority> frontier;
		set<location*> visitedNodes;

		frontier.push(pathingTuple(0, fromId, deque<int>(), 0));

		while (frontier.size() > 0)
		{
			float costToCurrent = frontier.top().costToHere;
			int currentId = frontier.top().pathNodeId;
			location* currentNode = &targetGraph->nodeAt(currentId);
			deque<int> currentPath = frontier.top().pathToHere;
			frontier.pop();

			if (visitedNodes.find(currentNode) != visitedNodes.end()) continue;	// currentNode has been visited already

			if (*currentNode == *to)
			{
				return currentPath;
			}

			visitedNodes.insert(currentNode);

			graph<location, road>::edge_container_type& neighbours = targetGraph->edgesAt(currentId);

			for (auto it = neighbours.begin(); it != neighbours.end(); it++)
			{
				road* connectingRoad = &it->second;
				int neighbourId = it->first;
				location& neighbour = targetGraph->nodeAt(neighbourId);

				float costToNeighbour = connectingRoad->cost();
				float heuristicCostToTarget = neighbour.pathingHeuristic(to);
				float cost;
				float newPriority;

				// Prevent overflow
				if (costToCurrent == MAX_COST || costToNeighbour == MAX_COST || heuristicCostToTarget == MAX_COST)
				{
					cost = MAX_COST;
					newPriority = MAX_COST;
				}
				else
				{
					cost = costToCurrent + costToNeighbour;
					newPriority = cost + heuristicCostToTarget;
				}

				deque<int> newPath = currentPath;
				newPath.push_back(neighbourId);

				frontier.push(pathingTuple(newPriority, neighbourId, newPath, cost));
			}
		}

		// Path not found
		return deque<int>();
	}

	Vector2f pathEngine::getLocationPosition(int id)
	{
		return targetGraph->nodeAt(id).position();
	}

	bool pathEngine::isLocationTrafficControlled(int id)
	{
		return targetGraph->nodeAt(id).isControlledJunction();
	}

	Vector2f pathEngine::getRoadStartPosition(int fromId, int toId)
	{
		return targetGraph->edgesAt(fromId).at(toId).startPosition();
	}

	Vector2f pathEngine::getRoadEndPosition(int fromId, int toId)
	{
		return targetGraph->edgesAt(fromId).at(toId).endPosition();
	}

	junctionController::signalState pathEngine::getSignalAtRoadEnd(int fromId, int toId)
	{
		location& endLocation = targetGraph->nodeAt(toId);
		return endLocation.getSignalAtRoad(&targetGraph->edgeBetween(fromId, toId));
	}

	bool pathEngine::comparePriority::operator()(pathingTuple a, pathingTuple b)
	{
		return a.priority > b.priority;
	}
}