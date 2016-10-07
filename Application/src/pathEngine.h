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
#include <memory>
#include "junctionController.h"

using std::deque;
using std::priority_queue;
using std::pair;
using std::make_pair;
using std::set;
using std::greater;
using std::shared_ptr;
using sf::Vector2f;

namespace jw
{
	class pathEngine
	{
	public:
		using graph_type = graph<location, road>;

		pathEngine(shared_ptr<graph_type> p_targetGraph) : targetGraph(p_targetGraph) {}

		deque<int> findPath(int fromId, int toId);

		Vector2f getLocationPosition(int id);
		bool isLocationTrafficControlled(int id);
		Vector2f getRoadStartPosition(int fromId, int toId);
		Vector2f getRoadEndPosition(int fromId, int toId);
		junctionController::signalState getSignalAtRoadEnd(int fromId, int toId);

	private:
		shared_ptr<graph_type> targetGraph;

		struct pathingTuple
		{
			float priority;
			int pathNodeId;
			deque<int> pathToHere;
			float costToHere;

			pathingTuple(float p_priority, int p_pathNodeId, deque<int> p_pathToHere, float p_costToHere)
				: priority(p_priority), pathNodeId(p_pathNodeId), pathToHere(p_pathToHere), costToHere(p_costToHere)
			{}
		};

		struct comparePriority
		{
			bool operator()(pathingTuple a, pathingTuple b);
		};
	};
}