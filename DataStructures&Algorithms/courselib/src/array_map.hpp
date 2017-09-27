#ifndef ARRAY_MAP_HPP_
#define ARRAY_MAP_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include "pair.hpp"


namespace ics {


template<class KEY,class T> class ArrayMap {
  public:
    typedef pair<KEY,T> Entry;

    //Destructor/Constructors
    ~ArrayMap ();

	  ArrayMap          ();
	  explicit ArrayMap (int initial_length);
	  ArrayMap          (const ArrayMap<KEY,T>& to_copy);
	  explicit ArrayMap (const std::initializer_list<Entry>& il);

    //Iterable class must support "for-each" loop: .begin()/.end()/.size() and prefix ++ on returned result
    template <class Iterable>
    explicit ArrayMap (const Iterable& i);


    //Queries
    bool empty     () const;
    int  size       () const;
    bool has_key    (const KEY& key) const;
    bool has_value  (const T& value) const;
    std::string str () const; //supplies useful debugging information; contrast to operator <<


    //Commands
    T    put   (const KEY& key, const T& value);
    T    erase (const KEY& key);
    void clear ();

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    int put_all (const Iterable& i);


    //Operators
    T&       operator [] (const KEY&);
    const T& operator [] (const KEY&) const;
    ArrayMap<KEY,T>& operator = (const ArrayMap<KEY,T>& rhs);
    bool operator == (const ArrayMap<KEY,T>& rhs) const;
    bool operator != (const ArrayMap<KEY,T>& rhs) const;

    template<class KEY2,class T2>
    friend std::ostream& operator << (std::ostream& outs, const ArrayMap<KEY2,T2>& m);



    class Iterator {
      public:
        //Private constructor called in begin/end, which are friends of ArrayMap<T>
        ~Iterator();
        Entry       erase();
        std::string str  () const;
        ArrayMap<KEY,T>::Iterator& operator ++ ();
        ArrayMap<KEY,T>::Iterator  operator ++ (int);
        bool operator == (const ArrayMap<KEY,T>::Iterator& rhs) const;
        bool operator != (const ArrayMap<KEY,T>::Iterator& rhs) const;
        Entry& operator *  () const;
        Entry* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const ArrayMap<KEY,T>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }
        friend Iterator ArrayMap<KEY,T>::begin () const;
        friend Iterator ArrayMap<KEY,T>::end   () const;

      private:
        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
        int              current;
        ArrayMap<KEY,T>* ref_map;
        int              expected_mod_count;
        bool             can_erase = true;

        //Called in friends begin/end
        Iterator(ArrayMap<KEY,T>* iterate_over, int initial);
    };


    Iterator begin () const;
    Iterator end   () const;


    private:
      Entry* map;        //Unordered contiguous array
      int length    = 0; //Physical length of array: must be >= .size()
      int used      = 0; //Amount of array used
      int mod_count = 0; //For sensing concurrent modification

      //Helper methods
      int  index_of     (const KEY& key) const;
      T    change_at    (int i, const T& value);
      T    erase_at     (int i);
      void ensure_length(int new_length);
  };





////////////////////////////////////////////////////////////////////////////////
//
//ArrayMap class and related definitions

//Destructor/Constructors

template<class KEY,class T>
ArrayMap<KEY,T>::~ArrayMap() {
  delete[] map;
}


template<class KEY,class T>
ArrayMap<KEY,T>::ArrayMap() {
  map = new Entry[length];
}


template<class KEY,class T>
ArrayMap<KEY,T>::ArrayMap(int initial_length)
: length(initial_length) {
  if (length < 0)
    length = 0;
  map = new Entry[length];
}


template<class KEY,class T>
ArrayMap<KEY,T>::ArrayMap(const ArrayMap<KEY,T>& to_copy)
: length(to_copy.length), used(to_copy.used) {
  map = new Entry[length];
  for (int i=0; i<to_copy.used; ++i)
    map[i] = to_copy.map[i];
}


template<class KEY,class T>
ArrayMap<KEY,T>::ArrayMap(const std::initializer_list<Entry>& il)
: length(il.size()) {
  map = new Entry[length];
  for (const Entry& m_entry : il)
    put(m_entry.first,m_entry.second);
}


template<class KEY,class T>
template <class Iterable>
ArrayMap<KEY,T>::ArrayMap(const Iterable& i)
: length(i.size()) {
  map = new Entry[length];
  for (const Entry& m_entry : i)
    put(m_entry.first,m_entry.second);
}


////////////////////////////////////////////////////////////////////////////////
//
//Queries

template<class KEY,class T>
bool ArrayMap<KEY,T>::empty() const {
  return used == 0;
}


template<class KEY,class T>
int ArrayMap<KEY,T>::size() const {
  return used;
}


template<class KEY,class T>
bool ArrayMap<KEY,T>::has_key (const KEY& element) const {
  for (int i=0; i<used; ++i)
    if (map[i].first == element)
      return true;

  return false;
}


template<class KEY,class T>
bool ArrayMap<KEY,T>::has_value (const T& element) const {
  for (int i=0; i<used; ++i)
    if (map[i].second == element)
      return true;

  return false;
}


template<class KEY,class T>
std::string ArrayMap<KEY,T>::str() const {
  std::ostringstream answer;
  answer << "ArrayMap[";

  if (length != 0) {
    answer << "0:" << map[0].first << "->" << map[0].second;
    for (int i = 1; i < length; ++i)
      answer << "," << i << ":" << map[i].first << "->" << map[i].second;
  }

  answer << "](length=" << length << ",used=" << used << ",mod_count=" << mod_count << ")";
  return answer.str();
}


////////////////////////////////////////////////////////////////////////////////
//
//Commands

template<class KEY,class T>
T ArrayMap<KEY,T>::put(const KEY& key, const T& value) {
  int i = index_of(key);
  if (i != -1)
    return change_at(i,value);

  this->ensure_length(used+1);
  map[used++] = Entry(key,value);
  ++mod_count;
  return map[used-1].second;
}


template<class KEY,class T>
T ArrayMap<KEY,T>::erase(const KEY& key) {
  int i = index_of(key);
  if (i != -1)
    return erase_at(i);

  std::ostringstream answer;
  answer << "ArrayMap::erase: key(" << key << ") not in Map";
  throw KeyError(answer.str());
}


template<class KEY,class T>
void ArrayMap<KEY,T>::clear() {
  used = 0;
  ++mod_count;
}


template<class KEY,class T>
template<class Iterable>
int ArrayMap<KEY,T>::put_all(const Iterable& i) {
  int count = 0;
  for (auto m_entry : i) {
    ++count;
    put(m_entry.first, m_entry.second);
  }

    return count;
}


////////////////////////////////////////////////////////////////////////////////
//
//Operators

template<class KEY,class T>
T& ArrayMap<KEY,T>::operator [] (const KEY& key) {
  int i = index_of(key);
  if (i != -1)
    return map[i].second;

  this->ensure_length(used+1);
  map[used++] = Entry(key,T());
  ++mod_count;
  return map[used-1].second;
}


template<class KEY,class T>
const T& ArrayMap<KEY,T>::operator [] (const KEY& key) const {
  int i = index_of(key);
  if (i != -1)
    return map[i].second;

  std::ostringstream answer;
  answer << "ArrayMap::operator []: key(" << key << ") not in Map";
  throw KeyError(answer.str());
}


template<class KEY,class T>
ArrayMap<KEY,T>& ArrayMap<KEY,T>::operator = (const ArrayMap<KEY,T>& rhs) {
  if (this == &rhs)
    return *this;

  this->ensure_length(rhs.used);
  used = rhs.used;
  for (int i=0; i<used; ++i)
    map[i] = rhs.map[i];

  ++mod_count;
  return *this;
}


template<class KEY,class T>
bool ArrayMap<KEY,T>::operator == (const ArrayMap<KEY,T>& rhs) const {
  if (this == &rhs)
    return true;

  if (used != rhs.size())
    return false;

  for (int i=0; i<used; ++i)
    // Uses ! and ==, so != on T need not be defined
    if (!rhs.has_key(map[i].first) || !(map[i].second == rhs[map[i].first]))
      return false;

  return true;
}


template<class KEY,class T>
bool ArrayMap<KEY,T>::operator != (const ArrayMap<KEY,T>& rhs) const {
  return !(*this == rhs);
}



template<class KEY,class T>
std::ostream& operator << (std::ostream& outs, const ArrayMap<KEY,T>& m) {
  outs << "map[";

  if (!m.empty()) {
    outs << m.map[0].first << "->" << m.map[0].second;
    for (int i = 1; i < m.used; ++i) {
      outs << "," << m.map[i].first << "->" << m.map[i].second;
    }
  }

  outs << "]";
  return outs;
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors

template<class KEY,class T>
auto ArrayMap<KEY,T>::begin () const -> ArrayMap<KEY,T>::Iterator {
   return Iterator(const_cast<ArrayMap<KEY,T>*>(this),0);
}


template<class KEY,class T>
auto ArrayMap<KEY,T>::end () const -> ArrayMap<KEY,T>::Iterator {
  return Iterator(const_cast<ArrayMap<KEY,T>*>(this),used);
}


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

template<class KEY,class T>
int ArrayMap<KEY,T>::index_of(const KEY& key) const {
  for (int i=0; i<used; ++i)
    if (map[i].first == key)
      return i;

  return -1;
}


template<class KEY,class T>
T ArrayMap<KEY,T>::change_at(int i, const T& value) {
  T old_value = map[i].second;
  map[i].second = value;
  ++mod_count;
  return old_value;
}


template<class KEY,class T>
T ArrayMap<KEY,T>::erase_at(int i) {
  T erased = map[i].second;
  map[i] = map[--used];
  ++mod_count;
  return erased;
}


template<class KEY,class T>
void ArrayMap<KEY,T>::ensure_length(int new_length) {
  if (length >= new_length)
    return;
  Entry* old_map = map;
  length = std::max(new_length,2*length);
  map = new Entry[length];
  for (int i=0; i<used; ++i)
    map[i] = old_map[i];

  delete [] old_map;
}


template<class KEY,class T>
ArrayMap<KEY,T>::Iterator::Iterator(ArrayMap<KEY,T>* iterate_over, int initial) : current(initial), ref_map(iterate_over) {
  expected_mod_count = ref_map->mod_count;
}





////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

template<class KEY,class T>
ArrayMap<KEY,T>::Iterator::~Iterator()
{}

template<class KEY,class T>
auto ArrayMap<KEY,T>::Iterator::erase() -> Entry {
  if (expected_mod_count != ref_map->mod_count)
    throw ConcurrentModificationError("ArrayMap::Iterator::erase");
  if (!can_erase)
    throw CannotEraseError("ArrayMap::Iterator::erase Iterator cursor already erased");
  if (current < 0 || current >= ref_map->used)
    throw CannotEraseError("ArrayMap::Iterator::erase Iterator cursor beyond data structure");

  can_erase = false;
  Entry to_return = ref_map->map[current];
  ref_map->erase_at(current);
  expected_mod_count = ref_map->mod_count;
  return to_return;
}


template<class KEY,class T>
std::string ArrayMap<KEY,T>::Iterator::str() const {
  std::ostringstream answer;
  answer << ref_map->str() << "(current=" << current << ",expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";
  return answer.str();
}


template<class KEY,class T>
auto ArrayMap<KEY,T>::Iterator::operator ++ () -> ArrayMap<KEY,T>::Iterator& {
  if (expected_mod_count != ref_map->mod_count)
    throw ConcurrentModificationError("ArrayMap::Iterator::operator ++");

  if (current >= ref_map->used)
    return *this;

  if (can_erase)
    ++current;
  else
    can_erase = true;  //current already indexes "one beyond" deleted value

  return *this;
}


template<class KEY,class T>
auto ArrayMap<KEY,T>::Iterator::operator ++ (int) -> ArrayMap<KEY,T>::Iterator {
  if (expected_mod_count != ref_map->mod_count)
    throw ConcurrentModificationError("ArrayMap::Iterator::operator ++(int)");

  if (current >= ref_map->used)
    return *this;

  Iterator to_return(*this);
  if (can_erase)
    ++current;
  else
    can_erase = true;  //current already indexes "one beyond" deleted value

  return to_return;
}


template<class KEY,class T>
bool ArrayMap<KEY,T>::Iterator::operator == (const ArrayMap<KEY,T>::Iterator& rhs) const {
  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
  if (rhsASI == 0)
    throw IteratorTypeError("ArrayMap::Iterator::operator ==");
  if (expected_mod_count != ref_map->mod_count)
    throw ConcurrentModificationError("ArrayMap::Iterator::operator ==");
  if (ref_map != rhsASI->ref_map)
    throw ComparingDifferentIteratorsError("ArrayMap::Iterator::operator ==");

  return current == rhsASI->current;
}


template<class KEY,class T>
bool ArrayMap<KEY,T>::Iterator::operator != (const ArrayMap<KEY,T>::Iterator& rhs) const {
  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
  if (rhsASI == 0)
    throw IteratorTypeError("ArrayMap::Iterator::operator !=");
  if (expected_mod_count != ref_map->mod_count)
    throw ConcurrentModificationError("ArrayMap::Iterator::operator !=");
  if (ref_map != rhsASI->ref_map)
    throw ComparingDifferentIteratorsError("ArrayMap::Iterator::operator !=");

  return current != rhsASI->current;
}


template<class KEY,class T>
pair<KEY,T>& ArrayMap<KEY,T>::Iterator::operator *() const {
  if (expected_mod_count != ref_map->mod_count)
    throw ConcurrentModificationError("ArrayMap::Iterator::operator *");
  if (!can_erase || current < 0 || current >= ref_map->used) {
    std::ostringstream where;
    where << current << " when size = " << ref_map->size();
    throw IteratorPositionIllegal("ArrayMap::Iterator::operator * Iterator illegal: "+where.str());
  }

  return ref_map->map[current];
}


template<class KEY,class T>
pair<KEY,T>* ArrayMap<KEY,T>::Iterator::operator ->() const {
  if (expected_mod_count != ref_map->mod_count)
    throw ConcurrentModificationError("ArrayMap::Iterator::operator ->");
  if (!can_erase || current < 0 || current >= ref_map->used) {
    std::ostringstream where;
    where << current << " when size = " << ref_map->size();
    throw IteratorPositionIllegal("ArrayMap::Iterator::operator -> Iterator illegal: "+where.str());
  }

  return &(ref_map->map[current]);
}


}

#endif /* ARRAY_MAP_HPP_ */
