#include "world.h"

#include <SFML/System/Vector2.hpp>
#include <fstream>

using std::ifstream;

namespace jw
{
	world::world(shared_ptr<graph_type> p_worldGraph) : worldGraph(p_worldGraph)
	{
		for (auto& graphPair : *worldGraph)
		{
			gameObject* currentLocation = &graphPair.second.first;
			worldObjects.insert(currentLocation);

			for (auto& roadPair : graphPair.second.second)
			{
				gameObject* currentRoad = &roadPair.second;
				worldObjects.insert(currentRoad);
			}
		}
	}

	world::graph_type* world::loadWorld(string filepath)
	{
		ifstream mapFile(filepath);
		nlohmann::json mapJson;

		mapFile >> mapJson;

		return loadWorld(mapJson);
	}

	world::graph_type* world::loadWorld(nlohmann::json mapJson)
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

		graph_type* outputGraph = new graph_type();
		
		for (auto& locationJson : mapJson[locationsKey])
		{
			nlohmann::json positionJson = locationJson.at(positionKey);
			sf::Vector2f position;
			position.x = positionJson.at(xKey);
			position.y = positionJson.at(zKey);
			int sourceId = locationJson.at(idKey);

			location newLocation(position);
			outputGraph->insertNode(sourceId, newLocation);
		}

		for (auto& locationJson : mapJson[junctionsKey])
		{
			nlohmann::json positionJson = locationJson.at(positionKey);
			sf::Vector2f position;
			position.x = positionJson.at(xKey);
			position.y = positionJson.at(zKey);
			int sourceId = locationJson.at(idKey);

			location newLocation(position);
			outputGraph->insertNode(sourceId, newLocation);
		}

		for (auto& roadJson : mapJson[roadsKey])
		{
			int sourceId = roadJson.at(fromKey);
			location* source = &outputGraph->nodeAt(sourceId);
			int destId = roadJson.at(toKey);
			location* dest = &outputGraph->nodeAt(destId);
			road roadEdge(source, dest);
			bool bidirectional = roadJson.at(bidirectionalKey);

			outputGraph->insertEdge(sourceId, destId, roadEdge, bidirectional);
		}

		return outputGraph;
	}

	void world::update(sf::Time timeSinceLastFrame)
	{
		for (auto worldObj : worldObjects)
		{
			worldObj->update(timeSinceLastFrame);
		}
	}

	void world::draw(sf::RenderWindow& renderTarget)
	{
		for (auto worldObj : worldObjects)
		{
			worldObj->draw(renderTarget);
		}
	}
}