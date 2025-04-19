#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <emscripten.h>

// Example using a simple buffer and mutexes (Illustrative)
#define BUFFER_SIZE 256
EMSCRIPTEN_KEEPALIVE

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
ThreadSafeStream* g_pshared_char_stream;
//ThreadSafeStream* o_pshread_char_stream;

void ThreadSafeStream_write(ThreadSafeStream* stream, char c) {
    pthread_mutex_lock(&stream->mutex);
    // Basic example - no overflow handling
    stream->buffer[stream->head] = c;
    stream->head = (stream->head + 1) % BUFFER_SIZE;
    pthread_cond_signal(&stream->data_available); // Signal that data is available
    pthread_mutex_unlock(&stream->mutex);
}// Cast void* to expected struct type

int ThreadSafeStream_read(ThreadSafeStream* stream) {
    pthread_mutex_lock(&stream->mutex);
    while (stream->head == stream->tail) {
        pthread_cond_wait(&stream->data_available, &stream->mutex); // Wait for data
    }
    char c = stream->buffer[stream->tail];
    stream->tail = (stream->tail + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&stream->mutex);
    return c;
}

void* writer_thread(void* arg, char* message) {
    ThreadSafeStream* stream = (ThreadSafeStream*)arg;
    //const char* message = "Hello, Threaded World!";
    for (int i = 0; message[i] != '\0'; ++i) {
        ThreadSafeStream_write(stream, message[i]);
        //usleep(100000); // Simulate some delay
    }
    return NULL;
}

int reader_thread(void* arg) {
    ThreadSafeStream* stream = (ThreadSafeStream*)arg;
    // for (int i = 0; i < 22; ++i) { //read 22 characters
    //     int c = ThreadSafeStream_read(stream);
    //     printf("Read: %c\n", (char)c);
    // }
    int c = ThreadSafeStream_read(stream);
    // In thread_stream.c, add debug prints:
    printf("Read: %c\n", (char)c);
    if(c == -62){
        
        //int d = ThreadSafeStream_read(stream);
        return 0xC2;
    }
    if(c == -73){
        
        //int d = ThreadSafeStream_read(stream);
        return 0xB7;
    }
    return c;
}
typedef struct {
    void* stream;
    char* message;
} writer_thread_args;

//wtw is the writer thread wrapper function that is called by the main thread
void* wtw(void* arg){
    writer_thread_args* args = (writer_thread_args*)arg; 
    writer_thread(args->stream, args->message);
}

void initializer(){
    printf("creating reader and writer threads\n");
    g_pshared_char_stream = ThreadSafeStream_create();
    //o_pshread_char_stream = ThreadSafeStream_create();
    pthread_t writer, reader;
    writer_thread_args* args = (writer_thread_args*)malloc(sizeof(writer_thread_args));
    args->stream = g_pshared_char_stream;
    args->message = "";
    pthread_create(&writer, NULL, wtw, args);
    pthread_create(&reader, NULL, reader_thread, g_pshared_char_stream);

    //removed pthread_join(writer, NULL);
    //removed pthread_join(reader, NULL);
}
// TODO:
// 1. Create  a method called writeInputStream() which writes to the input stream
// 2. this method above is called as follows: Module.ccall('writeInputStream()) when there is data to write.
EMSCRIPTEN_KEEPALIVE
void writeInputStream(char* message){
    //initializer();
    EM_ASM({
			console.log(UTF8ToString($0));
		},message);
    writer_thread(g_pshared_char_stream, message);
}

// reading from input stream
int readInputStream(){
    return reader_thread(g_pshared_char_stream);
}

// void writeOutputStream(void* data, const char* type) {
//     char buffer[64]; // Enough to hold any reasonable output

//     if (strcmp(type, "char") == 0) {
//         snprintf(buffer, sizeof(buffer), "%c", *((char*)data));
//     } else if (strcmp(type, "int") == 0) {
//         snprintf(buffer, sizeof(buffer), "%d", *((int*)data));
//     } else if (strcmp(type, "str") == 0) {
//         snprintf(buffer, sizeof(buffer), "%s", (char*)data);
//     } else {
//         snprintf(buffer, sizeof(buffer), "[Unsupported type]");
//     }

//     // Write each character to the stream
//     for (int i = 0; buffer[i] != '\0'; ++i) {
//         ThreadSafeStream_write(o_pshared_char_stream, buffer[i]);
//     }
// }

// void readOutputStream(){
//     int c = ThreadSafeStream_read(o_pshread_char_stream);
//     EM_ASM({
//             console.log(UTF8ToString($0));
//         },c);
// }