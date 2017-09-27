#ifndef ARRAY_PRIORITY_QUEUE_HPP_
#define ARRAY_PRIORITY_QUEUE_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"


namespace ics {


//Instantiate the templated class supplying tgt(a,b): true, iff a has higher priority than b.
//If tgt is defaulted to nullptr in the template, then a constructor must supply cgt.
//If both tgt and cgt are supplied, then they must be the same (by ==) function.
//If neither is supplied, or both are supplied but different, TemplateFunctionError is raised.
//The (unique) non-nullptr value supplied by tgt/cgt is stored in the instance variable gt.
template<class T, bool (*tgt)(const T& a, const T& b) = nullptr> class ArrayPriorityQueue {
  public:
    //Destructor/Constructors
    ~ArrayPriorityQueue ();

    ArrayPriorityQueue          (bool (*cgt)(const T& a, const T& b) = nullptr);
    explicit ArrayPriorityQueue (int initial_length, bool (*cgt)(const T& a, const T& b) = nullptr);
    ArrayPriorityQueue          (const ArrayPriorityQueue<T,tgt>& to_copy, bool (*cgt)(const T& a, const T& b) = nullptr);
    explicit ArrayPriorityQueue (const std::initializer_list<T>& il, bool (*cgt)(const T& a, const T& b) = nullptr);

    //Iterable class must support "for-each" loop: .begin()/.end()/.size() and prefix ++ on returned result
    template <class Iterable>
    explicit ArrayPriorityQueue (const Iterable& i, bool (*cgt)(const T& a, const T& b) = nullptr);


    //Queries
    bool empty      () const;
    int  size       () const;
    T&   peek       () const;
    std::string str () const; //supplies useful debugging information; contrast to operator <<


    //Commands

    int  enqueue (const T& element);
    T    dequeue ();
    void clear   ();

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    int enqueue_all (const Iterable& i);


    //Operators
    ArrayPriorityQueue<T,tgt>& operator = (const ArrayPriorityQueue<T,tgt>& rhs);
    bool operator == (const ArrayPriorityQueue<T,tgt>& rhs) const;
    bool operator != (const ArrayPriorityQueue<T,tgt>& rhs) const;

    template<class T2, bool (*gt2)(const T2& a, const T2& b)>
    friend std::ostream& operator << (std::ostream& outs, const ArrayPriorityQueue<T2,gt2>& p);



    class Iterator {
      public:
        //Private constructor called in begin/end, which are friends of ArrayPriorityQueue<T,tgt>
        ~Iterator();
        T           erase();
        std::string str  () const;
        ArrayPriorityQueue<T,tgt>::Iterator& operator ++ ();
        ArrayPriorityQueue<T,tgt>::Iterator  operator ++ (int);
        bool operator == (const ArrayPriorityQueue<T,tgt>::Iterator& rhs) const;
        bool operator != (const ArrayPriorityQueue<T,tgt>::Iterator& rhs) const;
        T& operator *  () const;
        T* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const ArrayPriorityQueue<T,tgt>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }

        friend Iterator ArrayPriorityQueue<T,tgt>::begin () const;
        friend Iterator ArrayPriorityQueue<T,tgt>::end   () const;

      private:
        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
        int                        current;
        ArrayPriorityQueue<T,tgt>* ref_pq;
        int                        expected_mod_count;
        bool                       can_erase = true;

        //Called in friends begin/end
        Iterator(ArrayPriorityQueue<T,tgt>* iterate_over, int initial);
    };


    Iterator begin () const;
    Iterator end   () const;


  private:
    bool (*gt) (const T& a, const T& b); // The gt used by enqueue (from template or constructor)
    T*  pq;                              // Smaller values in lower indexes (biggest is at used-1)
    int length    = 0;                   //Physical length of array: must be >= .size()
    int used      = 0;                   //Amount of array used:  invariant: 0 <= used <= length
    int mod_count = 0;                   //For sensing concurrent modification


    //Helper methods
    int erase_at      (int i);
    void ensure_length(int new_length);
  };





////////////////////////////////////////////////////////////////////////////////
//
//ArrayPriorityQueue class and related definitions

//Destructor/Constructors

template<class T, bool (*tgt)(const T& a, const T& b)>
ArrayPriorityQueue<T,tgt>::~ArrayPriorityQueue() {
  delete[] pq;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
ArrayPriorityQueue<T,tgt>::ArrayPriorityQueue(bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt) {
  if (gt == nullptr)
    throw TemplateFunctionError("ArrayPriorityQueue::default constructor: neither specified");
  if (tgt != nullptr && cgt != nullptr && tgt != cgt)
    throw TemplateFunctionError("ArrayPriorityQueue::default constructor: both specified and different");

  pq = new T[length];
}


template<class T, bool (*tgt)(const T& a, const T& b)>
ArrayPriorityQueue<T,tgt>::ArrayPriorityQueue(int initial_length, bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt), length(initial_length) {
  if (gt == nullptr)
    throw TemplateFunctionError("ArrayPriorityQueue::length constructor: neither specified");
  if (tgt != nullptr && cgt != nullptr && tgt != cgt)
    throw TemplateFunctionError("ArrayPriorityQueue::length constructor: both specified and different");

  if (length < 0)
    length = 0;
  pq = new T[length];
}


template<class T, bool (*tgt)(const T& a, const T& b)>
ArrayPriorityQueue<T,tgt>::ArrayPriorityQueue(const ArrayPriorityQueue<T,tgt>& to_copy, bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt), length(to_copy.length) {
  if (gt == nullptr)
    gt = to_copy.gt;//throw TemplateFunctionError("ArrayPriorityQueue::copy constructor: neither specified");
  if (tgt != nullptr && cgt != nullptr && tgt != cgt)
    throw TemplateFunctionError("ArrayPriorityQueue::copy constructor: both specified and different");

  pq = new T[length];

  if (gt == to_copy.gt) {
    used = to_copy.used;
    for (int i=0; i<to_copy.used; ++i)
      pq[i] = to_copy.pq[i];
  }else
    for (int i=0; i<to_copy.used; ++i)
      enqueue(to_copy.pq[i]);
}


template<class T, bool (*tgt)(const T& a, const T& b)>
ArrayPriorityQueue<T,tgt>::ArrayPriorityQueue(const std::initializer_list<T>& il, bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt), length(il.size()) {
  if (gt == nullptr)
    throw TemplateFunctionError("ArrayPriorityQueue::initializer_list constructor: neither specified");
  if (tgt != nullptr && cgt != nullptr && tgt != cgt)
    throw TemplateFunctionError("ArrayPriorityQueue::initializer_list constructor: both specified and different");

  pq = new T[length];
  for (const T& pq_elem : il)
    enqueue(pq_elem);
}


template<class T, bool (*tgt)(const T& a, const T& b)>
template<class Iterable>
ArrayPriorityQueue<T,tgt>::ArrayPriorityQueue(const Iterable& i, bool (*cgt)(const T& a, const T& b))
  : gt(tgt != nullptr ? tgt : cgt), length(i.size()) {
  if (gt == nullptr)
    throw TemplateFunctionError("ArrayPriorityQueue::Iterable constructor: neither specified");
  if (tgt != nullptr && cgt != nullptr && tgt != cgt)
    throw TemplateFunctionError("ArrayPriorityQueue::Iterable constructor: both specified and different");

  pq = new T[length];
  for (const T& v : i)
    enqueue(v);
  }


////////////////////////////////////////////////////////////////////////////////
//
//Queries

template<class T, bool (*tgt)(const T& a, const T& b)>
bool ArrayPriorityQueue<T,tgt>::empty() const {
  return used == 0;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
int ArrayPriorityQueue<T,tgt>::size() const {
  return used;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
T& ArrayPriorityQueue<T,tgt>::peek () const {
  if (empty())
    throw EmptyError("ArrayPriorityQueue::peek");

  return pq[used-1];
}


template<class T, bool (*tgt)(const T& a, const T& b)>
std::string ArrayPriorityQueue<T,tgt>::str() const {
  std::ostringstream answer;
  answer << "ArrayPriorityQueue[";

  if (length != 0) {
    answer << "0:" << pq[0];
    for (int i = 1; i < length; ++i)
      answer << "," << i << ":" << pq[i];
  }

  answer << "](length=" << length << ",used=" << used << ",mod_count=" << mod_count << ")";
  return answer.str();
}


////////////////////////////////////////////////////////////////////////////////
//
//Commands

template<class T, bool (*tgt)(const T& a, const T& b)>
int ArrayPriorityQueue<T,tgt>::enqueue(const T& element) {
  this->ensure_length(used+1);
  pq[used++] = element;
  for (int i=used-2; i>=0; --i)
    if (gt(pq[i],pq[i+1]))
      std::swap(pq[i],pq[i+1]);
    else
      break;
  ++mod_count;
  return 1;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
T ArrayPriorityQueue<T,tgt>::dequeue() {
  if (this->empty())
    throw EmptyError("ArrayPriorityQueue::dequeue");

  ++mod_count;
  return pq[--used];
}


template<class T, bool (*tgt)(const T& a, const T& b)>
void ArrayPriorityQueue<T,tgt>::clear() {
  used = 0;
  ++mod_count;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
template <class Iterable>
int ArrayPriorityQueue<T,tgt>::enqueue_all (const Iterable& i) {
  int count = 0;
  for (const T& v : i)
     count += enqueue(v);

  return count;
}


////////////////////////////////////////////////////////////////////////////////
//
//Operators

template<class T, bool (*tgt)(const T& a, const T& b)>
ArrayPriorityQueue<T,tgt>& ArrayPriorityQueue<T,tgt>::operator = (const ArrayPriorityQueue<T,tgt>& rhs) {
  if (this == &rhs)
    return *this;

  gt = rhs.gt;   // if tgt != nullptr, gts are already equal (or compiler error)
  this->ensure_length(rhs.used);
  used = rhs.used;
  for (int i=0; i<used; ++i)
    pq[i] = rhs.pq[i];

  ++mod_count;
  return *this;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
  bool ArrayPriorityQueue<T,tgt>::operator == (const ArrayPriorityQueue<T,tgt>& rhs) const {
  if (this == &rhs)
    return true;

  if (gt != rhs.gt) //For PriorityQueues to be equal, they need the same gt function, and values
    return false;

  if (used != rhs.size())
    return false;
  ArrayPriorityQueue<T,tgt>::Iterator rhs_i = rhs.begin();
  for (int i=used-1; i>=0; --i,++rhs_i)
    // Uses ! and ==, so != on T need not be defined
    if (!(pq[i] == *rhs_i))
      return false;

  return true;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool ArrayPriorityQueue<T,tgt>::operator != (const ArrayPriorityQueue& rhs) const {
  return !(*this == rhs);
}


template<class T, bool (*tgt)(const T& a, const T& b)>
std::ostream& operator << (std::ostream& outs, const ArrayPriorityQueue<T,tgt>& p) {
  outs << "priority_queue[";

  if (!p.empty()) {
    outs << p.pq[0];
    for (int i = 1; i < p.used; ++i)
      outs << ","<< p.pq[i];
  }

  outs << "]:highest";
  return outs;
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors

template<class T, bool (*tgt)(const T& a, const T& b)>
auto ArrayPriorityQueue<T,tgt>::begin () const -> ArrayPriorityQueue<T,tgt>::Iterator {
  return Iterator(const_cast<ArrayPriorityQueue<T,tgt>*>(this),used-1);
}


template<class T, bool (*tgt)(const T& a, const T& b)>
auto ArrayPriorityQueue<T,tgt>::end () const -> ArrayPriorityQueue<T,tgt>::Iterator {
  return Iterator(const_cast<ArrayPriorityQueue<T,tgt>*>(this),-1);
}


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

template<class T, bool (*tgt)(const T& a, const T& b)>
int ArrayPriorityQueue<T,tgt>::erase_at(int i) {
  for (int j=i; j<used-1; ++j)
    pq[j] = pq[j+1];
  --used;
  ++mod_count;
  return 1;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
void ArrayPriorityQueue<T,tgt>::ensure_length(int new_length) {
  if (length >= new_length)
    return;
  T* old_pq = pq;
  length = std::max(new_length,2*length);
  pq = new T[length];
  for (int i=0; i<used; ++i)
    pq[i] = old_pq[i];

  delete [] old_pq;
}





////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

template<class T, bool (*tgt)(const T& a, const T& b)>
ArrayPriorityQueue<T,tgt>::Iterator::Iterator(ArrayPriorityQueue<T,tgt>* iterate_over, int initial)
: current(initial), ref_pq(iterate_over), expected_mod_count(ref_pq->mod_count) {
}


template<class T, bool (*tgt)(const T& a, const T& b)>
ArrayPriorityQueue<T,tgt>::Iterator::~Iterator()
{}


template<class T, bool (*tgt)(const T& a, const T& b)>
T ArrayPriorityQueue<T,tgt>::Iterator::erase() {
  if (expected_mod_count != ref_pq->mod_count)
    throw ConcurrentModificationError("ArrayPriorityQueue::Iterator::erase");
  if (!can_erase)
    throw CannotEraseError("ArrayPriorityQueue::Iterator::erase Iterator cursor already erased");
  if (current < 0 || current >= ref_pq->used)
    throw CannotEraseError("ArrayPriorityQueue::Iterator::erase Iterator cursor beyond data structure");

  can_erase = false;
  T to_return = ref_pq->pq[current];
  ref_pq->erase_at(current);
  --current;
  expected_mod_count = ref_pq->mod_count;
  return to_return;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
std::string ArrayPriorityQueue<T,tgt>::Iterator::str() const {
  std::ostringstream answer;
  answer << ref_pq->str() << "/current=" << current << "/expected_mod_count=" << expected_mod_count << "/can_erase=" << can_erase;
  return answer.str();
}


template<class T, bool (*tgt)(const T& a, const T& b)>
auto ArrayPriorityQueue<T,tgt>::Iterator::operator ++ () -> ArrayPriorityQueue<T,tgt>::Iterator& {
  if (expected_mod_count != ref_pq->mod_count)
    throw ConcurrentModificationError("ArrayPriorityQueue::Iterator::operator ++");

  if (current < 0)
    return *this;

  if (can_erase)
    --current;         //decreasing priority goes backward in array, towards 0
  else
    can_erase = true;  //current already indexes "one beyond" deleted value

  return *this;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
auto ArrayPriorityQueue<T,tgt>::Iterator::operator ++ (int) -> ArrayPriorityQueue<T,tgt>::Iterator {
  if (expected_mod_count != ref_pq->mod_count)
    throw ConcurrentModificationError("ArrayPriorityQueue::Iterator::operator ++(int)");

  if (current < 0)
    return *this;

  Iterator to_return(*this);
  if (can_erase)
    --current;         //decreasing priority goes backward in array, towards 0
  else
    can_erase = true;  //current already indexes "one beyond" deleted value

  return to_return;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool ArrayPriorityQueue<T,tgt>::Iterator::operator == (const ArrayPriorityQueue<T,tgt>::Iterator& rhs) const {
  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
  if (rhsASI == 0)
    throw IteratorTypeError("ArrayPriorityQueue::Iterator::operator ==");
  if (expected_mod_count != ref_pq->mod_count)
    throw ConcurrentModificationError("ArrayPriorityQueue::Iterator::operator ==");
  if (ref_pq != rhsASI->ref_pq)
    throw ComparingDifferentIteratorsError("ArrayPriorityQueue::Iterator::operator ==");

  return current == rhsASI->current;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool ArrayPriorityQueue<T,tgt>::Iterator::operator != (const ArrayPriorityQueue<T,tgt>::Iterator& rhs) const {
  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
  if (rhsASI == 0)
    throw IteratorTypeError("ArrayPriorityQueue::Iterator::operator !=");
  if (expected_mod_count != ref_pq->mod_count)
    throw ConcurrentModificationError("ArrayPriorityQueue::Iterator::operator !=");
  if (ref_pq != rhsASI->ref_pq)
    throw ComparingDifferentIteratorsError("ArrayPriorityQueue::Iterator::operator !=");

  return current != rhsASI->current;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
T& ArrayPriorityQueue<T,tgt>::Iterator::operator *() const {
  if (expected_mod_count != ref_pq->mod_count)
    throw ConcurrentModificationError("ArrayPriorityQueue::Iterator::operator *");
  if (!can_erase || current < 0 || current >= ref_pq->used) {
    std::ostringstream where;
    where << current << " when size = " << ref_pq->size();
    throw IteratorPositionIllegal("ArrayPriorityQueue::Iterator::operator * Iterator illegal: "+where.str());
  }

  return ref_pq->pq[current];
}


template<class T, bool (*tgt)(const T& a, const T& b)>
T* ArrayPriorityQueue<T,tgt>::Iterator::operator ->() const {
  if (expected_mod_count !=  ref_pq->mod_count)
    throw ConcurrentModificationError("ArrayPriorityQueue::Iterator::operator ->");
  if (!can_erase || current < 0 || current >= ref_pq->used) {
    std::ostringstream where;
    where << current << " when size = " << ref_pq->size();
    throw IteratorPositionIllegal("ArrayPriorityQueue::Iterator::operator -> Iterator illegal: "+where.str());
  }

  return &ref_pq->pq[current];
}


}

#endif /* ARRAY_PRIORITY_QUEUE_HPP_ */
