//(Iyer, Aditya) asiyer
#ifndef BST_MAP_HPP_
#define BST_MAP_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include "pair.hpp"
#include "array_queue.hpp"   //For traversal


namespace ics {


//Instantiate such that tlt(a,b) is true, iff a is in the left subtree rooted by b
//With a tlt specified in the template, the constructor cannot specify a clt.
//If a tlt is defaulted, then the constructor must supply a clt (they cannot both be nullptr)
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b) = nullptr> class BSTMap {
  public:
    typedef pair<KEY,T> Entry;

    //Destructor/Constructors
    ~BSTMap();

    BSTMap          (bool (*clt)(const KEY& a, const KEY& b) = nullptr);
    BSTMap          (const BSTMap<KEY,T,tlt>& to_copy, bool (*clt)(const KEY& a, const KEY& b) = nullptr);
    explicit BSTMap (const std::initializer_list<Entry>& il, bool (*clt)(const KEY& a, const KEY& b) = nullptr);

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    explicit BSTMap (const Iterable& i, bool (*clt)(const KEY& a, const KEY& b) = nullptr);


    //Queries
    bool empty      () const;
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
    int put_all(const Iterable& i);


    //Operators

    T&       operator [] (const KEY&);
    const T& operator [] (const KEY&) const;
    BSTMap<KEY,T,tlt>& operator = (const BSTMap<KEY,T,tlt>& rhs);
    bool operator == (const BSTMap<KEY,T,tlt>& rhs) const;
    bool operator != (const BSTMap<KEY,T,tlt>& rhs) const;

    template<class KEY2,class T2, bool (*lt2)(const KEY2& a, const KEY2& b)>
    friend std::ostream& operator << (std::ostream& outs, const BSTMap<KEY2,T2,lt2>& m);



    class Iterator {
      public:
        //Private constructor called in begin/end, which are friends of BSTMap<T>
        ~Iterator();
        Entry       erase();
        std::string str  () const;
        BSTMap<KEY,T,tlt>::Iterator& operator ++ ();
        BSTMap<KEY,T,tlt>::Iterator  operator ++ (int);
        bool operator == (const BSTMap<KEY,T,tlt>::Iterator& rhs) const;
        bool operator != (const BSTMap<KEY,T,tlt>::Iterator& rhs) const;
        Entry& operator *  () const;
        Entry* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const BSTMap<KEY,T,tlt>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }
        friend Iterator BSTMap<KEY,T,tlt>::begin () const;
        friend Iterator BSTMap<KEY,T,tlt>::end   () const;

      private:
        //If can_erase is false, the value has been removed from "it" (++ does nothing)
        ArrayQueue<Entry> it;                 //Queue for all associations (from begin), to use as iterator via dequeue
        BSTMap<KEY,T,tlt>* ref_map;
        int               expected_mod_count;
        bool              can_erase = true;

        //Called in friends begin/end
        Iterator(BSTMap<KEY,T,tlt>* iterate_over, bool from_begin);
    };


    Iterator begin () const;
    Iterator end   () const;


  private:
    class TN {
      public:
        TN ()                     : left(nullptr), right(nullptr){}
        TN (const TN& tn)         : value(tn.value), left(tn.left), right(tn.right){}
        TN (Entry v, TN* l = nullptr,
                     TN* r = nullptr) : value(v), left(l), right(r){}

        Entry value;
        TN*   left;
        TN*   right;
    };

  bool (*lt) (const KEY& a, const KEY& b); // The lt used for searching BST (from template or constructor)
  TN* map       = nullptr;
  int used      = 0;                       //Cache for number of key->value pairs in the BST
  int mod_count = 0;                       //For sensing concurrent modification

  //Helper methods (find_key written iteratively, the rest recursively)
  TN*   find_key            (TN*  root, const KEY& key)                 const; //Returns reference to key's node or nullptr
  bool  has_value           (TN*  root, const T& value)                 const; //Returns whether value is is root's tree
  TN*   copy                (TN*  root)                                 const; //Copy the keys/values in root's tree (identical structure)
  void  copy_to_queue       (TN* root, ArrayQueue<Entry>& q)            const; //Fill queue with root's tree value
  bool  equals              (TN*  root, const BSTMap<KEY,T,tlt>& other) const; //Returns whether root's keys/value are all in other
  std::string string_rotated(TN* root, std::string indent)              const; //Returns string representing root's tree

  T     insert              (TN*& root, const KEY& key, const T& value);       //Put key->value, returning key's old value (or new one's, if key absent)
  T&    find_addempty       (TN*& root, const KEY& key);                       //Return reference to key's value (adding key->T() first, if key absent)
  Entry remove_closest      (TN*& root);                                       //Helper for remove
  T     remove              (TN*& root, const KEY& key);                       //Remove key->value from root's tree
  void  delete_BST          (TN*& root);                                       //Deallocate all TN in tree; root == nullptr
};




////////////////////////////////////////////////////////////////////////////////
//
//BSTMap class and related definitions

//Destructor/Constructors

template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
BSTMap<KEY,T,tlt>::~BSTMap() {
	this->delete_BST(this->map);
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
BSTMap<KEY,T,tlt>::BSTMap(bool (*clt)(const KEY& a, const KEY& b))
: lt(tlt != nullptr ? tlt : clt) {
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
BSTMap<KEY,T,tlt>::BSTMap(const BSTMap<KEY,T,tlt>& to_copy, bool (*clt)(const KEY& a, const KEY& b))
: lt(tlt != nullptr ? tlt : clt) {
	if (lt == to_copy.lt) {
	    used = to_copy.used;
	    map  = copy(to_copy.map);
	  }else {
	    for (Entry entry : to_copy)
	      put(entry.first,entry.second);
	  }
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
BSTMap<KEY,T,tlt>::BSTMap(const std::initializer_list<Entry>& il, bool (*clt)(const KEY& a, const KEY& b))
: lt(tlt != nullptr ? tlt : clt) {
	for (const Entry& entry : il) {
	    put(entry.first,entry.second);
	}
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
template <class Iterable>
BSTMap<KEY,T,tlt>::BSTMap(const Iterable& i, bool (*clt)(const KEY& a, const KEY& b))
: lt(tlt != nullptr ? tlt : clt) {
	for (const Entry& entry : i) {
	    put(entry.first,entry.second);
	}
}


////////////////////////////////////////////////////////////////////////////////
//
//Queries

template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
bool BSTMap<KEY,T,tlt>::empty() const {
	return (used == 0);
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
int BSTMap<KEY,T,tlt>::size() const {
	return used;
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
bool BSTMap<KEY,T,tlt>::has_key (const KEY& key) const {
	return find_key(map,key) != nullptr;
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
bool BSTMap<KEY,T,tlt>::has_value (const T& value) const {
	return has_value(map,value);
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
std::string BSTMap<KEY,T,tlt>::str() const {
	std::ostringstream string;
	string << "BstMap[";

	if (used != 0) {
		string << std::endl << string_rotated(map,"");
	}

	string  << "](used=" << used << ",mod_count=" << mod_count << ")";
	return string.str();
}


////////////////////////////////////////////////////////////////////////////////
//
//Commands

template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
T BSTMap<KEY,T,tlt>::put(const KEY& key, const T& value) {
	++mod_count;
	return insert(map,key,value);
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
T BSTMap<KEY,T,tlt>::erase(const KEY& key) {
	T remove_val = remove(map,key);
	++mod_count;
	--used;
	return remove_val;
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
void BSTMap<KEY,T,tlt>::clear() {
	delete_BST(map);
	used = 0;
	++mod_count;
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
template<class Iterable>
int BSTMap<KEY,T,tlt>::put_all(const Iterable& i) {
	int counter = 0;
	for (const Entry& entry : i) {
		counter++;
		put(entry.first, entry.second);
	}
	return counter;
}


////////////////////////////////////////////////////////////////////////////////
//
//Operators

template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
T& BSTMap<KEY,T,tlt>::operator [] (const KEY& key) {
	return find_addempty(map,key);
}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
const T& BSTMap<KEY,T,tlt>::operator [] (const KEY& key) const {
	  TN* n = find_key(map,key);
	  if (n != nullptr) {
		  return n->value.second;
	  }
	  std::ostringstream string;
	  string << "BSTMap::operator []: key(" << key << ") not in Map";
	  throw KeyError(string.str());
}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
BSTMap<KEY,T,tlt>& BSTMap<KEY,T,tlt>::operator = (const BSTMap<KEY,T,tlt>& rhs) {
	if (this == &rhs) {
		return *this;
	}

	clear(); // not starting from empty map
	if (lt == rhs.lt) {
		used = rhs.used;
		map  = copy(rhs.map);
	}else
		put_all(rhs);

	++mod_count;
	return *this;
}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
bool BSTMap<KEY,T,tlt>::operator == (const BSTMap<KEY,T,tlt>& rhs) const {
	if (this == &rhs) {
		return true;
	}

	if (used != rhs.size()) {
		return false;
	}

	return equals(map,rhs);
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
bool BSTMap<KEY,T,tlt>::operator != (const BSTMap<KEY,T,tlt>& rhs) const {
	return !(*this == rhs);
}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
std::ostream& operator << (std::ostream& outs, const BSTMap<KEY,T,tlt>& m) {
	outs << "map[";

	if (m.used != 0) {
		typename BSTMap<KEY,T,tlt>::Iterator iterator = m.begin();
		outs << iterator->first << "->" << iterator->second;
		++iterator;
		for (; iterator != m.end(); ++iterator) {
			outs << "," << iterator->first << "->" << iterator->second;
		}
	}
	outs << "]";

	return outs;
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors
//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
auto BSTMap<KEY,T,tlt>::begin () const -> BSTMap<KEY,T,tlt>::Iterator {
	return Iterator(const_cast<BSTMap<KEY,T,tlt>*>(this),true);
}
//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
auto BSTMap<KEY,T,tlt>::end () const -> BSTMap<KEY,T,tlt>::Iterator {
	return Iterator(const_cast<BSTMap<KEY,T,tlt>*>(this),false);
}

////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
typename BSTMap<KEY,T,tlt>::TN* BSTMap<KEY,T,tlt>::find_key (TN* root, const KEY& key) const {
	for (TN* n=root; n!=nullptr;) {
		if (key == n->value.first) {
			return n;
		}
		if (lt(key,n->value.first)) {
			n = n->left;
		} else {
			n = n->right;
		}
	}
	return nullptr;
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
bool BSTMap<KEY,T,tlt>::has_value (TN* root, const T& value) const {
	if (root == nullptr) {
		return false;
	} else {
		return (root->value.second == value || has_value(root->left,value) || has_value(root->right,value));
	}
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
typename BSTMap<KEY,T,tlt>::TN* BSTMap<KEY,T,tlt>::copy (TN* root) const {
	if (root == nullptr) {
		return nullptr;
	} else {
		return new TN(root->value, copy(root->left), copy(root->right));
	}
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
void BSTMap<KEY,T,tlt>::copy_to_queue (TN* root, ArrayQueue<Entry>& q) const {
	if (root == nullptr)
		return;
	else {
		copy_to_queue(root->left, q);
		q.enqueue(root->value);
		copy_to_queue(root->right, q);
	}
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
bool BSTMap<KEY,T,tlt>::equals (TN* root, const BSTMap<KEY,T,tlt>& other) const {
	if (root == nullptr)
		return true;
	else // use [] for return value
		return (other.has_key(root->value.first) && root->value.second == other[root->value.first] && equals(root->left,other) && equals(root->right,other));
}

//EDIT
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
std::string BSTMap<KEY,T,tlt>::string_rotated(TN* root, std::string indent) const {
	std::ostringstream string;
	if (root == nullptr) {
		return string.str();
	} else {
		string << string_rotated(root->right,indent+"  ") << indent << root->value.first << "->" << root->value.second << "\n" << string_rotated(root->left,indent+"  ");
	}
	return string.str();
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
T BSTMap<KEY,T,tlt>::insert (TN*& root, const KEY& key, const T& value) {
	if (root == nullptr) {
		root = new TN(Entry(key,value));
		++used;
		return root->value.second;
	} else if (root->value.first == key) {
		T val = root->value.second;
		root->value.second = value;
		return val;
	} else if (lt(key,root->value.first)) {
		return insert(root->left,key,value);
	} else {
		return insert(root->right,key,value);
	}
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
T& BSTMap<KEY,T,tlt>::find_addempty (TN*& root, const KEY& key) {
	if (root == nullptr) {
		root = new TN(Entry(key,T()));
		++used;
		++mod_count;
		return root->value.second;
	} else if (root->value.first == key) {
		return root->value.second;
	} else if (lt(key,root->value.first)) {
		return find_addempty(root->left,key);
	} else {
		return find_addempty(root->right,key);
	}
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
pair<KEY,T> BSTMap<KEY,T,tlt>::remove_closest(TN*& root) {
  if (root->right != nullptr) {
    return remove_closest(root->right);
  } else{
    Entry val = root->value;
    TN* root_delete = root;
    root = root->left;
    delete root_delete;
    return val;
  }
}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
T BSTMap<KEY,T,tlt>::remove (TN*& root, const KEY& key) {
  if (root == nullptr) { //from arraymap erase
    std::ostringstream answer;
    answer << "BSTMap::erase: key(" << key << ") not in Map";
    throw KeyError(answer.str());
  }else
    if (key == root->value.first) {
      T to_return = root->value.second;
      if (root->left == nullptr) {
        TN* to_delete = root;
        root = root->right;
        delete to_delete;
      }else if (root->right == nullptr) {
        TN* to_delete = root;
        root = root->left;
        delete to_delete;
      }else
        root->value = remove_closest(root->left);
      return to_return;
    }else
      return remove((lt(key,root->value.first) ? root->left : root->right), key);
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
void BSTMap<KEY,T,tlt>::delete_BST (TN*& root) {
	if (root == nullptr) { //don't raise errors for empty tree
		return;
	} else {
		delete(root->left);
		delete(root->right);
		delete root;
		root = nullptr;
	}
}






////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions
//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
BSTMap<KEY,T,tlt>::Iterator::Iterator(BSTMap<KEY,T,tlt>* iterate_over, bool from_begin)
{
	ref_map = iterate_over;
	expected_mod_count = ref_map->mod_count;
	if (from_begin) {
		ref_map->copy_to_queue(ref_map->map,it);
	}
}


template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
BSTMap<KEY,T,tlt>::Iterator::~Iterator()
{}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
auto BSTMap<KEY,T,tlt>::Iterator::erase() -> Entry {
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("BSTMap::Iterator::erase");
	if (!can_erase)
		throw CannotEraseError("BSTMap::Iterator::erase Iterator cursor already erased");

	Entry val = it.dequeue();
	can_erase = false;
	ref_map->erase(val.first);
	expected_mod_count = ref_map->mod_count;
	return val;
}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
std::string BSTMap<KEY,T,tlt>::Iterator::str() const {
	std::ostringstream string;
	string << ref_map->str() << "(it=" << it << ",expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";
	return string.str();
}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
auto  BSTMap<KEY,T,tlt>::Iterator::operator ++ () -> BSTMap<KEY,T,tlt>::Iterator& {
	if (expected_mod_count != ref_map->mod_count) {
		throw ConcurrentModificationError("BSTMap::Iterator::operator ++");
	}

	if (it.empty()) {
		return *this;
	}

	if (can_erase) {
		it.dequeue();
	} else {
		can_erase = true;
	}
	return *this;
}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
auto BSTMap<KEY,T,tlt>::Iterator::operator ++ (int) -> BSTMap<KEY,T,tlt>::Iterator {
	if (expected_mod_count != ref_map->mod_count) {
		throw ConcurrentModificationError("BSTMap::Iterator::operator ++(int)");
	}

	Iterator return_it(*this);

	if (it.empty()) {
		return return_it;
	}

	if (can_erase) {
		it.dequeue();
	} else {
		can_erase = true;
	}

	return return_it;
}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
bool BSTMap<KEY,T,tlt>::Iterator::operator == (const BSTMap<KEY,T,tlt>::Iterator& rhs) const {
	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
	if (rhsASI == 0)
		throw IteratorTypeError("BSTMap::Iterator::operator ==");
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("BSTMap::Iterator::operator ==");
	if (ref_map != rhs.ref_map)
		throw ComparingDifferentIteratorsError("BSTMap::Iterator::operator ==");

	return this->it.size() == rhs.it.size();
}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
bool BSTMap<KEY,T,tlt>::Iterator::operator != (const BSTMap<KEY,T,tlt>::Iterator& rhs) const {
	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
	if (rhsASI == 0)
		throw IteratorTypeError("BSTMap::Iterator::operator !=");
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("BSTMap::Iterator::operator !=");
	if (ref_map != rhs.ref_map)
		throw ComparingDifferentIteratorsError("BSTMap::Iterator::operator !=");

	return this->it.size() != rhs.it.size();
}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
pair<KEY,T>& BSTMap<KEY,T,tlt>::Iterator::operator *() const {
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("BSTMap::Iterator::operator *");
	if (!can_erase || it.empty())
		throw IteratorPositionIllegal("BSTMap::Iterator::operator * Iterator illegal: exhausted");
	return it.peek();
}

//code modified from array map
template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
pair<KEY,T>* BSTMap<KEY,T,tlt>::Iterator::operator ->() const {
	if (expected_mod_count !=ref_map->mod_count)
		throw ConcurrentModificationError("BSTMap::Iterator::operator ->");
	if (!can_erase || it.empty())
		throw IteratorPositionIllegal("BSTMap::Iterator::operator -> Iterator illegal: exhausted");
	return &(it.peek());
}


}

#endif /* BST_MAP_HPP_ */
