## System Design
All Programs written in C

* C String Library Functions
  - parseC.c -> parses input to be used to test string library implementations.
  - strlen(): Recursive function that calculates the length of a string given a char* as input.
  - strncmp(): Compares the first n bytes of str1 and str2. Return value < 0 iff str1 < str2, > 0 if str1 > str2, and 0 if str == str2.
  - strcmp(): Given two char* str1 and str2, compare the two strings. Return values equivalent to those specified in strncmp above^^.
  - strncpy(): Copies up to n characters of a string pointed to by char* src to char* dest.
  - strcpy(): Copies the string pointed to by char* src to char* dest.
  - strncat(): Appends the string src to the end of dest up to n characters long.
  - strcat(): Appends the string src to the end of dest.
  - strchr(): Returns the first occurrence of a character in a given string.
  - strcspn(): Computes the length of the first segment of str1 which only contains characters not in str2.
  - strpbrk(): Finds the first character in str1 that matches any specified character in str2.
  - strspn(): Computes the length of the first segment of str1 which only contains characters in str2.
  - strrchr(): Returns the last occurence of a given character in a string.
  - strstr(): Finds the first occurrence of an entire string str1 that occurs in str2. Returns Null if str1 is not present.
  - strtok(): Breaks a string into a series of tokens given a character delimiter.
  
* Client : Server
  - client.c/server.c -> iterative client/server architecture that accepts connections, prints to stdout and closes the connection.
  - client_conc.c/server_conc.c -> concurrent client/server architecture that accepts connections from multiple clients simultaneously.
  
* Concurrent Execution
  - Thread Leader/Follower: Given a series of tasks, tasks are divided between the number of threads available and a waiting thread takes the next incompleted task. If there is an odd number of tasks compared to threads, some threads may end up performing more tasks than others.
  - Thread Pool: A thread accepts incoming requests and assigns the request to a queue of threads waiting to be assigned to available to be assigned to tasks and removed from the queue. Once a thread completes it's tasks, it is added back to the queue to wait for the arrival of another task. This architecture removes the need for the creation and destruction of multiple threads.
  - Thread Request: Similar to the Thread Pool. Requests are stored in a queue as well so requests can arrive concurrently alongside the work of threads in the thread pool.

* Shell
  - Implementation of a Unix Shell in C that accepts standard Unix commands.
  - Commands from stdin are parsed and executed by a call to execvpe() in a separate process (fork()) so that the shell may continue running.
  - Handles Signals such as '^C' to quit the shell.
  
* fork()
  - Program used to illustrate the memory required by each new process
  - created a "fork bomb" by calling fork() 10000 times to crash the CPU.
  
* ls
  - Implementation of the "ls" Unix command.
  - Handles calls to ls -l and ls -a and prints out file access options as well as timestamp of the file's creation.
