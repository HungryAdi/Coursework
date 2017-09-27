#ifndef LINKED_SET_HPP_
#define LINKED_SET_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"


namespace ics {


template<class T> class LinkedSet {
  public:
    //Destructor/Constructors
    ~LinkedSet();

    LinkedSet          ();
    explicit LinkedSet (int initialLength);
    LinkedSet          (const LinkedSet<T>& to_copy);
    explicit LinkedSet (const std::initializer_list<T>& il);

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    explicit LinkedSet (const Iterable& i);


    //Queries
    bool empty      () const;
    int  size       () const;
    bool contains   (const T& element) const;
    std::string str () const; //supplies useful debugging information; contrast to operator <<

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    bool contains_all (const Iterable& i) const;


    //Commands
    int  insert (const T& element);
    int  erase  (const T& element);
    void clear  ();

    //Iterable class must support "for" loop: .begin()/.end() and prefix ++ on returned result

    template <class Iterable>
    int insert_all(const Iterable& i);

    template <class Iterable>
    int erase_all(const Iterable& i);

    template<class Iterable>
    int retain_all(const Iterable& i);


    //Operators
    LinkedSet<T>& operator = (const LinkedSet<T>& rhs);
    bool operator == (const LinkedSet<T>& rhs) const;
    bool operator != (const LinkedSet<T>& rhs) const;
    bool operator <= (const LinkedSet<T>& rhs) const;
    bool operator <  (const LinkedSet<T>& rhs) const;
    bool operator >= (const LinkedSet<T>& rhs) const;
    bool operator >  (const LinkedSet<T>& rhs) const;

    template<class T2>
    friend std::ostream& operator << (std::ostream& outs, const LinkedSet<T2>& s);



  private:
    class LN;

  public:
    class Iterator {
      public:
        //Private constructor called in begin/end, which are friends of LinkedSet<T>
        ~Iterator();
        T           erase();
        std::string str  () const;
        LinkedSet<T>::Iterator& operator ++ ();
        LinkedSet<T>::Iterator  operator ++ (int);
        bool operator == (const LinkedSet<T>::Iterator& rhs) const;
        bool operator != (const LinkedSet<T>::Iterator& rhs) const;
        T& operator *  () const;
        T* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const LinkedSet<T>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }
        friend Iterator LinkedSet<T>::begin () const;
        friend Iterator LinkedSet<T>::end   () const;

      private:
        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
        LN*           current;  //if can_erase is false, this value is unusable
        LinkedSet<T>* ref_set;
        int           expected_mod_count;
        bool          can_erase = true;

        //Called in friends begin/end
        Iterator(LinkedSet<T>* iterate_over, LN* initial);
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
        LN* next   = nullptr;
    };


    LN* front     = new LN();
    LN* trailer   = front;         //Must always point to special trailer LN
    int used      =  0;            //Cache of number of values in linked list
    int mod_count = 0;             //For sensing concurrent modification

    //Helper methods
    int  erase_at   (LN* p);
    void delete_list(LN*& front);  //Deallocate all LNs (but trailer), and set front's argument to trailer;
};





////////////////////////////////////////////////////////////////////////////////
//
//LinkedSet class and related definitions

//Destructor/Constructors

template<class T>
LinkedSet<T>::~LinkedSet() {
}


template<class T>
LinkedSet<T>::LinkedSet() {
}


template<class T>
LinkedSet<T>::LinkedSet(const LinkedSet<T>& to_copy) : used(to_copy.used) {
}


template<class T>
LinkedSet<T>::LinkedSet(const std::initializer_list<T>& il) {
}


template<class T>
template<class Iterable>
LinkedSet<T>::LinkedSet(const Iterable& i) {
}


////////////////////////////////////////////////////////////////////////////////
//
//Queries

template<class T>
bool LinkedSet<T>::empty() const {
}


template<class T>
int LinkedSet<T>::size() const {
}


template<class T>
bool LinkedSet<T>::contains (const T& element) const {
}


template<class T>
std::string LinkedSet<T>::str() const {
}


template<class T>
template<class Iterable>
bool LinkedSet<T>::contains_all (const Iterable& i) const {
}


////////////////////////////////////////////////////////////////////////////////
//
//Commands


template<class T>
int LinkedSet<T>::insert(const T& element) {
}


template<class T>
int LinkedSet<T>::erase(const T& element) {
}


template<class T>
void LinkedSet<T>::clear() {
}


template<class T>
template<class Iterable>
int LinkedSet<T>::insert_all(const Iterable& i) {
}


template<class T>
template<class Iterable>
int LinkedSet<T>::erase_all(const Iterable& i) {
}


template<class T>
template<class Iterable>
int LinkedSet<T>::retain_all(const Iterable& i) {
}


////////////////////////////////////////////////////////////////////////////////
//
//Operators

template<class T>
LinkedSet<T>& LinkedSet<T>::operator = (const LinkedSet<T>& rhs) {
}


template<class T>
bool LinkedSet<T>::operator == (const LinkedSet<T>& rhs) const {
}


template<class T>
bool LinkedSet<T>::operator != (const LinkedSet<T>& rhs) const {
}


template<class T>
bool LinkedSet<T>::operator <= (const LinkedSet<T>& rhs) const {
}


template<class T>
bool LinkedSet<T>::operator < (const LinkedSet<T>& rhs) const {
}


template<class T>
bool LinkedSet<T>::operator >= (const LinkedSet<T>& rhs) const {
}


template<class T>
bool LinkedSet<T>::operator > (const LinkedSet<T>& rhs) const {
}


template<class T>
std::ostream& operator << (std::ostream& outs, const LinkedSet<T>& s) {
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors

template<class T>
auto LinkedSet<T>::begin () const -> LinkedSet<T>::Iterator {
}


template<class T>
auto LinkedSet<T>::end () const -> LinkedSet<T>::Iterator {
}


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

template<class T>
int LinkedSet<T>::erase_at(LN* p) {
}


template<class T>
void LinkedSet<T>::delete_list(LN*& front) {
}





////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

template<class T>
LinkedSet<T>::Iterator::Iterator(LinkedSet<T>* iterate_over, LN* initial)
{
}


template<class T>
LinkedSet<T>::Iterator::~Iterator()
{}


template<class T>
T LinkedSet<T>::Iterator::erase() {
}


template<class T>
std::string LinkedSet<T>::Iterator::str() const {
}


template<class T>
auto LinkedSet<T>::Iterator::operator ++ () -> LinkedSet<T>::Iterator& {
}


template<class T>
auto LinkedSet<T>::Iterator::operator ++ (int) -> LinkedSet<T>::Iterator {
}


template<class T>
bool LinkedSet<T>::Iterator::operator == (const LinkedSet<T>::Iterator& rhs) const {
}


template<class T>
bool LinkedSet<T>::Iterator::operator != (const LinkedSet<T>::Iterator& rhs) const {
}


template<class T>
T& LinkedSet<T>::Iterator::operator *() const {
}


template<class T>
T* LinkedSet<T>::Iterator::operator ->() const {
}


}

#endif /* LINKED_SET_HPP_ */
