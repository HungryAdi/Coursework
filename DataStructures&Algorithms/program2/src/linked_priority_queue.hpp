// Submitter: asiyer(Iyer, Aditya)
  // Partner  : ssthothr(Sthothra Bhashyam, Sreeja)
  // We certify that we worked cooperatively on this programming
  //   assignment, according to the rules for pair programming

#ifndef LINKED_PRIORITY_QUEUE_HPP_
#define LINKED_PRIORITY_QUEUE_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include "array_stack.hpp"      //See operator <<


namespace ics {


//Instantiate the template such that tgt(a,b) is true, iff a has higher priority than b
//With a tgt specified in the template, the constructor cannot specify a cgt.
//If a tgt is defaulted, then the constructor must supply a cgt (they cannot both be nullptr)
template<class T, bool (*tgt)(const T& a, const T& b) = nullptr> class LinkedPriorityQueue {
  public:
    //Destructor/Constructors
    ~LinkedPriorityQueue();

    LinkedPriorityQueue          (bool (*cgt)(const T& a, const T& b) = nullptr);
    LinkedPriorityQueue          (const LinkedPriorityQueue<T,tgt>& to_copy, bool (*cgt)(const T& a, const T& b) = nullptr);
    explicit LinkedPriorityQueue (const std::initializer_list<T>& il, bool (*cgt)(const T& a, const T& b) = nullptr);

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    explicit LinkedPriorityQueue (const Iterable& i, bool (*cgt)(const T& a, const T& b) = nullptr);


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
    LinkedPriorityQueue<T,tgt>& operator = (const LinkedPriorityQueue<T,tgt>& rhs);
    bool operator == (const LinkedPriorityQueue<T,tgt>& rhs) const;
    bool operator != (const LinkedPriorityQueue<T,tgt>& rhs) const;

    template<class T2, bool (*gt2)(const T2& a, const T2& b)>
    friend std::ostream& operator << (std::ostream& outs, const LinkedPriorityQueue<T2,gt2>& pq);



  private:
    class LN;

  public:
    class Iterator {
      public:
        //Private constructor called in begin/end, which are friends of LinkedPriorityQueue<T,tgt>
        ~Iterator();
        T           erase();
        std::string str  () const;
        LinkedPriorityQueue<T,tgt>::Iterator& operator ++ ();
        LinkedPriorityQueue<T,tgt>::Iterator  operator ++ (int);
        bool operator == (const LinkedPriorityQueue<T,tgt>::Iterator& rhs) const;
        bool operator != (const LinkedPriorityQueue<T,tgt>::Iterator& rhs) const;
        T& operator *  () const;
        T* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const LinkedPriorityQueue<T,tgt>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }
        friend Iterator LinkedPriorityQueue<T,tgt>::begin () const;
        friend Iterator LinkedPriorityQueue<T,tgt>::end   () const;

      private:
        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
        LN*             prev;            //prev should be initalized to the header
        LN*             current;         //current == prev->next
        LinkedPriorityQueue<T,tgt>* ref_pq;
        int             expected_mod_count;
        bool            can_erase = true;

        //Called in friends begin/end
        Iterator(LinkedPriorityQueue<T,tgt>* iterate_over, LN* initial);
    };


    Iterator begin () const;
    Iterator end   () const;


  private:
    class LN {
      public:
        LN ()                      {}
        LN (const LN& ln)          : value(ln.value), next(ln.next){}
        LN (T v,  LN* n = nullptr) : value(v), next(n){}

        T   value;
        LN* next = nullptr;
    };


    bool (*gt) (const T& a, const T& b); // The gt used by enqueue (from template or constructor)
    LN* front     =  new LN();
    int used      =  0;                  //Cache for number of values in linked list
    int mod_count =  0;                  //For sensing concurrent modification

    //Helper methods
    void delete_list(LN*& front);        //Deallocate all LNs, and set front's argument to nullptr;
};





////////////////////////////////////////////////////////////////////////////////
//
//LinkedPriorityQueue class and related definitions

//Destructor/Constructors

template<class T, bool (*tgt)(const T& a, const T& b)>
LinkedPriorityQueue<T,tgt>::~LinkedPriorityQueue() {
  delete_list(front);
}


template<class T, bool (*tgt)(const T& a, const T& b)>
LinkedPriorityQueue<T,tgt>::LinkedPriorityQueue(bool (*cgt)(const T& a, const T& b))
{
	this->gt = cgt;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
LinkedPriorityQueue<T,tgt>::LinkedPriorityQueue(const LinkedPriorityQueue<T,tgt>& to_copy, bool (*cgt)(const T& a, const T& b))
{
	if (this->gt == to_copy.gt) {
		this->used = to_copy.used;
		for (LN* i = front, *j = to_copy.front->next; j != nullptr; i = i->next, j = j->next) {
			i->next = new LN(j->value);
		}
	} else {
		for (LN* temp = to_copy.front->next; temp != nullptr; temp = temp->next) {
			enqueue(temp->value);
		}
	}

}


template<class T, bool (*tgt)(const T& a, const T& b)>
LinkedPriorityQueue<T,tgt>::LinkedPriorityQueue(const std::initializer_list<T>& il, bool (*cgt)(const T& a, const T& b))
{
	this->gt = cgt;
	for (const T& entry : il) {
		this->enqueue(entry);
	}
}


template<class T, bool (*tgt)(const T& a, const T& b)>
template<class Iterable>
LinkedPriorityQueue<T,tgt>::LinkedPriorityQueue(const Iterable& i, bool (*cgt)(const T& a, const T& b))
{
	this->gt = cgt;
	for (const T& entry : i) {
		this->enqueue(entry);
	}
}


////////////////////////////////////////////////////////////////////////////////
//
//Queries

template<class T, bool (*tgt)(const T& a, const T& b)>
bool LinkedPriorityQueue<T,tgt>::empty() const {
	return (this->used == 0);
}


template<class T, bool (*tgt)(const T& a, const T& b)>
int LinkedPriorityQueue<T,tgt>::size() const {
	return this->used;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
T& LinkedPriorityQueue<T,tgt>::peek () const {
	if (this->empty()) {
		throw EmptyError("LinkedPriorityQueue::peek");
	}
	return front->next->value;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
std::string LinkedPriorityQueue<T,tgt>::str() const {
	std::ostringstream answer;
	answer << "LinkedPriorityQueue[";

	if (used != 0) {
		answer << "->" << front->next->value;
		for (LN* temp = front->next->next; temp != nullptr; temp = temp->next)
			answer << "->" << temp->value;
	}

	answer << "](used=" << used << ",front=" << front << ",mod_count=" << mod_count << ")";
	return answer.str();
}

////////////////////////////////////////////////////////////////////////////////
//
//Commands

template<class T, bool (*tgt)(const T& a, const T& b)>
int LinkedPriorityQueue<T,tgt>::enqueue(const T& element) {
  LN* temp = front;
  while (temp->next != nullptr && gt(temp->next->value,element)) {
    temp = temp->next;
  }
  temp->next = new LN(element,temp->next);
  ++used;
  ++mod_count;
  return 1;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
T LinkedPriorityQueue<T,tgt>::dequeue() {
	if (this->empty()) {
		throw EmptyError("LinkedPriorityQueue::dequeue");
	}

	T return_value = front->next->value;
	LN* temp = front->next;
	front->next = temp->next;
	delete temp;
	--used;
	++mod_count;
	return return_value;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
void LinkedPriorityQueue<T,tgt>::clear() {
	delete_list(front->next);
	++mod_count;
	used = 0;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
template <class Iterable>
int LinkedPriorityQueue<T,tgt>::enqueue_all (const Iterable& i) {
	int count = 0;
	for (auto v : i) {
		enqueue(v);
		count++;
	}

	return count;
}


////////////////////////////////////////////////////////////////////////////////
//
//Operators

template<class T, bool (*tgt)(const T& a, const T& b)>
LinkedPriorityQueue<T,tgt>& LinkedPriorityQueue<T,tgt>::operator = (const LinkedPriorityQueue<T,tgt>& rhs) {
	if (this == &rhs) {
		return *this;
	}

	gt = rhs.gt;
	used = rhs.used;
	LN** temp = &(front->next);
	for (LN* i = rhs.front->next; i != nullptr; temp = &((*temp)->next), i = i->next) {
		if (*temp != nullptr) {
			(*temp)->value = i->value;
		} else {
			*temp = new LN(i->value);
		}
	}

	if (*temp != nullptr) {
		delete_list(*temp);
	}

	++mod_count;
	return *this;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool LinkedPriorityQueue<T,tgt>::operator == (const LinkedPriorityQueue<T,tgt>& rhs) const {
	if (this == &rhs) {
		return true;
	}

	int used = this->size();

	if (used != rhs.size()) {
		return false;
	}

	if (gt != rhs.gt) {
		return false;
	}

	LinkedPriorityQueue<T,tgt>::Iterator rhs_iterator = rhs.begin();

	for (LN* p = front->next; p != nullptr; p = p->next,++rhs_iterator) {
		if (p->value != *rhs_iterator) {
			return false;
		}
	}

	return true;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool LinkedPriorityQueue<T,tgt>::operator != (const LinkedPriorityQueue<T,tgt>& rhs) const {
	return !(*this == rhs);
}


template<class T, bool (*tgt)(const T& a, const T& b)>
std::ostream& operator << (std::ostream& outs, const LinkedPriorityQueue<T,tgt>& pq) {
	outs << "priority_queue[";

	if (!pq.empty()) {
		ArrayStack<T> temp_stack;
		for (typename LinkedPriorityQueue<T,tgt>::LN* p = pq.front->next; p != nullptr; p = p->next) {
			temp_stack.push(p->value);
		}
		outs << temp_stack.pop();
		while (!temp_stack.empty()) {
			outs << "," << temp_stack.pop();
		}
	}

	outs <<"]:highest";
	return outs;
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors


template<class T, bool (*tgt)(const T& a, const T& b)>
auto LinkedPriorityQueue<T,tgt>::begin () const -> LinkedPriorityQueue<T,tgt>::Iterator {
	return Iterator(const_cast<LinkedPriorityQueue<T,tgt>*>(this),front->next);
}


template<class T, bool (*tgt)(const T& a, const T& b)>
auto LinkedPriorityQueue<T,tgt>::end () const -> LinkedPriorityQueue<T,tgt>::Iterator {
	return Iterator(const_cast<LinkedPriorityQueue<T,tgt>*>(this),nullptr);
}


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

template<class T, bool (*tgt)(const T& a, const T& b)>
void LinkedPriorityQueue<T,tgt>::delete_list(LN*& front) {
	LN* pointer = front;

	while (pointer != nullptr) {
		LN* temp = pointer;
		pointer = pointer->next;
		delete temp;
	}

	front = nullptr;
}





////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

template<class T, bool (*tgt)(const T& a, const T& b)>
LinkedPriorityQueue<T,tgt>::Iterator::Iterator(LinkedPriorityQueue<T,tgt>* iterate_over, LN* initial)
{
	this->ref_pq = iterate_over;
	this->expected_mod_count = ref_pq->mod_count;
	this->prev = iterate_over->front;
	this->current = initial;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
LinkedPriorityQueue<T,tgt>::Iterator::~Iterator()
{}


template<class T, bool (*tgt)(const T& a, const T& b)>
T LinkedPriorityQueue<T,tgt>::Iterator::erase() {
	if (expected_mod_count != ref_pq->mod_count) {
		throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::erase");
	}

	if (!can_erase) {
		throw CannotEraseError("LinkedPriorityQueue::Iterator::erase Iterator cursor already erased");
	}

	if (current == nullptr) {
		throw CannotEraseError("LinkedPriorityQueue::Iterator::erase Iterator cursor beyond data structure");
	}

	can_erase = false;

	T return_value = current->value;

	prev->next = current->next;
	delete current;
	current = prev->next;

	ref_pq->used -= 1;
	expected_mod_count = ref_pq->mod_count;
	return return_value;

}


template<class T, bool (*tgt)(const T& a, const T& b)>
std::string LinkedPriorityQueue<T,tgt>::Iterator::str() const {
	std::ostringstream string;

	string << ref_pq->str() << "(current=" << current << ",expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";

	return string.str();
}


template<class T, bool (*tgt)(const T& a, const T& b)>
auto LinkedPriorityQueue<T,tgt>::Iterator::operator ++ () -> LinkedPriorityQueue<T,tgt>::Iterator& {
	if (expected_mod_count != ref_pq->mod_count) {
		throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ++");
	}

	if (current == nullptr) {
		return *this;
	}

	if (can_erase) {
		prev = current;
		current = current->next;
	} else {
		can_erase = true;
	}

	return *this;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
auto LinkedPriorityQueue<T,tgt>::Iterator::operator ++ (int) -> LinkedPriorityQueue<T,tgt>::Iterator {
	if (expected_mod_count != ref_pq->mod_count) {
		throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ++(int)");
	}

	if (current == nullptr) {
		return *this;
	}

	Iterator return_iterator(*this);

	if (can_erase) {
		prev = current;
		current = current->next;
	} else {
		can_erase = true;
	}

	return return_iterator;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool LinkedPriorityQueue<T,tgt>::Iterator::operator == (const LinkedPriorityQueue<T,tgt>::Iterator& rhs) const {
  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);

  if (rhsASI == 0) {
    throw IteratorTypeError("LinkedPriorityQueue::Iterator::operator ==");
  }

  if (expected_mod_count != ref_pq->mod_count) {
    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ==");
  }

  if (ref_pq != rhsASI->ref_pq) {
    throw ComparingDifferentIteratorsError("LinkedPriorityQueue::Iterator::operator ==");
  }

  return current == rhsASI->current;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool LinkedPriorityQueue<T,tgt>::Iterator::operator != (const LinkedPriorityQueue<T,tgt>::Iterator& rhs) const {
  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);

  if (rhsASI == 0) {
    throw IteratorTypeError("LinkedPriorityQueue::Iterator::operator !=");
  }

  if (expected_mod_count != ref_pq->mod_count) {
    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator !=");
  }

  if (ref_pq != rhsASI->ref_pq) {
    throw ComparingDifferentIteratorsError("LinkedPriorityQueue::Iterator::operator !=");
  }

  return current != rhsASI->current;
}

template<class T, bool (*tgt)(const T& a, const T& b)>
T& LinkedPriorityQueue<T,tgt>::Iterator::operator *() const {
  if (expected_mod_count != ref_pq->mod_count) {
    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator *");
  }

  if (!can_erase || current == nullptr) {
    std::ostringstream where;
    where << current << " when front = " << ref_pq->front;
    throw IteratorPositionIllegal("LinkedPriorityQueue::Iterator::operator * Iterator illegal: "+where.str());
  }

  return current->value;
}

template<class T, bool (*tgt)(const T& a, const T& b)>
T* LinkedPriorityQueue<T,tgt>::Iterator::operator ->() const {
  if (expected_mod_count != ref_pq->mod_count) {
    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator *");
  }

  if (!can_erase || current == nullptr) {
    std::ostringstream where;
    where << current << " when front = " << ref_pq->front;
    throw IteratorPositionIllegal("LinkedPriorityQueue::Iterator::operator * Iterator illegal: "+where.str());
  }

  return &(current->value);
}


}

#endif /* LINKED_PRIORITY_QUEUE_HPP_ */
