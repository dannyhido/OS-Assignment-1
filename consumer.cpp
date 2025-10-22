// Danny Hido
// Operating Systems Assignment 1
// 10/22/25
// consumer.cpp

#include "header.hpp"
static void perror_exit(const char* msg);

int main() {

    // let producer start first
    sleep(2);

    //open shared memory
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) perror_exit("shm_open consumer");

    void* ptr = mmap(nullptr, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) perror_exit("mmap consumer");

    SharedBuffer* buffer = static_cast<SharedBuffer*>(ptr);

    // semaphores
    sem_t* empty = sem_open(SEM_EMPTY, 0);
    sem_t* full  = sem_open(SEM_FULL, 0);
    sem_t* mutex = sem_open(SEM_MUTEX, 0);
    if (empty == SEM_FAILED || full == SEM_FAILED || mutex == SEM_FAILED) perror_exit("sem_open consumer");

    // consumption loop
    while (true) {
        sem_wait(full);
        sem_wait(mutex);

        int slot = buffer->out;           
        int value = buffer->items[slot];
        buffer->out = (buffer->out + 1) % BUFFER_SIZE;

        sem_post(mutex);
        sem_post(empty);

        // stopping signal
        if (value == -1) {  
            break;
        }

        std::cout << "Consumed item " << value << " at slot " << slot << std::endl;

        usleep(500000);
    }
    std::cout << "Consumer is finished." << std::endl;

    // clean up
    munmap(ptr, sizeof(SharedBuffer));
    close(shm_fd);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_MUTEX);

    return 0;
}
