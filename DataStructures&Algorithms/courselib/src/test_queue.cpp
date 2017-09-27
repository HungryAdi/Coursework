//#include <iostream>
//#include <sstream>
//#include "ics46goody.hpp"
//#include "gtest/gtest.h"
//#include "array_stack.hpp"   // must leave in for constructor
//#include "array_queue.hpp"
//
//typedef ics::ArrayQueue<std::string> QueueType;
//typedef ics::ArrayQueue<int>         QueueType2;
//
//int test_size  = ics::prompt_int ("Enter large scale test size");
//int trace      = ics::prompt_bool("Trace large scale test",false);
//int speed_size = ics::prompt_int ("Enter large scale speed test size");
//
//
//class QueueTest : public ::testing::Test {
//protected:
//    virtual void SetUp()    {}
//    virtual void TearDown() {}
//};
//
//
//void load(QueueType& q, std::string values) {
//  std::string* v = new std::string[values.size()];
//  for (unsigned i=0; i<values.size(); ++i)
//    v[i] = values[i];
//  for (unsigned i=0; i<values.size(); ++i)
//    q.enqueue(v[i]);
//  delete[] v;
//}
//
//
//::testing::AssertionResult unload(QueueType& q, std::string values) {
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
//
//
//TEST_F(QueueTest, empty) {
//  QueueType q;
//  ASSERT_TRUE(q.empty());
//}
//
//
//TEST_F(QueueTest, size) {
//  QueueType q;
//  ASSERT_EQ(0,q.size());
//}
//
//
//TEST_F(QueueTest, peek) {
//  QueueType q;
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//}
//
//
//TEST_F(QueueTest, enqueue) {
//  QueueType q;
//  ASSERT_EQ(1,q.enqueue("a"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(1, q.size());
//  ASSERT_EQ("a",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("b"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(2, q.size());
//  ASSERT_EQ("a",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("c"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(3, q.size());
//  ASSERT_EQ("a",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("d"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(4, q.size());
//  ASSERT_EQ("a",q.peek());
//
//  ASSERT_EQ(1,q.enqueue("e"));
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(5, q.size());
//  ASSERT_EQ("a",q.peek());
//}
//
//
//TEST_F(QueueTest, operator_eq_ne) {// == and !=
//  QueueType q1,q2;
//  ASSERT_EQ(q1,q2);
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
//  q2.enqueue("a");
//  ASSERT_NE(q1,q2);
//
//  q2.enqueue("b");
//  ASSERT_NE(q1,q2);
//
//  q2.enqueue("c");
//  ASSERT_EQ(q1,q2);
//
//  q2.enqueue("d");
//  ASSERT_NE(q1,q2);
//
//  q2.enqueue("e");
//  ASSERT_NE(q1,q2);
//
//  q1.enqueue("d");
//  ASSERT_NE(q1,q2);
//
//  q1.enqueue("e");
//  ASSERT_EQ(q1,q2);
//
//
//  ASSERT_EQ(q1,q1);
//  ASSERT_EQ(q2,q2);
//}
//
//
//TEST_F(QueueTest, operator_stream_insert) {// <<
//  std::ostringstream value;
//  QueueType q;
//  value << q;
//  ASSERT_EQ("queue[]:rear", value.str());
//
//  value.str("");
//  q.enqueue("a");
//  value << q;
//  ASSERT_EQ("queue[a]:rear", value.str());
//
//  value.str("");
//  q.enqueue("b");
//  value << q;
//  ASSERT_EQ("queue[a,b]:rear", value.str());
//
//  value.str("");
//  q.enqueue("c");
//  value << q;
//  ASSERT_EQ("queue[a,b,c]:rear", value.str());
//
//  value.str("");
//  q.enqueue("d");
//  value << q;
//  ASSERT_EQ("queue[a,b,c,d]:rear", value.str());
//
//  value.str("");
//  q.enqueue("e");
//  value << q;
//  ASSERT_EQ("queue[a,b,c,d,e]:rear", value.str());
//}
//
//
//TEST_F(QueueTest, enqueue_all) {
//  QueueType q,q1;
//  load(q1,"abcde");
//  q.enqueue_all(q1);
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(5, q.size());
//  ASSERT_EQ(q,q1);
//}
//
//
//TEST_F(QueueTest, clear) {
//  QueueType q;
//  q.clear();
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//
//  load(q,"a");
//  q.clear();
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//
//  load(q,"ab");
//  q.clear();
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//
//  load(q,"abc");
//  q.clear();
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//
//  load(q,"abcd");
//  q.clear();
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//
//  load(q,"abcde");
//  q.clear();
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//}
//
//
//TEST_F(QueueTest, dequeue1) {
//  QueueType q;
//  load(q,"abcde");
//  ASSERT_EQ("a",q.dequeue());
//  ASSERT_EQ("b",q.dequeue());
//  ASSERT_EQ("c",q.dequeue());
//  ASSERT_EQ("d",q.dequeue());
//  ASSERT_EQ("e",q.dequeue());
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//}
//
//
//TEST_F(QueueTest, dequeue2) {
//  QueueType q;
//  load(q,"abcde");
//  ASSERT_EQ("a",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(4, q.size());
//  ASSERT_EQ("b", q.peek());
//
//  ASSERT_EQ("b",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(3, q.size());
//  ASSERT_EQ("c", q.peek());
//
//  ASSERT_EQ("c",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(2, q.size());
//  ASSERT_EQ("d", q.peek());
//
//  ASSERT_EQ("d",q.dequeue());
//  ASSERT_FALSE(q.empty());
//  ASSERT_EQ(1, q.size());
//  ASSERT_EQ("e", q.peek());
//
//  ASSERT_EQ("e",q.dequeue());
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0, q.size());
//  ASSERT_THROW(q.peek(),ics::EmptyError);
//}
//
//
//TEST_F(QueueTest, assignment) {
//  QueueType q1,q2;
//  load(q2,"abcde");
//  q1 = q2;
//  ASSERT_EQ(q1,q2);
//
//  q2.clear();
//  load(q2,"ab");
//  q1 = q2;
//  ASSERT_EQ(q1,q2);
//
//  q2.clear();
//  load(q2,"abcdefghi");
//  q1 = q2;
//  ASSERT_EQ(q1,q2);
//
//}
//
//
//TEST_F(QueueTest, iterator_plusplus) {
//  QueueType q;
//  load(q,"abcde");
//  QueueType::Iterator end(q.end());
//
//  QueueType::Iterator i(q.begin());
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
//  QueueType::Iterator j(q.begin());
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
//TEST_F(QueueTest, iterator_simple) {
//  std::string qvalues[] ={"a","b","c","d","e"};
//  QueueType q;
//  load(q,"abcde");
//
//  //for-each iterator (using .begin/.end)
//  int i = 0;
//  for (std::string x : q)
//    ASSERT_EQ(qvalues[i++],x);
//  ASSERT_EQ(5,q.size());
//
//  //explicit iterator (using .begin/.end and ++it)
//  i = 0;
//  for (QueueType::Iterator it(q.begin()); it != q.end(); ++it)
//    ASSERT_EQ(qvalues[i++],*it);
//  ASSERT_EQ(5,q.size());
//
//  //explicit iterator (using .begin/.end and it++)
//  i = 0;
//  for (QueueType::Iterator it(q.begin()); it != q.end(); it++)
//    ASSERT_EQ(qvalues[i++],*it);
//  ASSERT_EQ(5,q.size());
//
//  //iteration did not change the queue
//  i = 0;
//  ASSERT_EQ(5,q.size());
//  while (!q.empty())
//    ASSERT_EQ(qvalues[i++],q.dequeue());
//  ASSERT_EQ(0,q.size());
//}
//
//
//TEST_F(QueueTest, iterator_erase) {
//  std::string qvalues[] ={"a","b","c","d","e","f","g","h"};
//  QueueType q;
//  load(q,"abcdefgh");
//  QueueType::Iterator it(q.begin());
//
//  ASSERT_EQ("a", it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it; //to "b"
//  ++it; //to "c"
//
//  ASSERT_EQ("c", it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it; //to "d"
//
//  ASSERT_EQ("d", it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//  ++it; //to "e"
//  ASSERT_EQ("e", it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it; //to "f"
//  ++it; //to "g"
//  ++it; //to "h"
//  ASSERT_EQ("h", it.erase());
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//
//  ++it; //beyond last
//  ASSERT_THROW(it.erase(),ics::CannotEraseError);
//  ASSERT_THROW(*it,ics::IteratorPositionIllegal);
//  std::string remaining[] ={"b","f","g"};
//  int i = 0;
//  for (std::string x : q)
//    ASSERT_EQ(remaining[i++],x);
//
//
//  //erase all in the queue
//  q.clear();
//  load(q,"abcdefgh");
//  i = 0;
//  for (QueueType::Iterator it(q.begin()); it != q.end(); ++it)
//    ASSERT_EQ(qvalues[i++],it.erase());
//  ASSERT_TRUE(q.empty());
//  ASSERT_EQ(0,q.size());
//}
//
//
//TEST_F(QueueTest, iterator_exception_concurrent_modification_error) {
//  QueueType q;
//  load(q,"abcdefgh");
//  QueueType::Iterator it(q.begin());
//
//  q.dequeue();
//  ASSERT_THROW(it.erase(),ics::ConcurrentModificationError);
//  ASSERT_THROW(++it,ics::ConcurrentModificationError);
//  ASSERT_THROW(it++,ics::ConcurrentModificationError);
//  ASSERT_THROW(*it,ics::ConcurrentModificationError);
//}
//
//
//TEST_F(QueueTest, constructors) {
//  //copy
//  QueueType q;
//  load(q,"abcdefghij");
//  ASSERT_TRUE(unload(q,"abcdefghij"));
//
//  //default
//  load(q,"abcdefghij");
//  QueueType q2(q);
//  ASSERT_EQ(q,q2);
//  ASSERT_TRUE(unload(q, "abcdefghij"));
//  ASSERT_NE(q,q2);
//  ASSERT_TRUE(unload(q2,"abcdefghij"));
//  ASSERT_EQ(q,q2);
//
//  //initializer
//  load(q,"abcdefghij");
//  QueueType q3({"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"});
//  ASSERT_EQ(q,q3);
//  ASSERT_TRUE(unload(q, "abcdefghij"));
//  ASSERT_NE(q,q3);
//  ASSERT_TRUE(unload(q3,"abcdefghij"));
//  ASSERT_EQ(q,q3);
//
//  //iterator
//  ics::ArrayStack<std::string> qa({"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"});
//  QueueType q4(qa);
//  ASSERT_TRUE(unload(q4,"jihgfedcba"));
//}
//
//
//TEST_F(QueueTest, large_scale) {
//  QueueType2 lq;
//  for (int test=1; test<=5; ++test) {
//    int enqueued = 0;
//    int dequeued = 0;
//    while (dequeued != test_size) {
//      int to_enqueue = ics::rand_range(0,test_size-enqueued);
//      if (trace)
//        std::cout << "Enqueue " << to_enqueue << std::endl;
//      for (int i=0; i <to_enqueue; ++i)
//        ASSERT_EQ(1,lq.enqueue(enqueued++));
//      int start = dequeued;
//      for (int v : lq)
//        ASSERT_EQ(start++,v);
//
//      int to_dequeue = ics::rand_range(0,enqueued-dequeued);
//      if (trace)
//        std::cout << "Dequeue " << to_dequeue << std::endl;
//      for (int i=0; i <to_dequeue; ++i) {
//        ASSERT_EQ(dequeued,lq.peek());
//        ASSERT_EQ(dequeued++,lq.dequeue());
//      }
//      start = dequeued;
//      for (int v : lq)
//        ASSERT_EQ(start++,v);
//    }
//  }
//  ASSERT_TRUE(lq.empty());
//  ASSERT_EQ(0,lq.size());
//}
//
//
//TEST_F(QueueTest, large_scale_speed) {
//  QueueType2 lq;
//  for (int test=1; test<=5; ++test) {
//    int enqueued = 0;
//    int dequeued = 0;
//    while (dequeued != speed_size) {
//      int to_enqueue = ics::rand_range(0,speed_size-enqueued);
//      for (int i=0; i <to_enqueue; ++i)
//        lq.enqueue(enqueued++);
//      for (int v : lq)
//        ;
//
//      int to_dequeue = ics::rand_range(0,enqueued-dequeued);
//      for (int i=0; i <to_dequeue; ++i) {
//        dequeued++;
//        lq.dequeue();
//      }
//      for (int v : lq)
//        ;
//    }
//  }
//}
//
//
//
//
//
//int main(int argc, char **argv) {
//  ::testing::InitGoogleTest(&argc, argv);
//  return RUN_ALL_TESTS();
//}
