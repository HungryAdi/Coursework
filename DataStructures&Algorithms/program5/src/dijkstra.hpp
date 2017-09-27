//(Iyer, Aditya Subramani) asiyer
#ifndef DIJKSTRA_HPP_
#define DIJKSTRA_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>                    //Biggest int: std::numeric_limits<int>::max()
#include "array_queue.hpp"
#include "array_stack.hpp"
#include "heap_priority_queue.hpp"
#include "hash_graph.hpp"


namespace ics {


class Info {
	public:
  Info() {}
  Info(std::string a_node) : node(a_node) {}

		bool operator == (const Info& rhs) const
		{return cost == rhs.cost && from == rhs.from;}

		bool operator !=( const Info& rhs) const
    {return !(*this == rhs);}

		friend std::ostream& operator<<(std::ostream& outs, const Info& i) {
			outs << "Info[" << i.node << "," << i.cost << "," << i.from << "]";
			return outs;
		}

		//Public instance variable definitions
    std::string node = "?";
    int         cost = std::numeric_limits<int>::max();
    std::string from = "?";
};


bool gt_info (const Info& a, const Info& b) {return a.cost < b.cost;}

typedef ics::HashGraph<int>                                  DistGraph;
typedef ics::HeapPriorityQueue<Info, gt_info>                CostPQ;
typedef ics::HashMap<std::string, Info, DistGraph::hash_str> CostMap;
typedef ics::pair<std::string, Info>                         CostMapEntry;


//Return the final_map as specified in the lecture-node description of
//  extended Dijkstra algorithm
CostMap extended_dijkstra(const DistGraph& g, std::string start_node) {
	CostMap info_map;
	CostMap answer_map;
	for (auto i : g.all_nodes())
	{
		info_map[i.first] = ics::Info();
	}
	info_map[start_node].cost = 0;
	while (!info_map.empty())
	{
		std::string min = start_node;
		for (auto entry : info_map)
		{
			if (!info_map.has_key(min) || entry.second.cost < info_map[min].cost)
			{
				min = entry.first;
			}
		}
		answer_map[min] = info_map[min];
		info_map.erase(min);
		for (auto entry : g.out_nodes(min))
		{
			if (!answer_map.has_key(entry))
			{
				if (info_map[entry].cost == std::numeric_limits<int>::max() || info_map[entry].cost > answer_map[min].cost + g.edge_value(min, entry))
				{
					info_map[entry].cost = answer_map[min].cost + g.edge_value(min, entry);
					info_map[entry].from = min;
				}
			}
		}
	}
	return answer_map;
}


//Return a queue whose front is the start node (implicit in answer_map) and whose
//  rear is the end node
ArrayQueue<std::string> recover_path(const CostMap& answer_map, std::string end_node) {
	ArrayQueue<std::string> shortest_path;
	ArrayStack<std::string> path;
	path.push(end_node);
	while (answer_map[path.peek()].from != "?")
	{
		path.push(answer_map[path.peek()].from);
	}
	while (!path.empty())
	{
		shortest_path.enqueue(path.pop());
	}
	return shortest_path;
}


}

#endif /* DIJKSTRA_HPP_ */
