#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>                 // std::random_shuffle
#include <string>                    // std::hash<std::string>
#include "ics46goody.hpp"
#include "gtest/gtest.h"
#include "array_set.hpp"
#include "hash_graph.hpp"

typedef std::string                  NodeType;
typedef ics::pair<NodeType,NodeType> EdgeType;
typedef ics::HashGraph<int>          GraphType;

typedef ics::ArraySet<NodeType>      NodeSet;
typedef ics::ArraySet<EdgeType>      EdgeSet;


class GraphTest : public ::testing::Test {
protected:
    virtual void SetUp()    {}
    virtual void TearDown() {}
};


EdgeType od(std::string o, std::string d)
{return ics::make_pair(o,d);}

void build_standard_graph(GraphType& g) {
  g.add_edge("a","b",12);
  g.add_edge("a","c",13);
  g.add_edge("b","d",24);
  g.add_edge("c","d",34);
  g.add_edge("a","d",14);
  g.add_edge("d","a",41);
  g.add_node("e");
}

template<class Iterable>
int produces(const Iterable& i) {
  int count = 0;
  for (auto e : i)
    ++count;
  return count;
}


TEST_F(GraphTest, empty) {
  GraphType g;
  ASSERT_TRUE(g.empty());
}


TEST_F(GraphTest, size) {
  GraphType g;
  ASSERT_EQ(0,g.node_count());
  ASSERT_EQ(0,g.edge_count());
  ASSERT_EQ(0, produces(g.all_nodes()));
  ASSERT_EQ(0, produces(g.all_edges()));
}


TEST_F(GraphTest, add_node) {
  GraphType g;
  g.add_node("a");
  g.add_node("b");
  g.add_node("b");
  ASSERT_TRUE(g.has_node("a"));
  ASSERT_TRUE(g.has_node("b"));
  ASSERT_FALSE(g.has_node("c"));
  ASSERT_EQ(2,g.node_count());
  ASSERT_EQ(0,g.edge_count());
  ASSERT_EQ(2, produces(g.all_nodes()) );
}


TEST_F(GraphTest, add_edge1) {
  GraphType g;

  g.add_edge("a","b",12);

  ASSERT_TRUE(g.has_node("a"));
  ASSERT_TRUE(g.has_node("b"));

  ASSERT_TRUE (g.has_edge("a","b"));
  ASSERT_FALSE(g.has_edge("b","a"));
  ASSERT_FALSE(g.has_edge("a","c"));
  ASSERT_FALSE(g.has_edge("b","c"));
  ASSERT_EQ(12,g.edge_value("a","b"));
  ASSERT_THROW(g.edge_value("a","c"),ics::GraphError);
  ASSERT_THROW(g.edge_value("b","c"),ics::GraphError);

  ASSERT_EQ(1,g.out_degree("a"));
  ASSERT_EQ(0,g.in_degree ("a"));
  ASSERT_EQ(1,g.degree    ("a"));
  ASSERT_EQ(0,g.out_degree("b"));
  ASSERT_EQ(1,g.in_degree ("b"));
  ASSERT_EQ(1,g.degree    ("b"));
  ASSERT_THROW(g.out_degree("c"),ics::GraphError);
  ASSERT_THROW(g.in_degree("c"), ics::GraphError);
  ASSERT_THROW(g.out_degree("c"),ics::GraphError);

  ASSERT_EQ(2,g.node_count());
  ASSERT_EQ(1,g.edge_count());

  ASSERT_EQ(2, produces(g.all_nodes()) );
  ASSERT_EQ(1, produces(g.all_edges()) );


  g.add_edge("a","c",13);
  ASSERT_TRUE(g.has_node("a"));
  ASSERT_TRUE(g.has_node("b"));
  ASSERT_TRUE(g.has_node("c"));

  ASSERT_TRUE (g.has_edge("a","b"));
  ASSERT_TRUE (g.has_edge("a","c"));
  ASSERT_FALSE(g.has_edge("b","a"));
  ASSERT_FALSE(g.has_edge("c","a"));
  ASSERT_FALSE(g.has_edge("b","c"));
  ASSERT_EQ(12,g.edge_value("a","b"));
  ASSERT_EQ(13,g.edge_value("a","c"));
  ASSERT_THROW(g.edge_value("b","c"),ics::GraphError);

  ASSERT_EQ(2,g.out_degree("a"));
  ASSERT_EQ(0,g.in_degree ("a"));
  ASSERT_EQ(2,g.degree    ("a"));
  ASSERT_EQ(0,g.out_degree("b"));
  ASSERT_EQ(1,g.in_degree ("b"));
  ASSERT_EQ(1,g.degree    ("b"));
  ASSERT_EQ(0,g.out_degree("c"));
  ASSERT_EQ(1,g.in_degree ("c"));
  ASSERT_EQ(1,g.degree    ("c"));

  ASSERT_EQ(3,g.node_count());
  ASSERT_EQ(2,g.edge_count());

  ASSERT_EQ(3, produces(g.all_nodes()) );
  ASSERT_EQ(2, produces(g.all_edges()) );


  g.add_edge("b","c",23);
  ASSERT_TRUE(g.has_node("a"));
  ASSERT_TRUE(g.has_node("b"));
  ASSERT_TRUE(g.has_node("c"));

  ASSERT_TRUE (g.has_edge("a","b"));
  ASSERT_TRUE (g.has_edge("a","c"));
  ASSERT_TRUE (g.has_edge("b","c"));
  ASSERT_FALSE(g.has_edge("b","a"));
  ASSERT_FALSE(g.has_edge("c","a"));
  ASSERT_FALSE(g.has_edge("c","b"));
  ASSERT_EQ(12,g.edge_value("a","b"));
  ASSERT_EQ(13,g.edge_value("a","c"));
  ASSERT_EQ(23,g.edge_value("b","c"));

  ASSERT_EQ(2,g.out_degree("a"));
  ASSERT_EQ(0,g.in_degree ("a"));
  ASSERT_EQ(2,g.degree    ("a"));
  ASSERT_EQ(1,g.out_degree("b"));
  ASSERT_EQ(1,g.in_degree ("b"));
  ASSERT_EQ(2,g.degree    ("b"));
  ASSERT_EQ(0,g.out_degree("c"));
  ASSERT_EQ(2,g.in_degree ("c"));
  ASSERT_EQ(2,g.degree    ("c"));

  ASSERT_EQ(3,g.node_count());
  ASSERT_EQ(3,g.edge_count());

  ASSERT_EQ(3, produces(g.all_nodes()) );
  ASSERT_EQ(3, produces(g.all_edges()) );

  {//node tests
    NodeSet aout; aout.insert("b"); aout.insert("c");
    NodeSet ain;
    NodeSet bout; bout.insert("c");
    NodeSet bin;  bin.insert("a");
    NodeSet cout;
    NodeSet cin;  cin.insert("a"); cin.insert("b");

    ASSERT_EQ(NodeSet(g.out_nodes("a")),aout);
    ASSERT_EQ(NodeSet(g.in_nodes ("a")),ain);
    ASSERT_EQ(NodeSet(g.out_nodes("b")),bout);
    ASSERT_EQ(NodeSet(g.in_nodes ("b")),bin);
    ASSERT_EQ(NodeSet(g.out_nodes("c")),cout);
    ASSERT_EQ(NodeSet(g.in_nodes ("c")),cin);
  }

  {//edge tests
    EdgeSet aout; aout.insert(od("a","b")); aout.insert(od("a","c"));
    EdgeSet ain;
    EdgeSet bout; bout.insert(od("b","c"));
    EdgeSet bin;  bin.insert(od("a","b"));
    EdgeSet cout;
    EdgeSet cin;  cin.insert(od("a","c")); cin.insert(od("b","c"));

    ASSERT_EQ(EdgeSet(g.out_edges("a")),aout);
    ASSERT_EQ(EdgeSet(g.in_edges ("a")),ain);
    ASSERT_EQ(EdgeSet(g.out_edges("b")),bout);
    ASSERT_EQ(EdgeSet(g.in_edges ("b")),bin);
    ASSERT_EQ(EdgeSet(g.out_edges("c")),cout);
    ASSERT_EQ(EdgeSet(g.in_edges ("c")),cin);
  }
}


TEST_F(GraphTest, add_edge2) {
  GraphType g;

  build_standard_graph(g);

  ASSERT_TRUE(g.has_node("a"));
  ASSERT_TRUE(g.has_node("b"));
  ASSERT_TRUE(g.has_node("c"));
  ASSERT_TRUE(g.has_node("d"));
  ASSERT_TRUE(g.has_node("e"));

  ASSERT_TRUE(g.has_edge("a","b"));
  ASSERT_TRUE(g.has_edge("a","c"));
  ASSERT_TRUE(g.has_edge("b","d"));
  ASSERT_TRUE(g.has_edge("c","d"));
  ASSERT_TRUE(g.has_edge("a","d"));
  ASSERT_TRUE(g.has_edge("d","a"));

  ASSERT_EQ(12,g.edge_value("a","b"));
  ASSERT_EQ(13,g.edge_value("a","c"));
  ASSERT_EQ(24,g.edge_value("b","d"));
  ASSERT_EQ(34,g.edge_value("c","d"));
  ASSERT_EQ(14,g.edge_value("a","d"));
  ASSERT_EQ(41,g.edge_value("d","a"));

  ASSERT_EQ(3,g.out_degree("a"));
  ASSERT_EQ(1,g.in_degree ("a"));
  ASSERT_EQ(4,g.degree    ("a"));
  ASSERT_EQ(1,g.out_degree("b"));
  ASSERT_EQ(1,g.in_degree ("b"));
  ASSERT_EQ(2,g.degree    ("b"));
  ASSERT_EQ(1,g.out_degree("c"));
  ASSERT_EQ(1,g.in_degree ("c"));
  ASSERT_EQ(2,g.degree    ("c"));
  ASSERT_EQ(1,g.out_degree("d"));
  ASSERT_EQ(3,g.in_degree ("d"));
  ASSERT_EQ(4,g.degree    ("d"));
  ASSERT_EQ(0,g.out_degree("e"));
  ASSERT_EQ(0,g.in_degree ("e"));
  ASSERT_EQ(0,g.degree    ("e"));

  ASSERT_EQ(5,g.node_count());
  ASSERT_EQ(6,g.edge_count());

  ASSERT_EQ(5, produces(g.all_nodes()) );
  ASSERT_EQ(6, produces(g.all_edges()) );


  {//node tests
    NodeSet aout; aout.insert("b"); aout.insert("c"); aout.insert("d");
    NodeSet ain;  ain.insert("d");
    NodeSet bout; bout.insert("d");
    NodeSet bin;  bin.insert("a");
    NodeSet cout; cout.insert("d");
    NodeSet cin;  cin.insert("a");
    NodeSet dout; dout.insert("a");
    NodeSet din;  din.insert("b"); din.insert("c"); din.insert("a");
    NodeSet eout;
    NodeSet ein;

    ASSERT_EQ(NodeSet(g.out_nodes("a")),aout);
    ASSERT_EQ(NodeSet(g.in_nodes ("a")),ain);
    ASSERT_EQ(NodeSet(g.out_nodes("b")),bout);
    ASSERT_EQ(NodeSet(g.in_nodes ("b")),bin);
    ASSERT_EQ(NodeSet(g.out_nodes("c")),cout);
    ASSERT_EQ(NodeSet(g.in_nodes ("c")),cin);
    ASSERT_EQ(NodeSet(g.out_nodes("d")),dout);
    ASSERT_EQ(NodeSet(g.in_nodes ("d")),din);
    ASSERT_EQ(NodeSet(g.out_nodes("e")),eout);
    ASSERT_EQ(NodeSet(g.in_nodes ("e")),ein);
  }

  {//edge tests
    EdgeSet aout; aout.insert(od("a","b")); aout.insert(od("a","c")); aout.insert(od("a","d"));
    EdgeSet ain;  ain.insert(od("d","a"));
    EdgeSet bout; bout.insert(od("b","d"));
    EdgeSet bin;  bin.insert(od("a","b"));
    EdgeSet cout; cout.insert(od("c","d"));
    EdgeSet cin;  cin.insert(od("a","c"));
    EdgeSet dout; dout.insert(od("d","a"));
    EdgeSet din;  din.insert(od("b","d")); din.insert(od("c","d")); din.insert(od("a","d"));
    EdgeSet eout;
    EdgeSet ein;

    ASSERT_EQ(EdgeSet(g.out_edges("a")),aout);
    ASSERT_EQ(EdgeSet(g.in_edges ("a")),ain);
    ASSERT_EQ(EdgeSet(g.out_edges("b")),bout);
    ASSERT_EQ(EdgeSet(g.in_edges ("b")),bin);
    ASSERT_EQ(EdgeSet(g.out_edges("c")),cout);
    ASSERT_EQ(EdgeSet(g.in_edges ("c")),cin);
    ASSERT_EQ(EdgeSet(g.out_edges("d")),dout);
    ASSERT_EQ(EdgeSet(g.in_edges ("d")),din);
    ASSERT_EQ(EdgeSet(g.out_edges("e")),eout);
    ASSERT_EQ(EdgeSet(g.in_edges ("e")),ein);
  }
}


TEST_F(GraphTest, remove_edge) {
  GraphType g;

  build_standard_graph(g);

  g.remove_edge("a","b");
  g.remove_edge("b","d");
  g.remove_edge("d","b");  //non-existant; just return

  ASSERT_TRUE(g.has_node("a"));
  ASSERT_TRUE(g.has_node("b"));
  ASSERT_TRUE(g.has_node("c"));
  ASSERT_TRUE(g.has_node("d"));
  ASSERT_TRUE(g.has_node("e"));

  ASSERT_TRUE(g.has_edge("a","c"));
  ASSERT_TRUE(g.has_edge("c","d"));
  ASSERT_TRUE(g.has_edge("a","d"));
  ASSERT_TRUE(g.has_edge("d","a"));

  ASSERT_EQ(13,g.edge_value("a","c"));
  ASSERT_EQ(34,g.edge_value("c","d"));
  ASSERT_EQ(14,g.edge_value("a","d"));
  ASSERT_EQ(41,g.edge_value("d","a"));

  ASSERT_EQ(2,g.out_degree("a"));
  ASSERT_EQ(1,g.in_degree ("a"));
  ASSERT_EQ(3,g.degree    ("a"));
  ASSERT_EQ(0,g.out_degree("b"));
  ASSERT_EQ(0,g.in_degree ("b"));
  ASSERT_EQ(0,g.degree    ("b"));
  ASSERT_EQ(1,g.out_degree("c"));
  ASSERT_EQ(1,g.in_degree ("c"));
  ASSERT_EQ(2,g.degree    ("c"));
  ASSERT_EQ(1,g.out_degree("d"));
  ASSERT_EQ(2,g.in_degree ("d"));
  ASSERT_EQ(3,g.degree    ("d"));
  ASSERT_EQ(0,g.out_degree("e"));
  ASSERT_EQ(0,g.in_degree ("e"));
  ASSERT_EQ(0,g.degree    ("e"));

  ASSERT_EQ(5,g.node_count());
  ASSERT_EQ(4,g.edge_count());

  ASSERT_EQ(5, produces(g.all_nodes()) );
  ASSERT_EQ(4, produces(g.all_edges()) );

  {//node tests
    NodeSet aout; aout.insert("c"); aout.insert("d");
    NodeSet ain;  ain.insert("d");
    NodeSet bout;
    NodeSet bin;
    NodeSet cout; cout.insert("d");
    NodeSet cin;  cin.insert("a");
    NodeSet dout; dout.insert("a");
    NodeSet din;  din.insert("c"); din.insert("a");
    NodeSet eout;
    NodeSet ein;

    ASSERT_EQ(NodeSet(g.out_nodes("a")),aout);
    ASSERT_EQ(NodeSet(g.in_nodes ("a")),ain);
    ASSERT_EQ(NodeSet(g.out_nodes("b")),bout);
    ASSERT_EQ(NodeSet(g.in_nodes ("b")),bin);
    ASSERT_EQ(NodeSet(g.out_nodes("c")),cout);
    ASSERT_EQ(NodeSet(g.in_nodes ("c")),cin);
    ASSERT_EQ(NodeSet(g.out_nodes("d")),dout);
    ASSERT_EQ(NodeSet(g.in_nodes ("d")),din);
    ASSERT_EQ(NodeSet(g.out_nodes("e")),eout);
    ASSERT_EQ(NodeSet(g.in_nodes ("e")),ein);
  }

  {//edge tests
    EdgeSet aout; aout.insert(od("a","c")); aout.insert(od("a","d"));
    EdgeSet ain;  ain.insert(od("d","a"));
    EdgeSet bout;
    EdgeSet bin;
    EdgeSet cout; cout.insert(od("c","d"));
    EdgeSet cin;  cin.insert(od("a","c"));
    EdgeSet dout; dout.insert(od("d","a"));
    EdgeSet din;  din.insert(od("c","d")); din.insert(od("a","d"));
    EdgeSet eout;
    EdgeSet ein;

    ASSERT_EQ(EdgeSet(g.out_edges("a")),aout);
    ASSERT_EQ(EdgeSet(g.in_edges ("a")),ain);
    ASSERT_EQ(EdgeSet(g.out_edges("b")),bout);
    ASSERT_EQ(EdgeSet(g.in_edges ("b")),bin);
    ASSERT_EQ(EdgeSet(g.out_edges("c")),cout);
    ASSERT_EQ(EdgeSet(g.in_edges ("c")),cin);
    ASSERT_EQ(EdgeSet(g.out_edges("d")),dout);
    ASSERT_EQ(EdgeSet(g.in_edges ("d")),din);
    ASSERT_EQ(EdgeSet(g.out_edges("e")),eout);
    ASSERT_EQ(EdgeSet(g.in_edges ("e")),ein);
  }
}


TEST_F(GraphTest, remove_node) {
  GraphType g;

  build_standard_graph(g);

  g.remove_node("d");

  ASSERT_TRUE(g.has_node("a"));
  ASSERT_TRUE(g.has_node("b"));
  ASSERT_TRUE(g.has_node("c"));
  ASSERT_TRUE(g.has_node("e"));

  ASSERT_TRUE(g.has_edge("a","b"));
  ASSERT_TRUE(g.has_edge("a","c"));

  ASSERT_EQ(12,g.edge_value("a","b"));
  ASSERT_EQ(13,g.edge_value("a","c"));

  ASSERT_EQ(2,g.out_degree("a"));
  ASSERT_EQ(0,g.in_degree ("a"));
  ASSERT_EQ(2,g.degree    ("a"));
  ASSERT_EQ(0,g.out_degree("b"));
  ASSERT_EQ(1,g.in_degree ("b"));
  ASSERT_EQ(1,g.degree    ("b"));
  ASSERT_EQ(0,g.out_degree("c"));
  ASSERT_EQ(1,g.in_degree ("c"));
  ASSERT_EQ(1,g.degree    ("c"));
  ASSERT_THROW(g.out_degree("d"),ics::GraphError);
  ASSERT_THROW(g.in_degree ("d"),ics::GraphError);
  ASSERT_THROW(g. degree   ("d"),ics::GraphError);
  ASSERT_EQ(0,g.out_degree("e"));
  ASSERT_EQ(0,g.in_degree ("e"));
  ASSERT_EQ(0,g.degree    ("e"));

  ASSERT_EQ(4,g.node_count());
  ASSERT_EQ(2,g.edge_count());

  ASSERT_EQ(4, produces(g.all_nodes()) );
  ASSERT_EQ(2, produces(g.all_edges()) );


  {//node tests
    NodeSet aout; aout.insert("b"); aout.insert("c");
    NodeSet ain;
    NodeSet bout;
    NodeSet bin;  bin.insert("a");
    NodeSet cout;
    NodeSet cin;  cin.insert("a");
    NodeSet eout;
    NodeSet ein;

    ASSERT_EQ(NodeSet(g.out_nodes("a")),aout);
    ASSERT_EQ(NodeSet(g.in_nodes ("a")),ain);
    ASSERT_EQ(NodeSet(g.out_nodes("b")),bout);
    ASSERT_EQ(NodeSet(g.in_nodes ("b")),bin);
    ASSERT_EQ(NodeSet(g.out_nodes("c")),cout);
    ASSERT_EQ(NodeSet(g.in_nodes ("c")),cin);
    ASSERT_THROW(NodeSet(g.out_nodes("d")),ics::GraphError);
    ASSERT_THROW(NodeSet(g.in_nodes("d")),ics::GraphError);
    ASSERT_EQ(NodeSet(g.out_nodes("e")),eout);
    ASSERT_EQ(NodeSet(g.in_nodes ("e")),ein);
  }

  {//edge tests
    EdgeSet aout; aout.insert(od("a","b")); aout.insert(od("a","c"));
    EdgeSet ain;
    EdgeSet bout;
    EdgeSet bin;  bin.insert(od("a","b"));
    EdgeSet cout;
    EdgeSet cin;  cin.insert(od("a","c"));
    EdgeSet eout;
    EdgeSet ein;

    ASSERT_EQ(EdgeSet(g.out_edges("a")),aout);
    ASSERT_EQ(EdgeSet(g.in_edges ("a")),ain);
    ASSERT_EQ(EdgeSet(g.out_edges("b")),bout);
    ASSERT_EQ(EdgeSet(g.in_edges ("b")),bin);
    ASSERT_EQ(EdgeSet(g.out_edges("c")),cout);
    ASSERT_EQ(EdgeSet(g.in_edges ("c")),cin);
    ASSERT_THROW(EdgeSet(g.out_edges("d")),ics::GraphError);
    ASSERT_THROW(EdgeSet(g.in_edges("d")),ics::GraphError);
    ASSERT_EQ(EdgeSet(g.out_edges("e")),eout);
    ASSERT_EQ(EdgeSet(g.in_edges ("e")),ein);
  }
}


TEST_F(GraphTest, relations) {
  GraphType g,g2;
  build_standard_graph(g);
  build_standard_graph(g2);

  ASSERT_EQ(g,g2);

  g.remove_edge("c","d");
  ASSERT_NE(g,g2);

  g2.remove_edge("c","d");
  ASSERT_EQ(g,g2);

  g.remove_node("d");
  ASSERT_NE(g,g2);

  g2.remove_node("d");
  ASSERT_EQ(g,g2);

  g2.remove_node("a");
  ASSERT_NE(g,g2);

  g.clear();
  g2.clear();
  ASSERT_EQ(g,g2);
}


TEST_F(GraphTest, load) {
  GraphType g,g2;
  build_standard_graph(g);
  std::ifstream in("standard.txt");
  g2.load(in,"/");
  in.close();
  ASSERT_EQ(g,g2);
}


TEST_F(GraphTest, store) {
  GraphType g,g2;
  build_standard_graph(g);
  std::ofstream out("store_test.txt");
  g.store(out,"/");
  out.close();

  std::ifstream in("store_test.txt");
  g2.load(in,"/");
  in.close();
  ASSERT_EQ(g,g2);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
