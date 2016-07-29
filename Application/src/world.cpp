#include "world.h"

#include <SFML/System/Vector2.hpp>
#include <fstream>

using std::ifstream;

namespace jw
{
	world::world(string filepath)
	{
		worldGraph = loadGraph(filepath);
	}

	world::graph_type world::loadGraph(string filepath)
	{
		ifstream mapFile(filepath);
		nlohmann::json mapJson;

		mapFile >> mapJson;

		return loadGraph(mapJson);
	}

	world::graph_type world::loadGraph(nlohmann::json mapJson)
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

		graph_type outputGraph;
		
		if (mapJson.find(locationsKey) != mapJson.end())
		{
			for (auto& locationJson : mapJson.at(locationsKey))
			{
				nlohmann::json positionJson = locationJson.at(positionKey);
				sf::Vector2f position;
				position.x = positionJson.at(xKey);
				position.y = positionJson.at(zKey);
				int sourceId = locationJson.at(idKey);

				location newLocation(position);
				outputGraph.insertNode(sourceId, newLocation);
			}
		}

		if (mapJson.find(junctionsKey) != mapJson.end())
		{
			for (auto& locationJson : mapJson.at(junctionsKey))
			{
				nlohmann::json positionJson = locationJson.at(positionKey);
				sf::Vector2f position;
				position.x = positionJson.at(xKey);
				position.y = positionJson.at(zKey);
				int sourceId = locationJson.at(idKey);

				location newLocation(position);
				outputGraph.insertNode(sourceId, newLocation);
			}
		}

		if (mapJson.find(roadsKey) != mapJson.end())
		{
			for (auto& roadJson : mapJson.at(roadsKey))
			{
				int sourceId = roadJson.at(fromKey);
				location* source = &outputGraph.nodeAt(sourceId);
				int destId = roadJson.at(toKey);
				location* dest = &outputGraph.nodeAt(destId);
				road roadEdge(source, dest);
				bool bidirectional = roadJson.at(bidirectionalKey);

				outputGraph.insertEdge(sourceId, destId, roadEdge, bidirectional);
			}
		}

		return outputGraph;
	}

	void world::update()
	{
		for (auto& graphPair : worldGraph)
		{
			location& currentLocation = graphPair.second.first;
			// TODO roads

			currentLocation.update();
		}
	}

	void world::draw(sf::RenderWindow& renderTarget)
	{
		for (auto& graphPair : worldGraph)
		{
			location& currentLocation = graphPair.second.first;
			// TODO roads

			currentLocation.draw(renderTarget);
		}
	}
}