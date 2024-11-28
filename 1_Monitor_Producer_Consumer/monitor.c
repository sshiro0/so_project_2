#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define LOG_FILE "execution.log"

typedef struct Monitor{
    int *items;
    int size;
    int count;
    int in;
    int out;
    int time;;
    pthread_cond_t produce;
    pthread_cond_t consume;
    pthread_mutex_t mutex;
} Monitor;

typedef struct ThreadArgs {
    int id;        
    Monitor *monitor; 
} ThreadArgs;


long p = 0;
long c = 0;
long s = 0;
long t = 0;
long p_done = 0;
FILE *log_file;
char log_msg[100];

void logEvent(FILE *log_file, const char *event) {
    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strlen(timestamp) - 1] = '\0';

    fprintf(log_file, "[%s] %s\n", timestamp, event);
}

void monitorDestroy(Monitor *m) {
    free(m->items);
    pthread_cond_destroy(&m->produce);
    pthread_cond_destroy(&m->consume);
    pthread_mutex_destroy(&m->mutex);
    logEvent(log_file, "Monitor destruido.");
}

void duplicateSize(Monitor *monitor) {
    if (monitor->count == monitor->size) {
        free(monitor->items);
        monitor->size *= 2;
        monitor->items = (int *)malloc(monitor->size * sizeof(int));

        for (int i = 0; i < monitor->count; i++) {
            monitor->items[i] = 1;
        }
        for (int i = monitor->count; i < monitor->size; i++) {
            monitor->items[i] = 0;
        }

        monitor->out = 0;
        monitor->in = monitor->count;
    
        snprintf(log_msg, sizeof(log_msg), "Cambio de tamaño de la cola a %d.", monitor->size);
        logEvent(log_file, log_msg);
    }
}

void reduceSize(Monitor *monitor) {
    if (monitor->count <= monitor->size / 4 && monitor->size > 1) {
        free(monitor->items);
        monitor->size /= 2;
        monitor->items = (int *)malloc(monitor->size * sizeof(int));

        for (int i = 0; i < monitor->count; i++) {
            monitor->items[i] = 1;
        }
        for (int i = monitor->count; i < monitor->size; i++) {
            monitor->items[i] = 0;
        }

        monitor->out = 0;
        monitor->in = (monitor->size == 1) ? 0 : monitor->count;

        snprintf(log_msg, sizeof(log_msg), "Cambio de tamaño de la cola a %d.", monitor->size);
        logEvent(log_file, log_msg);
    }
}

void consume(Monitor *monitor, int id) {
    while (1) {
        pthread_mutex_lock(&monitor->mutex);

        snprintf(log_msg, sizeof(log_msg), "Consumidor %d inicializado.", id);
        logEvent(log_file, log_msg);

        while (monitor->count == 0 && p_done < p) {
            snprintf(log_msg, sizeof(log_msg), "Consumidor %d se va a dormir.", id);
            logEvent(log_file, log_msg);
            pthread_cond_wait(&monitor->consume, &monitor->mutex);
            snprintf(log_msg, sizeof(log_msg), "Consumidor %d despierta.", id);
            logEvent(log_file, log_msg);
        }

        if (p_done == p && monitor->count == 0) {
            pthread_mutex_unlock(&monitor->mutex);
            snprintf(log_msg, sizeof(log_msg), "Consumidor %d termina.", id);
            logEvent(log_file, log_msg);
            break;
        }

        monitor->items[monitor->out] = 0;
        monitor->out = (monitor->out + 1) % monitor->size;
        monitor->count--;

        snprintf(log_msg, sizeof(log_msg), "Consumidor %d consumió un ítem.", id);
        logEvent(log_file, log_msg);

        reduceSize(monitor);

        pthread_cond_signal(&monitor->produce);
        pthread_mutex_unlock(&monitor->mutex);

        sleep(monitor->time);
    }
}

void produce(Monitor *monitor, int id) {
    pthread_mutex_lock(&monitor->mutex);
    snprintf(log_msg, sizeof(log_msg), "Productor %d inicializado.", id);
    logEvent(log_file, log_msg);

    while (monitor->count == monitor->size) {
        pthread_cond_wait(&monitor->produce, &monitor->mutex);
        snprintf(log_msg, sizeof(log_msg), "Productor %d despierta!", id);
        logEvent(log_file, log_msg);
    }

    monitor->items[monitor->in] = 1;
    monitor->in = (monitor->in + 1) % monitor->size;
    monitor->count++;

    duplicateSize(monitor);

    snprintf(log_msg, sizeof(log_msg),  "Productor %d terminó.", id);
    logEvent(log_file, log_msg);
    p_done++;

    if (p_done == p) {
        pthread_cond_broadcast(&monitor->consume);
    } else {
        pthread_cond_signal(&monitor->consume);
    }

    pthread_mutex_unlock(&monitor->mutex);
}

void *producerJob(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    produce(args->monitor, args->id);
    free(arg);
    return NULL;
}

void *consumerJob(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    consume(args->monitor, args->id);
    free(arg);
    return NULL;
}

int main(int argc, char **argv) {
    log_file = fopen(LOG_FILE, "a");

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            p = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-c") == 0) {
            c = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-s") == 0) {
            s = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-t") == 0) {
            t = atoi(argv[i + 1]);
        }
    }

    Monitor monitor = {
        .items = malloc(s * sizeof(int)),
        .size = s,
        .count = 0,
        .in = 0,
        .out = 0,
        .time = t,
        .produce = PTHREAD_COND_INITIALIZER,
        .consume = PTHREAD_COND_INITIALIZER,
        .mutex = PTHREAD_MUTEX_INITIALIZER,
    };

    pthread_t producers_threads[p];
    pthread_t consumers_threads[c];

    for (int i = 0; i < p; i++) {
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->id = i + 1;
        args->monitor = &monitor;
        pthread_create(&producers_threads[i], NULL, producerJob, args);
    }
    for (int i = 0; i < c; i++) {
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->id = i + 1;
        args->monitor = &monitor;
        pthread_create(&consumers_threads[i], NULL, consumerJob, args);
    }


    for (int i = 0; i < p; i++) {
        pthread_join(producers_threads[i], NULL);
    }
    for (int i = 0; i < c; i++) {
        pthread_join(consumers_threads[i], NULL);
    }

    monitorDestroy(&monitor);
    logEvent(log_file, "Ejecución terminada.\n");
    fclose(log_file);
    return 0;
}