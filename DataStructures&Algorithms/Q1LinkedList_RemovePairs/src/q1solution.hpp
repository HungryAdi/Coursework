//Aditya Iyer (Access student in the process of enrolling) ID: 24377286

#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <math.h>            /* for atan2 and sqrt */
#include "ics46goody.hpp"
#include "ics_exceptions.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_map.hpp"


class Point {
public:
  Point() : x(0), y(0) {} // Needed for pair
  Point(int x_val, int y_val) : x(x_val), y(y_val) {}
  friend bool operator == (const Point& p1, const Point& p2) {
    return p1.x == p2.x && p1.y == p2.y;
  }
  friend std::ostream& operator << (std::ostream& outs, const Point& p) {
    outs << "(" << p.x << "," << p.y << ")";
    return outs;
  }
  int x;
  int y;
};


//Helper Functions (you decide what is useful)
//Hint: I used helpers for sort_xys, sort_angle, points, and first_quad

void sort_array(ics::pair<int,Point> pairArray[], int arraySize) {
	for (int i = 0; i < arraySize; ++i) {
		for (int j = i; j < arraySize; ++j) {
			if (pairArray[i].second.x > pairArray[j].second.x) {
				ics::pair<int,Point> temp_pair = pairArray[i];
				pairArray[i] = pairArray[j];
				pairArray[j] = temp_pair;
			} else if (pairArray[i].second.x == pairArray[j].second.x) {
				if (pairArray[i].second.y < pairArray[j].second.y) {
					ics::pair<int,Point> temp_pair = pairArray[i];
					pairArray[i] = pairArray[j];
					pairArray[j] = temp_pair;
				}
			}
		}
	}
}

void sort_array_angle(Point pointArray[], int arraySize) {
	for (int i = 0; i < arraySize; ++i) {
		for (int j = i; j < arraySize; ++j) {
			if (atan2(pointArray[i].y,pointArray[i].x) > atan2(pointArray[j].y,pointArray[j].x)) {
				Point temp_point = pointArray[i];
				pointArray[i] = pointArray[j];
				pointArray[j] = temp_point;
			}
		}
	}
}

void sort_array_ordinal(int intArray[], int arraySize) {
	for (int i = 0; i < arraySize; ++i) {
		for (int j = i; j < arraySize; ++j) {
			if (intArray[i] > intArray[j]) {
				int temp = intArray[i];
				intArray[i] = intArray[j];
				intArray[j] = temp;
			}
		}
	}
}

bool is_first_quad(Point point) {
	return ((point.x >= 0) && (point.y >= 0));
}


//Problem #1a and #1b
template<class KEY,class T>
void swap (ics::ArrayMap<KEY,T>& m, KEY key1, KEY key2) {
//Write code here
	m[key1] = m.put(key2,m[key1]);
}


template<class KEY,class T>
void values_set_to_queue (const ics::ArrayMap<KEY,ics::ArraySet<T>>& m1,
                          ics::ArrayMap<KEY,ics::ArrayQueue<T>>&     m2) {
//Write code here
	for (auto entry : m1) {
		m2[entry.first] = ics::ArrayQueue<T>(entry.second);
	}
}


//Problem #2a, #2b, #2c, and #2d
ics::ArrayQueue<ics::pair<int,Point>> sort_xys (const ics::ArrayMap<int,Point>& m) {
//Write code here
	ics::ArrayQueue<ics::pair<int,Point>> temp_queue;
	int size = m.size(), counter = 0;
	ics::pair<int,Point> pairArray[size];
	for (auto entry: m) {
		ics::pair<int,Point> temp_pair(entry.first,entry.second);
		pairArray[counter] = temp_pair;
		counter++;
	}
	sort_array(pairArray, size);
	for (int i = 0; i < size; ++i) {
		temp_queue.enqueue(pairArray[i]);
	}
	return temp_queue;
}

ics::ArrayQueue<Point> sort_angle (const ics::ArrayMap<int,Point>& m) {
//Write code here
	ics::ArrayQueue<Point> queue;
	int size = m.size(), counter = 0;
	Point pointArray[size];
	for (auto entry: m) {
		Point temp_point = entry.second;
		pointArray[counter] = temp_point;
		counter++;
	}
	sort_array_angle(pointArray, size);
	for (int i = 0; i < size; ++i) {
		queue.enqueue(pointArray[i]);
	}
	return queue;
}

ics::ArrayQueue<Point> points (const ics::ArrayMap<int,Point>& m) {
//Write code here
	ics::ArrayQueue<Point> queue;
		int size = m.size(), counter = 0;
		int ord_array[size];
		for (auto entry: m) {
			ord_array[counter] = entry.first;
			counter++;
		}
		sort_array_ordinal(ord_array, size);
		for (int i = 0; i < size; ++i) {
			queue.enqueue(m[ord_array[i]]);
		}
		return queue;
}


ics::ArrayMap<Point,double> first_quad (const ics::ArrayMap<int,Point>& m) {
//Write code here
	ics::ArrayMap<Point,double> return_map;
	for (auto entry : m) {
		if (is_first_quad(entry.second)) {
			double x = entry.second.x;
			double y = entry.second.y;
			double dist = sqrt(((x*x) + (y*y)));
			return_map[entry.second] = dist;
		}
	}
	return return_map;
}


//Problem #3
ics::ArrayMap<char,ics::ArraySet<char>> near(const std::string word, int dist) {
//Write code here
	ics::ArrayMap<char,ics::ArraySet<char>> char_map;
	for (int i = 0; i < int(word.size()); ++i) {
		//ics::ArraySet<char> char_set;
		for (int j = std::max<int>(0,i-dist); j <= std::min<int>(word.size()-1,i+dist); ++j) {
			char_map[word[i]].insert(word[j]);
		}
	}
	return char_map;
}


//Problem #4a and #4b
ics::ArrayMap<std::string,int> got_called(const  ics::ArrayMap<std::string,ics::ArrayMap<std::string,int>>& calls) {
//Write code here
	ics::ArrayMap<std::string,int> answer_map;
	for (auto entry : calls) {
		for (auto entry1 : entry.second) {
			answer_map[entry1.first] += entry1.second;
		}
	}
	return answer_map;
}


  ics::ArrayMap<std::string,ics::ArrayMap<std::string,int>> invert (const ics::ArrayMap<std::string,ics::ArrayMap<std::string,int>>& calls) {
//Write code here
	  ics::ArrayMap<std::string,ics::ArrayMap<std::string,int>> answer_map;
	  for (auto entry : calls) {
		  for (auto entry1 : entry.second) {
			  answer_map[entry1.first][entry.first] = entry1.second;
		  }
	  }
	  return answer_map;
}

#endif /* SOLUTION_HPP_ */


