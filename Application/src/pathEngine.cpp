#include "pathEngine.h"

#include <typeinfo>

namespace jw
{
	vector<sf::Vector2f> pathEngine::findPath(location* from, location* to)
	{
		const float MAX_COST = std::numeric_limits<float>::max();	// TODO infinity?

		priority_queue<pathingTuple, vector<pathingTuple>, comparePriority> frontier;
		set<location*> visitedNodes;

		frontier.push(pathingTuple(0, from, vector<sf::Vector2f>(), 0));

		while (frontier.size() > 0)
		{
			float costToCurrent = frontier.top().costToHere;
			location* currentNode = frontier.top().pathNode;
			vector<sf::Vector2f> currentPath = frontier.top().pathToHere;
			frontier.pop();

			if (visitedNodes.find(currentNode) != visitedNodes.end()) continue;	// currentNode has been visited already

			if (*currentNode == *to)
			{
				return currentPath;
			}

			visitedNodes.insert(currentNode);

			graph<location, road>::edge_container_type& neighbours = targetGraph->at(*currentNode);

			for (auto it = neighbours.begin(); it != neighbours.end(); it++)
			{
				float costToNeighbour = it->first.cost();
				float heuristicCostToTarget = it->second.heuristic(to);
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

				vector<sf::Vector2f> newPath = currentPath;
				newPath.push_back(it->second.position());

				frontier.push(pathingTuple(newPriority, &it->second, newPath, cost));
			}
		}

		// Path not found
		return vector<sf::Vector2f>();
	}

	bool pathEngine::comparePriority::operator()(pathingTuple a, pathingTuple b)
	{
		return a.priority > b.priority;
	}
}