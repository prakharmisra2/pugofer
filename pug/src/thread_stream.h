#ifndef THREAD_STREAM_H
#define THREAD_STREAM_H

#include <pthread.h>

#define BUFFER_SIZE 256

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char buffer[BUFFER_SIZE];
    int head;
    int tail;
    pthread_mutex_t mutex;
    pthread_cond_t data_available;
} ThreadSafeStream;

// Stream instances
extern ThreadSafeStream* g_pshared_char_stream;
extern ThreadSafeStream* o_pshared_char_stream;

// Thread-safe stream operations
ThreadSafeStream* ThreadSafeStream_create();
extern void ThreadSafeStream_write(ThreadSafeStream* stream, char c);
extern int ThreadSafeStream_read(ThreadSafeStream* stream);

// Reader/writer helpers
extern void writeInputStream(char* message);
extern int readInputStream();
extern void writeOutputStream(void* data, const char* type);
extern void readOutputStream(); // optionally: int readOutputStream();

extern void initializer(); // sets up streams

#ifdef __cplusplus
}
#endif

#endif // THREAD_STREAM_H
