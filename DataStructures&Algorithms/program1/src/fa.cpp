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


typedef ics::ArrayQueue<std::string>                InputsQueue;
typedef ics::ArrayMap<std::string,std::string>      InputStateMap;

typedef ics::ArrayMap<std::string,InputStateMap>    FA;
typedef ics::pair<std::string,InputStateMap>        FAEntry;

bool gt_FAEntry (const FAEntry& a, const FAEntry& b)
{return a.first<b.first;}

typedef ics::ArrayPriorityQueue<FAEntry,gt_FAEntry> FAPQ;

typedef ics::pair<std::string,std::string>          Transition;
typedef ics::ArrayQueue<Transition>                 TransitionQueue;


//Read an open file describing the finite automaton (each line starts with
//  a state name followed by pairs of transitions from that state: (input
//  followed by new state, all separated by semicolons), and return a Map
//  whose keys are states and whose associated values are another Map with
//  each input in that state (keys) and the resulting state it leads to.
const FA read_fa(std::ifstream &file) {
	FA fa;
	while (!file.eof())
	{
		InputStateMap tempstatemap;
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
			tempstatemap.put(tempvect[2*i], tempvect[2*i+1]);
		}
		fa[start_state] =tempstatemap;
	}
	file.close();
	return fa;
}


//Print a label and all the entries in the finite automaton Map, in
//  alphabetical order of the states: each line has a state, the text
//  "transitions:" and the Map of its transitions.
void print_fa(const FA& fa) {
	for (auto i : fa)
	{
		std::cout << "\t" << i.first << " transitions: " << i.second << std::endl;
	}
}


//Return a queue of the calculated transition pairs, based on the finite
//  automaton, initial state, and queue of inputs; each pair in the returned
//  queue is of the form: input, new state.
//The first pair contains "" as the input and the initial state.
//If any input i is illegal (does not lead to a state in the finite
//  automaton), then the last pair in the returned queue is i,"None".
TransitionQueue process(const FA& fa, std::string state, const InputsQueue& inputs) {
	Transition temp = ics::pair<std::string, std::string>("", state);
	TransitionQueue transq;
	transq.enqueue(temp);
	for (auto i : inputs)
	{
		state = temp.second;
		if (fa[state].has_key(i))
		{
			temp = Transition(i, fa[state][i]);
			transq.enqueue(temp);
		}
		else
		{
			temp = Transition(state, "None");
			transq.enqueue(temp);
			break;
		}
	}
	return transq;
}


//Print a TransitionQueue (the result of calling the process function above)
// in a nice form.
//Print the Start state on the first line; then print each input and the
//  resulting new state (or "illegal input: terminated", if the state is
//  "None") indented on subsequent lines; on the last line, print the Stop
//  state (which may be "None").
void interpret(TransitionQueue& tq) {  //or TransitionQueue or TransitionQueue&&
	std::string currentstate;
	std::cout << "\nStart state = " << tq.dequeue().second << std::endl;
	for (auto i : tq)
	{
		currentstate = i.second;
		if (i.second!="None")
		{
			std::cout << "\tInput = " << i.first << "; new state = " << i.second << std::endl;
		}
		else
		{
			std::cout << "\tInput = " << i.first << "; illegal input: terminated" << std::endl;
		}
	}
	std::cout << "Stop state = " << currentstate << std::endl;
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
	  ics::safe_open(file, "Enter the name of a file with a Finite Automaton", "faparity.txt");

	  FA fa = read_fa(file);

	  std::cout << "\nFinite Automaton Description: " << std::endl;
	  print_fa(fa);

	  ics::safe_open(file2, "\nEnter the name of a file with the start-state and input", "fainputparity.txt");
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
		  TransitionQueue tq = process(fa, start_state, inputq);
		  interpret(tq);
	  }
	  file2.close();

  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
