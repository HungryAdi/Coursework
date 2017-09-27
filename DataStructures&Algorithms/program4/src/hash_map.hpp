//(Iyer, Aditya Subramani) asiyer
#ifndef HASH_MAP_HPP_
#define HASH_MAP_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include "pair.hpp"


namespace ics {


//Instantiate the templated class supplying thash(a): produces a hash value for a.
//If thash is defaulted to nullptr in the template, then a constructor must supply chash.
//If both thash and chash are supplied, then they must be the same (by ==) function.
//If neither is supplied, or both are supplied but different, TemplateFunctionError is raised.
//The (unique) non-nullptr value supplied by thash/chash is stored in the instance variable hash.
template<class KEY,class T, int (*thash)(const KEY& a) = nullptr> class HashMap {
  public:
    typedef ics::pair<KEY,T>   Entry;

    //Destructor/Constructors
    ~HashMap ();

    HashMap          (double the_load_threshold = 1.0, int (*chash)(const KEY& a) = nullptr);
    explicit HashMap (int initial_bins, double the_load_threshold = 1.0, int (*chash)(const KEY& k) = nullptr);
    HashMap          (const HashMap<KEY,T,thash>& to_copy, double the_load_threshold = 1.0, int (*chash)(const KEY& a) = nullptr);
    explicit HashMap (const std::initializer_list<Entry>& il, double the_load_threshold = 1.0, int (*chash)(const KEY& a) = nullptr);

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    explicit HashMap (const Iterable& i, double the_load_threshold = 1.0, int (*chash)(const KEY& a) = nullptr);


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
    HashMap<KEY,T,thash>& operator = (const HashMap<KEY,T,thash>& rhs);
    bool operator == (const HashMap<KEY,T,thash>& rhs) const;
    bool operator != (const HashMap<KEY,T,thash>& rhs) const;

    template<class KEY2,class T2, int (*hash2)(const KEY2& a)>
    friend std::ostream& operator << (std::ostream& outs, const HashMap<KEY2,T2,hash2>& m);



  private:
    class LN;

  public:
    class Iterator {
      public:
         typedef pair<int,LN*> Cursor;

        //Private constructor called in begin/end, which are friends of HashMap<T>
        ~Iterator();
        Entry       erase();
        std::string str  () const;
        HashMap<KEY,T,thash>::Iterator& operator ++ ();
        HashMap<KEY,T,thash>::Iterator  operator ++ (int);
        bool operator == (const HashMap<KEY,T,thash>::Iterator& rhs) const;
        bool operator != (const HashMap<KEY,T,thash>::Iterator& rhs) const;
        Entry& operator *  () const;
        Entry* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const HashMap<KEY,T,thash>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }
        friend Iterator HashMap<KEY,T,thash>::begin () const;
        friend Iterator HashMap<KEY,T,thash>::end   () const;

      private:
        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
        Cursor               current; //Bin Index with Pointer; stop: LN* == nullptr
        HashMap<KEY,T,thash>* ref_map;
        int                  expected_mod_count;
        bool                 can_erase = true;

        //Helper methods
        void advance_cursors();

        //Called in friends begin/end
        Iterator(HashMap<KEY,T,thash>* iterate_over, bool from_begin);
    };


    Iterator begin () const;
    Iterator end   () const;


  private:
    class LN {
    public:
      LN ()                         : next(nullptr){}
      LN (const LN& ln)             : value(ln.value), next(ln.next){}
      LN (Entry v, LN* n = nullptr) : value(v), next(n){}

      Entry value;
      LN*   next;
  };

  int (*hash)(const KEY& k);  //Hashing function used (from template or constructor)
  LN** map      = nullptr;    //Pointer to array of pointers: each bin stores a list with a trailer node
  double load_threshold;      //used/bins <= load_threshold
  int bins      = 1;          //# bins in array (should start > 0 so hash_compress doesn't % 0)
  int used      = 0;          //Cache for number of key->value pairs in the hash table
  int mod_count = 0;          //For sensing concurrent modification


  //Helper methods
  int   hash_compress        (const KEY& key)          const;  //hash function ranged to [0,bins-1]
  LN*   find_key             (int bin, const KEY& key) const;  //Returns reference to key's node or nullptr
  LN*   copy_list            (LN*   l)                 const;  //Copy the keys/values in a bin (order irrelevant)
  LN**  copy_hash_table      (LN** ht, int bins)       const;  //Copy the bins/keys/values in ht tree (order in bins irrelevant)

  void  ensure_load_threshold(int new_used);                   //Reallocate if load_factor > load_threshold
  void  delete_hash_table    (LN**& ht, int bins);             //Deallocate all LN in ht (and the ht itself; ht == nullptr)
};




////////////////////////////////////////////////////////////////////////////////
//
//HashMap class and related definitions

//Destructor/Constructors

template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::~HashMap() {
	delete_hash_table(this->map, this->bins);

}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::HashMap(double the_load_threshold, int (*chash)(const KEY& k))
{
	if (thash != nullptr) {
		this->hash = thash;
	} else {
		this->hash = chash;
	}

	if (this->hash == nullptr) {
		throw TemplateFunctionError("HashMap::default constructor: neither specified");
	}

	if (thash != nullptr && chash != nullptr && thash != chash) {
		throw TemplateFunctionError("HashMap::default constructor: both specified and different");
	}

	this->map = new LN*[this->bins];
	map[0] = new LN();
	this->load_threshold = the_load_threshold;
}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::HashMap(int initial_bins, double the_load_threshold, int (*chash)(const KEY& k))
{
	if (thash != nullptr) {
		this->hash = thash;
	} else {
		this->hash = chash;
	}

	if (this->hash == nullptr) {
		throw TemplateFunctionError("HashMap::default constructor: neither specified");
	}

	if (thash != nullptr && chash != nullptr && thash != chash) {
		throw TemplateFunctionError("HashMap::default constructor: both specified and different");
	}

	this->bins = initial_bins;
	this->map = new LN*[this->bins];
	this->load_threshold = the_load_threshold;

	for (int i=0; i<this->bins; ++i) {
		this->map[i] = new LN();
	}
}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::HashMap(const HashMap<KEY,T,thash>& to_copy, double the_load_threshold, int (*chash)(const KEY& a))
{
	if (thash != nullptr) {
		this->hash = thash;
	} else {
		this->hash = chash;
	}

	if (this->hash == nullptr) {
		throw TemplateFunctionError("HashMap::copy constructor: neither specified");
	}

	if (thash != nullptr && chash != nullptr && thash != chash) {
		throw TemplateFunctionError("HashMap::copy constructor: both specified and different");
	}

	this->load_threshold = the_load_threshold;
	this->map = new LN*[this->bins];
	this->map[0] = new LN();
	this->put_all(to_copy);
}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::HashMap(const std::initializer_list<Entry>& il, double the_load_threshold, int (*chash)(const KEY& k))
{
	if (thash != nullptr) {
		this->hash = thash;
	} else {
		this->hash = chash;
	}

	if (this->hash == nullptr) {
		throw TemplateFunctionError("HashMap::Initializer List constructor: neither specified");
	}

	if (thash != nullptr && chash != nullptr && thash != chash) {
		throw TemplateFunctionError("HashMap::Intializer List constructor: both specified and different");
	}

	this->load_threshold = the_load_threshold;
	this->map = new LN*[this->bins];
	this->map[0] = new LN();
	put_all(il);

}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
template <class Iterable>
HashMap<KEY,T,thash>::HashMap(const Iterable& i, double the_load_threshold, int (*chash)(const KEY& k))
{
	if (thash != nullptr) {
		this->hash = thash;
	} else {
		this->hash = chash;
	}

	if (this->hash == nullptr) {
		throw TemplateFunctionError("HashMap::Initializer List constructor: neither specified");
	}

	if (thash != nullptr && chash != nullptr && thash != chash) {
		throw TemplateFunctionError("HashMap::Intializer List constructor: both specified and different");
	}

	this->load_threshold = the_load_threshold;
	this->map = new LN*[this->bins];
	this->map[0] = new LN();
	put_all(i);
}


////////////////////////////////////////////////////////////////////////////////
//
//Queries

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::empty() const {
	return this->used == 0;
}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
int HashMap<KEY,T,thash>::size() const {
	return this->used;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::has_key (const KEY& key) const {
	int index = this->hash_compress(key);
	return (find_key(index, key) != nullptr);
}


template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::has_value (const T& value) const {
	LN* temp;
	for (int i=0; i<this->bins; ++i) {
		temp = this->map[i];
		while (temp->next != nullptr) {
			if (temp->value.second == value) {
				return true;
			}
			temp = temp->next;
		}
	}
	return false;
}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
std::string HashMap<KEY,T,thash>::str() const {
	std::ostringstream answer;
	LN* temp;
	for (int i=0; i<this->bins; ++i) {
		answer << "bin[" << i << "]: ";
		temp = this->map[i];
		while (temp->next != nullptr) {
			answer << temp->value.first << "->" << temp->value.second << " -> ";
			temp = temp->next;
		}
	}
	answer << "(bins=" << this->bins << ",used=" << this->used << ",mod_count=" << this->mod_count << ")";
	return answer.str();
}


////////////////////////////////////////////////////////////////////////////////
//
//Commands

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
T HashMap<KEY,T,thash>::put(const KEY& key, const T& value) {
		ensure_load_threshold(this->used+1);
		int index = this->hash_compress(key);
		LN* n = this->find_key(index, key);
		T to_return;

		if (n != nullptr) {
			to_return = n->value.second;
			n->value.second = value;
			return to_return;
		} else {
			n = this->map[index];
			this->map[index] = new LN(Entry(key, value));
			this->map[index]->next = n;
			++this->mod_count;
			++this->used;
			return value;
		}
}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
T HashMap<KEY,T,thash>::erase(const KEY& key) {
	int index = this->hash_compress(key);
	LN* n = this->find_key(index, key);

	if (n != nullptr) {
		T value = n->value.second;
		LN* node = n->next;
		n->value = n->next->value;
		n->next = n->next->next;
		delete node;
		++this->mod_count;
		--this->used;
		return value;
	}

	std::ostringstream answer;
	answer << "HashMap::erase: key(" << key << ") not in Map";
	throw KeyError(answer.str());
}


template<class KEY,class T, int (*thash)(const KEY& a)>
void HashMap<KEY,T,thash>::clear() {
	this->delete_hash_table(this->map, this->bins);
}


template<class KEY,class T, int (*thash)(const KEY& a)>
template<class Iterable>
int HashMap<KEY,T,thash>::put_all(const Iterable& i) {
	int put_count = 0;
	for (auto entry : i) {
		put_count++;
		this->put(entry.first, entry.second);
	}
	return put_count;
}


////////////////////////////////////////////////////////////////////////////////
//
//Operators

template<class KEY,class T, int (*thash)(const KEY& a)>
T& HashMap<KEY,T,thash>::operator [] (const KEY& key) {
	int index = hash_compress(key);
	LN* n = find_key(index, key);
	if (n != nullptr) {
		return n->value.second;
	}

	ensure_load_threshold(used+1);

	index = hash_compress(key); // call after load threshold is adjusted
	n = this->map[index];
	this->map[index] = new LN(Entry(key, T()));
	this->map[index]->next = n;
	++this->mod_count;
	++this->used;
	return this->map[index]->value.second;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
const T& HashMap<KEY,T,thash>::operator [] (const KEY& key) const {
	int index = hash_compress(key);
	LN* n = find_key(index, key);

	if (n != nullptr) {
		return n->value.second;
	}

	std::ostringstream answer;
	answer << "HashMap::operator []: key(" << key << ") not in Map";
	throw KeyError(answer.str());
}


template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>& HashMap<KEY,T,thash>::operator = (const HashMap<KEY,T,thash>& rhs) {
	if (this == &rhs)
	{
		return *this;
	}
	delete_hash_table(this->map, this->bins);
	this->used = rhs.used;
	this->bins = rhs.bins;
	this->map = copy_hash_table(rhs.map, rhs.bins);
	++this->mod_count;
	return *this;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::operator == (const HashMap<KEY,T,thash>& rhs) const {
	if (this == &rhs)
	{
		return true;
	}
	if (used != rhs.size())
	{
		return false;
	}
	for (int i = 0; i < bins; i++)
	{
		LN* n = map[i];
		while (n->next != nullptr)
		{
			if (n->value.second != rhs[n->value.first])
			{
				return false;
			}
			n = n->next;
		}
	}
	return true;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::operator != (const HashMap<KEY,T,thash>& rhs) const {
	return !(*this == rhs);
}


template<class KEY,class T, int (*thash)(const KEY& a)>
std::ostream& operator << (std::ostream& outs, const HashMap<KEY,T,thash>& m) {
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors

template<class KEY,class T, int (*thash)(const KEY& a)>
auto HashMap<KEY,T,thash>::begin () const -> HashMap<KEY,T,thash>::Iterator {
	return Iterator(const_cast<HashMap<KEY,T,thash>*>(this), true);
}


template<class KEY,class T, int (*thash)(const KEY& a)>
auto HashMap<KEY,T,thash>::end () const -> HashMap<KEY,T,thash>::Iterator {
	return Iterator(const_cast<HashMap<KEY,T,thash>*>(this), false);
}


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

template<class KEY,class T, int (*thash)(const KEY& a)>
int HashMap<KEY,T,thash>::hash_compress (const KEY& key) const {
	int hash = this->hash(key);
	int index = abs(hash)%bins;
	return index;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
typename HashMap<KEY,T,thash>::LN* HashMap<KEY,T,thash>::find_key (int bin, const KEY& key) const {
	LN* n = this->map[bin];
	while (n->next != nullptr) {
		if (n->value.first == key) {
			return n;
		}
		n = n->next;
	}
	return nullptr;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
typename HashMap<KEY,T,thash>::LN* HashMap<KEY,T,thash>::copy_list (LN* l) const {
	if (l == nullptr) {
		return nullptr;
	} else {
		return new LN(l->value, copy_list(l->next));
	}
}


template<class KEY,class T, int (*thash)(const KEY& a)>
typename HashMap<KEY,T,thash>::LN** HashMap<KEY,T,thash>::copy_hash_table (LN** ht, int bins) const {
	LN** new_map = new LN*[bins];
	for(int i = 0; i < this->bins; i++){
		new_map[i] = copy_list(ht[i]);
	}
	return new_map;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
void HashMap<KEY,T,thash>::ensure_load_threshold(int new_used) {
	if ((double)(new_used)/bins <= load_threshold) {
		return;
	}

	LN** old_map = this->map;
	int old_bin = this->bins;
	this->bins = this->bins*2;
	this->map = new LN*[bins];

	for (int i=0; i<this->bins; ++i) {
		this->map[i] = new LN();
	}

	LN* current;
	LN* next;
	int index = 0;
	for (int i=0; i<old_bin; ++i) {
		current = old_map[i];
		while (current->next != nullptr) {
			index = this->hash_compress(current->value.first);
			next = current->next;

			current->next = this->map[index];
			map[index] = current;
			old_map[i] = next;
			current = next;
		}
	}

	delete_hash_table(old_map, old_bin);

}


template<class KEY,class T, int (*thash)(const KEY& a)>
void HashMap<KEY,T,thash>::delete_hash_table (LN**& ht, int bins) {
	LN* current;
	LN* next;
	for (int i=0; i<this->bins; ++i) {
		current = ht[i];
		while (current != nullptr) {
			next = current->next;
			delete current;
			current = next;
		}
	}
	delete[] ht;
}






////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

template<class KEY,class T, int (*thash)(const KEY& a)>
void HashMap<KEY,T,thash>::Iterator::advance_cursors(){
	if (this->current.second->next->next != nullptr) {
		current.second = current.second->next;
		return;
	}

	for (int i=current.first+1; i<=(ref_map->bins-1); ++i) {
		if (ref_map->map[i]->next != nullptr) {
			current.first = i;
			current.second = ref_map->map[i];
			return;
		}
	}
	current.first = -1; // end of the map
	current.second = nullptr;
	return;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::Iterator::Iterator(HashMap<KEY,T,thash>* iterate_over, bool from_begin)
: ref_map(iterate_over), expected_mod_count(ref_map->mod_count) {
	if (from_begin == false || ref_map->empty()) {
		current = Cursor(-1, nullptr);
	}
	else {
		for (int i=0; i<ref_map->bins; ++i) {
			if (ref_map->map[i]->next != nullptr) {
				current.first = i;
				current.second = ref_map->map[i];
				break;
			}
		}
	}
}


template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::Iterator::~Iterator()
{}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
auto HashMap<KEY,T,thash>::Iterator::erase() -> Entry {
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("HashMap::Iterator::erase");
	if (!can_erase)
		throw CannotEraseError("HashMap::Iterator::erase Iterator cursor already erased");
	if (current.first == -1 && current.second == nullptr)
		throw CannotEraseError("HashMap::Iterator::erase Iterator cursor beyond data structure");

	can_erase = false;
	Entry to_return = current.second->value;

	if (current.second->next->next == nullptr) {
		advance_cursors();
	}

	ref_map->erase(to_return.first);
	expected_mod_count = ref_map->mod_count;
	return to_return;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
std::string HashMap<KEY,T,thash>::Iterator::str() const {
}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
auto  HashMap<KEY,T,thash>::Iterator::operator ++ () -> HashMap<KEY,T,thash>::Iterator& {
	if (expected_mod_count != ref_map->mod_count) {
		throw ConcurrentModificationError("HashMap::Iterator::operator ++");
	}

	if (current.first == -1 && current.second == nullptr) {
		return *this;
	}

	if (can_erase) {
		advance_cursors();
	} else {
		can_erase = true;
	}

	return *this;
}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
auto  HashMap<KEY,T,thash>::Iterator::operator ++ (int) -> HashMap<KEY,T,thash>::Iterator {
	if (expected_mod_count != ref_map->mod_count) {
		throw ConcurrentModificationError("HashMap::Iterator::operator ++(int)");
	}

	if (current.first == -1 && current.second == nullptr) {
		return *this;
	}

	Iterator to_return(*this);

	if (can_erase) {
		advance_cursors();
	}
	else {
		can_erase = true;
	}

	return to_return;
}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::Iterator::operator == (const HashMap<KEY,T,thash>::Iterator& rhs) const {
	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
	if (rhsASI == 0)
		throw IteratorTypeError("HashMap::Iterator::operator ==");
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("HashMap::Iterator::operator ==");
	if (ref_map != rhsASI->ref_map)
		throw ComparingDifferentIteratorsError("HashMap::Iterator::operator ==");

	return current == rhsASI->current;
}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::Iterator::operator != (const HashMap<KEY,T,thash>::Iterator& rhs) const {
	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
	if (rhsASI == 0)
		throw IteratorTypeError("HashMap::Iterator::operator ==");
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("HashMap::Iterator::operator ==");
	if (ref_map != rhsASI->ref_map)
		throw ComparingDifferentIteratorsError("HashMap::Iterator::operator ==");

	return current != rhsASI->current;
}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
pair<KEY,T>& HashMap<KEY,T,thash>::Iterator::operator *() const {
	if (expected_mod_count != ref_map->mod_count) {
		throw ConcurrentModificationError("HashMape::Iterator::operator *");
	}
	if (!can_erase || (current.first == -1 && current.second == nullptr)) {
		std::ostringstream where;
		where << current.first << " when size = " << ref_map->bins;
		throw IteratorPositionIllegal("HashMape::Iterator::operator * Iterator illegal: "+where.str());
	}
	return current.second->value;
}

//Some code modified from previous programs
template<class KEY,class T, int (*thash)(const KEY& a)>
pair<KEY,T>* HashMap<KEY,T,thash>::Iterator::operator ->() const {
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("HashMape::Iterator::operator ->");
	if (!can_erase || (current.first == -1 && current.second == nullptr)) {
		std::ostringstream where;
		where << current.first << " when bins = " << ref_map->bins;
		throw IteratorPositionIllegal("HashMape::Iterator::operator -> Iterator illegal: "+where.str());
	}
	return &current.second->value;
}


}

#endif /* HASH_MAP_HPP_ */
