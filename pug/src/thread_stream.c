#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#if WASM
#include <emscripten.h>
#endif
#include <unistd.h>

#define BUFFER_SIZE 256

typedef struct {
    char buffer[BUFFER_SIZE];
    int head;
    int tail;
    pthread_mutex_t mutex;
    pthread_cond_t data_available;
} ThreadSafeStream;

ThreadSafeStream* ThreadSafeStream_create() {
    ThreadSafeStream* stream = (ThreadSafeStream*)malloc(sizeof(ThreadSafeStream));
    stream->head = stream->tail = 0;
    pthread_mutex_init(&stream->mutex, NULL);
    pthread_cond_init(&stream->data_available, NULL);
    return stream;
}

void ThreadSafeStream_write(ThreadSafeStream* stream, char c) {
    pthread_mutex_lock(&stream->mutex);
    stream->buffer[stream->head] = c;
    stream->head = (stream->head + 1) % BUFFER_SIZE;
    pthread_cond_signal(&stream->data_available);
    pthread_mutex_unlock(&stream->mutex);
}

int ThreadSafeStream_read(ThreadSafeStream* stream) {
    pthread_mutex_lock(&stream->mutex);
    while (stream->head == stream->tail) {
        pthread_cond_wait(&stream->data_available, &stream->mutex);
    }
    char c = stream->buffer[stream->tail];
    stream->tail = (stream->tail + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&stream->mutex);
    return c;
}

ThreadSafeStream* input_char_stream;
ThreadSafeStream* output_char_stream;
ThreadSafeStream* writer_queue;

void* persistent_writer_thread(void* arg) {
    while (1) {
        char c = ThreadSafeStream_read(writer_queue);
        ThreadSafeStream_write(input_char_stream, c);
    }
    return NULL;
}

void* persistent_reader_thread(void* arg) {
    while (1) {
        int c = ThreadSafeStream_read(output_char_stream);
        // Simulate sending back to JS
        #if WASM
        EM_ASM({
            console.log("Output from WASM:", String.fromCharCode($0));
        }, c);
        #else
        printf("%c", c);
        #endif
    }
    return NULL;
}

#if WASM
EMSCRIPTEN_KEEPALIVE
#endif
void writeInputStream(char* message) {
    for (int i = 0; message[i] != '\0'; ++i) {
        ThreadSafeStream_write(writer_queue, message[i]);
    }
    ThreadSafeStream_write(output_char_stream, '\n'); // Add newline to indicate end of input
    
}

#if WASM
EMSCRIPTEN_KEEPALIVE
#endif
int readInputStream() {
    return ThreadSafeStream_read(input_char_stream);
}

void writeOutputStream(void* data, const char* type) {
    char buffer[64];

    if (strcmp(type, "char") == 0) {
        snprintf(buffer, sizeof(buffer), "%c", *((char*)data));
    } else if (strcmp(type, "int") == 0) {
        snprintf(buffer, sizeof(buffer), "%d", *((int*)data));
    } else if (strcmp(type, "str") == 0) {
        snprintf(buffer, sizeof(buffer), "%s", (char*)data);
    } else {
        snprintf(buffer, sizeof(buffer), "[Unsupported type]");
    }

    for (int i = 0; buffer[i] != '\0'; ++i) {
        ThreadSafeStream_write(output_char_stream, buffer[i]);
    }
    fflush(stdout); // Ensure the output is flushed immediately
    
}

#if WASM
EMSCRIPTEN_KEEPALIVE
#endif

void initializer() {
    input_char_stream = ThreadSafeStream_create();
    output_char_stream = ThreadSafeStream_create();
    writer_queue = ThreadSafeStream_create();

    pthread_t writer, reader;
    pthread_create(&writer, NULL, persistent_writer_thread, NULL);
    pthread_create(&reader, NULL, persistent_reader_thread, NULL);
}
