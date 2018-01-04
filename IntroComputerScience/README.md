## [Intro to Computer Science](https://github.com/HungryAdi/Coursework/tree/master/IntroComputerScience)
### Eclipse
All programs written in Java

* Lab 0: Introduction to Eclipse
  - Tour of the Eclipse IDE and hwo to compile and run a program.

* Project 1: Simple Voting Ballot
  - Candidate Class: Each object represents one candidate on a ballot. Candidate object contains a name, a party affiliation, and the number of votes received by that candidate so far.
  - Ballot Class: Each object represents a single ballot, and contains an ArrayList of Candidates, and the name of the office that is up for a vote.
  - BallotReader Class: Contains a static method that reads a ballot from an input file.
  - ResultWriter Class: Contains a static method that writes the election results into an output file.
 
* Project 2: Mail Forwarding 
  - A list of forwarding entries (operations to add, remove or mail addresses) are stored in a singly linked list with a head reference
  - An input string of a series of commands is read from stdin and processed in order, program execution updates the linked list to ensure mail is sent to the proper address.
  
* Project 3: Facile Interpreter
  - Facile -> stripped down version of BASIC (fewer commands)
  - Interpreter reads Facile commands, parses the input to execute the commands with Java equivalent operations.
  - Facile commands range from arithmetic operations to creating subroutines (methods in Java)
  
* Project 4: Ticket Queues
  - Simulate 2 models: multiple ticket windows and lines or multiple windows with a single line with Queues.
  - Input data concerning time spent per customer per window, customer flow and # of lines/windows is read in from input file.
  - Program executes simulation by performing operations on Queues and computes the time taken for both models to determine the optimal ticket model.
  
* Project 5: Random Grammer and JUnit Testing
  - Generate large amounts of data specified by a grammar specified by objects stored in memory.
  - JUnit to test the generation of a Grammar, creation of a Mock Random series of numbers (pseudorandom), and then generation of random sentences described by grammar rules.
  
### Racket
Functions written in Racket to explore functional programming and recursion

* Fourth Element
  - Receives a list of numbers and returns the 4th element in the list, or an empty list if 4 or more elements are not present.
  
* Nth Element
  - Similar to Fourth Element but function returns the specified nth element of a list..
  
* List Length
  - Takes a list and returns the number of elements in the list
  
* Count Matches
  - Takes a list and symbol S and returns the number of occurrences of S in the list.
  
* My Append
  - Concatenates two lists, maintaining the order of elements in each list.
  
* Is Increasing?
  - Returns "True" if elements in a list are increasing in value and "False" if not.
  
* Remove Duplicates
  - Takes a list and returns a new list with all duplicate values in the input list removed.
  
* Calculate Running Sums
  - Takes a list and returns a new list where the nth element in the output list is the sum of the first n elements in the input list.
  
* Recursive Sums
  - Takes a list of numbers (and/or sublists of numbers) and returns the sum of all atoms in the list.
  
* Filter Items
  - Takes a function F (returns a boolean) and list L as input and returns a list where all elements are elements in L that returned "True" when passed to function F.
  
* Bonus Queue
  - Implement a Queue as a list in Racket with constructor, and enqueue/dequeue operations
  - Improve performance by using two lists, one of the first i elements starting at the front and moving forward, and one of the last j elements starting with the back of the queue and moving backward.
  - Reduces the need for list traversal which is expensive in a functional language.
  
* Find Max
  - Takes the list and returns the maximum value.
