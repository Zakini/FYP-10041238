#include "pathEngine.h"

#include <typeinfo>

namespace jw
{
	deque<sf::Vector2f> pathEngine::findPath(int fromId, int toId)
	{
		const float MAX_COST = std::numeric_limits<float>::max();	// TODO infinity?

		location* to = &targetGraph->nodeAt(toId);
		priority_queue<pathingTuple, deque<pathingTuple>, comparePriority> frontier;
		set<location*> visitedNodes;

		frontier.push(pathingTuple(0, fromId, deque<sf::Vector2f>(), 0));

		while (frontier.size() > 0)
		{
			float costToCurrent = frontier.top().costToHere;
			int currentId = frontier.top().pathNodeId;
			location* currentNode = &targetGraph->nodeAt(currentId);
			deque<sf::Vector2f> currentPath = frontier.top().pathToHere;
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
				float heuristicCostToTarget = neighbour.heuristic(to);
				float cost;
				float newPriority;

				// Prevent overflow
				if (costToCurrent == MAX_COST || costToNeighbour == MAX_COST || heuristicCostToTarget == MAX_COST)
				{
					cost = MAX_COST;
				}
				else
				{
					cost = costToCurrent + costToNeighbour;
					newPriority = cost + heuristicCostToTarget;
				}

				deque<sf::Vector2f> newPath = currentPath;
				newPath.push_back(neighbour.position());

				frontier.push(pathingTuple(newPriority, neighbourId, newPath, cost));
			}
		}

		// Path not found
		return deque<sf::Vector2f>();
	}

	sf::Vector2f pathEngine::getLocationPosition(int id)
	{
		return targetGraph->nodeAt(id).position();
	}

	void pathEngine::setGraph(graph_type* newGraph)
	{
		targetGraph = newGraph;
	}

	bool pathEngine::comparePriority::operator()(pathingTuple a, pathingTuple b)
	{
		return a.priority > b.priority;
	}
}