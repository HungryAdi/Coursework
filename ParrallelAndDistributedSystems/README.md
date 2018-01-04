## Parallel and Distributed Systems
Program written in C and uses MPI to distribute work across multiple computers.

* Dining Philosopher's Problem
  - Each Philospher is implemented by a thread that attempts to access two mutexes (forks).
  - A struct containing the two adjacent forks and philosopher id are passed to each thread who then attempt to eat only if both forks are available, and think if otherwise.
  - MPI is used to distribute threads across a cluster of computers and the difference in execution time compared to a single computer is measured.
