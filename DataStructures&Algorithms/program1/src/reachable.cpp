// Submitter: asiyer(Iyer, Aditya)
// Partner: ssthothr(Sthothra Bhashyam, Sreeja)
// We certify that we worked cooperatively on this programming
//   assignment, according to the rules for pair programming
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_set.hpp"
#include "array_map.hpp"


typedef ics::ArraySet<std::string>          NodeSet;
typedef ics::pair<std::string,NodeSet>      GraphEntry;

bool graph_entry_gt (const GraphEntry& a, const GraphEntry& b)
{return a.first<b.first;}

typedef ics::ArrayPriorityQueue<GraphEntry,graph_entry_gt> GraphPQ;
typedef ics::ArrayMap<std::string,NodeSet>  Graph;


//Read an open file of edges (node names separated by semicolons, with an
//  edge going from the first node name to the second node name) and return a
//  Graph (Map) of each node name associated with the Set of all node names to
//  which there is an edge from the key node name.
Graph read_graph(std::ifstream &file) {
	Graph graph;
	int count1 = 0, count2 = 0;
	while (!file.eof())
	{
		std::string temp_string, firstnode, secondnode;
		getline(file,firstnode,';');
		getline(file,secondnode,'\r');
		file.get();
		graph[firstnode].insert(secondnode);
		count1++;
	}
	file.close();
	return graph;
}


//Print a label and all the entries in the Graph in alphabetical order
//  (by source node).
//Use a "->" to separate the source node name from the Set of destination
//  node names to which it has an edge.
void print_graph(const Graph& graph) {
	std::cout << std::endl;
	for (auto entry : graph)
	{
		std::cout << "  " << entry.first << "-> " << entry.second << std::endl;
	}
}


//Return the Set of node names reaching in the Graph starting at the
//  specified (start) node.
//Use a local Set and a Queue to respectively store the reachable nodes and
//  the nodes that are being explored.
NodeSet reachable(const Graph& graph, std::string start) {
	NodeSet tempSet;
	ics::ArrayQueue<std::string> tempQ;
	tempQ.enqueue(start);
	while (tempQ.size()!=0)
	{
		std::string current = tempQ.dequeue();
		tempSet.insert(current);
		if (graph.has_key(current))
		{
			for (auto x : graph[current])
			{
				if (!tempSet.contains(x))
				{
					tempQ.enqueue(x);
				}
			}
		}
	}
	return tempSet;
}





//Prompt the user for a file, create a graph from its edges, print the graph,
//  and then repeatedly (until the user enters "quit") prompt the user for a
//  starting node name and then either print an error (if that the node name
//  is not a source node in the graph) or print the Set of node names
//  reachable from it by using the edges in the Graph.
int main() {
  try {

	  std::string filename;
	  std:: ifstream file;
	  ics::safe_open(file,"Enter the name of a file with a graph","graph1.txt");

	  Graph graph = read_graph(file);

	  std::cout << "\nGraph: source -> {destination} edges";
	  print_graph(graph);

	  std::string input = "";
	  while (input!="quit")
	  {
		  std::cout << "\nEnter the name of a starting node (enter quit to quit): ";
		  std::cin >> input;
		  if (graph.has_key(input))
		  {
			  std::cout << "Reachable from node name " << reachable(graph, input) << std::endl;
		  }
		  else if (input=="quit")
		  {
			  break;
		  }
		  else
		  {
			  std::cout << "  " << input << " is not a source node name in the graph" << std::endl;
		  }
	  }


  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
