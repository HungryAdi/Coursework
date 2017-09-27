#include <string>
#include <iostream>
#include <fstream>
#include "ics46goody.hpp"
#include "ics_exceptions.hpp"
#include "hash_graph.hpp"

namespace ics {

HashGraph<int> g;

ics::HashGraph<int>& prompt_graph() {
  static ics::HashGraph<int> g2;
  g2.clear();
  for (;;) {
    std::string n = ics::prompt_string("  Enter degree 0 node name (QUIT to quit)");
    if (n == "QUIT")
      break;
    g2.add_node(n);
  }

  for (;;) {
    std::string o = ics::prompt_string("  Enter origin node name (QUIT to quit)");
    if (o == "QUIT")
      break;
    std::string d = ics::prompt_string("  Enter origin      node name (QUIT to quit)");
    if (d == "QUIT")
      break;
    int v = ics::prompt_int("  Enter edge value");
    g2.add_edge(o,d,v);
  }

  return g2;
}

std::string menu_prompt (std::string preface) {
  std::cout << std::endl << "graph g = " << g << std::endl;
  std::cout << preface+"\nMutators           Accessors              General" << std::endl;
  std::cout << preface+"  an - add_node      m  - empty             lf - load from file" << std::endl;
  std::cout << preface+"  ae - add_edge      #n - node_count        s  - store to file" << std::endl;
  std::cout << preface+"  rn - remove_node   #e - edge_count        q  - quit" << std::endl;
  std::cout << preface+"  re - remove_edge   n  - has_node"        << std::endl;
  std::cout << preface+"  x  - clear         e  - has_edge"      << std::endl;
  std::cout << preface+"  =  - =             v  - edge_value"             << std::endl;
  std::cout << preface+"                     i  - in_degree"      << std::endl;
  std::cout << preface+"                     o  - out_degree"      << std::endl;
  std::cout << preface+"                     d  - degree"      << std::endl;
  std::cout << preface+"                     <  - <<"      << std::endl;
  std::cout << preface+"                     r  - relations"      << std::endl;

  std::string allowable[] = {"an","ae","rn","re","x","=","m","#n","#e","n","e","v","i","o","d","<","r","lf","s","q",""};
  return ics::prompt_string("\n"+preface+"Enter graph command","",allowable);
}

void process_commands(std::string preface) {
  for (;;) try {
    std::string command = menu_prompt(preface);

    if (command == "an") {
      std::string n = ics::prompt_string("  Enter node name");
      g.add_node(n);
    }

    else if (command == "ae") {
      std::string o = ics::prompt_string("  Enter origin      node name");
      std::string d = ics::prompt_string("  Enter destination node name");
      int v = ics::prompt_int           ("  Enter value for this edge  ");
      g.add_edge(o,d,v);
    }

    else if (command == "rn") {
      std::string n = ics::prompt_string("  Enter node name");
      g.remove_node(n);
    }

    else if (command == "re") {
      std::string o = ics::prompt_string("  Enter origin      node name");
      std::string d = ics::prompt_string("  Enter destination node name");
      g.remove_edge(o,d);
    }

    else if (command == "x")
      g.clear();

    else if (command == "=") {
      ics::HashGraph<int> g2(ics::prompt_graph());
      g = g2;
    }

    else if (command == "m")
      std::cout << "  empty() = " << g.empty() << std::endl;

    else if (command == "#n")
      std::cout << "  node_count() = " << g.node_count() << std::endl;

    else if (command == "#e")
      std::cout << "  edge_count() = " << g.edge_count() << std::endl;

    else if (command == "n") {
      std::string n = ics::prompt_string("  Enter node name");
      std::cout << "  has_node(...) = " << g.has_node(n) << std::endl;
    }

    else if (command == "e") {
      std::string o = ics::prompt_string("  Enter origin      node name");
      std::string d = ics::prompt_string("  Enter destination node name");
      std::cout << "  has_edge(...) = " << g.has_edge(o,d) << std::endl;
    }

    else if (command == "v") {
      std::string o = ics::prompt_string("  Enter origin      node name");
      std::string d = ics::prompt_string("  Enter destination node name");
      std::cout << "  edge_value(...) = " << g.edge_value(o,d) << std::endl;
    }

    else if (command == "i") {
      std::string n = ics::prompt_string("  Enter node name");
      std::cout << "  in_degree(...) = " << g.in_degree(n) << std::endl;
    }

    else if (command == "o") {
      std::string n = ics::prompt_string("  Enter node name");
      std::cout << "  out_degree(...) = " << g.out_degree(n) << std::endl;
    }

    else if (command == "d") {
      std::string n = ics::prompt_string("  Enter node name");
      std::cout << "  degree(...) = " << g.degree(n) << std::endl;
    }

    else if (command == "<")
      std::cout << preface+"  << = " << g << std::endl;

    else if (command == "r") {
      std::cout << "  g == g = " << (g == g) << std::endl;
      std::cout << "  g != g = " << (g != g) << std::endl;

      ics::HashGraph<int> g2(ics::prompt_graph());
      std::cout << "  g == g2 = " << (g == g2) << std::endl;
      std::cout << "  g != g2 = " << (g != g2) << std::endl;
    }

    else if (command == "lf") {
      std::string separator = prompt_string("  Enter separator used in file");
      std::ifstream in_file;
      ics::safe_open(in_file,"  Enter file name to read graph from","standard.txt");
      g.load(in_file,separator);
    }

    else if (command == "s") {
      std::string separator = prompt_string("  Enter separator to use in file");
      std::ofstream out_file;
      out_file.open(ics::prompt_string("  Enter file name to store graph in").c_str());
      if (out_file.fail())
        std::cout << "  Illegal file name" << std :: endl;
      else
        g.store(out_file,separator);
    }

    else if (command == "q")
      break;

    else
      std::cout << preface+"\""+command+"\" is unknown command" << std::endl;

  } catch (ics::IcsError& e) {
    std::cout << preface+"  " << e.what() << std::endl;
  }

}

}

int main() {
  ics::process_commands("");
  return 0;
}



