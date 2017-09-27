// Submitter: asiyer(Iyer, Aditya)
// Partner: ssthothr(Sthothra Bhashyam, Sreeja)
// We certify that we worked cooperatively on this programming
//   assignment, according to the rules for pair programming

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_set.hpp"
#include "array_map.hpp"


typedef ics::ArraySet<std::string>                     States;
typedef ics::ArrayQueue<std::string>                   InputsQueue;
typedef ics::ArrayMap<std::string,States>              InputStatesMap;

typedef ics::ArrayMap<std::string,InputStatesMap>       NDFA;
typedef ics::pair<std::string,InputStatesMap>           NDFAEntry;

bool gt_NDFAEntry (const NDFAEntry& a, const NDFAEntry& b)
{return a.first<b.first;}

typedef ics::ArrayPriorityQueue<NDFAEntry,gt_NDFAEntry> NDFAPQ;

typedef ics::pair<std::string,States>                   Transitions;
typedef ics::ArrayQueue<Transitions>                    TransitionsQueue;


//Read an open file describing the non-deterministic finite automaton (each
//  line starts with a state name followed by pairs of transitions from that
//  state: (input followed by a new state, all separated by semicolons), and
//  return a Map whose keys are states and whose associated values are another
//  Map with each input in that state (keys) and the resulting set of states it
//  can lead to.
const NDFA read_ndfa(std::ifstream &file) {
	NDFA ndfa;
	while (!file.eof())
	{
		InputStatesMap tempstatemap;
		std::string start_state, temp_string;
		std::vector<std::string> tempvect;
		getline(file, temp_string);
		if (temp_string=="")
		{
			break;
		}
		tempvect = ics::split(temp_string, ";");
		start_state = tempvect[0];
		tempvect.erase(tempvect.begin());
		for (int i = 0; i < tempvect.size()/2; i++)
		{
			States tempset;
			tempstatemap[tempvect[2*i]].insert(tempvect[2*i+1]);
		}
		ndfa[start_state] =tempstatemap;
	}
	file.close();
	return ndfa;
}


//Print a label and all the entries in the finite automaton Map, in
//  alphabetical order of the states: each line has a state, the text
//  "transitions:" and the Map of its transitions.
void print_ndfa(const NDFA& ndfa) {
	for (auto i : ndfa)
	{
		std::cout << "\t" << i.first << " transitions: " << i.second << std::endl;
	}
}


//Return a queue of the calculated transition pairs, based on the non-deterministic
//  finite automaton, initial state, and queue of inputs; each pair in the returned
//  queue is of the form: input, set of new states.
//The first pair contains "" as the input and the initial state.
//If any input i is illegal (does not lead to any state in the non-deterministic finite
//  automaton), ignore it.
TransitionsQueue process(const NDFA& ndfa, std::string state, const InputsQueue& inputs) {
	States startstates; //ics::ArraySet<std::string>
	startstates.insert(state);
	Transitions temp = ics::pair<std::string, States>("", startstates);
	TransitionsQueue transq;
	transq.enqueue(temp);
	for (auto i : inputs)
	{
		Transitions temptransition;
		temptransition.first=i;
		for (auto s : startstates)
		{
			if (ndfa[s].has_key(i))
			{
				for (auto x : ndfa[s][i])
				{
					temptransition.second.insert(x);
				}
			}
		}
		transq.enqueue(temptransition);
		startstates=temptransition.second;
	}
	return transq;
}


//Print a TransitionsQueue (the result of calling process) in a nice form.
//Print the Start state on the first line; then print each input and the
//  resulting new states indented on subsequent lines; on the last line, print
//  the Stop state.
void interpret(TransitionsQueue& tq) {  //or TransitionsQueue or TransitionsQueue&&
	ics::ArraySet<std::string> currentstate;
	std::cout << "\nStart state = " << tq.dequeue().second << std::endl;
	for (auto i : tq)
	{
		currentstate = i.second;
		std::cout << "\tInput = " << i.first << "; new state = " << i.second << std::endl;

	}
	std::cout << "Stop state(s) = " << currentstate << std::endl;
	std::cout << "" << std::endl;
}



//Prompt the user for a file, create a finite automaton Map, and print it.
//Prompt the user for a file containing any number of simulation descriptions
//  for the finite automaton to process, one description per line; each
//  description contains a start state followed by its inputs, all separated by
//  semicolons.
//Repeatedly read a description, print that description, put each input in a
//  Queue, process the Queue and print the results in a nice form.
int main() {
  try {

	  std::ifstream file, file2;

	  ics::safe_open(file, "Enter the name of a file with a Non-Deterministic Finite Automaton", "ndfaendin01.txt");

	  NDFA ndfa = read_ndfa(file);

	  std::cout << "Non-Deterministic Finite Automaton Description: " << std::endl;
	  print_ndfa(ndfa);
	  std::cout << "" << std::endl;

	  ics::safe_open(file2, "Enter the name of a file with the start-state and input", "ndfainputendin01.txt");
	  std::cout << "" << std::endl;

	  while (!file2.eof())
	  {
		  std::string currentline, start_state;
		  InputsQueue inputq;
		  std::vector<std::string> currentvector;
		  getline(file2, currentline,'\r');
		  file2.get();
		  if (currentline=="")
		  {
			  break;
		  }
		  std::cout << "Starting new simulation with description: " << currentline;
		  currentvector = ics::split(currentline, ";");
		  inputq.enqueue_all(currentvector);
		  start_state = inputq.dequeue();
		  TransitionsQueue tq = process(ndfa, start_state, inputq);
		  interpret(tq);
	  }
	  file2.close();

  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
