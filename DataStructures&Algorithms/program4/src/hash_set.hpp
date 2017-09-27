#ifndef HASH_SET_HPP_
#define HASH_SET_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include "pair.hpp"
#include "iterator.hpp"
#include "set.hpp"


namespace ics {

template<class T> class HashSet : public Set<T>	{
  public:
    HashSet() = delete;
    HashSet(int (*ahash)(const T& element), double the_load_factor = 1.0);
    HashSet(int initial_bins, int (*ahash)(const T& element), double the_load_factor = 1.0);
    HashSet(const HashSet<T>& to_copy);
    //HashSet(std::initializer_list<Entry> il, int (*ahash)(const T& element), double the_load_factor = 1.0);
    HashSet(ics::Iterator<T>& start, const ics::Iterator<T>& stop, int (*ahash)(const T& element), double the_load_factor = 1.0);
    virtual ~HashSet();

    virtual bool empty      () const;
    virtual int  size       () const;
    virtual bool contains   (const T& element) const;
    virtual std::string str () const;

    virtual bool contains (ics::Iterator<T>& start, const ics::Iterator<T>& stop) const;

    virtual int  insert (const T& element);
    virtual int  erase  (const T& element);
    virtual void clear  ();

    virtual int insert (ics::Iterator<T>& start, const ics::Iterator<T>& stop);
    virtual int erase  (ics::Iterator<T>& start, const ics::Iterator<T>& stop);
    virtual int retain (ics::Iterator<T>& start, const ics::Iterator<T>& stop);

    virtual HashSet<T>& operator = (const HashSet<T>& rhs);
    virtual bool operator == (const Set<T>& rhs) const;
    virtual bool operator != (const Set<T>& rhs) const;
    virtual bool operator <= (const Set<T>& rhs) const;
    virtual bool operator <  (const Set<T>& rhs) const;
    virtual bool operator >= (const Set<T>& rhs) const;
    virtual bool operator >  (const Set<T>& rhs) const;

    template<class T2>
    friend std::ostream& operator << (std::ostream& outs, const HashSet<T2>& s);

    virtual ics::Iterator<T>& abegin () const;
    virtual ics::Iterator<T>& aend   () const;

  private:
    class LN;

  public:
    class Iterator : public ics::Iterator<T> {
      public:
        //KLUDGE should be callable only in begin/end
        Iterator(HashSet<T>* fof, bool begin);
        Iterator(const Iterator& i);
        virtual ~Iterator();
        virtual T           erase();
        virtual std::string str  () const;
        virtual const ics::Iterator<T>& operator ++ ();
        virtual const ics::Iterator<T>& operator ++ (int);
        virtual bool operator == (const ics::Iterator<T>& rhs) const;
        virtual bool operator != (const ics::Iterator<T>& rhs) const;
        virtual T& operator *  () const;
        virtual T* operator -> () const;
      private:
        ics::pair<int,LN*> current; //Bin Index and Cursor; stop: LN* == nullptr
        HashSet<T>*        ref_set;
        int                expected_mod_count;
        bool               can_erase = true;
        void advance_cursors();
    };

    virtual Iterator begin () const;
    virtual Iterator end   () const;

  private:
    class LN {
      public:
        LN ()                      : next(nullptr){}
        LN (const LN& ln)          : value(ln.value), next(ln.next){}
        LN (T v,  LN* n = nullptr) : value(v), next(n){}

        T   value;
        LN* next;
    };

    LN** set      = nullptr;
    int (*hash)(const T& element);
    double load_factor;//used/bins <= load_factor
    int bins      = 1; //# bins in array
    int used      = 0; //# of key->value pairs in the hash table
    int mod_count = 0; //For sensing concurrent modification
    int   hash_compress (const T& element) const;
    void  ensure_load_factor(int new_used);
    LN*   find_element (int bin, const T& element) const;
    LN*   copy_list(LN*   l) const;
    LN**  copy_hash_table(LN** ht, int bins) const;
    void  delete_hash_table(LN**& ht, int bins);
  };





template<class T>
HashSet<T>::HashSet(int (*ahash)(const T& element), double the_load_factor) : hash(ahash), load_factor(the_load_factor) {
	set = new LN*[bins];
	for(int i = 0; i < bins; i++){
		set[i] = new LN();
	}
}

template<class T>
HashSet<T>::HashSet(int initial_bins, int (*ahash)(const T& element), double the_load_factor) : bins(initial_bins), hash(ahash), load_factor(the_load_factor) {
	if(bins < 1){
		bins = 1;
	}
	set = new LN*[bins];
	for(int i = 0; i < bins; i++){
		set[i] = new LN();
	}
}

template<class T>
HashSet<T>::HashSet(const HashSet<T>& to_copy) : hash(to_copy.hash), load_factor(to_copy.load_factor), bins(to_copy.bins), used(to_copy.used) {
	set = copy_hash_table(to_copy.set, bins);
}

template<class T>
HashSet<T>::HashSet(ics::Iterator<T>& start, const ics::Iterator<T>& stop, int (*ahash)(const T& element), double the_load_factor) : hash(ahash), load_factor(the_load_factor) {
	set = new LN*[bins];
	for(int i = 0; i < bins; i++){
		set[i] = new LN();
	}
	insert(start, stop);
}

//KLUDGE: Not usable yet
//template<class KEY,class T>
//HashSet<T>::HashSet(std::initializer_list<Entry> i,int (*ahash)(const KEY& k), double the_load_factor = 1.0) {
//  set = new LN*[bins];
//  put(il.abegin(),il.aend());
//}

template<class T>
HashSet<T>::~HashSet() {
	delete_hash_table(set, bins);
}


template<class T>
inline bool HashSet<T>::empty() const {
	return used == 0;
}

template<class T>
int HashSet<T>::size() const {
	return used;
}

template<class T>
bool HashSet<T>::contains (const T& element) const {
	return (find_element(hash_compress(element), element) == nullptr ? false : true);
}

template<class T>
std::string HashSet<T>::str() const {
	std::ostringstream answer;
	answer << *this << "(used=" << used << ",mod_count=" << mod_count << ")";
	return answer.str();
}

template<class T>
bool HashSet<T>::contains(ics::Iterator<T>& start, const ics::Iterator<T>& stop) const {
	for(; start != stop; ++start){
		if(!contains(*start)){
			return false;
		}
	}
	return true;
}

template<class T>
int HashSet<T>::insert(const T& element) {
	int index = hash_compress(element);
	LN* temp = find_element(index, element);
	if(temp != nullptr){
	      return 0;
	}

	ensure_load_factor(++used);
	index = hash_compress(element);
	set[index] = new LN(element, set[index]);
	++mod_count;
	return 1;
}

template<class T>
int HashSet<T>::erase(const T& element) {
	int index = hash_compress(element);
	LN* temp = find_element(index, element);
	if(temp != nullptr){
		LN* temp2 = temp->next;
		*temp = *(temp->next);
		delete temp2;
		used--;
		mod_count++;
		return 1;
	}
	return 0;
}

template<class T>
void HashSet<T>::clear() {
	for(int i = 0; i<bins; i++)
	{
		for (LN* temp = set[i]; set[i]->next != nullptr; temp = set[i])
		{
			set[i] = set[i]->next;
			delete temp;
			--used;
		}
	}
}

template<class T>
int HashSet<T>::insert(ics::Iterator<T>& start, const ics::Iterator<T>& stop) {
	 int count = 0;
	 for (; start != stop; ++start){
	   count += insert(*start);
	 }
	 return count;
}

template<class T>
int HashSet<T>::erase(ics::Iterator<T>& start, const ics::Iterator<T>& stop) {
	 int count = 0;
	 for (; start != stop; ++start){
	   count += erase(*start);
	 }
	 return count;
}

template<class T>
int HashSet<T>::retain(ics::Iterator<T>& start, const ics::Iterator<T>& stop) {
	HashSet<T> s(start, stop, hash, load_factor);
	int count = 0;
	for (int i = 0; i < bins; i++){
		for(LN* temp = set[i]; temp->next != nullptr; ){
			if (!s.contains(temp->value)) {
				LN* temp2 = temp->next;
				erase(temp->value);
				temp = temp2;
			}
			else{
				++count;
				temp = temp->next;
			}
		}
	}
	return count;
}

template<class T>
HashSet<T>& HashSet<T>::operator = (const HashSet<T>& rhs) {
	if (this == &rhs)
	{
		return *this;
	}
	delete_hash_table(set, bins);
	bins = rhs.bins;
	used = rhs.used;
	set = copy_hash_table(rhs.set, rhs.bins);
	++mod_count;
	return *this;
}

template<class T>
bool HashSet<T>::operator == (const Set<T>& rhs) const {
	if (this == &rhs){
		return true;
	}
	if (used != rhs.size()){
		return false;
	}
	for (int bucket = 0; bucket < bins; bucket++){
		LN* node = set[bucket];
		while (node->next != nullptr){
			if (!rhs.contains(node->value)){
				return false;
			}
			node = node->next;
		}
	}
	return true;
}

template<class T>
bool HashSet<T>::operator != (const Set<T>& rhs) const {
	return !(*this == rhs);
}

template<class T>
bool HashSet<T>::operator <= (const Set<T>& rhs) const {
  return *this < rhs || *this == rhs;
}

template<class T>
bool HashSet<T>::operator < (const Set<T>& rhs) const {
	if (this == &rhs){
		return false;
	}
	if (used >= rhs.size()){
		return false;
	}
	for (int i=0; i<bins; i++){
		LN* node = set[i];
		while(node->next != nullptr){
			if (!rhs.contains(node->value)){
				return false;
			}
			node = node->next;
		}
	}
	  return true;
}

template<class T>
bool HashSet<T>::operator >= (const Set<T>& rhs) const {
	return !(*this < rhs);
}

template<class T>
bool HashSet<T>::operator > (const Set<T>& rhs) const {
  return !(*this < rhs) && !(*this == rhs);
}

template<class T>
std::ostream& operator << (std::ostream& outs, const HashSet<T>& s) {
	if (s.empty()){
		outs << "set[]";
	}
	else
	{
		outs << "set[";
		for(ics::Iterator<T>& i = s.abegin(); i != s.aend();)
		{
			outs <<*i;
			outs << (++i == s.aend() ? "]" : ",");
		}
	}
	return outs;
}

//KLUDGE: memory-leak
template<class T>
auto HashSet<T>::abegin () const -> ics::Iterator<T>& {
  return *(new Iterator(const_cast<HashSet<T>*>(this),true));
}

//KLUDGE: memory-leak
template<class T>
auto HashSet<T>::aend () const -> ics::Iterator<T>& {
  return *(new Iterator(const_cast<HashSet<T>*>(this),false));
}

template<class T>
auto HashSet<T>::begin () const -> HashSet<T>::Iterator {
  return Iterator(const_cast<HashSet<T>*>(this),true);
}

template<class T>
auto HashSet<T>::end () const -> HashSet<T>::Iterator {
  return Iterator(const_cast<HashSet<T>*>(this),false);
}

template<class T>
int HashSet<T>::hash_compress (const T& element) const {
	return std::abs(hash(element)) % bins;
}

template<class T>
void HashSet<T>::ensure_load_factor(int new_used) {
	double current_load_factor = new_used/bins;
	if(current_load_factor > load_factor){
		int temp_bin = bins;
		LN** old_hash = copy_hash_table(set,bins);
		delete_hash_table(set, bins);
		bins *= 2;
		set = new LN*[bins];
		for(int i = 0; i < bins; i++){
			set[i] = new LN();
		}
		for(int b = 0; b<temp_bin; b++){
			while(old_hash[b]->next != nullptr){
				int index = hash_compress(old_hash[b]->value);  // change here for set
				LN* previous_front = set[index];
				set[index] = old_hash[b];
				old_hash[b] = old_hash[b]->next;
				set[index]->next = previous_front;
				++used;
			}
			delete old_hash[b];
		}
	}
}

template<class T>
typename HashSet<T>::LN* HashSet<T>::find_element (int bin, const T& element) const {
  for (LN* c = set[bin]; c->next!=nullptr; c=c->next)
    if (element == c->value)
      return c;

  return nullptr;
}

template<class T>
typename HashSet<T>::LN* HashSet<T>::copy_list (LN* l) const {
  if (l == nullptr)
    return nullptr;
  else
    return new LN(l->value, copy_list(l->next));
}

template<class T>
typename HashSet<T>::LN** HashSet<T>::copy_hash_table (LN** ht, int bins) const {
	LN** new_table = new LN*[bins];
	for(int i = 0; i < bins; i++){
		new_table[i] = copy_list(ht[i]);
	}
	return new_table;
}

template<class T>
void HashSet<T>::delete_hash_table (LN**& ht, int bins) {
	for(int i = 0; i<bins; i++){
		for (LN* temp = ht[i]; ht[i]->next != nullptr; temp = ht[i])
		{
			ht[i] = ht[i]->next;
			delete temp;
			--used;
		}
		delete ht[i];
	}
	delete[] ht;
}


template<class T>
void HashSet<T>::Iterator::advance_cursors(){
  //write code here
	if (current.second->next != nullptr)
	{
		current.second = current.second->next;
		if (current.second->next == nullptr)
		{
			advance_cursors();
		}
	}
	else
	{
		for (++current.first ; current.first < ref_set->bins; ++current.first)
		{
			current.second = ref_set->set[current.first];
			if (current.second->next != nullptr)
			{
				break;
			}
		}
		if (current.first >= ref_set->bins)
		{
			current.first = -1;
			current.second = nullptr;
		}
	}
}




template<class T>
HashSet<T>::Iterator::Iterator(HashSet<T>* fof, bool begin) : ref_set(fof) {
	if(begin && !ref_set->empty())
	{
		current.first = 0;
		current.second = ref_set->set[0];
		if (current.second->next == nullptr)
		{
			advance_cursors();
		}
	}
	else
	{
		current.first = -1;
		current.second = nullptr;
	}
	expected_mod_count = ref_set->mod_count;
}


template<class T>
HashSet<T>::Iterator::Iterator(const Iterator& i) :
    current(i.current), ref_set(i.ref_set), expected_mod_count(i.expected_mod_count), can_erase(i.can_erase) {}

template<class T>
HashSet<T>::Iterator::~Iterator() {}

template<class T>
T HashSet<T>::Iterator::erase() {
  if (expected_mod_count != ref_set->mod_count)
    throw ConcurrentModificationError("HashSet::Iterator::erase");
  if (!can_erase)
    throw CannotEraseError("HashSet::Iterator::erase Iterator cursor already erased");
  if (current.second == nullptr)
    throw CannotEraseError("HashSet::Iterator::erase Iterator cursor beyond data structure");

  //write code here
  auto to_return = current.second->value;
    LN* to_delete = current.second->next;
    *current.second = *(current.second->next);
    delete to_delete;
    if (current.second->next == nullptr)
    {
  	  can_erase = false;
    }
    --ref_set->used;
    ++ref_set->mod_count;
    expected_mod_count = ref_set->mod_count;
    return to_return;
}

template<class T>
std::string HashSet<T>::Iterator::str() const {
	  std::ostringstream answer;
	  answer << ref_set->str() << "(current=" << current.first << "/" << current.second << ",expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";
	  return answer.str();
}

template<class T>
const ics::Iterator<T>& HashSet<T>::Iterator::operator ++ () {
  if (expected_mod_count != ref_set->mod_count)
    throw ConcurrentModificationError("HashSet::Iterator::operator ++");
  if (can_erase && current.first < ref_set->bins)
  {
	  advance_cursors();
  }
  can_erase = true;
  return *this;
  //write code here
}

//KLUDGE: creates garbage! (can return local value!)
template<class T>
const ics::Iterator<T>& HashSet<T>::Iterator::operator ++ (int) {
  if (expected_mod_count != ref_set->mod_count)
    throw ConcurrentModificationError("HashSet::Iterator::operator ++(int)");

  //write code here
  Iterator* to_return = new Iterator(*this);
  if(can_erase && current.first < ref_set->bins)
  {
	  advance_cursors();
  }
  can_erase = true;
  return *to_return;
}

template<class T>
bool HashSet<T>::Iterator::operator == (const ics::Iterator<T>& rhs) const {
  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
  if (rhsASI == 0)
    throw IteratorTypeError("HashSet::Iterator::operator ==");
  if (expected_mod_count != ref_set->mod_count)
    throw ConcurrentModificationError("HashSet::Iterator::operator ==");
  if (ref_set != rhsASI->ref_set)
    throw ComparingDifferentIteratorsError("HashSet::Iterator::operator ==");

  //write code here
  return current.first == rhsASI->current.first && current.second == rhsASI->current.second;
}


template<class T>
bool HashSet<T>::Iterator::operator != (const ics::Iterator<T>& rhs) const {
  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
  if (rhsASI == 0)
    throw IteratorTypeError("HashSet::Iterator::operator !=");
  if (expected_mod_count != ref_set->mod_count)
    throw ConcurrentModificationError("HashSet::Iterator::operator !=");
  if (ref_set != rhsASI->ref_set)
    throw ComparingDifferentIteratorsError("HashSet::Iterator::operator !=");

  //write code here
  return current.first != rhsASI->current.first || current.second != rhsASI->current.second;
}

template<class T>
T& HashSet<T>::Iterator::operator *() const {
  if (expected_mod_count !=
      ref_set->mod_count)
    throw ConcurrentModificationError("HashSet::Iterator::operator *");
  if (!can_erase || current.second == nullptr)
    throw IteratorPositionIllegal("HashSet::Iterator::operator * Iterator illegal: exhausted");

  //write code here
  return current.second->value;
}

template<class T>
T* HashSet<T>::Iterator::operator ->() const {
  if (expected_mod_count !=
      ref_set->mod_count)
    throw ConcurrentModificationError("HashSet::Iterator::operator *");
  if (!can_erase || current.second == nullptr)
    throw IteratorPositionIllegal("HashSet::Iterator::operator * Iterator illegal: exhausted");

  //write code here
  return &(current.second->value);
}

}

#endif /* HASH_SET_HPP_ */
