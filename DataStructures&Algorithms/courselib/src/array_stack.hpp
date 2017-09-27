#ifndef ARRAY_STACK_HPP_
#define ARRAY_STACK_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"


namespace ics {


template<class T> class ArrayStack {
  public:
    //Destructor/Constructors
    ~ArrayStack();

    ArrayStack          ();
    explicit ArrayStack (int initial_length);
    ArrayStack          (const ArrayStack<T>&     to_copy);
    explicit ArrayStack (const std::initializer_list<T>& il);

    //Iterable class must support "for-each" loop: .begin()/.end()/.size() and prefix ++ on returned result
    template <class Iterable>
    explicit ArrayStack (const Iterable& i);


    //Queries
    bool empty      () const;
    int  size       () const;
    T&   peek       () const;
    std::string str () const; //supplies useful debugging information; contrast to operator <<


    //Commands
    int  push (const T& element);
    T    pop  ();
    void clear();

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    int push_all(const Iterable& i);


    //Operators
    ArrayStack<T>& operator = (const ArrayStack<T>& rhs);
    bool operator == (const ArrayStack<T>& rhs) const;
    bool operator != (const ArrayStack<T>& rhs) const;

    template<class T2>
    friend std::ostream& operator << (std::ostream& outs, const ArrayStack<T2>& s);



    class Iterator {
      public:
        //Private constructor called in begin/end, which are friends of ArrayStack<T>
        ~Iterator();
        T           erase();
        std::string str  () const;
        ArrayStack<T>::Iterator& operator ++ ();
        ArrayStack<T>::Iterator  operator ++ (int);
        bool operator == (const ArrayStack<T>::Iterator& rhs) const;
        bool operator != (const ArrayStack<T>::Iterator& rhs) const;
        T& operator *  () const;
        T* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const ArrayStack<T>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }
        friend Iterator ArrayStack<T>::begin () const;
        friend Iterator ArrayStack<T>::end   () const;

      private:
        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
        int            current;
        ArrayStack<T>* ref_stack;
        int            expected_mod_count;
        bool           can_erase = true;

        //Called in friends begin/end
        Iterator(ArrayStack<T>* iterate_over, int initial);
    };


    Iterator begin () const;
    Iterator end   () const;


    private:
      T*  stack;         //Bottom of stack is at index 0, grows to bigger indexes
      int length    = 0; //Physical length of array: must be >= .size()
      int used      = 0; //Amount of array used: invariant: 0 <= used <= length
      int mod_count = 0; //For sensing attempts to concurrently modify stack during iteration

      //Helper methods
      int erase_at      (int i);
      void ensure_length(int new_length);
  };





////////////////////////////////////////////////////////////////////////////////
//
//ArrayStack class and related definitions

//Destructor/Constructors

template<class T>
ArrayStack<T>::~ArrayStack() {
  delete[] stack;
}


template<class T>
ArrayStack<T>::ArrayStack() {
  stack = new T[length];
}


template<class T>
ArrayStack<T>::ArrayStack(int initial_length)
: length(initial_length) {
  if (length < 0)
    length = 0;
  stack = new T[length];
}

template<class T>
ArrayStack<T>::ArrayStack(const ArrayStack<T>& to_copy)
: length(to_copy.length), used(to_copy.used) {
  stack = new T[length];
  for (int i=0; i<to_copy.used; ++i)
    stack[i] = to_copy.stack[i];
}


template<class T>
ArrayStack<T>::ArrayStack(const std::initializer_list<T>& il)
: length(il.size()) {
  stack = new T[length];
  for (const T& s_elem : il)
    push(s_elem);
}


template<class T>
template<class Iterable>
ArrayStack<T>::ArrayStack(const Iterable& i)
: length(i.size()) {
  stack = new T[length];
  for (const T& v : i)
    push(v);
}


////////////////////////////////////////////////////////////////////////////////
//
//Queries

template<class T>
bool ArrayStack<T>::empty() const {
  return used == 0;
}


template<class T>
int ArrayStack<T>::size() const {
  return used;
}


template<class T>
T& ArrayStack<T>::peek () const {
  if (empty())
    throw EmptyError("ArrayStack::peek");

  return stack[used-1];
}


template<class T>
std::string ArrayStack<T>::str() const {
  std::ostringstream answer;
  answer << "ArrayStack[";

  if (length != 0) {
    answer << "0:" << stack[0];
    for (int i = 1; i < length; ++i)
      answer << "," << i << ":" << stack[i];
  }

  answer << "](length=" << length << ",used=" << used << ",mod_count=" << mod_count << ")";
  return answer.str();
}


////////////////////////////////////////////////////////////////////////////////
//
//Commands

template<class T>
int ArrayStack<T>::push(const T& element) {
  this->ensure_length(used+1);
  stack[used++] = element;
  ++mod_count;
  return 1;
}


template<class T>
T ArrayStack<T>::pop() {
  if (this->empty())
    throw EmptyError("ArrayStack::pop");

  ++mod_count;
  return stack[--used];
}


template<class T>
void ArrayStack<T>::clear() {
  used = 0;
  ++mod_count;
}


template<class T>
template<class Iterable>
int ArrayStack<T>::push_all(const Iterable& i) {
  int count = 0;
  for (const T& v : i)
     count += push(v);

    return count;
}


////////////////////////////////////////////////////////////////////////////////
//
//Operators

template<class T>
ArrayStack<T>& ArrayStack<T>::operator = (const ArrayStack<T>& rhs) {
  if (this == &rhs)
    return *this;

  this->ensure_length(rhs.used);
  used = rhs.used;
  for (int i=0; i<used; ++i)
    stack[i] = rhs.stack[i];

  ++mod_count;
  return *this;
}


template<class T>
bool ArrayStack<T>::operator == (const ArrayStack<T>& rhs) const {
  if (this == &rhs)
    return true;

  if (used != rhs.size())
    return false;

  ArrayStack<T>::Iterator rhs_i = rhs.begin();
  // Uses ! and ==, so != on T need not be defined
  for (int i=used-1; i>=0; --i,++rhs_i)
    if (!(stack[i] == *rhs_i))
      return false;

  return true;
}


template<class T>
bool ArrayStack<T>::operator != (const ArrayStack<T>& rhs) const {
  return !(*this == rhs);
}


template<class T>
std::ostream& operator << (std::ostream& outs, const ArrayStack<T>& s) {
  outs << "stack[";

  if (!s.empty()) {
    outs << s.stack[0];
    for (int i=1; i<s.used; ++i)
      outs << "," << s.stack[i];
  }

  outs << "]:top";
  return outs;
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors

template<class T>
auto ArrayStack<T>::begin () const -> ArrayStack<T>::Iterator {
  return Iterator(const_cast<ArrayStack<T>*>(this),used-1);
}


template<class T>
auto ArrayStack<T>::end () const -> ArrayStack<T>::Iterator {
  return Iterator(const_cast<ArrayStack<T>*>(this),-1);
}


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

template<class T>
int ArrayStack<T>::erase_at(int i) {
  for (int j=i; j<used-1; ++j)
    stack[j] = stack[j+1];
  --used;
  ++mod_count;
  return 1;
}


template<class T>
void ArrayStack<T>::ensure_length(int new_length) {
  if (length >= new_length)
    return;
  T* old_stack = stack;
  length = std::max(new_length,2*length);
  stack = new T[length];
  for (int i=0; i<used; ++i)
    stack[i] = old_stack[i];

  delete [] old_stack;
}





////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

template<class T>
ArrayStack<T>::Iterator::Iterator(ArrayStack<T>* iterate_over, int initial)
: current(initial), ref_stack(iterate_over), expected_mod_count(ref_stack->mod_count) {
}


template<class T>
ArrayStack<T>::Iterator::~Iterator()
{}


template<class T>
T ArrayStack<T>::Iterator::erase() {
  if (expected_mod_count != ref_stack->mod_count)
    throw ConcurrentModificationError("ArrayStack::Iterator::erase");
  if (!can_erase)
    throw CannotEraseError("ArrayStack::Iterator::erase Iterator cursor already erased");
  if (current < 0 || current >= ref_stack->used)
    throw CannotEraseError("ArrayStack::Iterator::erase Iterator cursor beyond data structure");

  can_erase = false;
  T to_return = ref_stack->stack[current];
  ref_stack->erase_at(current);
  --current;
  expected_mod_count = ref_stack->mod_count;
  return to_return;
}


template<class T>
std::string ArrayStack<T>::Iterator::str() const {
  std::ostringstream answer;
  answer << ref_stack->str() << "(current=" << current << ",expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";
  return answer.str();
}


template<class T>
 auto ArrayStack<T>::Iterator::operator ++ () -> ArrayStack<T>::Iterator& {
  if (expected_mod_count != ref_stack->mod_count)
    throw ConcurrentModificationError("ArrayStack::Iterator::operator ++");

  if (current < 0)
    return *this;

  if (can_erase)
    --current;         //decreasing priority goes backward in array, towards 0
  else
    can_erase = true;  //current already indexes "one beyond" deleted value

  return *this;
}


template<class T>
auto ArrayStack<T>::Iterator::operator ++ (int) -> ArrayStack<T>::Iterator {
  if (expected_mod_count != ref_stack->mod_count)
    throw ConcurrentModificationError("ArrayStack::Iterator::operator ++(int)");

  if (current < 0)
    return *this;

  Iterator to_return(*this);
  if (can_erase)
    --current;         //decreasing priority goes backward in array, towards 0
  else
    can_erase = true;  //current already indexes "one beyond" deleted value

  return to_return;
}


template<class T>
bool ArrayStack<T>::Iterator::operator == (const ArrayStack<T>::Iterator& rhs) const {
  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
  if (rhsASI == 0)
    throw IteratorTypeError("ArrayStack::Iterator::operator ==");
  if (expected_mod_count != ref_stack->mod_count)
    throw ConcurrentModificationError("ArrayStack::Iterator::operator ==");
  if (ref_stack != rhsASI->ref_stack)
    throw ComparingDifferentIteratorsError("ArrayStack::Iterator::operator ==");

  return current == rhsASI->current;
}


template<class T>
bool ArrayStack<T>::Iterator::operator != (const ArrayStack<T>::Iterator& rhs) const {
  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
  if (rhsASI == 0)
    throw IteratorTypeError("ArrayStack::Iterator::operator !=");
  if (expected_mod_count != ref_stack->mod_count)
    throw ConcurrentModificationError("ArrayStack::Iterator::operator !=");
  if (ref_stack != rhsASI->ref_stack)
    throw ComparingDifferentIteratorsError("ArrayStack::Iterator::operator !=");

  return current != rhsASI->current;
}


template<class T>
T& ArrayStack<T>::Iterator::operator *() const {
  if (expected_mod_count != ref_stack->mod_count)
    throw ConcurrentModificationError("ArrayStack::Iterator::operator *");
  if (!can_erase || current < 0 || current >= ref_stack->used) {
    std::ostringstream where;
    where << current << " when size = " << ref_stack->size();
    throw IteratorPositionIllegal("ArrayStack::Iterator::operator * Iterator illegal: "+where.str());
  }

  return ref_stack->stack[current];
}


template<class T>
T* ArrayStack<T>::Iterator::operator ->() const {
  if (expected_mod_count != ref_stack->mod_count)
    throw ConcurrentModificationError("ArrayStack::Iterator::operator ->");
  if (!can_erase || current < 0 || current >= ref_stack->used) {
    std::ostringstream where;
    where << current << " when size = " << ref_stack->size();
    throw IteratorPositionIllegal("ArrayStack::Iterator::operator -> Iterator illegal: "+where.str());
  }

  return &ref_stack->stack[current];
}


}

#endif /* ARRAY_STACK_HPP_ */
