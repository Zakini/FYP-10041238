#pragma once

#include <map>
#include <string>
#include <istream>
#include <fstream>

namespace jw
{
	// ###################
	// ### Declaration ###
	// ###################
	template<typename node, typename edge>
	class graph
	{
	public:
		using node_type = node;
		using edge_type = edge;
		using edge_container_type = std::map<edge_type, node_type&>;	// TODO edge becomes const, is this gonna work?
		using container_type = std::map<node_type, edge_container_type>;
		using iterator = typename container_type::iterator;

		// Access
		edge_container_type& at(node_type& target);

		// Iterators
		iterator begin();
		iterator end();

		// Modifiers
		bool insertNode(node_type newNode);
		bool insertEdge(node_type from, node_type to, edge_type link, bool bidirectional = false);

	private:
		container_type graphMap;
	};

	// ###################
	// ### Definitions ###
	// ###################
	template<typename node, typename edge>
	typename graph<node, edge>::edge_container_type& graph<node, edge>::at(node_type& target)
	{
		return graphMap.at(target);
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
	bool graph<node, edge>::insertNode(node_type newNode)
	{
		if (graphMap.find(newNode) != graphMap.end()) return false;

		nodes[newNode];	// Create an empty edge map for newNode
		return true;
	}

	template<typename node, typename edge>
	bool graph<node, edge>::insertEdge(node_type from, node_type to, edge_type link, bool bidirectional)
	{
		// Check nodes exist
		if (graphMap.find(from) == graphMap.end() || graphMap.find(to) == graphMap.end()) return false;

		// Link nodes (Note: overrides existing edges)
		graphMap[from][link] = to;
		return true;
	}
}