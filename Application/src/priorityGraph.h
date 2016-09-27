#pragma once

#include "graphInterface.h"
#include <stdexcept>
#include <map>
#include <algorithm>
#include <utility>

namespace jw
{
	template<typename node, typename edge>
	class priorityGraph : public graphInterface<node, edge>
	{
	public:
		using node_id_type = int;
		using node_type = node;
		using edge_type = edge;
		using priority_type = int;
		using edge_id_type = std::pair<priority_type, node_id_type>;
		using edge_container_type = std::map<edge_id_type, edge_type>;
		using value_type = std::pair<node_type, edge_container_type>;
		using container_type = std::map<node_id_type, value_type>;
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;

		// Access
		value_type& at(node_id_type targetId);
		virtual node_type& nodeAt(node_id_type targetId) override;
		edge_container_type& edgesAt(node_id_type targetId);
		edge_type& edgeBetween(node_id_type from, node_id_type to);
		edge_type& edgeBetween(node_id_type from, node_id_type to, priority_type edgePriority);

		// Meta
		virtual int size() override;

		// Iterators
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;

		// Modifiers
		virtual void insertNode(node_id_type id, node_type newNode) override;
		virtual void insertEdge(node_id_type fromId, node_id_type toId, edge_type link, bool bidirectional = false) override;
		virtual void insertEdge(node_id_type fromId, node_id_type toId, edge_type link, priority_type priority, bool bidirectional = false);

	private:
		container_type graphMap;
	};

	template<typename node, typename edge>
	typename priorityGraph<node, edge>::value_type& priorityGraph<node, edge>::at(node_id_type targetId)
	{
		return graphMap.at(targetId);
	}

	template<typename node, typename edge>
	typename priorityGraph<node, edge>::node_type& priorityGraph<node, edge>::nodeAt(node_id_type targetId)
	{
		return at(targetId).first;
	}

	template<typename node, typename edge>
	typename priorityGraph<node, edge>::edge_container_type& priorityGraph<node, edge>::edgesAt(node_id_type targetId)
	{
		return at(targetId).second;
	}

	template<typename node, typename edge>
	typename priorityGraph<node, edge>::edge_type& priorityGraph<node, edge>::edgeBetween(node_id_type from, node_id_type to)
	{
		return edgesAt(from).begin()->second;	// highest priority edge
	}

	template<typename node, typename edge>
	typename priorityGraph<node, edge>::edge_type& priorityGraph<node, edge>::edgeBetween(node_id_type from, node_id_type to, priority_type edgePriority)
	{
		return edgesAt(from).at(std::make_pair(edgePriority, to));
	}

	template<typename node, typename edge>
	inline int priorityGraph<node, edge>::size()
	{
		return graphMap.size();
	}

	template<typename node, typename edge>
	typename priorityGraph<node, edge>::iterator priorityGraph<node, edge>::begin()
	{
		return graphMap.begin();
	}

	template<typename node, typename edge>
	typename priorityGraph<node, edge>::const_iterator priorityGraph<node, edge>::begin() const
	{
		return graphMap.begin();
	}

	template<typename node, typename edge>
	typename priorityGraph<node, edge>::iterator priorityGraph<node, edge>::end()
	{
		return graphMap.end();
	}

	template<typename node, typename edge>
	typename priorityGraph<node, edge>::const_iterator priorityGraph<node, edge>::end() const
	{
		return graphMap.end();
	}

	template<typename node, typename edge>
	void priorityGraph<node, edge>::insertNode(node_id_type id, node_type newNode)
	{
		graphMap[id].first = newNode;	// Also constructs an empty edge map
	}

	template<typename node, typename edge>
	void priorityGraph<node, edge>::insertEdge(node_id_type fromId, node_id_type toId, edge_type link, bool bidirectional)
	{
		int lowestPriority = 0;

		edge_container_type fromNodeEdges = edgesAt(fromId);

		if (!fromNodeEdges.empty())
			lowestPriority = fromNodeEdges.rbegin()->first.first;

		insertEdge(fromId, toId, link, lowestPriority + 1, bidirectional);
	}

	template<typename node, typename edge>
	inline void priorityGraph<node, edge>::insertEdge(node_id_type fromId, node_id_type toId, edge_type link, priority_type priority, bool bidirectional)
	{
		// Check nodes exist
		if (graphMap.find(fromId) == graphMap.end() || graphMap.find(toId) == graphMap.end()) throw std::invalid_argument("invalid node ID");

		edge_id_type edgeId = std::make_pair(priority, toId);

		graphMap[fromId].second[edgeId] = link;

		if (bidirectional)
		{
			insertEdge(toId, fromId, link, priority, false);
		}
	}
}