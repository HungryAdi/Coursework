// Submitter: asiyer(Iyer, Aditya)
// Partner: ssthothr(Sthothra Bhashyam, Sreeja)
// We certify that we worked cooperatively on this programming
//   assignment, according to the rules for pair programming
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>                    //Biggest int: std::numeric_limits<int>::max()
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_set.hpp"
#include "array_map.hpp"


typedef ics::ArrayQueue<std::string>              CandidateQueue;
typedef ics::ArraySet<std::string>                CandidateSet;
typedef ics::ArrayMap<std::string,int>            CandidateTally;

typedef ics::ArrayMap<std::string,CandidateQueue> Preferences;
typedef ics::pair<std::string,CandidateQueue>     PreferencesEntry;
typedef ics::ArrayPriorityQueue<PreferencesEntry> PreferencesEntryPQ; //Must supply gt at construction

typedef ics::pair<std::string,int>                TallyEntry;
typedef ics::ArrayPriorityQueue<TallyEntry>       TallyEntryPQ;



//Read an open file stating voter preferences (each line is (a) a voter
//  followed by (b) all the candidates the voter would vote for, in
//  preference order (from most to least preferred candidate, separated
//  by semicolons), and return a Map of preferences: a Map whose keys are
//  voter names and whose values are a queue of candidate preferences.
Preferences read_voter_preferences(std::ifstream &file) {
	Preferences pref;
	std::string delimiter = ";";
	int line_length;
	while (!file.eof())
		{
		CandidateQueue candidate;
			std::string line, voter;
			getline(file,line,'\r');
			file.get();
			voter = line.substr(0,line.find(delimiter));
			line.erase(0, line.find(delimiter) + delimiter.length());
			for (int i = 0; i < line.length(); ++i) {
				std::string temp_string;
				temp_string += line[i];
				if (temp_string != delimiter) {
					candidate.enqueue(temp_string);
				}
			}
			pref[voter] = candidate;
		}
		file.close();
		return pref;
}


//Print a label and all the entries in the preferences Map, in alphabetical
//  order according to the voter.
//Use a "->" to separate the voter name from the Queue of candidates.
void print_voter_preferences(const Preferences& preferences) {
	std::string voters[preferences.size()];
	int counter = 0;
	for (auto entry : preferences) {
		voters[counter++] = entry.first;
	}
	std::cout << "\nVoter Preferences" << std::endl;

	for (int i = 0; i < preferences.size(); ++i) {
		std::cout << "  " << voters[i] << " -> " << preferences[voters[i]] << std::endl;
	}
	std::cout << std::endl;

}


//Print the message followed by all the entries in the CandidateTally, in
//  the order specified by has_higher_priority: i is printed before j, if
//  has_higher_priority(i,j) returns true: sometimes alphabetically by candidate,
//  other times by decreasing votes for the candidate.
//Use a "->" to separate the candidat name from the number of votes they
//  received.
void print_tally(std::string message, const CandidateTally& tally, bool (*has_higher_priority)(const TallyEntry& i,const TallyEntry& j)) {
	std::cout << message << std::endl;
	TallyEntryPQ pq(tally,has_higher_priority);
	for (auto entry1: pq) {
		std::cout << entry1.first << " -> " << entry1.second << std::endl;
	}
}


//Return the CandidateTally: a Map of candidates (as keys) and the number of
//  votes they received, based on the unchanging Preferences (read from the
//  file) and the candidates who are currently still in the election (which changes).
//Every possible candidate should appear as a key in the resulting tally.
//Each voter should tally one vote: for their highest-ranked candidate who is
//  still in the the election.
CandidateTally evaluate_ballot(const Preferences& preferences, const CandidateSet& candidates) {
	CandidateTally tally;
	for (auto candidate : candidates) {
		int vote_count = 0;
		for (auto vote : preferences) {
			//std::cout << vote << std::endl;
			for (CandidateQueue::Iterator i = vote.second.begin(); i != vote.second.end(); ++i) {
				if (candidates.contains(*i)) {
					if (*i == candidate) {
						vote_count++;
						break;
					} else {
						break;
					}
				}
			}
		}
		tally[candidate] = vote_count;
	}
	return tally;
}


//Return the Set of candidates who are still in the election, based on the
//  tally of votes: compute the minimum number of votes and return a Set of
//  all candidates receiving more than that minimum; if all candidates
//  receive the same number of votes (that would be the minimum), the empty
//  Set is returned.
CandidateSet remaining_candidates(const CandidateTally& tally) {
	int minimum = std::numeric_limits<int>::max();
	CandidateSet candidates;
	for (auto entry : tally) {
		if (entry.second < minimum) {
			minimum = entry.second;
		}
	}
	for (auto entry1 : tally) {
		if (entry1.second > minimum) {
			candidates.insert(entry1.first);
		}
	}
	return candidates;
}


//Prompt the user for a file, create a voter preference Map, and print it.
//Determine the Set of all the candidates in the election, from this Map.
//Repeatedly evaluate the ballot based on the candidates (still) in the
//  election, printing the vote count (tally) two ways: with the candidates
//  (a) shown alphabetically increasing and (b) shown with the vote count
//  decreasing (candidates with equal vote counts are shown alphabetically
//  increasing); from this tally, compute which candidates remain in the
//  election: all candidates receiving more than the minimum number of votes;
//  continue this process until there are less than 2 candidates.
//Print the final result: there may 1 candidate left (the winner) or 0 left
//   (no winner).
int main() {
  try {
	  std::string filename;
	  std:: ifstream file;
	  ics::safe_open(file,"Enter the name of a file with a graph","votepref1.txt");

	  Preferences pref = read_voter_preferences(file);
	  print_voter_preferences(pref);

	  CandidateSet still;
	  for (auto entry: pref) {
		  still.insert(entry.second.peek());
	  }
	  int counter = 1;
	  while(still.size() > 1) {
		 CandidateTally tally = evaluate_ballot(pref, still);

		 std::ostringstream message_alph;
		 message_alph << "\nVote count on ballot #" << counter << " with candidates in alphabetical order: still in election = " << still;
		 print_tally(message_alph.str(), tally, [](const TallyEntry& i,const TallyEntry& j) { return i.first == j.first ? i.second < j.second : i.first < j.first; });

		 std::ostringstream message_num;
		 message_num << "\nVote count on ballot #" << counter << " with candidates in numerical order: still in election = " << still;
		 print_tally(message_num.str(), tally, [](const TallyEntry& i,const TallyEntry& j) { return i.second == j.second ? i.first < j.first : i.second > j.second; });

		 still = remaining_candidates(tally);
		 counter++;
	  }
	  if (still.size() == 1) {
		  std::cout << "\nWinner is ";
		  for (auto entry : still) {
			  std::cout << entry << " ";
		  }
	  } else {
		  std::cout << "\nNo winner: election is a tie among all candidates remaining on the last ballot." << std::endl;
	  }

  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
