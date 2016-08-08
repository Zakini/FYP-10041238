#pragma once

#include <deque>
#include <queue>
#include <utility>
#include <set>
#include <functional>
#include "graph.h"
#include "location.h"
#include "road.h"
#include "SFML/System/Vector2.hpp"

using std::deque;
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

		pathEngine(graph_type* p_targetGraph = nullptr) : targetGraph(p_targetGraph) {}

		deque<sf::Vector2f> findPath(int fromId, int toId);
		sf::Vector2f getLocationPosition(int id);

		void setGraph(graph_type* newGraph);

	private:
		graph_type* targetGraph;

		struct pathingTuple
		{
			float priority;
			int pathNodeId;
			deque<sf::Vector2f> pathToHere;
			float costToHere;

			pathingTuple(float p_priority, int p_pathNodeId, deque<sf::Vector2f> p_pathToHere, float p_costToHere)
				: priority(p_priority), pathNodeId(p_pathNodeId), pathToHere(p_pathToHere), costToHere(p_costToHere)
			{}
		};

		struct comparePriority
		{
			bool operator()(pathingTuple a, pathingTuple b);
		};
	};
}