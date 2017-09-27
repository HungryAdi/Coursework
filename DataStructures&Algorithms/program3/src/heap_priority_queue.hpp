//(Iyer, Aditya) asiyer
#ifndef HEAP_PRIORITY_QUEUE_HPP_
#define HEAP_PRIORITY_QUEUE_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include <utility>              //For std::swap function
#include "array_stack.hpp"      //See operator <<


namespace ics {


//Instantiate the templated class supplying tgt(a,b): true, iff a has higher priority than b.
//If tgt is defaulted to nullptr in the template, then a constructor must supply cgt.
//If both tgt and cgt are supplied, then they must be the same (by ==) function.
//If neither is supplied, or both are supplied but different, TemplateFunctionError is raised.
//The (unique) non-nullptr value supplied by tgt/cgt is stored in the instance variable gt.
template<class T, bool (*tgt)(const T& a, const T& b) = nullptr> class HeapPriorityQueue {
  public:
    //Destructor/Constructors
    ~HeapPriorityQueue();

    HeapPriorityQueue          (bool (*cgt)(const T& a, const T& b) = nullptr);
    explicit HeapPriorityQueue (int initial_length, bool (*cgt)(const T& a, const T& b));
    HeapPriorityQueue          (const HeapPriorityQueue<T,tgt>& to_copy, bool (*cgt)(const T& a, const T& b) = nullptr);
    explicit HeapPriorityQueue (const std::initializer_list<T>& il, bool (*cgt)(const T& a, const T& b) = nullptr);

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    explicit HeapPriorityQueue (const Iterable& i, bool (*cgt)(const T& a, const T& b) = nullptr);


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
    HeapPriorityQueue<T,tgt>& operator = (const HeapPriorityQueue<T,tgt>& rhs);
    bool operator == (const HeapPriorityQueue<T,tgt>& rhs) const;
    bool operator != (const HeapPriorityQueue<T,tgt>& rhs) const;

    template<class T2, bool (*gt2)(const T2& a, const T2& b)>
    friend std::ostream& operator << (std::ostream& outs, const HeapPriorityQueue<T2,gt2>& pq);



    class Iterator {
      public:
        //Private constructor called in begin/end, which are friends of HeapPriorityQueue<T,tgt>
        ~Iterator();
        T           erase();
        std::string str  () const;
        HeapPriorityQueue<T,tgt>::Iterator& operator ++ ();
        HeapPriorityQueue<T,tgt>::Iterator  operator ++ (int);
        bool operator == (const HeapPriorityQueue<T,tgt>::Iterator& rhs) const;
        bool operator != (const HeapPriorityQueue<T,tgt>::Iterator& rhs) const;
        T& operator *  () const;
        T* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const HeapPriorityQueue<T,tgt>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }

        friend Iterator HeapPriorityQueue<T,tgt>::begin () const;
        friend Iterator HeapPriorityQueue<T,tgt>::end   () const;

      private:
        //If can_erase is false, the value has been removed from "it" (++ does nothing)
        HeapPriorityQueue<T,tgt>  it;                 //copy of HPQ (from begin), to use as iterator via dequeue
        HeapPriorityQueue<T,tgt>* ref_pq;
        int                      expected_mod_count;
        bool                     can_erase = true;

        //Called in friends begin/end
        //These constructors have different initializers (see it(...) in first one)
        Iterator(HeapPriorityQueue<T,tgt>* iterate_over, bool from_begin);    // Called by begin
        Iterator(HeapPriorityQueue<T,tgt>* iterate_over);                     // Called by end
    };


    Iterator begin () const;
    Iterator end   () const;


  private:
    bool (*gt) (const T& a, const T& b); // The gt used by enqueue (from template or constructor)
    T*  pq;                              // Array represents a heap, so it uses heap ordering property
    int length    = 0;                   //Physical length of pq array: must be >= .size()
    int used      = 0;                   //Amount of array used:  invariant: 0 <= used <= length
    int mod_count = 0;                   //For sensing concurrent modification


    //Helper methods
    void ensure_length  (int new_length);
    int  left_child     (int i) const;         //Useful abstractions for heaps as arrays
    int  right_child    (int i) const;
    int  parent         (int i) const;
    bool is_root        (int i) const;
    bool in_heap        (int i) const;
    void percolate_up   (int i);
    void percolate_down (int i);
    void heapify        ();                   // Percolate down all value is array (from indexes used-1 to 0): O(N)
  };





////////////////////////////////////////////////////////////////////////////////
//
//HeapPriorityQueue class and related definitions

//Destructor/Constructors

template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::~HeapPriorityQueue() {
	delete[] pq;
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::HeapPriorityQueue(bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt) {
	//if (this->gt == nullptr)
		//throw TemplateFunctionError("HeapPriorityQueue::default constructor: neither specified");
	if (tgt != nullptr && cgt != nullptr && tgt != cgt)
		throw TemplateFunctionError("HeapPriorityQueue::default constructor: both specified and different");

	pq = new T[length];
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::HeapPriorityQueue(int initial_length, bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt), length(initial_length) {
	if (gt == nullptr)
		throw TemplateFunctionError("HeapPriorityQueue::length constructor: neither specified");
	if (tgt != nullptr && cgt != nullptr && tgt != cgt)
		throw TemplateFunctionError("HeapPriorityQueue::length constructor: both specified and different");

	if (length < 0)
		length = 0;
	pq = new T[length];
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::HeapPriorityQueue(const HeapPriorityQueue<T,tgt>& to_copy, bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt), length(to_copy.length) {
	if (gt == nullptr)
		gt = to_copy.gt;//throw TemplateFunctionError("ArrayPriorityQueue::copy constructor: neither specified");
	if (tgt != nullptr && cgt != nullptr && tgt != cgt)
		throw TemplateFunctionError("HeapPriorityQueue::copy constructor: both specified and different");

	pq = new T[length];

	if (gt == to_copy.gt) {
		used = to_copy.used;
		for (int i=0; i<to_copy.used; ++i)
			pq[i] = to_copy.pq[i];
	} else {
		for (int i=0; i<to_copy.used; ++i) {
			pq[i] = to_copy.pq[i];
			++used;
		}
		this->heapify();
	}
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::HeapPriorityQueue(const std::initializer_list<T>& il, bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt), length(il.size()) {
	if (gt == nullptr)
		throw TemplateFunctionError("HeapPriorityQueue::initializer_list constructor: neither specified");
	if (tgt != nullptr && cgt != nullptr && tgt != cgt)
		throw TemplateFunctionError("HeapPriorityQueue::initializer_list constructor: both specified and different");

	pq = new T[length];
	int counter = 0;
	for (const T& pq_elem : il) {
		pq[counter++] = pq_elem;
		++used;
	}
	this->heapify();
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
template<class Iterable>
HeapPriorityQueue<T,tgt>::HeapPriorityQueue(const Iterable& i, bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt), length(i.size()) {
	if (gt == nullptr)
		throw TemplateFunctionError("ArrayPriorityQueue::Iterable constructor: neither specified");
	if (tgt != nullptr && cgt != nullptr && tgt != cgt)
		throw TemplateFunctionError("ArrayPriorityQueue::Iterable constructor: both specified and different");

	pq = new T[length];
	int counter = 0;
	for (const T& elem : i) {
		pq[counter++] = elem;
	}
	this->used = this->length;
	this->heapify();
}



////////////////////////////////////////////////////////////////////////////////
//
//Queries

template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::empty() const {
	return (used == 0);
}


template<class T, bool (*tgt)(const T& a, const T& b)>
int HeapPriorityQueue<T,tgt>::size() const {
	return used;
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
T& HeapPriorityQueue<T,tgt>::peek () const {
	if (empty())
		throw EmptyError("HeapPriorityQueue::peek");

	return pq[0];
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
std::string HeapPriorityQueue<T,tgt>::str() const {
	std::ostringstream answer;
	answer << "HeapPriorityQueue[";

	if (used != 0) {
		answer << "0:" << pq[0];
		for (int i = 1; i < used; ++i)
			answer << "," << i << ":" << pq[i];
	}

	answer << "](length=" << length << ",used=" << used << ",mod_count=" << mod_count << ")";
	return answer.str();
}


////////////////////////////////////////////////////////////////////////////////
//
//Commands
//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
int HeapPriorityQueue<T,tgt>::enqueue(const T& element) {
	  this->ensure_length(used+1);
	  pq[used++] = element;
	  this->percolate_up(used-1);
	  ++mod_count;
	  return 1;
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
T HeapPriorityQueue<T,tgt>::dequeue() {
	if (this->empty())
	    throw EmptyError("HeapPriorityQueue::dequeue");

	T to_return = pq[0];
	pq[0] = pq[--used];
	percolate_down(0);

	++mod_count;
	return to_return;
}

//Code from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
void HeapPriorityQueue<T,tgt>::clear() {
	used = 0;
	++mod_count;
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
template <class Iterable>
int HeapPriorityQueue<T,tgt>::enqueue_all (const Iterable& i) {
	int count = 0;
	for (const T& v : i)
		count += enqueue(v);

	return count;
}


////////////////////////////////////////////////////////////////////////////////
//
//Operators
//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>& HeapPriorityQueue<T,tgt>::operator = (const HeapPriorityQueue<T,tgt>& rhs) {
	if (this == &rhs)
	    return *this;

	gt = rhs.gt;
	this->ensure_length(rhs.used);
	used = rhs.used;

	for (int i=0; i<rhs.used; ++i)
		pq[i] = rhs.pq[i];

	++mod_count;
	return *this;
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::operator == (const HeapPriorityQueue<T,tgt>& rhs) const {
	if (this == &rhs)
		return true;

	if (this->gt != rhs.gt) //For PriorityQueues to be equal, they need the same gt function, and values
		return false;

	if (this->size() != rhs.size())
		return false;

	HeapPriorityQueue<T,tgt>::Iterator rhs_i = rhs.begin();
	for (int i=used-1; i>=0; --i,++rhs_i) {
		// Uses ! and ==, so != on T need not be defined
		if (!(pq[i] == *rhs_i)) {
			return false;
		}
	}

	return true;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::operator != (const HeapPriorityQueue<T,tgt>& rhs) const {
	return !(*this == rhs);
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
std::ostream& operator << (std::ostream& outs, const HeapPriorityQueue<T,tgt>& p) {
	outs << "priority_queue[";

	if (!p.empty()) {
		outs << p.pq[0];
		for (int i = 1; i < p.used; ++i)
			outs << ","<< p.pq[i];
	}

	outs << "]:lowest";
	return outs;
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors

template<class T, bool (*tgt)(const T& a, const T& b)>
auto HeapPriorityQueue<T,tgt>::begin () const -> HeapPriorityQueue<T,tgt>::Iterator {
	return Iterator(const_cast<HeapPriorityQueue<T,tgt>*>(this),true);
}


template<class T, bool (*tgt)(const T& a, const T& b)>
auto HeapPriorityQueue<T,tgt>::end () const -> HeapPriorityQueue<T,tgt>::Iterator {
	return Iterator(const_cast<HeapPriorityQueue<T,tgt>*>(this));
}


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
void HeapPriorityQueue<T,tgt>::ensure_length(int new_length) {
	if (length >= new_length) {
		return;
	}
	T* old_pq = pq;
	length = std::max(new_length,2*length);
	pq = new T[length];
	for (int i=0; i<used; ++i) {
		pq[i] = old_pq[i];
	}

	delete [] old_pq;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
int HeapPriorityQueue<T,tgt>::left_child(int i) const
{
	return (2*i+1);
}

template<class T, bool (*tgt)(const T& a, const T& b)>
int HeapPriorityQueue<T,tgt>::right_child(int i) const
{
	return (2*i+2);
}

template<class T, bool (*tgt)(const T& a, const T& b)>
int HeapPriorityQueue<T,tgt>::parent(int i) const
{
	if (i == 0) {
		return i;
	}

	if (i%2 != 0) {
		return (i-1)/2;
	} else {
		return (i-2)/2;
	}
}

template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::is_root(int i) const
{
	return (i == 0);
}

template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::in_heap(int i) const
{
	return (i >= 0 && i <= used);
}


template<class T, bool (*tgt)(const T& a, const T& b)>
void HeapPriorityQueue<T,tgt>::percolate_up(int i) {
	for (int j = i; j >  0; j = parent(j)) {
		if (this->gt(pq[j],pq[parent(j)])) {
			std::swap(pq[j],pq[parent(j)]);
		}
	}
}


template<class T, bool (*tgt)(const T& a, const T& b)>
void HeapPriorityQueue<T,tgt>::percolate_down(int i) {
	for (int j = i; j < used-1; ++j) {
		if (!(this->gt(pq[j],pq[left_child(j)])) && !(this->gt(pq[j],pq[right_child(j)]))) {
			if (gt(pq[left_child(j)],pq[right_child(j)])) {
				std::swap(pq[j],pq[left_child(j)]);
			} else {
				std::swap(pq[j],pq[right_child(j)]);
			}
		} else if (!gt(pq[j],pq[left_child(j)])) {
			std::swap(pq[j],pq[left_child(j)]);
		} else if (!gt(pq[j],pq[right_child(j)])) {
			std::swap(pq[j],pq[right_child(j)]);
		}
	}
}


template<class T, bool (*tgt)(const T& a, const T& b)>
void HeapPriorityQueue<T,tgt>::heapify() {
for (int i = used-1; i >= 0; --i)
  percolate_down(i);
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::Iterator::Iterator(HeapPriorityQueue<T,tgt>* iterate_over, bool tgt_nullptr)
: ref_pq(iterate_over), expected_mod_count(ref_pq->mod_count) {
	this->it = *this->ref_pq;
	//it.gt = iterate_over->gt;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::Iterator::Iterator(HeapPriorityQueue<T,tgt>* iterate_over)
: ref_pq(iterate_over), expected_mod_count(ref_pq->mod_count) {
	this->it.clear();
}


template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::Iterator::~Iterator()
{}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
T HeapPriorityQueue<T,tgt>::Iterator::erase() {
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError("HeapPriorityQueue::Iterator::erase");
	if (!can_erase)
		throw CannotEraseError("HeapPriorityQueue::Iterator::erase Iterator cursor already erased");

	can_erase = false;

	T value = it.dequeue();
	int index = 0;
	for (int i = ref_pq->used-1; i >= 0; --i) {
		if (value == ref_pq->pq[i]) {
			index = i;
		}
	}

	ref_pq->pq[index] = ref_pq->pq[--ref_pq->used];
	if (ref_pq->gt(ref_pq->pq[index],ref_pq->pq[ref_pq->parent(index)])) {
		ref_pq->percolate_up(index);
	} else {
		ref_pq->percolate_down(index);
	}
	++ref_pq->mod_count;
	this->expected_mod_count = ref_pq->mod_count;

	return value;
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
std::string HeapPriorityQueue<T,tgt>::Iterator::str() const {
	std::ostringstream answer;
	answer << ref_pq->str() << "/expected_mod_count=" << expected_mod_count << "/can_erase=" << can_erase;
	return answer.str();
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
auto HeapPriorityQueue<T,tgt>::Iterator::operator ++ () -> HeapPriorityQueue<T,tgt>::Iterator& {
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError("HeapPriorityQueue::Iterator::operator ++");

	if (it.size() < 0)
		return *this;

	if (can_erase)
		it.dequeue();         //decreasing priority goes backward in array, towards 0
	else
		can_erase = true;  //current already indexes "one beyond" deleted value

	return *this;
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
auto HeapPriorityQueue<T,tgt>::Iterator::operator ++ (int) -> HeapPriorityQueue<T,tgt>::Iterator {
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError("HeapPriorityQueue::Iterator::operator ++(int)");

	if (it.size() < 0)
		return *this;

	Iterator to_return(*this);
	if (can_erase)
		it.dequeue();
	else
		can_erase = true;  //current already indexes "one beyond" deleted value

	return to_return;
}

//Code modified from Array Priority Queue
template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::Iterator::operator == (const HeapPriorityQueue<T,tgt>::Iterator& rhs) const {
	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
	if (rhsASI == 0)
		throw IteratorTypeError("HeapPriorityQueue::Iterator::operator ==");
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError("HeapPriorityQueue::Iterator::operator ==");
	if (ref_pq != rhsASI->ref_pq)
		throw ComparingDifferentIteratorsError("HeapPriorityQueue::Iterator::operator ==");

	return this->it.size() == rhsASI->it.size();
}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::Iterator::operator != (const HeapPriorityQueue<T,tgt>::Iterator& rhs) const {
	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
	if (rhsASI == 0)
		throw IteratorTypeError("HeapPriorityQueue::Iterator::operator !=");
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError("HeapPriorityQueue::Iterator::operator !=");
	if (ref_pq != rhsASI->ref_pq)
		throw ComparingDifferentIteratorsError("HeapPriorityQueue::Iterator::operator !=");

	return this->it.size() != rhsASI->it.size();
}


template<class T, bool (*tgt)(const T& a, const T& b)>
T& HeapPriorityQueue<T,tgt>::Iterator::operator *() const {
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError("ArrayPriorityQueue::Iterator::operator *");
	if (!can_erase || !it.in_heap(0)) {
		std::ostringstream where;
		where << it.peek() << " when size = " << it.size();
		throw IteratorPositionIllegal("ArrayPriorityQueue::Iterator::operator * Iterator illegal: "+where.str());
	}

	return it.peek();
}


template<class T, bool (*tgt)(const T& a, const T& b)>
T* HeapPriorityQueue<T,tgt>::Iterator::operator ->() const {
	if (expected_mod_count !=  ref_pq->mod_count)
		throw ConcurrentModificationError("ArrayPriorityQueue::Iterator::operator ->");
	if (!can_erase || !it.in_heap(0)) {
		std::ostringstream where;
		where << it.peek() << " when size = " << it.size();
		throw IteratorPositionIllegal("ArrayPriorityQueue::Iterator::operator * Iterator illegal: "+where.str());
	}

	return &it.peek();
}

}

#endif /* HEAP_PRIORITY_QUEUE_HPP_ */
