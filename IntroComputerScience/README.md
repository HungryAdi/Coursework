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
  
