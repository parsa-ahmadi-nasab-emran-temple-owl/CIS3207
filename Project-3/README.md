# CIS3207
Project 3: Networked Spell Checker

I will be doing the server.c and the client.c along with the pseudo code for this project since I have waited til the weekend before its due.

I have finished the following things for the server program:
  
  created a thread that would monitor a log queue and process entries by removing and writing them to the log file
  
  created a thread that would accept and distribute the connection requests
  
  created a thread that would check to see if the word is in the array list of words and it will return result that either the      word is spelled correctly or is spelled incorrectly
  
  set up a socket for this server program
  
  defined a constant DEFAULT_DICTIONARY for it
  
  defined a constant DEFAULT_PORT for it

I have finished the following things for this client program:
  
  set up a socket for this client program
  
  defined a constant DEFAULT_PORT for it

Description of the project:

the high level of network sockets is that they are communication channels between pairs of processes

they differ from pipes in that a pair of processes communicating via a socket may reside on different machines, and that the channel is bi-directional (functioning in two directions)

"socket programming" involves the mechanics of setting up the channel (i.e., the socket) for communication along with a socket descriptor

the spell checker is to be a process that will acquire (read) sequences of words
  if the word is in its dictionary, then it's considered to be spelled correctly
  if the word is not in its dictionary, then it's not considered to be spelled correctly
  
the server will have two functions:
1. accept and distribute connection requests from clients
2. construct a log file of all spell check activities

When a server starts:
1. the main thread opens the dictionary file and reads it into some data structure accessible by all the threads in the program
2. it also creates a fixed-sized data structure that will be used to store the socket descriptors of the clients that will connect to it and will be accessible to all server threads
3. it also creates a pool of NUM_WORKERS worker threads
4. a second server thread will monitor a log queue and process entries by removing and writing them to a log file

Worker Thread:
needs to be an instance of the Producer-Consumer problem

the work queue is a shared data structure, with the main thread acting as a producer, adding socket descriptors to the queue, and the worker threads acting as consumers, removing socket descriptors from the queue

the log queue is a shared data structure, with the worker threads acting as producers if adding the results into the buffer and a server log thread acting as a consumer, removing the results from the buffer

Because we have concurrent access to these shared data structures, we must synchronize access to them so that:
1. each client is serviced
2. the queues do not become corrupted
  the use of the mutual exclusion will help prevent deadlocks
  
the solution should include attempts at synchronizaton using locks and condition variables and they will be used for the following conditions:
1. a producer should not attempt to produce if the queue is full, nor should consumers attempt to consume when the queue is empty
2. when a producer attempts tp add to the queue and finds it full, it should cease to execute until a producer has notified it that a new item has been added to the queue

Setting Up A Socket For Communication:
the steps involved in establishing a socket on the client side are as follows:
1. create a socket with the socket() system call
2. construct a socket to the address of the server by using the connect() system call
3. send and receive data using read() and write() system calls

the steps involved in establishing a socket on the server side are are follows:
1. create a socket with the socket() system call
2. bind the socket to an address using the bind() system call
3. listen for connections with the listen() system call
4. accept a communication with the accept() system call, this call typically blocks until a client connects to the server
5. send and receive data using read() and write() system calls
