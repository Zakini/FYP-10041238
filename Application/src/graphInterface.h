#pragma once

namespace jw
{
	// POSSIBLE UT? using polymorphism?
	template<typename node, typename edge>
	class graphInterface
	{
	public:
		using node_id_type = int;
		using node_type = node;
		using edge_type = edge;

		// Access
		virtual node_type& nodeAt(node_id_type targetId) = 0;

		// Meta
		virtual int size() = 0;

		// Modifiers
		virtual void insertNode(node_id_type id, node_type newNode) = 0;
		virtual void insertEdge(node_id_type fromId, node_id_type toId, edge_type link, bool bidirectional = false) = 0;
	};
}