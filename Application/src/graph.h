#pragma once

#include <map>
#include <string>
#include <istream>
#include <fstream>
#include <utility>

namespace jw
{
	// ###################
	// ### Declaration ###
	// ###################
	template<typename node, typename edge>
	class graph	// TODO UT?
	{
	public:
		// TODO make edges a separate adjacency matrix?
		using node_type = node;
		using edge_type = edge;
		using edge_container_type = std::map<int, edge_type>;
		using value_type = std::pair<node_type, edge_container_type>;
		using container_type = std::map<int, value_type>;
		using iterator = typename container_type::iterator;

		// Access
		value_type& at(int targetId);
		node_type& nodeAt(int targetId);
		edge_container_type& edgesAt(int targetId);

		// Iterators
		iterator begin();
		iterator end();

		// Modifiers
		bool insertNode(int id, node_type newNode);
		bool insertEdge(int fromId, int toId, edge_type link, bool bidirectional = false);

	private:
		container_type graphMap;
	};

	// ###################
	// ### Definitions ###
	// ###################
	template<typename node, typename edge>
	typename graph<node, edge>::value_type& graph<node, edge>::at(int targetId)
	{
		return graphMap.at(targetId);
	}

	template<typename node, typename edge>
	typename graph<node, edge>::node_type& graph<node, edge>::nodeAt(int targetId)
	{
		return at(targetId).first;
	}

	template<typename node, typename edge>
	typename graph<node, edge>::edge_container_type& graph<node, edge>::edgesAt(int targetId)
	{
		return at(targetId).second;
	}

	template<typename node, typename edge>
	typename graph<node, edge>::iterator graph<node, edge>::begin()
	{
		return graphMap.begin();
	}

	template<typename node, typename edge>
	typename graph<node, edge>::iterator graph<node, edge>::end()
	{
		return graphMap.end();
	}

	template<typename node, typename edge>
	bool graph<node, edge>::insertNode(int id, node_type newNode)
	{
		if (graphMap.find(id) != graphMap.end()) return false;

		graphMap[id].first = newNode;	// Also constructs an empty edge map
		return true;
	}

	template<typename node, typename edge>
	bool graph<node, edge>::insertEdge(int fromId, int toId, edge_type link, bool bidirectional)
	{
		// Check nodes exist
		if (graphMap.find(fromId) == graphMap.end() || graphMap.find(toId) == graphMap.end()) return false;

		graphMap[fromId].second[toId] = link;

		bool bidirectionalResult = true;

		if (bidirectional)
		{
			bidirectionalResult = insertEdge(toId, fromId, link, false);
		}

		// first link result && second link result
		return bidirectionalResult;
	}
}