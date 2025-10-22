// Danny Hido
// Operating Systems Assignment 1
// 10/22/25
// header.hpp


#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <sys/stat.h>

#define SHM_NAME "/prodcons_shm"
#define SEM_EMPTY "/sem_empty"
#define SEM_FULL  "/sem_full"
#define SEM_MUTEX "/sem_mutex"
#define BUFFER_SIZE 2

struct SharedBuffer {
    int items[BUFFER_SIZE];
    int in;
    int out;
    bool done;
};

static void perror_exit(const char* msg) {
    std::cerr << msg << ": " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
}

#endif