//#include <iostream>
//#include <sstream>
//#include <algorithm>                 // std::random_shuffle
//#include "ics46goody.hpp"
//#include "gtest/gtest.h"
//#include "array_stack.hpp"           // must leave in for constructor
//#include "array_set.hpp"             // must leave in when testing other kinds of sets
//#include "array_set.hpp"
//
//typedef ics::ArraySet<std::string> SetTypeStr;
//typedef ics::ArraySet<int>         SetTypeInt;
//
//int test_size  = ics::prompt_int ("Enter large scale test size");
//int trace      = ics::prompt_bool("Trace large scale test",false);
//int speed_size = ics::prompt_int ("Enter large scale speed test size");
//
//
//class SetTest : public ::testing::Test {
//protected:
//    virtual void SetUp()    {}
//    virtual void TearDown() {}
//};
//
//
//void load(SetTypeStr& s, std::string values) {
//  std::string* v = new std::string[values.size()];
//  for (unsigned i=0; i<values.size(); ++i)
//    v[i] = values[i];
//  for (unsigned i=0; i<values.size(); ++i)
//    s.insert(v[i]);
//  //delete[] v;
//}
//
//
//::testing::AssertionResult contains(SetTypeStr& s, std::string values) {
//  if (s.size() != int(values.size()))
//    return ::testing::AssertionFailure();
//  std::string* v = new std::string[values.size()];
//  for (unsigned i=0; i<values.size(); ++i)
//    v[i] = values[i];
//  for (unsigned i=0; i<values.size(); ++i)
//    if (!s.contains(v[i]))
//      return ::testing::AssertionFailure();
//  delete[] v;
//  return ::testing::AssertionSuccess();
//}
//
//
//::testing::AssertionResult not_contains(SetTypeStr& s, std::string values) {
//  std::string* v = new std::string[values.size()];
//  for (unsigned i=0; i<values.size(); ++i)
//    v[i] = values[i];
//  for (unsigned i=0; i<values.size(); ++i)
//    if (s.contains(v[i]))
//      return ::testing::AssertionFailure();
//  delete[] v;
//  return ::testing::AssertionSuccess();
//}
//
//
//
//TEST_F(SetTest, empty) {
//  SetTypeStr s;
//  ASSERT_TRUE(s.empty());
//}
//
//
//TEST_F(SetTest, size) {
//  SetTypeStr s;
//  ASSERT_EQ(0,s.size());
//}
//
//
//TEST_F(SetTest, contains) {
//  SetTypeStr s;
//  ASSERT_FALSE(s.contains("a"));
//}
//
//TEST_F(SetTest, insert) {
//  SetTypeStr s;
//  ASSERT_FALSE(s.contains("x"));
//
//  ASSERT_EQ(1,s.insert("a"));
//  ASSERT_FALSE(s.empty());
//  ASSERT_EQ(1, s.size());
//  ASSERT_TRUE(contains(s,"a"));
//  ASSERT_FALSE(s.contains("x"));
//
//  ASSERT_EQ(1,s.insert("b"));
//  ASSERT_FALSE(s.empty());
//  ASSERT_EQ(2, s.size());
//  ASSERT_TRUE(contains(s,"ab"));
//  ASSERT_FALSE(s.contains("x"));
//
//  ASSERT_EQ(1,s.insert("c"));
//  ASSERT_FALSE(s.empty());
//  ASSERT_EQ(3, s.size());
//  ASSERT_TRUE(contains(s,"abc"));
//  ASSERT_FALSE(s.contains("x"));
//
//  ASSERT_EQ(1,s.insert("d"));
//  ASSERT_FALSE(s.empty());
//  ASSERT_EQ(4, s.size());
//  ASSERT_TRUE(contains(s,"abcd"));
//  ASSERT_FALSE(s.contains("x"));
//
//  ASSERT_EQ(1,s.insert("e"));
//  ASSERT_FALSE(s.empty());
//  ASSERT_EQ(5, s.size());
//  ASSERT_TRUE(contains(s,"abcde"));
//  ASSERT_FALSE(s.contains("x"));
//
//  ASSERT_EQ(1,s.insert("f"));
//  ASSERT_FALSE(s.empty());
//  ASSERT_EQ(6, s.size());
//  ASSERT_TRUE(contains(s,"abcdef"));
//  ASSERT_FALSE(s.contains("x"));
//
//  ASSERT_EQ(1,s.insert("g"));
//  ASSERT_FALSE(s.empty());
//  ASSERT_EQ(7, s.size());
//  ASSERT_TRUE(contains(s,"abcdefg"));
//  ASSERT_FALSE(s.contains("x"));
//
//  ASSERT_EQ(1,s.insert("h"));
//  ASSERT_FALSE(s.empty());
//  ASSERT_EQ(8, s.size());
//  ASSERT_TRUE(contains(s,"abcdefgh"));
//  ASSERT_FALSE(s.contains("x"));
//
//  ASSERT_EQ(0,s.insert("a"));
//  ASSERT_EQ(8, s.size());
//
//  ASSERT_EQ(0,s.insert("b"));
//  ASSERT_EQ(8, s.size());
//
//  ASSERT_EQ(0,s.insert("c"));
//  ASSERT_EQ(8, s.size());
//
//  ASSERT_EQ(0,s.insert("d"));
//  ASSERT_EQ(8, s.size());
//
//  ASSERT_EQ(0,s.insert("e"));
//  ASSERT_EQ(8, s.size());
//
//  ASSERT_EQ(0,s.insert("f"));
//  ASSERT_EQ(8, s.size());
//
//  ASSERT_EQ(0,s.insert("g"));
//  ASSERT_EQ(8, s.size());
//
//  ASSERT_EQ(0,s.insert("h"));
//  ASSERT_EQ(8, s.size());
//}
//
//
//TEST_F(SetTest, operator_rel) {// == and != and (strict) subset
//  SetTypeStr s1,s2;
//  ASSERT_EQ   (s1,s2);
//  ASSERT_FALSE(s1 != s2);
//  ASSERT_FALSE(s1 <  s2);
//  ASSERT_TRUE (s1 <= s2);
//  ASSERT_TRUE (s1 >= s2);
//  ASSERT_FALSE(s1 >  s2);
//
//  s1.insert("a");
//  ASSERT_NE   (s1,s2);
//  ASSERT_FALSE(s1 == s2);
//  ASSERT_FALSE(s1 <  s2);
//  ASSERT_FALSE(s1 <= s2);
//  ASSERT_TRUE (s1 >= s2);
//  ASSERT_TRUE (s1 >  s2);
//
//  s1.insert("b");
//  ASSERT_NE   (s1,s2);
//  ASSERT_FALSE(s1 == s2);
//  ASSERT_FALSE(s1 <  s2);
//  ASSERT_FALSE(s1 <= s2);
//  ASSERT_TRUE (s1 >= s2);
//  ASSERT_TRUE (s1 >  s2);
//
//  s1.insert("c");
//  ASSERT_NE   (s1,s2);
//  ASSERT_FALSE(s1 == s2);
//  ASSERT_FALSE(s1 <  s2);
//  ASSERT_FALSE(s1 <= s2);
//  ASSERT_TRUE (s1 >= s2);
//  ASSERT_TRUE (s1 >  s2);
//
//  s2.insert("c");
//  ASSERT_NE   (s1,s2);
//  ASSERT_FALSE(s1 == s2);
//  ASSERT_FALSE(s1 <  s2);
//  ASSERT_FALSE(s1 <= s2);
//  ASSERT_TRUE (s1 >= s2);
//  ASSERT_TRUE (s1 >  s2);
//
//  s2.insert("b");
//  ASSERT_NE   (s1,s2);
//  ASSERT_FALSE(s1 == s2);
//  ASSERT_FALSE(s1 <  s2);
//  ASSERT_FALSE(s1 <= s2);
//  ASSERT_TRUE (s1 >= s2);
//  ASSERT_TRUE (s1 >  s2);
//
//  s2.insert("a");
//  ASSERT_EQ   (s1,s2);
//  ASSERT_FALSE(s1 != s2);
//  ASSERT_FALSE(s1 <  s2);
//  ASSERT_TRUE (s1 <= s2);
//  ASSERT_TRUE (s1 >= s2);
//  ASSERT_FALSE(s1 >  s2);
//
//  s2.insert("d");
//  ASSERT_NE   (s1,s2);
//  ASSERT_FALSE(s1 == s2);
//  ASSERT_TRUE (s1 <  s2);
//  ASSERT_TRUE (s1 <= s2);
//  ASSERT_FALSE(s1 >= s2);
//  ASSERT_FALSE(s1 >  s2);
//
//  s2.insert("e");
//  ASSERT_NE   (s1,s2);
//  ASSERT_FALSE(s1 == s2);
//  ASSERT_TRUE (s1 <  s2);
//  ASSERT_TRUE (s1 <= s2);
//  ASSERT_FALSE(s1 >= s2);
//  ASSERT_FALSE(s1 >  s2);
//
//  s1.insert("e");
//  ASSERT_NE   (s1,s2);
//  ASSERT_FALSE(s1 == s2);
//  ASSERT_TRUE (s1 <  s2);
//  ASSERT_TRUE (s1 <= s2);
//  ASSERT_FALSE(s1 >= s2);
//  ASSERT_FALSE(s1 >  s2);
//
//  s1.insert("d");
//  ASSERT_EQ   (s1,s2);
//  ASSERT_FALSE(s1 != s2);
//  ASSERT_FALSE(s1 <  s2);
//  ASSERT_TRUE (s1 <= s2);
//  ASSERT_TRUE (s1 >= s2);
//  ASSERT_FALSE(s1 >  s2);
//
//  s1.erase("c");
//  ASSERT_NE   (s1,s2);
//  ASSERT_FALSE(s1 == s2);
//  ASSERT_TRUE (s1 <  s2);
//  ASSERT_TRUE (s1 <= s2);
//  ASSERT_FALSE(s1 >= s2);
//  ASSERT_FALSE(s1 >  s2);
//
//  s2.erase("c");
//  ASSERT_EQ   (s1,s2);
//  ASSERT_FALSE(s1 != s2);
//  ASSERT_FALSE(s1 <  s2);
//  ASSERT_TRUE (s1 <= s2);
//  ASSERT_TRUE (s1 >= s2);
//  ASSERT_FALSE(s1 >  s2);
//
//  ASSERT_EQ(s1,s1);
//  ASSERT_EQ(s2,s2);
//}
//
//
//TEST_F(SetTest, operator_stream_insert) {// <<
//  std::ostringstream value;
//  SetTypeStr s;
//  value << s;
//  ASSERT_EQ("set[]", value.str());
//
//  value.str("");
//  s.insert("c");
//  value << s;
//  ASSERT_EQ("set[c]", value.str());
//
//  //Cannot further test: order not fixed
//}
//
//
//TEST_F(SetTest, insert_all) {
//  SetTypeStr s,s1;
//  load(s1,"abcdefghij");
//  s.insert_all(s1);
//  ASSERT_FALSE(s.empty());
//  ASSERT_EQ(10, s.size());
//  ASSERT_EQ(s,s1);
//}
//
//
//TEST_F(SetTest, contains_all) {
//  SetTypeStr s,s1,s2;
//  load(s,"abcdefghij");
//  load(s1,"abdij");
//  load(s2,"abdxij");
//  ASSERT_TRUE(s.contains_all(s1));
//  ASSERT_FALSE(s.contains_all(s2));
//}
//
//
//TEST_F(SetTest, clear) {
//  SetTypeStr s;
//  s.clear();
//  ASSERT_TRUE(s.empty());
//  ASSERT_EQ(0, s.size());
//  ASSERT_FALSE(s.contains("a"));
//
//  load(s,"a");
//  s.clear();
//  ASSERT_TRUE(s.empty());
//  ASSERT_EQ(0, s.size());
//  ASSERT_FALSE(s.contains("a"));
//
//  load(s,"ab");
//  s.clear();
//  ASSERT_TRUE(s.empty());
//  ASSERT_EQ(0, s.size());
//  ASSERT_FALSE(s.contains("a"));
//  ASSERT_FALSE(s.contains("b"));
//
//  load(s,"bac");
//  s.clear();
//  ASSERT_TRUE(s.empty());
//  ASSERT_EQ(0, s.size());
//  ASSERT_FALSE(s.contains("a"));
//  ASSERT_FALSE(s.contains("b"));
//  ASSERT_FALSE(s.contains("c"));
//
//  load(s,"dcba");
//  s.clear();
//  ASSERT_TRUE(s.empty());
//  ASSERT_EQ(0, s.size());
//  ASSERT_FALSE(s.contains("a"));
//  ASSERT_FALSE(s.contains("b"));
//  ASSERT_FALSE(s.contains("c"));
//  ASSERT_FALSE(s.contains("d"));
//
//  load(s,"bcead");
//  s.clear();
//  ASSERT_TRUE(s.empty());
//  ASSERT_EQ(0, s.size());
//  ASSERT_FALSE(s.contains("a"));
//  ASSERT_FALSE(s.contains("b"));
//  ASSERT_FALSE(s.contains("c"));
//  ASSERT_FALSE(s.contains("d"));
//  ASSERT_FALSE(s.contains("e"));
//}
//
//
//TEST_F(SetTest, erase) {
//  SetTypeStr s;
//  load(s,"fcijbdegah");
//  ASSERT_EQ(1,s.erase("a"));
//  ASSERT_EQ(0,s.erase("x"));
//  ASSERT_TRUE(contains(s,"bcdefghij"));
//  ASSERT_TRUE(not_contains(s,"a"));
//  ASSERT_EQ(0,s.erase("a"));
//
//  ASSERT_EQ(1,s.erase("b"));
//  ASSERT_EQ(0,s.erase("x"));
//  ASSERT_TRUE(contains(s,"cdefghij"));
//  ASSERT_TRUE(not_contains(s,"ab"));
//  ASSERT_EQ(0,s.erase("b"));
//
//  ASSERT_EQ(1,s.erase("c"));
//  ASSERT_EQ(0,s.erase("x"));
//  ASSERT_TRUE(contains(s,"defghij"));
//  ASSERT_TRUE(not_contains(s,"abc"));
//  ASSERT_EQ(0,s.erase("c"));
//
//  ASSERT_EQ(1,s.erase("d"));
//  ASSERT_EQ(0,s.erase("x"));
//  ASSERT_TRUE(contains(s,"efghij"));
//  ASSERT_TRUE(not_contains(s,"abcd"));
//  ASSERT_EQ(0,s.erase("d"));
//
//  ASSERT_EQ(1,s.erase("e"));
//  ASSERT_EQ(0,s.erase("x"));
//  ASSERT_TRUE(contains(s,"fghij"));
//  ASSERT_TRUE(not_contains(s,"abcde"));
//  ASSERT_EQ(0,s.erase("e"));
//
//  ASSERT_EQ(1,s.erase("f"));
//  ASSERT_EQ(0,s.erase("x"));
//  ASSERT_TRUE(contains(s,"ghij"));
//  ASSERT_TRUE(not_contains(s,"abcdef"));
//  ASSERT_EQ(0,s.erase("f"));
//
//  ASSERT_EQ(1,s.erase("g"));
//  ASSERT_EQ(0,s.erase("x"));
//  ASSERT_TRUE(contains(s,"hij"));
//  ASSERT_TRUE(not_contains(s,"abcdefg"));
//  ASSERT_EQ(0,s.erase("g"));
//
//  ASSERT_EQ(1,s.erase("h"));
//  ASSERT_EQ(0,s.erase("x"));
//  ASSERT_TRUE(contains(s,"ij"));
//  ASSERT_TRUE(not_contains(s,"abcdefgh"));
//  ASSERT_EQ(0,s.erase("h"));
//
//  ASSERT_EQ(1,s.erase("i"));
//  ASSERT_EQ(0,s.erase("x"));
//  ASSERT_TRUE(contains(s,"j"));
//  ASSERT_TRUE(not_contains(s,"abcdefghi"));
//  ASSERT_EQ(0,s.erase("i"));
//
//  ASSERT_EQ(1,s.erase("j"));
//  ASSERT_EQ(0,s.erase("x"));
//  ASSERT_TRUE(not_contains(s,"abcdefghij"));
//  ASSERT_EQ(0,s.erase("j"));
//
//  ASSERT_EQ(0,s.erase("a"));
//  ASSERT_EQ(0,s.erase("e"));
//  ASSERT_EQ(0,s.erase("j"));
//
//  ASSERT_TRUE(s.empty());
//  ASSERT_EQ(0, s.size());
//}
//
//
//TEST_F(SetTest, erase_all) {
//  SetTypeStr s,s1,s2;
//  load(s,"abcdefghij");
//  load(s1,"abdij");
//  ASSERT_EQ(5,s.erase_all(s1));
//  ASSERT_FALSE(s.empty());
//  ASSERT_EQ(5, s.size());
//  load(s2,"cefgh");
//  ASSERT_EQ(s,s2);
//}
//
//
//TEST_F(SetTest, retain_all) {
//  SetTypeStr s,s1;
//  load(s,"abcdefghij");
//  load(s1,"abdij");
//  s.retain_all(s1);
//  ASSERT_EQ(s,s1);
//
//  s.clear();
//  s1.clear();
//  load(s,"abdij");
//  SetTypeStr s2(s);
//  load(s1,"abcdefghij");
//  s.retain_all(s1);
//  ASSERT_EQ(s,s2);
//
//  SetTypeStr s3;
//  s.retain_all(s3);
//  ASSERT_EQ(s,s3);
//}
//
//
//TEST_F(SetTest, assignment) {
//  SetTypeStr s1,s2;
//  load(s2,"abcde");
//  s1 = s2;
//  ASSERT_EQ(s1,s2);
//
//  s2.clear();
//  load(s2,"ab");
//  s1 = s2;
//  ASSERT_EQ(s1,s2);
//
//  s2.clear();
//  load(s2,"abcdefghij");
//  s1 = s2;
//  ASSERT_EQ(s1,s2);
//
//}
//
//
//TEST_F(SetTest, iterator_plusplus) {
//  SetTypeStr s,s_iter;
//  load(s,"abcde");
//  SetTypeStr::Iterator end = s.end();
//
//  SetTypeStr::Iterator i(s.begin());
//  s_iter.insert(*i);
//  for (int x=0; x<4; ++x) {
//    std::string out1 = *(++i);
//    std::string out2 = *i;
//    ASSERT_EQ(out1,out2);
//    s_iter.insert(out1);
//  }
//  ASSERT_EQ(end, ++i);
//  ASSERT_EQ(end, i);
//  ASSERT_EQ(end, ++i);
//  ASSERT_EQ(s,s_iter);
//
//  s_iter.clear();
//  SetTypeStr::Iterator j(s.begin());
//  for (int x=0; x<5; ++x) {
//    std::string out1 = *j;
//    std::string out2 = *(j++);
//    ASSERT_EQ(out1,out2);
//    s_iter.insert(out1);
//  }
//  ASSERT_EQ(end, j);
//  ASSERT_EQ(end, j++);
//  ASSERT_EQ(end, j);
//  ASSERT_EQ(end, j++);
//  ASSERT_EQ(s,s_iter);
//}
//
//
//TEST_F(SetTest, iterator_simple) {
//  std::string values[] ={"a","b","c","d","e","f","g","h","i","j"};
//  std::string seen  [] ={"?","?","?","?","?","?","?","?","?","?"};
//
//  SetTypeStr s;
//  load(s,"fcijbdegah");
//  SetTypeStr s2(s);
//
//  //for-each iterator (using .begin/.end)
//  int i = 0;
//  for (std::string x : s)
//    seen[i++] = x;
//  std::sort(seen,seen+10);
//  for (int j=0; j<10; ++j)
//    ASSERT_EQ(values[j],seen[j]);
//  ASSERT_EQ(10,s.size());
//
//  //explicit iterator (using .begin/.end and ++it)
//  i = 0;
//  for (SetTypeStr::Iterator it(s.begin()); it != s.end(); ++it)
//    seen[i++] = *it;
//  std::sort(seen,seen+10);
//  for (int j=0; j<10; ++j)
//    ASSERT_EQ(values[j],seen[j]);
//  ASSERT_EQ(10,s.size());
//
//  //explicit iterator (using .begin/.end and it++)
//  i = 0;
//  for (SetTypeStr::Iterator it(s.begin()); it != s.end(); it++)
//    seen[i++] = *it;
//  std::sort(seen,seen+10);
//  for (int j=0; j<10; ++j)
//    ASSERT_EQ(values[j],seen[j]);
//  ASSERT_EQ(10,s.size());
//
//  //all these iterations didn't change the set
//  ASSERT_EQ(s,s2);
//}
//
//
//TEST_F(SetTest, iterator_erase) {
//  std::vector<std::string> erased;
//  SetTypeStr s;
//  load(s,"abcdefghihj");
//  SetTypeStr::Iterator it(s.begin());
//
//  erased.push_back(it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it;
//
//  ++it;
//  erased.push_back(it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//  ++it;
//  erased.push_back(it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it;
//
//  ++it;
//  erased.push_back(it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it;
//  ++it;
//
//  ++it;
//  erased.push_back(it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//  ++it;
//  erased.push_back(it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it;
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  SetTypeStr s2;
//  load(s2,ics::join(erased));
//  ASSERT_EQ(6,s2.size());
//  for (std::string x : s2)
//    ASSERT_FALSE(s.contains(x));
//  for (std::string x : s)
//    ASSERT_FALSE(s2.contains(x));
//
//
//  //erase all in the set
//  s.clear();
//  load(s,"abcdefghihj");
//  for (SetTypeStr::Iterator it(s.begin()); it != s.end(); ++it)
//    ASSERT_FALSE(s.contains(it.erase()));
//  ASSERT_TRUE(s.empty());
//  ASSERT_EQ(0,s.size());
//}
//
//
//TEST_F(SetTest, iterator_exception_concurrent_modification_error) {
//  SetTypeStr s;
//  load(s,"fcijbdegabh");
//  SetTypeStr::Iterator it(s.begin());
//
//  s.erase("a");
//  ASSERT_THROW(it.erase(),ics::ConcurrentModificationError);
//  ASSERT_THROW(++it,ics::ConcurrentModificationError);
//  ASSERT_THROW(it++,ics::ConcurrentModificationError);
//  ASSERT_THROW(*it,ics::ConcurrentModificationError);
//}
//
//
//TEST_F(SetTest, constructors) {
//  //default
//  SetTypeStr s;
//  load(s,"fcifjbdaegah");
//  ASSERT_TRUE(contains(s,"abcdefghij"));
//
//  //copy
//  load(s,"fcifjbdaegah");
//  SetTypeStr s2(s);
//  ASSERT_TRUE(contains(s2,"abcdefghij"));
//  ASSERT_EQ(s,s2);
//  s.clear();
//  ASSERT_NE(s,s2);
//
//  //initializer
//  load(s,"fcifjbdaegah");
//  SetTypeStr s3({"f", "c", "i", "f", "j", "b", "d", "a", "e", "g", "a", "h"});
//  ASSERT_TRUE(contains(s3,"abcdefghij"));
//  ASSERT_EQ(s,s3);
//  s.clear();
//  ASSERT_NE(s,s3);
//
//  //iterator
//  ics::ArrayStack<std::string> sa({"f", "c", "i", "j", "b", "d", "e", "g", "a", "h"});
//  SetTypeStr s4(sa);
//  ASSERT_TRUE(contains(s4,"abcdefghij"));
//}
//
//
//TEST_F(SetTest, large_scale) {
//  SetTypeInt ls;
//  ics::ArraySet<int> ls_ref;
//
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
//        ls_ref.insert(values[inserted]);
//        ASSERT_EQ(1,ls.insert(values[inserted++]));
//      };
//      ASSERT_EQ(ls,ls_ref);
//
//      int to_erase = ics::rand_range(0,inserted-erased);
//      if (trace)
//        std::cout << "Erased " << to_erase << std::endl;
//      for (int i=0; i <to_erase; ++i) {
//        ASSERT_EQ(1,ls.erase(values[erased]));
//        ls_ref.erase(values[erased]);
//        ++erased;
//      }
//      ASSERT_EQ(ls,ls_ref);
//    }
//  }
//  ASSERT_TRUE(ls.empty());
//  ASSERT_EQ(0,ls.size());
//
//}
//
//
//TEST_F(SetTest, large_scale_speed) {
//  SetTypeInt ls;
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
//      for (int i=0; i <to_insert; ++i)
//        ls.insert(values[inserted++] );
//      for (int v : ls)
//        ;
//
//      int to_erase = ics::rand_range(0,inserted-erased);
//      for (int i=0; i <to_erase; ++i)
//        ls.erase(values[erased++]);
//      for (int v : ls)
//        ;
//    }
//  }
//}
//
//
//int main(int argc, char **argv) {
//  ::testing::InitGoogleTest(&argc, argv);
//  return RUN_ALL_TESTS();
//}
