//#include <iostream>
//#include <sstream>
//#include <algorithm>                 // std::random_shuffle
//#include "ics46goody.hpp"
//#include "gtest/gtest.h"
//#include "array_priority_queue.hpp"  // must leave in for use in iterator_simple
//#include "array_queue.hpp"           // must leave in for use in iterator_erase
//#include "array_stack.hpp"           // must leave in for use in constructor
//#include "array_map.hpp"
//
//typedef ics::pair<std::string,int>     EntryType;
//typedef ics::ArrayMap<std::string,int> MapType;
//typedef ics::ArrayMap<int,int>         MapType2;
//
//int test_size  = ics::prompt_int ("Enter large scale test size");
//int trace      = ics::prompt_bool("Trace large scale test",false);
//int speed_size = ics::prompt_int ("Enter large scale speed test size");
//
//bool gt_Entry (const EntryType& a, const EntryType& b) {return a.first < b.first;}
//
//class MapTest : public ::testing::Test {
//protected:
//    virtual void SetUp()    {}
//    virtual void TearDown() {}
//};
//
//
//void load(MapType& m, std::string keys, int values[]) {
//  std::string* k = new std::string[keys.size()];
//  for (unsigned i=0; i<keys.size(); ++i)
//    k[i] = keys[i];
//  for (unsigned i=0; i<keys.size(); ++i)
//    m[k[i]] = values[i];
//  delete[] k;
//}
//
//
//::testing::AssertionResult mapsto(MapType& m, std::string keys, int values[]) {
//  std::string* k = new std::string[keys.size()];
//  for (unsigned i=0; i<keys.size(); ++i)
//    k[i] = keys[i];
//  for (unsigned i=0; i<keys.size(); ++i)
//    if (m[k[i]] != values[i])
//      return ::testing::AssertionFailure();
//  delete[] k;
//  return ::testing::AssertionSuccess();
//}
//
//
//::testing::AssertionResult not_has_keys(MapType& m, std::string keys) {
//  std::string* k = new std::string[keys.size()];
//  for (unsigned i=0; i<keys.size(); ++i)
//    k[i] = keys[i];
//  for (unsigned i=0; i<keys.size(); ++i)
//    if (m.has_key(k[i]))
//      return ::testing::AssertionFailure();
//  delete[] k;
//  return ::testing::AssertionSuccess();
//}
//
//
//::testing::AssertionResult not_has_values(MapType& m, int values[], int length) {
//  for (int i=0; i<length; ++i)
//    if (m.has_value(values[i]))
//      return ::testing::AssertionFailure();
//  return ::testing::AssertionSuccess();
//}
//
//
//
//TEST_F(MapTest, empty) {
//  MapType m;
//  ASSERT_TRUE(m.empty());
//}
//
//
//TEST_F(MapTest, size) {
//  MapType m;
//  ASSERT_EQ(0,m.size());
//}
//
//
//TEST_F(MapTest, has_key) {
//  MapType m;
//  ASSERT_FALSE(m.has_key("a"));
//}
//
//
//TEST_F(MapTest, has_value) {
//  MapType m;
//  ASSERT_FALSE(m.has_value(1));
//}
//
//
//TEST_F(MapTest, put) {
//  MapType m;
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  ASSERT_EQ(4,m.put("d",4));
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(1, m.size());
//  ASSERT_TRUE(mapsto(m,"d",new int[1]{4}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  ASSERT_EQ(1,m.put("a",1));
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(2, m.size());
//  ASSERT_TRUE(mapsto(m,"da",new int[2]{4,1}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  ASSERT_EQ(3,m.put("c",3));
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(3, m.size());
//  ASSERT_TRUE(mapsto(m,"dac",new int[3]{4,1,3}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  ASSERT_EQ(2,m.put("b",2));
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(4, m.size());
//  ASSERT_TRUE(mapsto(m,"dacb",new int[4]{4,1,3,2}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  ASSERT_EQ(6,m.put("f",6));
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(5, m.size());
//  ASSERT_TRUE(mapsto(m,"dacbf",new int[5]{4,1,3,2,6}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  ASSERT_EQ(5,m.put("e",5));
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(6, m.size());
//  ASSERT_TRUE(mapsto(m,"dacbfe",new int[6]{4,1,3,2,6,5}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  ASSERT_EQ(7,m.put("g",7));
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(7, m.size());
//  ASSERT_TRUE(mapsto(m,"dacbfeg",new int[7]{4,1,3,2,6,5,7}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  ASSERT_EQ(8,m.put("h",8));
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(8, m.size());
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{4,1,3,2,6,5,7,8}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  ASSERT_EQ(4,m.put("d",14));
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,1,3,2,6,5,7,8}));
//  ASSERT_EQ(8, m.size());
//
//  ASSERT_EQ(1,m.put("a",11));
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,3,2,6,5,7,8}));
//  ASSERT_EQ(8, m.size());
//
//  ASSERT_EQ(3,m.put("c",13));
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,13,2,6,5,7,8}));
//  ASSERT_EQ(8, m.size());
//
//  ASSERT_EQ(2,m.put("b",12));
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,13,12,6,5,7,8}));
//  ASSERT_EQ(8, m.size());
//
//  ASSERT_EQ(6,m.put("f",16));
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,13,12,16,5,7,8}));
//  ASSERT_EQ(8, m.size());
//
//  ASSERT_EQ(5,m.put("e",15));
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,13,12,16,15,7,8}));
//  ASSERT_EQ(8, m.size());
//
//  ASSERT_EQ(7,m.put("g",17));
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,13,12,16,15,17,8}));
//  ASSERT_EQ(8, m.size());
//
//  ASSERT_EQ(8,m.put("h",18));
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,13,12,16,15,17,18}));
//  ASSERT_EQ(8, m.size());
//}
//
//
//TEST_F(MapTest, put_index) {
//  MapType m;
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  m["d"] = 4;
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(1, m.size());
//  ASSERT_TRUE(mapsto(m,"d",new int[1]{4}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  m["a"] = 1;
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(2, m.size());
//  ASSERT_TRUE(mapsto(m,"da",new int[2]{4,1}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  m["c"] = 3;
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(3, m.size());
//  ASSERT_TRUE(mapsto(m,"dac",new int[3]{4,1,3}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  m["b"] = 2;
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(4, m.size());
//  ASSERT_TRUE(mapsto(m,"dacb",new int[4]{4,1,3,2}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  m["f"] = 6;
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(5, m.size());
//  ASSERT_TRUE(mapsto(m,"dacbf",new int[5]{4,1,3,2,6}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  m["e"] = 5;
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(6, m.size());
//  ASSERT_TRUE(mapsto(m,"dacbfe",new int[6]{4,1,3,2,6,5}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  m["g"] = 7;
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(7, m.size());
//  ASSERT_TRUE(mapsto(m,"dacbfeg",new int[7]{4,1,3,2,6,5,7}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  m["h"] = 8;
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(8, m.size());
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{4,1,3,2,6,5,7,8}));
//  ASSERT_FALSE(m.has_key("x"));
//  ASSERT_FALSE(m.has_value(100));
//
//  m["d"] = 14;
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,1,3,2,6,5,7,8}));
//  ASSERT_EQ(8, m.size());
//
//  m["a"] = 11;
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,3,2,6,5,7,8}));
//  ASSERT_EQ(8, m.size());
//
//  m["c"] = 13;
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,13,2,6,5,7,8}));
//  ASSERT_EQ(8, m.size());
//
//  m["b"] = 12;
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,13,12,6,5,7,8}));
//  ASSERT_EQ(8, m.size());
//
//  m["f"] = 16;
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,13,12,16,5,7,8}));
//  ASSERT_EQ(8, m.size());
//
//  m["e"] = 15;
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,13,12,16,15,7,8}));
//  ASSERT_EQ(8, m.size());
//
//  m["g"] = 17;
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,13,12,16,15,17,8}));
//  ASSERT_EQ(8, m.size());
//
//  m["h"] = 18;
//  ASSERT_TRUE(mapsto(m,"dacbfegh",new int[8]{14,11,13,12,16,15,17,18}));
//  ASSERT_EQ(8, m.size());
//}
//
//
//TEST_F(MapTest, operator_rel) {// == and !=
//  MapType m1, m2;
//  ASSERT_EQ   (m1,m2);
//  ASSERT_FALSE(m1 !=  m2);
//
//  m1.put("a",1);
//  ASSERT_NE   (m1,m2);
//  ASSERT_FALSE(m1 ==  m2);
//
//  m1.put("b",2);
//  ASSERT_NE   (m1,m2);
//  ASSERT_FALSE(m1 ==  m2);
//
//  m1.put("c",3);
//  ASSERT_NE   (m1,m2);
//  ASSERT_FALSE(m1 ==  m2);
//
//  m2.put("c",3);
//  ASSERT_NE   (m1,m2);
//  ASSERT_FALSE(m1 ==  m2);
//
//  m2.put("b",2);
//  ASSERT_NE   (m1,m2);
//  ASSERT_FALSE(m1 ==  m2);
//
//  m2.put("a",1);
//  ASSERT_EQ   (m1,m2);
//  ASSERT_FALSE(m1 !=  m2);
//
//  m2.put("d",4);
//  ASSERT_NE   (m1,m2);
//  ASSERT_FALSE(m1 ==  m2);
//
//  m2.put("e",5);
//  ASSERT_NE   (m1,m2);
//  ASSERT_FALSE(m1 ==  m2);
//
//  m1.put("e",5);
//  ASSERT_NE   (m1,m2);
//  ASSERT_FALSE(m1 ==  m2);
//
//  m1.put("d",4);
//  ASSERT_EQ   (m1,m2);
//  ASSERT_FALSE(m1 !=  m2);
//
//  m1.put("b",12);
//  ASSERT_NE   (m1,m2);
//  ASSERT_FALSE(m1 ==  m2);
//
//  m2.put("b",12);
//  ASSERT_EQ   (m1,m2);
//  ASSERT_FALSE(m1 !=  m2);
//
//  m1.erase("c");
//  ASSERT_NE   (m1,m2);
//  ASSERT_FALSE(m1 ==  m2);
//
//  m2.erase("c");
//  ASSERT_EQ   (m1,m2);
//  ASSERT_FALSE(m1 !=  m2);
//
//  ASSERT_EQ(m1,m1);
//  ASSERT_EQ(m2,m2);
//}
//
//
//TEST_F(MapTest, operator_stream_insert) {// <<
//  std::ostringstream value;
//  MapType m;
//  value << m;
//  ASSERT_EQ("map[]", value.str());
//
//  value.str("");
//  m.put("c",3);
//  value << m;
//  ASSERT_EQ("map[c->3]", value.str());
//
//  //Cannot further test: order not fixed
//}
//
//
//TEST_F(MapTest, put_all) {
//  MapType m,m1;
//  load(m1,"fcijbdegah", new int[10]{6,3,9,10,2,4,5,7,1,8});
//  ASSERT_EQ(10, m.put_all(m1));
//  ASSERT_FALSE(m.empty());
//  ASSERT_EQ(10, m.size());
//  ASSERT_EQ(m,m1);
//}
//
//
//TEST_F(MapTest, clear) {
//  MapType m;
//  m.clear();
//  ASSERT_TRUE(m.empty());
//  ASSERT_EQ(0, m.size());
//  ASSERT_FALSE(m.has_key("a"));
//  ASSERT_FALSE(m.has_value(1));
//
//  load(m,"a",new int[1]{1});
//  m.clear();
//  ASSERT_TRUE(m.empty());
//  ASSERT_EQ(0, m.size());
//  ASSERT_FALSE(m.has_key("a"));
//  ASSERT_FALSE(m.has_value(1));
//
//  load(m,"ab",new int [2]{1,2});
//  m.clear();
//  ASSERT_TRUE(m.empty());
//  ASSERT_EQ(0, m.size());
//  ASSERT_FALSE(m.has_key("a"));
//  ASSERT_FALSE(m.has_key("b"));
//  ASSERT_FALSE(m.has_value(1));
//  ASSERT_FALSE(m.has_value(2));
//
//  load(m,"bac",new int[3]{1,2,3});
//  m.clear();
//  ASSERT_TRUE(m.empty());
//  ASSERT_EQ(0, m.size());
//  ASSERT_FALSE(m.has_key("a"));
//  ASSERT_FALSE(m.has_key("b"));
//  ASSERT_FALSE(m.has_key("c"));
//  ASSERT_FALSE(m.has_value(1));
//  ASSERT_FALSE(m.has_value(2));
//  ASSERT_FALSE(m.has_value(3));
//
//  load(m,"dcba",new int[4]{1,2,3,4});
//  m.clear();
//  ASSERT_TRUE(m.empty());
//  ASSERT_EQ(0, m.size());
//  ASSERT_FALSE(m.has_key("a"));
//  ASSERT_FALSE(m.has_key("b"));
//  ASSERT_FALSE(m.has_key("c"));
//  ASSERT_FALSE(m.has_key("d"));
//  ASSERT_FALSE(m.has_value(1));
//  ASSERT_FALSE(m.has_value(2));
//  ASSERT_FALSE(m.has_value(3));
//  ASSERT_FALSE(m.has_value(4));
//
//  load(m,"bcead",new int[5]{1,2,3,4,5});
//  m.clear();
//  ASSERT_TRUE(m.empty());
//  ASSERT_EQ(0, m.size());
//  ASSERT_FALSE(m.has_key("a"));
//  ASSERT_FALSE(m.has_key("b"));
//  ASSERT_FALSE(m.has_key("c"));
//  ASSERT_FALSE(m.has_key("d"));
//  ASSERT_FALSE(m.has_key("e"));
//  ASSERT_FALSE(m.has_value(1));
//  ASSERT_FALSE(m.has_value(2));
//  ASSERT_FALSE(m.has_value(3));
//  ASSERT_FALSE(m.has_value(4));
//  ASSERT_FALSE(m.has_value(5));
//}
//
//
//TEST_F(MapTest, erase) {
//  MapType m;
//  load(m,"fcijbdegah", new int[10]{6,3,9,10,2,4,5,7,1,8});
//  ASSERT_EQ(1,m.erase("a"));
//  ASSERT_THROW(m.erase("x"),ics::KeyError);
//  ASSERT_THROW(m.erase("a"),ics::KeyError);
//  ASSERT_TRUE(mapsto(m,"fcijbdegh",new int[9]{6,3,9,10,2,4,5,7,8}));
//  ASSERT_TRUE(not_has_keys(m,"a"));
//  ASSERT_EQ(9,m.size());
//
//  ASSERT_EQ(2,m.erase("b"));
//  ASSERT_THROW(m.erase("x"),ics::KeyError);
//  ASSERT_THROW(m.erase("b"),ics::KeyError);
//  ASSERT_TRUE(mapsto(m,"fcijdegh",new int[8]{6,3,9,10,4,5,7,8}));
//  ASSERT_TRUE(not_has_keys(m,"ab"));
//  ASSERT_EQ(8,m.size());
//
//  ASSERT_EQ(3,m.erase("c"));
//  ASSERT_THROW(m.erase("x"),ics::KeyError);
//  ASSERT_THROW(m.erase("c"),ics::KeyError);
//  ASSERT_TRUE(mapsto(m,"fijdegh",new int[7]{6,9,10,4,5,7,8}));
//  ASSERT_TRUE(not_has_keys(m,"abc"));
//  ASSERT_EQ(7,m.size());
//
//  ASSERT_EQ(4,m.erase("d"));
//  ASSERT_THROW(m.erase("x"),ics::KeyError);
//  ASSERT_THROW(m.erase("d"),ics::KeyError);
//  ASSERT_TRUE(mapsto(m,"fijegh",new int[6]{6,9,10,5,7,8}));
//  ASSERT_TRUE(not_has_keys(m,"abcd"));
//  ASSERT_EQ(6,m.size());
//
//  ASSERT_EQ(5,m.erase("e"));
//  ASSERT_THROW(m.erase("x"),ics::KeyError);
//  ASSERT_THROW(m.erase("e"),ics::KeyError);
//  ASSERT_TRUE(mapsto(m,"fijgh",new int[5]{6,9,10,7,8}));
//  ASSERT_TRUE(not_has_keys(m,"abcde"));
//  ASSERT_EQ(5,m.size());
//
//  ASSERT_EQ(6,m.erase("f"));
//  ASSERT_THROW(m.erase("x"),ics::KeyError);
//  ASSERT_THROW(m.erase("f"),ics::KeyError);
//  ASSERT_TRUE(mapsto(m,"ijgh",new int[4]{9,10,7,8}));
//  ASSERT_TRUE(not_has_keys(m,"abcdef"));
//  ASSERT_EQ(4,m.size());
//
//  ASSERT_EQ(7,m.erase("g"));
//  ASSERT_THROW(m.erase("x"),ics::KeyError);
//  ASSERT_THROW(m.erase("f"),ics::KeyError);
//  ASSERT_TRUE(mapsto(m,"ijh",new int[3]{9,10,8}));
//  ASSERT_TRUE(not_has_keys(m,"abcdefg"));
//  ASSERT_EQ(3,m.size());
//
//  ASSERT_EQ(8,m.erase("h"));
//  ASSERT_THROW(m.erase("x"),ics::KeyError);
//  ASSERT_THROW(m.erase("h"),ics::KeyError);
//  ASSERT_TRUE(mapsto(m,"ij",new int[2]{9,10}));
//  ASSERT_TRUE(not_has_keys(m,"abcdefgh"));
//  ASSERT_EQ(2,m.size());
//
//  ASSERT_EQ(9,m.erase("i"));
//  ASSERT_THROW(m.erase("x"),ics::KeyError);
//  ASSERT_THROW(m.erase("i"),ics::KeyError);
//  ASSERT_TRUE(mapsto(m,"j",new int[1]{10}));
//  ASSERT_TRUE(not_has_keys(m,"abcdefghi"));
//  ASSERT_EQ(1,m.size());
//
//  ASSERT_EQ(10,m.erase("j"));
//  ASSERT_THROW(m.erase("x"),ics::KeyError);
//  ASSERT_THROW(m.erase("j"),ics::KeyError);
//  ASSERT_TRUE(not_has_keys(m,"abcdefghij"));
//  ASSERT_EQ(0,m.size());
//
//  ASSERT_TRUE(m.empty());
//}
//
//
//TEST_F(MapTest, assignment) {
//  MapType m1,m2;
//  load(m2,"fcijbde", new int[7]{6,3,9,10,2,4,5});
//  m1 = m2;
//  ASSERT_EQ(m1,m2);
//
//  m2.clear();
//  load(m2,"fde", new int[7]{6,2,4,5});
//  m1 = m2;
//  ASSERT_EQ(m1,m2);
//
//  m2.clear();
//  load(m2,"fcijbdegah", new int[10]{6,3,9,10,2,4,5,7,1,8});
//  m1 = m2;
//  ASSERT_EQ(m1,m2);
//}
//
//
//TEST_F(MapTest, iterator_plusplus) {
//  MapType m,m_iter;
//  load(m,"fcijbdegah", new int[10]{6,3,9,10,2,4,5,7,1,8});
//  MapType::Iterator end = m.end();
//
//  MapType::Iterator i(m.begin());
//  m_iter.put((*i).first,(*i).second);
//  for (int x=0; x<9; ++x) {
//    EntryType out1 = *(++i);
//    EntryType out2 = *i;
//    ASSERT_EQ(out1,out2);
//    m_iter.put(out1.first,out1.second);
//  }
//  ASSERT_EQ(end, ++i);
//  ASSERT_EQ(end, i);
//  ASSERT_EQ(end, ++i);
//  ASSERT_EQ(m,m_iter);
//
//  m_iter.clear();
//  MapType::Iterator j(m.begin());
//  for (int x=0; x<10; ++x) {
//    EntryType out1 = *j;
//    EntryType out2 = *(j++);
//    ASSERT_EQ(out1,out2);
//    m_iter.put(out1.first,out1.second);
//  }
//  ASSERT_EQ(end, j);
//  ASSERT_EQ(end, j++);
//  ASSERT_EQ(end, j);
//  ASSERT_EQ(end, j++);
//  ASSERT_EQ(m,m_iter);
//}
//
//
//TEST_F(MapTest, iterator_simple) {
//  std::string qkeys  [] ={"a","b","c","d","e","f","g","h","i","j"};
//  int         qvalues[] ={1,2,3,4,5,6,7,8,9,10};
//  ics::ArrayPriorityQueue<EntryType> q(gt_Entry);
//
//  MapType m;
//  load(m,"fcijbdegah", new int[10]{6,3,9,10,2,4,5,7,1,8});
//  MapType m2(m);
//
//  //for-each iterator (using .begin/.end)
//  for (EntryType x : m)
//    q.enqueue(x);
//  int i = 0;
//  for (EntryType x : q) {
//    ASSERT_EQ(qkeys[i],    x.first);
//    ASSERT_EQ(qvalues[i++],x.second);
//  }
//  ASSERT_EQ(10,m.size());
//
//  //explicit iterator (using .begin/.end and ++it)
//  q.clear();
//  for (MapType::Iterator it(m.begin()); it != m.end(); ++it)
//    q.enqueue(*it);
//  i = 0;
//  for (EntryType x : q) {
//    ASSERT_EQ(qkeys[i],    x.first);
//    ASSERT_EQ(qvalues[i++],x.second);
//  }
//  ASSERT_EQ(10,m.size());
//
//  //explicit iterator (using .begin/.end and it++)
//  q.clear();
//  for (MapType::Iterator it(m.begin()); it != m.end(); it++)
//    q.enqueue(*it);
//  i = 0;
//  for (EntryType x : q) {
//    ASSERT_EQ(qkeys[i],    x.first);
//    ASSERT_EQ(qvalues[i++],x.second);
//  }
//  ASSERT_EQ(10,m.size());
//
//  //all these iterations didn't change the set
//  ASSERT_EQ(m,m2);
//}
//
//
//TEST_F(MapTest, iterator_erase) {
//  ics::ArrayQueue<EntryType> erased;
//  MapType m;
//  load(m,"fcijbdegah", new int[10]{6,3,9,10,2,4,5,7,1,8});
//  MapType::Iterator it(m.begin());
//
//  erased.enqueue(it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it;
//
//  ++it;
//  erased.enqueue(it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it;
//  erased.enqueue(it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it;
//
//  ++it;
//  erased.enqueue(it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it;
//  ++it;
//
//  ++it;
//  erased.enqueue(it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//  ++it;
//  erased.enqueue(it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it;
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  MapType m2;
//  ASSERT_EQ(6,m2.put_all(erased));
//  ASSERT_EQ(6,m2.size());
//  for (EntryType x : m2)
//    ASSERT_FALSE(m.has_key(x.first));
//  for (EntryType x : m)
//    ASSERT_FALSE(m2.has_key(x.first));
//
//
//  //erase all in the set
//  m.clear();
//  load(m,"fcijbdegah", new int[10]{6,3,9,10,2,4,5,7,1,8});
//  for (MapType::Iterator it(m.begin()); it != m.end(); ++it) {
//    std::string k  = (*it).first;
//    int         v  = (*it).second;
//    ASSERT_EQ(m[k], v);
//    EntryType kv = it.erase();
//    ASSERT_EQ(k,kv.first);
//    ASSERT_EQ(v,kv.second);
//    ASSERT_FALSE(m.has_key(k));
//    ASSERT_FALSE(m.has_value(v));
//  }
//  ASSERT_TRUE(m.empty());
//  ASSERT_EQ(0,m.size());
//}
//
//
//TEST_F(MapTest, iterator_exception_concurrent_modification_error) {
//  MapType m;
//  load(m,"fcijbdegah", new int[10]{6,3,9,10,2,4,5,7,1,8});
//  MapType::Iterator it(m.begin());
//
//  m.erase("a");
//  ASSERT_THROW(it.erase(),ics::ConcurrentModificationError);
//  ASSERT_THROW(++it,ics::ConcurrentModificationError);
//  ASSERT_THROW(it++,ics::ConcurrentModificationError);
//  ASSERT_THROW(*it,ics::ConcurrentModificationError);
//}
//
//
//TEST_F(MapTest, constructors) {
//  //default
//  MapType m;
//  load(m,"fcijbdegah", new int[10]{6,3,9,10,2,4,5,7,1,8});
//  ASSERT_TRUE(mapsto(m,"fcijbdegah",new int[10]{6,3,9,10,2,4,5,7,1,8}));
//
//  //copy
//  m.clear();
//  load(m,"fcijbdegah", new int[10]{6,3,9,10,2,4,5,7,1,8});
//  MapType m2(m);
//  ASSERT_TRUE(mapsto(m,"fcijbdegah",new int[10]{6,3,9,10,2,4,5,7,1,8}));
//  ASSERT_EQ(m,m2);
//  m.clear();
//  ASSERT_NE(m,m2);
//
//  //initializer
//  load(m,"fcijbdegah", new int[10]{6,3,9,10,2,4,5,7,1,8});
//  MapType m3({EntryType("f",6), EntryType("c",3), EntryType("i",9),
//                EntryType("j",10),EntryType("b",2), EntryType("d",4),
//                EntryType("e",5), EntryType("g",7), EntryType("a",1),
//                EntryType("h",8)});
//  ASSERT_TRUE(mapsto(m,"fcijbdegah",new int[10]{6,3,9,10,2,4,5,7,1,8}));
//  ASSERT_EQ(m,m3);
//  m.clear();
//  ASSERT_NE(m,m3);
//
//  //iterable
//  ics::ArrayStack<EntryType> sa({EntryType("f",6), EntryType("c",3), EntryType("i",9),
//    EntryType("j",10),EntryType("b",2), EntryType("d",4),
//    EntryType("e",5), EntryType("g",7), EntryType("a",1),
//    EntryType("h",8)});
//  MapType m4(sa);
//  ASSERT_TRUE(mapsto(m4,"fcijbdegah",new int[10]{6,3,9,10,2,4,5,7,1,8}));
//}
//
//
//TEST_F(MapTest, large_scale) {
//  MapType2 lm;
//
//  std::vector<int> values;
//  for (int i=0; i<test_size; ++i)
//    values.push_back(i);
//  std::random_shuffle(values.begin(),values.end());
//
//
//  for (int test=1; test<=5; ++test) {
//    int inserted = 0;
//    int erased   = 0;
//    while (erased != test_size) {
//      int to_insert = ics::rand_range(0,test_size-inserted);
//      if (trace)
//        std::cout << "Inserted " << to_insert << std::endl;
//      for (int i=0; i <to_insert; ++i) {
//        ASSERT_EQ(inserted,lm.put(values[inserted],inserted));
//        ASSERT_TRUE(lm.has_key(values[inserted]));
//        ASSERT_EQ(inserted,lm[values[inserted]]);
//        ++inserted;
//      };
//
//      int to_erase = ics::rand_range(0,inserted-erased);
//      if (trace)
//        std::cout << "Erased " << to_erase << std::endl;
//      for (int i=0; i <to_erase; ++i) {
//        ASSERT_EQ(erased,lm.erase(values[erased]));
//        ASSERT_FALSE(lm.has_key(values[erased]));
//        ++erased;
//      }
//    }
//  }
//  ASSERT_TRUE(lm.empty());
//  ASSERT_EQ(0,lm.size());
//
//}
//
//
//TEST_F(MapTest, large_scale_speed) {
//  MapType2 lm;
//
//  std::vector<int> values;
//  for (int i=0; i<speed_size; ++i)
//    values.push_back(i);
//  std::random_shuffle(values.begin(),values.end());
//
//  for (int test=1; test<=5; ++test) {
//    int inserted = 0;
//    int erased   = 0;
//    while (erased != speed_size) {
//      int to_insert = ics::rand_range(0,speed_size-inserted);
//      for (int i=0; i <to_insert; ++i) {
//        lm.put(values[inserted],inserted);
//        ++inserted;
//      }
//
//      int to_erase = ics::rand_range(0,inserted-erased);
//      for (int i=0; i <to_erase; ++i) {
//        lm.erase(values[erased]);
//        ++erased;
//      }
//    }
//  }
//}
//
//
//int main(int argc, char **argv) {
//  ::testing::InitGoogleTest(&argc, argv);
//  return RUN_ALL_TESTS();
//}
