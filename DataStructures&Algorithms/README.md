## [Data Structures and Algorithms](https://github.com/HungryAdi/Coursework/tree/master/DataStructures%26Algorithms)
### All programs written in C++
  * Quiz 1: Linked List Remove Pairs
    - Iterative and Recursive Functions that take a linked list as a parameter. The functions remove any duplicate pairs of values contained in the list while maintaining the order of the remaining elements.
  
  * Quiz 6: Sorting Methods
    - Selection Sort: O(n^2) sorting algorithm written to sort the values of a linked list by changing node values rather than the order of nodes.
  
  * Course Library
    - Library containing data structures and testing harnesses used in class quizzes and programming assignments.
  
  * Google Test Library
    - Version of the Google C++ Testing Library used to run tests on class assignments.
  
  * Program 1: ICS 46 Template Library
    - Reachability: Store information from a file representing a graph into a map, and compute all the nodes that are reachable from it by following edges in the graph.
    - Instant Runoff Voting: Store information from a file representing the candidate preferences of voters and store it in a Map. Display the vote count for ballots eliminating from the election the candidates receiving the fewest votes, until a winner or a tie remains.
    - Finite Automata: Read the information describing a finite automaton and store it in a Map. Prompt the user to enter the name of a file storing the start-state and inputs to process and compute the results of the finite automaton on each input, and then display a trace of the results.
    - Non-Deterministic Finite Automata: Program that solves for a Non-Deterministic Finite Automaton the same problem that was solved for a Deterministic Finite Automaton in the above problem.
    - Word Generator: Given a word order statistic and a file containing text, store the corpus of words in a Map. Prompt the user to enter the number of random words to generate, and randomly generate words from the corpus.
  
  * Program 2: Implement Queue, Priority Queue and Set (and Iterators) with Linked Lists
  	 - Queue: Implement with a linear linked list and two instance variables that refer to the front and back of the list. Track the size of the list with a variable so traversal is not required.
    - Priority Queue: Supply the queue with a "greater than" function to determine the priority of an object relative to another and a header node that points to the front of the list. Track the size of the list with a variable so traversal is not required.
    - Set: Implement with a trailer linked list that contains a trailer node that points to the back of the list. Track the size of the list with a variable so traversal is not required.
    
 * Program 3: Implement Priority Queue and Map (and Iterators) with Binary Trees
 		 - Priority Queue: Implement by storing values in a Max Heap, which makes enqueue and dequeue values take O(Log N) time. Supply the queue with a "greater than" function to determine the priority of an object relative to another.
    - Map: Implement by storing values in a Binary Search Tree where only the keys are compared. Supply the Map with a "less than" function to determine whether a pair containing key and value should be stored in the left or right subtree.
    
 *  Program 4: Implement Maps and Sets (and Iterators) with Hash Tables
    - Map: Implement using open addressing (by linear probing) and processing parallel arrays. The first array stores key value pairs while the second contains the status of each index: empty, occupied, was occupied. Supply the map with a hashing function that hashes only the key of the pair.
    - Set: Implement similarly to a Map but "values" in a Set are just values rather than key/values pairs.
    
 * Program 5: Implement Graphs with Maps and Sets and apply to Djikstra's Algorithm
    - Graph: Implement the Graph using a HashMap and HashSet (see Program 4). Each Graph is represented with twp Maps that can store Sets useful to the Djikstra's algorithm problem. One Map uses nodes as keys while the other uses edges as keys.
    - Djikstra's Shortest Path Algorithm: Reads a graph representing flight costs (edges) between cities (nodes). Given a start node from the user, a map is computed to give the minimum costs to all the reachable nodes from the start node. Given a stop node from the user, compute the minimum cost to reach that end node and display the nodes along the path from start->node.
