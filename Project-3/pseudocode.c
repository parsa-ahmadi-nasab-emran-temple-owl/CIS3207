// Project 3: Networked Spell Checker

// Testing:
    // I will be testing it using multiple servers and clients to make sure it's working well (to send and receive data) without any deadlocks and use debugging feature to help me debug my code.

// create a socket descriptor using dup2() function just like we did for the file descriptors

// Server Main Thread:

// create a DEFAULT_DICTIONARY which is a defined constant if the name of the dictionary file is not provided

// create a DEFAULT_PORT which is a defined constant that the program listens to if the port number is not provided

// open the dictionary file and reads it into an array list
    // will be accessible by all of the threads in the program

// create a fixed-sized using a queue to store the socket descriptors of the clients that connect to it
    // will be accessible by the server threads in the program

// create a log queue which will be a First-In First-Out queue
    //will be created and will be saved as queue.c

// create a log file to keep track of all spell check activities

// create a pool of NUM_WORKERS worker threads using pthreads and its functions in C

/* while (true) {
    connected_socket = accept(listening_socket);
    add connected_socket to the work queue
    signal any sleeping workers that there's a new socket in the queue
*\}

 // A second server thread will monitor a log queue and process entries by removing and writing them to a log file:
 /* while (true) {
        while (the log queue is not empty) {
            remove an entry from the log queue
            write the entry to the log file
        }
 *\}

 // Worker Thread:
 // A server worker thread's main loop is as follows:
 /* while (true) {
        while (the work queue is not empty) {
            remove a socket from the queue
            notify that there's an empty spot in the queue
            service client
        }
 *\}

 // and the client servicing logic is:
 /*  while (there's word left to read) {
        read word from the socket
        if (the word is in the dictionary) {
            echo the word back onto the socket concatenated with "OK"
    }
        else {
            echo the word back onto the ocket concatenated with "MISSPELLED"
    }
            write the word and the socket response value ("OK" or "MISSPELLED")
 }

 // set up a socket for communication:
 // the steps involved in establishing a socket on the client side are as follows:
    // 1. create a socket with the socket() system call
    // 2. construct a socket to the address of the server using the connect() system call
    // 3. send and receive data using the read() and write() system calls
    // will be created and will be saved as client.c
 
 // the steps involved in establishing a socket on the server side are as follows:
    // 1. create a socket with the socket() system call
    // 2. bind the socket to an address using the bind() system
    // 3. listen for connections with the listen() system call
    // 4. accept a connection with the accept() system call, this call typically blocks until a client connects to the server
    // 5. send and receive data using the read() and write() system calls
    // will be created and will be saved as server.c

 // checking to see if the word is in the dictionary or not:
    // if a word is in the dictionary, then it's considered to be spelled correctly
    // if a word is not in the dictionary, then it's not considered to be spelled correctly

// must synchronize the access to the work queue and the log queue using mutual exclusion, locks, and condition variables to help prevent deadlock
 
