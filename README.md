Danny Hido - CS 33211 Operating Systems - Fall 2025
# OS Assignment One

This project implements a producer-consumer problem in C++ using shared memory and semaphores.
On a buffer of size 2, the producer produces items. The consumer checks the buffer, and consumes available items. 
The producer produces a maximum of ten items. As this happens, action, item number, and buffer are outputted to
the console. 

## Files

consumer.cpp - conatins the consumer. Consumes items from shared buffer
producer.cpp - contains the produces. Produces items to the shared buffer
header.hpp - contains shared include statements and the shared buffer
example.txt - contains example output

## How to run

Must be run in a linux environment with g++ compiler

compile producer.cpp and consumer.cpp:

   g++ producer.cpp -o producer -lpthread
   g++ consumer.cpp -o consumer -lpthread

run in terminal:

  ./producer & ./consumer
