#include "q6solution.hpp"
#include <vector>
#include <algorithm>                 // std::random_shuffle
#include <random>


//Print a linked list/array nicely, prefaced by label,
//  optionally printing whether or not it is sorted
template<class T>
void print (std::string label, LN<T>* l, bool report_sorted) {
  bool sorted = true;
  std::cout << label;
  for (LN<T>* c=l; c!=nullptr; c=c->next) {
    std::cout << c->value;
    if (c->next == nullptr)
      std::cout << std::endl;
    else {
      std::cout << ", ";
      if (c->value > c->next->value)
        sorted = false;
    }
  }
  if (report_sorted)
    std::cout << (sorted ? "    Sorted" : "    Not Sorted") << std::endl;
}


template<class T>
void print (std::string label, T a[], int length, bool report_sorted) {
  bool sorted = true;
  std::cout << label;
  for (int i=0; i<length; ++i) {
    std::cout << a[i];
    if (i == length-1)
      std::cout << std::endl;
    else {
      std::cout << ", ";
      if (a[i] > a[i+1])
        sorted = false;
    }
  }
  if (report_sorted)
    std::cout << (sorted ? "    Sorted" : "    Not Sorted") << std::endl;
}


void shuffle(int values[], int length) {
  std::vector<int> v_values;
  for (int i=0; i<length; ++i)
    v_values.push_back(values[i]);
  std::random_shuffle(v_values.begin(),v_values.end());
  for (int i=0; i<length; ++i)
    values[i] = v_values[i];
}

template<class T>
LN<T>* build (T a[], int length, int i) {
  if (i >= length)
    return nullptr;
  else
    return new LN<T>(a[i],build(a,length,i+1));
}



//Recursive Merge Sort
template<class T>
void merge_sort(T a[], int low, int high) {
  if (high - low < 1)               //Base case: 0 or 1 value to sort -> sorted
    return;
  else {
    int mid = (low + high)/2;       //Split in 1/2
    merge_sort(a, low, mid);        //Recursively sort low to mid
    merge_sort(a, mid+1, high);     //Recursively sort mid+1 to high
    merge(a, low,mid, mid+1,high);  //Merge sorted parts of array
  }
}

//Standard Merge Sort (calls a generalized one, with more parameters)
template<class T>
void merge_sort(T a[], int length) {
  merge_sort(a, 0, length-1);
}


int main() {
  try {

    //Selection Sort (for linked list)
    std::cout << "\nTesting selection sort" << std::endl;
    int test_selection_sort[10] = {1,2,3,4,5,6,7,8,9,10};
    for (int i=0; i<5; i++) {
      shuffle(test_selection_sort,10);
      LN<int>* t = build(test_selection_sort, 10, 0);
      print("\n  List before sort: ", t, false);
      selection_sort(t);
      print("  List after  sort: ", t, true);
    }



    //Merge (tested individually and in mergeSort)
    std::cout << "\n\nTesting merge by itself" << std::endl;
    int test_merge[10] = {1,2,5,8,10,   3,4,6,7,9};
    print("\n  Array before merge: ", test_merge, 10, false);
    merge(test_merge,0,4, 5,9);
    print("  Array after  merge: ", test_merge, 10, true);

    std::cout << "\n\nTesting merge in merge sort" << std::endl;
    int test_merge_sort[10] = {1,2,3,4,5,6,7,8,9,10};
    for (int i=0; i<5; i++) {
      shuffle(test_merge_sort, 10);
      print("\n  Array before sort: ", test_merge_sort, 10, false);
      merge_sort(test_merge_sort, 10);
      print("  Array after  sort: ", test_merge_sort, 10, true);
    }



    //Radix Sort (for 6 digit numbers)
    std::cout << "\n\nTesting radix sort" << std::endl;
    int test_radix_sort [30];

    //Show as errors in Eclipse, but code witll runn
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,1000000);   //0 - 999,999: up to 6 digits

    for (int i=0; i<30; ++i)
      test_radix_sort[i] = distribution(generator);

    for (int i=0; i<5; i++) {
      shuffle(test_radix_sort, 30);
      print("\n  Array before sort: ", test_radix_sort, 30, false);
      radix_sort(test_radix_sort, 30);
      print("  Array after  sort: ", test_radix_sort, 30, true);
    }

  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
