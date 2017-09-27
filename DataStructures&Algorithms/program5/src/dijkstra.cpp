//(Iyer, Aditya Subramani) asiyer
#include <string>
#include <iostream>
#include <fstream>
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "hash_graph.hpp"
#include "dijkstra.hpp"



std::string get_node_in_graph(const ics::DistGraph& g, std::string prompt, bool allow_QUIT) {
  std::string node;
  for(;;) {
    node = ics::prompt_string(prompt + " (must be in graph" + (allow_QUIT ? " or QUIT" : "") + ")");
    if ((allow_QUIT && node == "QUIT") || g.has_node(node))
      break;
  }
  return node;
}


int main() {
  try {
	  ics::DistGraph g;
	  ics::CostMap map;
	  ics::ArrayQueue<std::string> queue;
	  std::string start_node, stop_node;
	  std::string graph_name = ics::prompt_string("Enter a graph file name","flightcost.txt");
	  std::ifstream stream;
	  stream.open(graph_name);
	  g.load(stream,";");
	  std::cout << g << std::endl;

	  start_node = get_node_in_graph(g,"Enter a start node",false);
	  map = extended_dijkstra(g,start_node);
	  std::cout << map << std::endl;
	  std::cout << std::endl;

	  while ((stop_node = get_node_in_graph(g,"Enter a stop node",true)) != "QUIT") {
		  queue = recover_path(map,stop_node);
		  std::cout << "Cost is " << map[stop_node].cost << "; path is " << queue << std::endl;
		  std::cout <<std::endl;
	  }


  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
