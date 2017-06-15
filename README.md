# UCLA CS 111

My solutions for [UCLA CS 111 - Operating Systems Principles](http://web.cs.ucla.edu/classes/spring17/cs111/) taught by [Mark Kampe](https://www.linkedin.com/in/markkampe) in Spring 2017.

### Lab 0: [Warm-Up](http://web.cs.ucla.edu/classes/spring17/cs111/projects/P0.html)
Trivial C program that copies content from a source to an output (defaults to copying stdin to stdout). Involved working with file descriptors and the getopt api for argument parsing. Also included an additional feature of catching and handling signals and segmentation faults.  


### Lab 1: [Terminal I/O and IPC](http://web.cs.ucla.edu/classes/spring17/cs111/projects/P1A.html) / [IPC and Encryption](http://web.cs.ucla.edu/classes/spring17/cs111/projects/P1B.html)

Client and server programs that communicate with each other through pipes (part A) and through a TCP socket (part B). The server program runs a shell, receives and processes shell commands from the client, and sends shell output back to the client. The client receives shell commands from the user, sends shell commands to the server, and displays shell output to the user. Involved working with the process api, pipes, and sockets.


### Lab 2: [Races and Synchronization](http://web.cs.ucla.edu/classes/spring17/cs111/projects/P2A.html) / [Lock Granularity and Performance](http://web.cs.ucla.edu/classes/spring17/cs111/projects/P2B.html)

Multithreaded C programs meant to depict the problems of race conditions and synchronization and how they can be solved with locks. Part A involved implementing a multithreaded counter and doubly-linked list, and locking critical sections with a mutex or spin lock. Part B illustrated various methods on optimizing lock performance by breaking tasks up into smaller tasks. Both parts involved measuring performance by generating result data and visualizing it with gnuplot.


### Lab 3: [File System Interpretation](http://web.cs.ucla.edu/classes/spring17/cs111/projects/P3A.html) / [File System Consistency Analysis](http://web.cs.ucla.edu/classes/spring17/cs111/projects/P3B.html)

C programs used to analyze file system structure and diagnose possible corruption. Part A required us to parse through a file system image and output a CSV summary of its contents. Part B was essentially a fsck (file system checker) and required us to parse through a CSV summary like the one we produced in part A and check for any inconsistencies or corruption. My partner for this lab was Jiwan Kang, and we chose to write part B in Python for parsing CSV's.


### Lab 4: [Bringup](http://web.cs.ucla.edu/classes/spring17/cs111/projects/P4A.html) / [Sensors and Communication](http://web.cs.ucla.edu/classes/spring17/cs111/projects/P4B.html) / [IOT Security](http://web.cs.ucla.edu/classes/spring17/cs111/projects/P4C.html)

C programs for the Intel Edison that measures temperature through a temperature sensor and sending temperature reports to a server using both insecure and secure communication methods.


Project | Score | Project | Score
---- | ---- | ---- | ----
Lab 0 | 99 / 100 |  Lab 3A |
Lab 1A | 96 / 100 | Lab 3B |
Lab 1B | 89 / 100 | Lab 4A | 100 / 100
Lab 2A | 90 / 100 | Lab 4B | 95 / 100
Lab 2B | 94 / 100 | Lab 4C |
