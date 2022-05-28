# Assignment5OS

# Assignment5OS
Server stack, this time using processes and not threads.

Name: David Ehevich

Name: Liel Zilberman


Assignment 5 in OS course: 

Implement a stack in a server which multiple clients can access and manipulate the stack , the stack is lock free using fcntl lock on the socket descriptor.

*NOTES:

The server is limited to handling 50 clients max.

Stack.cpp is implemntation of stack for test uses.

If stack is empty when using TOP command a "STACK IS EMPTY" will appear on the clients terminal.

If stack is not empty , a TOP: 'the string at the top' will appear on the clients terminal.

Running the program: 

Download all of the files in the repo , make sure all the files are in the same directory. Open terminal in this directory and type: "make all"
This will compile server.cpp , client.cpp and Test.cpp.

After compiling all the necessary file:

To run the server : ./server

To run the client : ./client "local host address"

To run the test : ./Test

After running the test, if all test have passed , a "Test has been done successfully" message will appear in the terminal.

Stack supported command , from clients terminal : 

PUSH "string" : will push the string to the top of the stack.

TOP : will display on the client's terminal the string last entered to the stack (LIFO).

POP: will pop the last entered string to the stack.

Example of the runnning program: 

![example](https://user-images.githubusercontent.com/54214707/164759511-ad2f25df-8c6e-4fe5-81a5-2a2bcc89fd42.PNG)



