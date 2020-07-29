#pragma once

#include <pthread.h>
#include <sys/epoll.h> /* for epoll_create1(), epoll_ctl(), struct epoll_event */
#include <stdbool.h>

#include "types.h"

#define LISTENER_THREAD_CORE_ID          0 /* Dedicated Listener Core */
#define LISTENER_THREAD_MAX_EPOLL_EVENTS 1024

#define RUNTIME_LOG_FILE                  "awesome.log"
#define RUNTIME_MAX_SANDBOX_REQUEST_COUNT (1 << 19) /* random! */
#define RUNTIME_READ_WRITE_VECTOR_LENGTH  16

extern int       runtime_epoll_file_descriptor;
extern float     runtime_processor_speed_MHz;
extern uint32_t  runtime_total_worker_processors;
extern pthread_t runtime_worker_threads[];

void         alloc_linear_memory(void);
void         expand_memory(void);
INLINE char *get_function_from_table(uint32_t idx, uint32_t type_id);
INLINE char *get_memory_ptr_for_runtime(uint32_t offset, uint32_t bounds_check);
void         runtime_initialize(void);
void         listener_thread_initialize(void);
void         stub_init(int32_t offset);

unsigned long long __getcycles(void);

/**
 * Used to determine if running in the context of a worker thread
 * @returns true if worker. false if listener core
 */
static inline bool
runtime_is_worker()
{
	pthread_t self = pthread_self();
	for (int i = 0; i < runtime_total_worker_processors; i++) {
		if (runtime_worker_threads[i] == self) return true;
	}

	return false;
}
