//#include <iostream>
//#include <sstream>
//#include <algorithm>                 // std::random_shuffle
//#include "ics46goody.hpp"
//#include "gtest/gtest.h"
//#include "array_stack.hpp"           // must leave in for constructor
//#include "array_priority_queue.hpp"
//
//bool gt_string  (const std::string& a, const std::string& b) {return a < b;}
//bool gt_string2 (const std::string& a, const std::string& b) {return a > b;}
//bool gt_int     (const int& a, const int& b) {return a < b;}
//
//typedef ics::ArrayPriorityQueue<std::string,gt_string>   PriorityQueueTypeStr;
//typedef ics::ArrayPriorityQueue<std::string,gt_string2>  PriorityQueueTypeStrR;
//typedef ics::ArrayPriorityQueue<int,gt_int>              PriorityQueueTypeInt;
//typedef ics::ArrayPriorityQueue<std::string>             PriorityQueueTypeNone;
//
//int test_size  = ics::prompt_int ("Enter large scale test size");
//int trace      = ics::prompt_bool("Trace large scale test",false);
//int speed_size = ics::prompt_int ("Enter large scale speed test size");
//
//
//class PriorityQueueTest : public ::testing::Test {
//protected:
//    virtual void SetUp()    {}
//    virtual void TearDown() {}
//};
//
//
//template<class T>
//void load(T& q, std::string values) {
//  std::string* v = new std::string[values.size()];
//  for (unsigned i=0; i<values.size(); ++i)
//    v[i] = values[i];
//  for (unsigned i=0; i<values.size(); ++i)
//    q.enqueue(v[i]);
//  delete[] v;
//}
//
//
//template<class T>
//::testing::AssertionResult unload(T& q, std::string values) {
//  std::string* v = new std::string[values.size()];
//  for (unsigned i=0; i<values.size(); ++i)
//    v[i] = values[i];
//  for (unsigned i=0; i<values.size(); ++i)
//    if (v[i] != q.dequeue())
//      return ::testing::AssertionFailure();
//  delete[] v;
//  return ::testing::AssertionSuccess();
//}
//
//
//TEST_F(PriorityQueueTest, empty) {
//  PriorityQueueTypeStr q;
//  ASSERT_TRUE(q.empty());
//}
//
//
//TEST_F(PriorityQueueTest, size) {
//  PriorityQueueTypeStr q;
//  ASSERT_EQ(0,q.size());
//}
//
//
//TEST_F(PriorityQueueTest, peek) {
//  PriorityQueueTypeStr q;
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//}
//
////order: fcijbdegabh
//TEST_F(PriorityQueueTest, enqueue) {
//  PriorityQueueTypeStr q;
//  ASSERT_EQ(1,q.enqueue("f"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(1, q.size());
//  ASSERT_EQ("f",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("c"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(2, q.size());
//  ASSERT_EQ("c",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("i"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(3, q.size());
//  ASSERT_EQ("c",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("j"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(4, q.size());
//  ASSERT_EQ("c",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("b"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(5, q.size());
//  ASSERT_EQ("b",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("d"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(6, q.size());
//  ASSERT_EQ("b",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("e"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(7, q.size());
//  ASSERT_EQ("b",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("g"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(8, q.size());
//  ASSERT_EQ("b",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("a"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(9, q.size());
//  ASSERT_EQ("a",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("h"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(10, q.size());
//  ASSERT_EQ("a",q.peek());
//}
//
//
//TEST_F(PriorityQueueTest, operator_eq_ne) {// == and !=
//  PriorityQueueTypeStr   q1,q2;
//  PriorityQueueTypeNone  q3(gt_string),q4(gt_string2);
//
//  ASSERT_EQ(q1,q2);
//  ASSERT_NE(q3,q4); //Both empty, but different gt functions
//
//  q1.enqueue("a");
//  ASSERT_NE(q1,q2);
//
//  q1.enqueue("b");
//  ASSERT_NE(q1,q2);
//
//  q1.enqueue("c");
//  ASSERT_NE(q1,q2);
//
//  q2.enqueue("c");
//  ASSERT_NE(q1,q2);
//
//  q2.enqueue("b");
//  ASSERT_NE(q1,q2);
//
//  q2.enqueue("a");
//  ASSERT_EQ(q1,q2);
//
//  q2.enqueue("d");
//  ASSERT_NE(q1,q2);
//
//  q2.enqueue("e");
//  ASSERT_NE(q1,q2);
//
//  q1.enqueue("e");
//  ASSERT_NE(q1,q2);
//
//  q1.enqueue("d");
//  ASSERT_EQ(q1,q2);
//
//  q1.enqueue("x");
//  ASSERT_NE(q1,q2);
//
//  q1.enqueue("y");
//  ASSERT_NE(q1,q2);
//
//  ASSERT_EQ(q1,q1);
//  ASSERT_EQ(q2,q2);
//}
//
//
//TEST_F(PriorityQueueTest, operator_stream_insert) {// <<
//  std::ostringstream value;
//  PriorityQueueTypeStr q;
//  value << q;
//  ASSERT_EQ("priority_queue[]:highest", value.str());
//
//  value.str("");
//  q.enqueue("c");
//  value << q;
//  ASSERT_EQ("priority_queue[c]:highest", value.str());
//
//  value.str("");
//  q.enqueue("b");
//  value << q;
//  ASSERT_EQ("priority_queue[c,b]:highest", value.str());
//
//  value.str("");
//  q.enqueue("d");
//  value << q;
//  ASSERT_EQ("priority_queue[d,c,b]:highest", value.str());
//
//  value.str("");
//  q.enqueue("a");
//  value << q;
//  ASSERT_EQ("priority_queue[d,c,b,a]:highest", value.str());
//
//  value.str("");
//  q.enqueue("e");
//  value << q;
//  ASSERT_EQ("priority_queue[e,d,c,b,a]:highest", value.str());
//}
//
//
//TEST_F(PriorityQueueTest, enqueue_all) {
//  PriorityQueueTypeStr q,q1;
//  load(q1,"fcijbdegah");
//  q.enqueue_all(q1);
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(10, q.size());
//  ASSERT_EQ(q,q1);
//}
//
//
//TEST_F(PriorityQueueTest, clear) {
//  PriorityQueueTypeStr q;
//  q.clear();
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//
//  load(q,"f");
//  q.clear();
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//
//  load(q,"fc");
//  q.clear();
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//
//  load(q,"fci");
//  q.clear();
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//
//  load(q,"fcij");
//  q.clear();
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//
//  load(q,"fcijb");
//  q.clear();
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//}
//
//
//TEST_F(PriorityQueueTest, dequeue1) {
//  PriorityQueueTypeStr q;
//  load(q,"fcijbdegah");
//  ASSERT_EQ("a",q.dequeue());
//  ASSERT_EQ("b",q.dequeue());
//  ASSERT_EQ("c",q.dequeue());
//  ASSERT_EQ("d",q.dequeue());
//  ASSERT_EQ("e",q.dequeue());
//  ASSERT_EQ("f",q.dequeue());
//  ASSERT_EQ("g",q.dequeue());
//  ASSERT_EQ("h",q.dequeue());
//  ASSERT_EQ("i",q.dequeue());
//  ASSERT_EQ("j",q.dequeue());
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//}
//
//
//TEST_F(PriorityQueueTest, dequeue2) {
//  PriorityQueueTypeStr q;
//  load(q,"fcijbdegah");
//  ASSERT_EQ("a",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(9, q.size());
//  ASSERT_EQ("b", q.peek());
//
//  ASSERT_EQ("b",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(8, q.size());
//  ASSERT_EQ("c", q.peek());
//
//  ASSERT_EQ("c",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(7, q.size());
//  ASSERT_EQ("d", q.peek());
//
//  ASSERT_EQ("d",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(6, q.size());
//  ASSERT_EQ("e", q.peek());
//
//  ASSERT_EQ("e",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(5, q.size());
//  ASSERT_EQ("f", q.peek());
//
//  ASSERT_EQ("f",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(4, q.size());
//  ASSERT_EQ("g", q.peek());
//
//  ASSERT_EQ("g",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(3, q.size());
//  ASSERT_EQ("h", q.peek());
//
//  ASSERT_EQ("h",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(2, q.size());
//  ASSERT_EQ("i", q.peek());
//
//  ASSERT_EQ("i",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(1, q.size());
//  ASSERT_EQ("j", q.peek());
//
//  ASSERT_EQ("j",q.dequeue());
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//}
//
//
//TEST_F(PriorityQueueTest, assignment) {
//  PriorityQueueTypeStr q1,q2;
//  load(q2,"fcijb");
//  q1 = q2;
//  ASSERT_EQ(q1,q2);
//
//  q2.clear();
//  load(q2,"ab");
//  q1 = q2;
//  ASSERT_EQ(q1,q2);
//
//  q2.clear();
//  load(q2,"fcijdegabh");
//  q1 = q2;
//  ASSERT_EQ(q1,q2);
//
//  PriorityQueueTypeNone q3(gt_string),q4(gt_string2);
//  load(q4,"fcijdegabh");
//  q3 = q4;
//  ASSERT_TRUE(unload(q3,"jihgfedcba"));
//  ASSERT_TRUE(unload(q4,"jihgfedcba"));
//}
//
//
//TEST_F(PriorityQueueTest, iterator_plusplus) {
//  PriorityQueueTypeStr q;
//  load(q,"cbdea");
//  PriorityQueueTypeStr::Iterator end = q.end();
//
//  PriorityQueueTypeStr::Iterator i = q.begin();
//  ASSERT_EQ("a", *i);
//  ASSERT_EQ("b", *(++i));
//  ASSERT_EQ("b", *i);
//  ASSERT_EQ("c", *(++i));
//  ASSERT_EQ("c", *i);
//  ASSERT_EQ("d", *(++i));
//  ASSERT_EQ("d", *i);
//  ASSERT_EQ("e", *(++i));
//  ASSERT_EQ("e", *i);
//  ASSERT_EQ(end, ++i);
//  ASSERT_EQ(end, i);
//  ASSERT_EQ(end, ++i);
//
//  PriorityQueueTypeStr::Iterator j = q.begin();
//  ASSERT_EQ("a", *j);
//  ASSERT_EQ("a", *(j++));
//  ASSERT_EQ("b", *j);
//  ASSERT_EQ("b", *(j++));
//  ASSERT_EQ("c", *j);
//  ASSERT_EQ("c", *(j++));
//  ASSERT_EQ("d", *j);
//  ASSERT_EQ("d", *(j++));
//  ASSERT_EQ("e", *j);
//  ASSERT_EQ("e", *(j++));
//  ASSERT_EQ(end, j);
//  ASSERT_EQ(end, j++);
//  ASSERT_EQ(end, j);
//  ASSERT_EQ(end, j++);
//}
//
//
//TEST_F(PriorityQueueTest, iterator_simple) {
//  std::string qvalues[] ={"a","b","c","d","e","f","g","h","i","j"};
//  PriorityQueueTypeStr q;
//  load(q,"fcijbdegah");
//
//  //for-each iterator (using .begin/.end)
//  int i = 0;
//  for (std::string x : q)
//    ASSERT_EQ(qvalues[i++],x);
//  ASSERT_EQ(10,q.size());
//
//  //explicit iterator (using .begin/.end and ++it)
//  i = 0;
//  for (PriorityQueueTypeStr::Iterator it(q.begin()); it != q.end(); ++it)
//    ASSERT_EQ(qvalues[i++],*it);
//  ASSERT_EQ(10,q.size());
//
//  //explicit iterator (using .begin/.end and it++)
//  i = 0;
//  for (PriorityQueueTypeStr::Iterator it(q.begin()); it != q.end(); it++)
//    ASSERT_EQ(qvalues[i++],*it);
//  ASSERT_EQ(10,q.size());
//
//  //all these iterations didn't change the queue
//  i = 0;
//  ASSERT_EQ(10,q.size());
//  while (!q.empty())
//    ASSERT_EQ(qvalues[i++],q.dequeue());
//  ASSERT_EQ(0,q.size());
//}
//
//
//TEST_F(PriorityQueueTest, iterator_erase) {
//  std::string qvalues[] ={"a","b","c","d","e","f","g","h","i","j"};
//  PriorityQueueTypeStr q;
//  load(q,"fcijbdegah");
//  PriorityQueueTypeStr::Iterator it(q.begin());
//
//  ASSERT_EQ("a", it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it; //to "b"
//
//  ++it; //to "c"
//  ASSERT_EQ("c", it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//  ++it; //to "d"
//  ASSERT_EQ("d", it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it; //to "e"
//
//  ++it; //to "f"
//  ASSERT_EQ("f", it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it; //to "g"
//  ++it; //to "h"
//
//  ++it; //to "i"
//  ASSERT_EQ("i", it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//  ++it; //to "j"
//  ASSERT_EQ("j", it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it; //beyond last
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//  std::string remaining[] ={"b","e","g","h"};
//  int i = 0;
//  for (std::string x : q)
//    ASSERT_EQ(remaining[i++],x);
//
//
//  //erase all in the queue
//  q.clear();
//  load(q,"fcijbdegah");
//  i = 0;
//  for (PriorityQueueTypeStr::Iterator it(q.begin()); it != q.end(); it++)
//    ASSERT_EQ(qvalues[i++],it.erase());
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0,q.size());
//}
//
//
//TEST_F(PriorityQueueTest, iterator_erase_heap_special_case) {
//  PriorityQueueTypeStr q;
//  load(q,"adbefc");
//  for (PriorityQueueTypeStr::Iterator i(q.begin()); i != q.end(); ++i)
//    if (*i == "f") {
//      i.erase();
//      break;
//    }
//  ASSERT_TRUE(unload(q,"abcde"));
//}
//
//
//TEST_F(PriorityQueueTest, iterator_exception_concurrent_modification_error) {
//  PriorityQueueTypeStr q;
//  load(q,"fcijbdegabh");
//  PriorityQueueTypeStr::Iterator it(q.begin());
//
//  q.dequeue();
//  ASSERT_THROW(it.erase(),ics::ConcurrentModificationError);
//  ASSERT_THROW(++it,ics::ConcurrentModificationError);
//  ASSERT_THROW(it++,ics::ConcurrentModificationError);
//  ASSERT_THROW(*it,ics::ConcurrentModificationError);
//}
//
//
//TEST_F(PriorityQueueTest, constructors) {
//  //default
//  PriorityQueueTypeStr q;
//  load(q,"fcijbdegah");
//  ASSERT_TRUE(unload(q,"abcdefghij"));
//
//  //copy
//  load(q,"fcijbdegah");
//  PriorityQueueTypeStr q2(q);
//  ASSERT_EQ(q,q2);
//  ASSERT_TRUE(unload(q, "abcdefghij"));
//  ASSERT_NE(q,q2);
//  ASSERT_TRUE(unload(q2,"abcdefghij"));
//  ASSERT_EQ(q,q2);
//
//  //initializer
//  load(q,"fcijbdegah");
//  PriorityQueueTypeStr q3({"f", "c", "i", "j", "b", "d", "e", "g", "a", "h"});
//  ASSERT_EQ(q,q3);
//  ASSERT_TRUE(unload(q, "abcdefghij"));
//  ASSERT_NE(q,q3);
//  ASSERT_TRUE(unload(q3,"abcdefghij"));
//  ASSERT_EQ(q,q3);
//
//  //iterator
//  ics::ArrayStack<std::string> qa({"f", "c", "i", "j", "b", "d", "e", "g", "a", "h"});
//  PriorityQueueTypeStr q4(qa);
//  ASSERT_TRUE(unload(q4,"abcdefghij"));
//
//  //iterator, different gt
//  load(q,"fcijbdegah");
//  PriorityQueueTypeNone q5r(q,gt_string2);
//  ASSERT_TRUE(unload(q, "abcdefghij"));
//  ASSERT_NE(PriorityQueueTypeNone(q,gt_string2),q5r);
//  ASSERT_TRUE(unload(q5r,"jihgfedcba"));
//}
//
//
//TEST_F(PriorityQueueTest, template_constructors) {
//  //function specified in neither Template nor Constructor: must fail
//  try {
//    PriorityQueueTypeNone q_f;
//    ADD_FAILURE();
//  } catch (ics::IcsError& e) {
//    SUCCEED();
//  }
//
//  //different functions specified in both Template and Constructor: must fail
//  try {
//    PriorityQueueTypeStr q_f(gt_string2);
//    ADD_FAILURE();
//  } catch (ics::IcsError& e) {
//    SUCCEED();
//  }
//
//  //same function specified in both Template and Constructor
//  PriorityQueueTypeStr q_f(gt_string);
//
//  //function specified in only in Template (by gt_string: ascending)
//  PriorityQueueTypeStr  q_t({"f","c","i","j","b","d","e","g","a","h"});
//  ASSERT_TRUE(unload(q_t,"abcdefghij"));
//
//  //function specified in only in Constructor (gt_string2: descending)
//  PriorityQueueTypeNone q_c({"f","c","i","j","b","d","e","g","a","h"},gt_string2);
//  ASSERT_TRUE(unload(q_c,"jihgfedcba"));
//
//  //function specified in neither Template nor Constructor: copy constructor gets from q_cc
//  PriorityQueueTypeNone q_cc({"f","c","i","j","b","d","e","g","a","h"},gt_string);
//  PriorityQueueTypeNone q_cc1(q_cc);
//  ASSERT_TRUE(unload(q_cc1,"abcdefghij"));
//}
//
//
//TEST_F(PriorityQueueTest, large_scale) {
//  PriorityQueueTypeInt lq;
//  ics::ArrayPriorityQueue<int,gt_int> lq_ref;
//
//
//  std::vector<int> values;
//  for (int i=0; i<test_size; ++i)
//    values.push_back(i);
//   std::random_shuffle(values.begin(),values.end());
//
//
//   for (int test=1; test<=5; ++test) {
//    int enqueued = 0;
//    int dequeued = 0;
//    while (dequeued != test_size) {
//      int to_enqueue = ics::rand_range(0,test_size-enqueued);
//      if (trace)
//        std::cout << "Enqueue " << to_enqueue << std::endl;
//      for (int i=0; i <to_enqueue; ++i) {
//        lq_ref.enqueue(values[enqueued]);
//        ASSERT_EQ(1,lq.enqueue(values[enqueued++]));
//      }
//      ics::ArrayPriorityQueue<int,gt_int>::Iterator  i = lq_ref.begin();
//      for (int v : lq) {
//        ASSERT_EQ(*i,v);
//        ++i;
//      }
//
//      int to_dequeue = ics::rand_range(0,enqueued-dequeued);
//      if (trace)
//        std::cout << "Dequeue " << to_dequeue << std::endl;
//      for (int i=0; i <to_dequeue; ++i) {
//        ASSERT_EQ(lq_ref.peek(),lq.peek());
//        ASSERT_EQ(lq_ref.dequeue(),lq.dequeue());
//        ++dequeued;
//      }
//      ics::ArrayPriorityQueue<int,gt_int>::Iterator j = lq_ref.begin();
//      for (int v : lq) {
//        ASSERT_EQ(*j,v);
//        ++j;
//      }
//    }
//  }
//  ASSERT_TRUE(lq.empty());
//  ASSERT_EQ(0,lq.size());
//}
//
//
//TEST_F(PriorityQueueTest, large_scale_speed) {
//  PriorityQueueTypeInt lq;
//
//  std::vector<int> values;
//  for (int i=0; i<speed_size; ++i)
//    values.push_back(i);
//  std::random_shuffle(values.begin(),values.end());
//
//  for (int test=1; test<=5; ++test) {
//    int enqueued = 0;
//    int dequeued = 0;
//    while (dequeued != speed_size) {
//      int to_enqueue = ics::rand_range(0,speed_size-enqueued);
//      for (int i=0; i <to_enqueue; ++i)
//        lq.enqueue(values[enqueued++]);
//      for (int v : lq)
//        ;
//      int to_dequeue = ics::rand_range(0,enqueued-dequeued);
//      for (int i=0; i <to_dequeue; ++i) {
//        lq.dequeue();
//        ++dequeued;
//      }
//      for (int v : lq)
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
