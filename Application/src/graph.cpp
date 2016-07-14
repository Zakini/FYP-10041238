#include "graph.h"

#include <typeinfo>

namespace jw
{
	// TODO remove
	//template<typename N, typename E>
	//bool graph<N, E>::insertNode(N newNode)
	//{
	//	if (graphMap.find(newNode) != graphMap.end()) return false;

	//	nodes[newNode];	// Create an empty edge map for newNode
	//	return true;
	//}

	//template<typename N, typename E>
	//bool graph<N, E>::insertEdge(N from, N to, E link, bool bidirectional)
	//{
	//	// Check nodes exist
	//	if (graphMap.find(from) == graphMap.end() || graphMap.find(to) == graphMap.end()) return false;

	//	// Link nodes (Note: overrides existing edges)
	//	graphMap[from][link] = to;
	//	return true;
	//}

	// TODO move to world.cpp
	/*istream& operator>> (istream& lhs, graph& rhs)
	{
		const string locationsKey = "locations";
		const string junctionsKey = "junctions";
		const string roadsKey = "roads";
		const string positionKey = "position";
		const string xKey = "x";
		const string zKey = "z";
		const string idKey = "id";
		const string fromKey = "from";
		const string toKey = "to";
		const string bidirectionalKey = "bidirectional";

		json mapJson;

		json positionJson;
		sf::Vector2f position;
		int sourceId, destId;
		road* roadEdge;
		location* fromLocation, *toLocation;
		bool bidirectional;

		lhs >> mapJson;

		for (auto& locationJson : mapJson.at(locationsKey))
		{
			positionJson = locationJson.at(positionKey);
			position.x = positionJson.at(xKey);
			position.y = positionJson.at(zKey);
			sourceId = locationJson.at(idKey);

			location* newLocation = new location(position);
			rhs.addNode(sourceId, newLocation);
		}

		for (auto& locationJson : mapJson.at(junctionsKey))
		{
			positionJson = locationJson.at(positionKey);
			position.x = positionJson.at(xKey);
			position.y = positionJson.at(zKey);
			sourceId = locationJson.at(idKey);

			location* newLocation = new location(position);
			rhs.addNode(sourceId, newLocation);
		}

		for (auto& roadJson : mapJson.at(roadsKey))
		{
			sourceId = roadJson.at(fromKey);
			fromLocation = dynamic_cast<location*>(rhs.getNode(sourceId));

			destId = roadJson.at(toKey);
			toLocation = dynamic_cast<location*>(rhs.getNode(destId));

			if (fromLocation && toLocation)
			{
				roadEdge = new road(fromLocation, toLocation);

				bidirectional = roadJson.at(bidirectionalKey);

				rhs.linkNodes(sourceId, destId, roadEdge, bidirectional);
			}
			else
			{
				throw std::bad_cast();;
			}
		}

		return lhs;
	}*/
}