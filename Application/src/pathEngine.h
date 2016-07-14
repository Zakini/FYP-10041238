// TODO

#pragma once

#include <vector>
#include <queue>
#include <utility>
#include <set>
#include <functional>
#include "graph.h"
#include "location.h"
#include "road.h"
#include "SFML/System/Vector2.hpp"

using std::vector;
using std::priority_queue;
using std::pair;
using std::make_pair;
using std::set;
using std::greater;

namespace jw
{
	class pathEngine
	{
	public:
		pathEngine(graph<location, road>* p_targetGraph) : targetGraph(p_targetGraph) {}

		vector<sf::Vector2f> findPath(location* from, location* to);	// TODO references instead?

	private:
		graph<location, road>* targetGraph;
		//float mindistancebetween(vector2d a, vector2d b);

		struct pathingTuple
		{
			float priority;
			location* pathNode;
			vector<sf::Vector2f> pathToHere;
			float costToHere;

			pathingTuple(float p_priority, location* p_pathNode, vector<sf::Vector2f> p_pathToHere, float p_costToHere)
				: priority(p_priority), pathNode(p_pathNode), pathToHere(p_pathToHere), costToHere(p_costToHere)
			{}
		};

		struct comparePriority
		{
			bool operator()(pathingTuple a, pathingTuple b);
		};
	};
}