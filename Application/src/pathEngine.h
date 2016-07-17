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
		using graph_type = graph<location, road>;

		pathEngine(graph_type* p_targetGraph) : targetGraph(p_targetGraph) {}

		vector<sf::Vector2f> findPath(int fromId, int toId);

	private:
		graph_type* targetGraph;
		//float mindistancebetween(vector2d a, vector2d b);	// TODO remove?

		struct pathingTuple
		{
			float priority;
			int pathNodeId;
			vector<sf::Vector2f> pathToHere;
			float costToHere;

			pathingTuple(float p_priority, int p_pathNodeId, vector<sf::Vector2f> p_pathToHere, float p_costToHere)
				: priority(p_priority), pathNodeId(p_pathNodeId), pathToHere(p_pathToHere), costToHere(p_costToHere)
			{}
		};

		struct comparePriority
		{
			bool operator()(pathingTuple a, pathingTuple b);
		};
	};
}