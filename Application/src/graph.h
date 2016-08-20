#pragma once

#include <map>
#include <string>
#include <istream>
#include <fstream>
#include <utility>
#include <stdexcept>

namespace jw
{
	// ###################
	// ### Declaration ###
	// ###################
	template<typename node, typename edge>
	class graph	// POSSIBLE UT?
	{
	public:
		// POSSIBLE make edges a separate adjacency matrix?
		using id_type = int;
		using node_type = node;
		using edge_type = edge;
		using edge_container_type = std::map<id_type, edge_type>;
		using value_type = std::pair<node_type, edge_container_type>;
		using container_type = std::map<id_type, value_type>;
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;

		// Access
		value_type& at(id_type targetId);
		node_type& nodeAt(id_type targetId);
		edge_container_type& edgesAt(id_type targetId);

		// Meta
		int size();

		// Iterators
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;

		// Modifiers
		void insertNode(id_type id, node_type newNode);
		void insertEdge(id_type fromId, id_type toId, edge_type link, bool bidirectional = false);

	private:
		container_type graphMap;
	};

	// ###################
	// ### Definitions ###
	// ###################
	template<typename node, typename edge>
	typename graph<node, edge>::value_type& graph<node, edge>::at(id_type targetId)
	{
		return graphMap.at(targetId);
	}

	template<typename node, typename edge>
	typename graph<node, edge>::node_type& graph<node, edge>::nodeAt(id_type targetId)
	{
		return at(targetId).first;
	}

	template<typename node, typename edge>
	typename graph<node, edge>::edge_container_type& graph<node, edge>::edgesAt(id_type targetId)
	{
		return at(targetId).second;
	}

	template<typename node, typename edge>
	inline int graph<node, edge>::size()
	{
		return graphMap.size();
	}

	template<typename node, typename edge>
	typename graph<node, edge>::iterator graph<node, edge>::begin()
	{
		return graphMap.begin();
	}

	template<typename node, typename edge>
	typename graph<node, edge>::const_iterator graph<node, edge>::begin() const
	{
		return graphMap.begin();
	}

	template<typename node, typename edge>
	typename graph<node, edge>::iterator graph<node, edge>::end()
	{
		return graphMap.end();
	}

	template<typename node, typename edge>
	typename graph<node, edge>::const_iterator graph<node, edge>::end() const
	{
		return graphMap.end();
	}

	template<typename node, typename edge>
	void graph<node, edge>::insertNode(id_type id, node_type newNode)
	{
		graphMap[id].first = newNode;	// Also constructs an empty edge map
	}

	template<typename node, typename edge>
	void graph<node, edge>::insertEdge(id_type fromId, id_type toId, edge_type link, bool bidirectional)
	{
		// Check nodes exist
		if (graphMap.find(fromId) == graphMap.end() || graphMap.find(toId) == graphMap.end()) throw std::invalid_argument("invalid node ID");

		graphMap[fromId].second[toId] = link;

		if (bidirectional)
		{
			insertEdge(toId, fromId, link, false);
		}
	}
}