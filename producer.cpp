// Danny Hido
// Operating Systems Assignment 1
// 10/22/25
// producer.cpp


#include "header.hpp"
static void perror_exit(const char* msg);

int main() {

    // clean up old memory
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_MUTEX);
    shm_unlink(SHM_NAME);

    // create shared memory
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) perror_exit("shm_open");

    if (ftruncate(shm_fd, sizeof(SharedBuffer)) == -1) perror_exit("ftruncate");

    void* ptr = mmap(nullptr, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) perror_exit("mmap");

    SharedBuffer* buffer = static_cast<SharedBuffer*>(ptr);
    memset(buffer, 0, sizeof(SharedBuffer));
    buffer->in = 0;
    buffer->out = 0;
    buffer->done = false;

    // semaphores
    sem_t* empty = sem_open(SEM_EMPTY, O_CREAT | O_EXCL, 0666, BUFFER_SIZE);
    sem_t* full = sem_open(SEM_FULL, O_CREAT | O_EXCL, 0666, 0);
    sem_t* mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, 0666, 1);

    if (mutex == SEM_FAILED) perror_exit("sem_open mutex");
    if (empty == SEM_FAILED) perror_exit("sem_open empty");
    if (full == SEM_FAILED) perror_exit("sem_open full");

    // item production loop
    int item = 0;
    const int MAX_ITEMS = 10; // num of items 
    for (int i = 0; i < MAX_ITEMS; ++i) {
        sem_wait(empty);
        sem_wait(mutex);

        buffer->items[buffer->in] = ++item;
        std::cout << "Produced item " << item << " at slot " << buffer->in << std::endl;
        buffer->in = (buffer->in + 1) % BUFFER_SIZE;

        sem_post(mutex);
        sem_post(full);

        sleep(1);
    }

    //send stopping signal to consumer
    sem_wait(empty);
    sem_wait(mutex);
    buffer->items[buffer->in] = -1;  
    buffer->in = (buffer->in + 1) % BUFFER_SIZE;
    sem_post(mutex);
    sem_post(full);

    // clean up
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    close(shm_fd);
    std::cout << "Producer is finished." << std::endl;

    return 0;
}
