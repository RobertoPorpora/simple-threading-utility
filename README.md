# STHR: Simple Threading Utility

The STHR library provides simple and cross-platform management of threads and mutexes in C.  
It provides a straightforward API for creating, destroying, and synchronizing threads and mutexes across different operating systems.

## Features

- Cross-platform thread management
- Basic mutex operations

## Usage example

```c
// ------------------------------------------------------
// Standard library dependencies

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ------------------------------------------------------
// Non-standard libraries

#include "STHR.h"

// ------------------------------------------------------
// Shared data

#define BUFFER_SIZE 10

// Shared buffer and its index
int buffer[BUFFER_SIZE];
int buffer_index = 0;
STHR_mutex_t buffer_mutex;

// ------------------------------------------------------
// Producer thread function (never ends)

void producer(void) {
    while (true) {
        STHR_sleep_ms(1000); // Simulate time taken to produce an item
        STHR_mutex_lock(&buffer_mutex);

        // Produce an item
        if (buffer_index < BUFFER_SIZE) {
            buffer[buffer_index++] = i;
            printf("Produced: %d\n", i);
        } else {
            printf("Buffer full, producer waiting...\n");
        }

        STHR_mutex_unlock(&buffer_mutex);
    }
}

// ------------------------------------------------------
// Consumer thread function (ends after some time)

void consumer(void) {
    for (int i = 0; i < 20; ++i) {
        STHR_mutex_lock(&buffer_mutex);

        // Consume an item
        if (buffer_index > 0) {
            int item = buffer[--buffer_index];
            printf("Consumed: %d\n", item);
        } else {
            printf("Buffer empty, consumer waiting...\n");
        }

        STHR_mutex_unlock(&buffer_mutex);
        STHR_sleep_ms(2000); // Simulate time taken to consume an item
    }
}

// ------------------------------------------------------
// Main

int main() {
    STHR_thread_t producer_thread;
    STHR_thread_t consumer_thread;

    // Initialize the mutex
    if (STHR_mutex_create(&buffer_mutex) != STHR_OK) {
        fprintf(stderr, "Failed to create mutex\n");
        return 1;
    }

    // Create the producer and consumer threads
    if (STHR_thread_create(&producer_thread, producer) != STHR_OK) {
        fprintf(stderr, "Failed to create producer thread\n");
        return 1;
    }
    if (STHR_thread_create(&consumer_thread, consumer) != STHR_OK) {
        fprintf(stderr, "Failed to create consumer thread\n");
        return 1;
    }

    // Wait for the consumer thread to finish
    if (STHR_thread_wait_end(&consumer_thread) != STHR_OK) {
        fprintf(stderr, "Failed to wait for consumer thread\n");
        return 1;
    }

    // Destroy the producer thread
    if (STHR_thread_destroy(&producer_thread) != STHR_OK) {
        fprintf(stderr, "Failed to destroy producer thread\n");
        return 1;
    }

    // Destroy the mutex
    if (STHR_mutex_destroy(&buffer_mutex) != STHR_OK) {
        fprintf(stderr, "Failed to destroy mutex\n");
        return 1;
    }

    return 0;
}
```

## Compiling

### Method 1

Use CMake.  
Copy the whole folder inside your project folder and add these to your CMakeLists.txt

```cmake
add_subdirectory(path/to/simple_threading_utility)
target_link_libraries(your_executable_name STHR)
```

You have, of course to customize "path/to/" and "your_executable_name" to your needs.

### Method 2

Manually copy files STHR.c and STHR.h inside your project folder sources.
(And configure your compiler to compile and link these).

## Testing

> If you know what you are doing, you can ignore Python and just compile the `test.c` script with cmake or anything you like.

First, you need gcc, make, cmake and python installed in your system, check it with these 4 commands:
```
gcc --version
make --version
cmake --version
python --version
```

Then, in order to launch the tests open a terminal with cwd as the folder of this README.md file.  

Then, run `python test/run.py`.

This will launch cmake, make, the compiler and then it will execute the executable output.

After a few seconds you should see something like this in the last lines of your terminal:
```
Run: "path/to/simple_threading_utility/test/bin/test(.exe)"
-----START-----
Complete test:
    Mutex create:
    PASS
    Thread 1 create:
    PASS
    Thread 2 create:
    PASS
    Thread 1 wait end:
        Thread 1 mutex lock:
        PASS
        Thread 1 mutex unlock:
        PASS
        Thread 2 mutex lock:
        PASS
        Thread 2 mutex unlock:
        PASS
        Thread 1 mutex lock:
        PASS
        Thread 2 mutex lock:
            Thread 1 mutex unlock:
            PASS
        PASS
        Thread 2 mutex unlock:
        PASS
    PASS
    Main mutex lock:
    PASS
    Thread 2 destroy:
    PASS
    Main mutex unlock:
    PASS
    Main mutex destroy:
    PASS
    Final array check:
    PASS
PASS
process finished with return code 0.
-----END-----
```

