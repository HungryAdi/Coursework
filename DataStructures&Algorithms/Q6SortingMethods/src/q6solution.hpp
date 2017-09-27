//(Iyer, Aditya) asiyer
#ifndef Q6SOLUTION_HPP_
#define Q6SOLUTION_HPP_


#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <algorithm>                 // see std::swap
#include "ics46goody.hpp"
#include "array_queue.hpp"


////////////////////////////////////////////////////////////////////////////////

//Problem #1

template<class T>
class LN {
  public:
    LN ()
      : value(), next()
    {}

    LN (const LN<T>& l)
      : value(l.value), next(l.next)
    {}

    LN (T value, LN* n = nullptr)
      : value(value), next(n)
    {}

  T   value;
  LN* next;
};


//Write this function
template<class T>
void selection_sort(LN<T>* l) {
	for (LN<T>* n = l; n != nullptr; n = n->next) {
		LN<T> * temp = n;
		for (LN<T>* node = temp->next; node != nullptr; node = node->next) {
			if (temp->value > node->value) {
				temp = node;
			}
		}
		std::swap(n->value,temp->value);
	}
}


////////////////////////////////////////////////////////////////////////////////

//Problem #2


//Precondition : Array values with indexes left_low  to left_high  are sorted
//Precondition : Array values with indexes right_low to right_high are sorted
//Note that left_high+1 = right_low (I pass both to make the code a bit simpler)
//Postcondition: Array values with indexes left_low  to right_high are sorted
//Hint: Merge into a temporary array (declared to be just big enough to store
//  all the needed values) and then copy that temporary array back into
//  the parameter's array (from left_low to right_high inclusively)
//See the quiz for pseudocode for this method

//Write this function
void merge(int a[], int left_low,  int left_high,
                    int right_low, int right_high) {
	int size = (right_high-left_low)+1;
	int temp[size];
	int l = left_low;
	int r = right_low;
	int i = 0;
	while (i < size) {
		if (l > left_high) {
			temp[i] = a[r++];
		} else if (r > right_high) {
			temp[i] = a[l++];
		} else if (a[l] <= a[r]) {
			temp[i] = a[l++];
		} else {
			temp[i] = a[r++];
		}
		i++;
	}

	for (int j = 0; j < size; ++j) {
		a[left_low++] = temp[j];
	}
}


////////////////////////////////////////////////////////////////////////////////

//Problem #3

int select_digit (int number, int place)
{return number/place % 10;}


//Write this function
void radix_sort(int a[], int length) {
	ics::ArrayQueue<int> array[10];
	for (int place = 1; place <= 100000; place*=10) {
		int count = 0;
		for (int i = 0; i < length; ++i) {
			array[select_digit(a[i],place)].enqueue(a[i]);
		}
		for (int j = 0; j < 10; ++j) {
			while (!array[j].empty()) {
				a[count++] = array[j].dequeue();
			}
		}
	}
}

#endif /* Q6SOLUTION_HPP_ */
