#ifndef SFRT_TYPES_H
#define SFRT_TYPES_H

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <printf.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/uio.h>

#define EXPORT __attribute__((visibility("default")))
#define IMPORT __attribute__((visibility("default")))

#define INLINE __attribute__((always_inline))
#define WEAK __attribute__((weak))

#ifndef CACHELINE_SIZE
#define CACHELINE_SIZE 32
#endif

#ifndef PAGE_SIZE
#define PAGE_SIZE (1 << 12)
#endif

#define CACHE_ALIGNED __attribute__((aligned(CACHELINE_SIZE)))
#define PAGE_ALIGNED __attribute__((aligned(PAGE_SIZE)))

/* For this family of macros, do NOT pass zero as the pow2 */
#define round_to_pow2(x, pow2) (((unsigned long)(x)) & (~((pow2)-1)))
#define round_up_to_pow2(x, pow2) (round_to_pow2(((unsigned long)x) + (pow2)-1, (pow2)))

#define round_to_page(x) round_to_pow2(x, PAGE_SIZE)
#define round_up_to_page(x) round_up_to_pow2(x, PAGE_SIZE)

// Type alias's so I don't have to write uint32_t a million times
typedef signed char   i8;
typedef unsigned char u8;
typedef int16_t       i16;
typedef uint16_t      u16;
typedef int32_t       i32;
typedef uint32_t      u32;
typedef int64_t       i64;
typedef uint64_t      u64;

// FIXME: per-module configuration?
#define WASM_PAGE_SIZE (1024 * 64) // 64KB
#define WASM_START_PAGES (1 << 8)  // 16MB
#define WASM_MAX_PAGES (1 << 15)   // 4GB
#define WASM_STACK_SIZE (1 << 19)  // 512KB.
#define SBOX_MAX_MEM (1L << 32)    // 4GB

// These are per module symbols and I'd need to dlsym for each module. instead just use global constants, see above
// macros. The code generator compiles in the starting number of wasm pages, and the maximum number of pages If we try
// and allocate more than max_pages, we should fault
// extern u32 starting_pages;
// extern u32 max_pages;

// The code generator also compiles in stubs that populate the linear memory and function table
void populate_memory(void);
void populate_table(void);

// memory/* also provides the table access functions
#define INDIRECT_TABLE_SIZE (1 << 10)

struct indirect_table_entry {
	u32   type_id;
	void *func_pointer;
};

extern __thread struct indirect_table_entry *module_indirect_table;

// for sandbox linear memory isolation
extern __thread void *sandbox_lmbase;
extern __thread u32   sandbox_lmbound;
extern i32            runtime__log_file_descriptor; // TODO: LOG_TO_FILE logic is untested

// functions in the module to lookup and call per sandbox.
typedef i32 (*mod_main_fn_t)(i32 a, i32 b);
typedef void (*mod_glb_fn_t)(void);
typedef void (*mod_mem_fn_t)(void);
typedef void (*mod_tbl_fn_t)(void);
typedef void (*mod_libc_fn_t)(i32, i32);

typedef enum
{
	MOD_ARG_MODPATH = 0,
	MOD_ARG_MODPORT,
	MOD_ARG_MODNAME,
	MOD_ARG_MODNARGS,
	MOD_ARG_MAX,
} mod_argindex_t;

/**
 * debuglog is a macro that behaves based on the macros DEBUG and LOG_TO_FILE
 * If DEBUG is not set, debuglog does nothing
 * If DEBUG is set and LOG_TO_FILE is set, debuglog prints to the logfile defined in runtime__log_file_descriptor
 * If DEBUG is set adn LOG_TO_FILE is not set, debuglog prints to STDOUT
 **/
#ifdef DEBUG
#ifdef LOG_TO_FILE
#define debuglog(fmt, ...)                                                                                   \
	dprintf(runtime__log_file_descriptor, "(%d,%lu) %s: " fmt, sched_getcpu(), pthread_self(), __func__, \
	        ##__VA_ARGS__)
#else // !LOG_TO_FILE
#define debuglog(fmt, ...) printf("(%d,%lu) %s: " fmt, sched_getcpu(), pthread_self(), __func__, ##__VA_ARGS__)
#endif // LOG_TO_FILE
#else  // !DEBUG
#define debuglog(fmt, ...)
#endif // DEBUG

#define HTTP__MAX_HEADER_COUNT 16
#define HTTP__MAX_HEADER_LENGTH 32
#define HTTP__MAX_HEADER_VALUE_LENGTH 64
#define HTTP__RESPONSE_200_OK "HTTP/1.1 200 OK\r\n"
#define HTTP__RESPONSE_CONTENT_LENGTH "Content-length:             \r\n\r\n" // content body follows this
#define HTTP__RESPONSE_CONTENT_TYPE "Content-type:                                 \r\n"
#define HTTP__RESPONSE_CONTENT_TYPE_PLAIN "text/plain"

#define JSON__MAX_ELEMENT_COUNT 16  // Max number of elements defined in JSON
#define JSON__MAX_ELEMENT_SIZE 1024 // Max size of a single module in JSON

#define LISTENER_THREAD__CORE_ID 0 // Dedicated Listener Core
#define LISTENER_THREAD__MAX_EPOLL_EVENTS 1024

#define MODULE__DEFAULT_REQUEST_RESPONSE_SIZE (PAGE_SIZE)
#define MODULE__INITIALIZE_GLOBALS "populate_globals" // From Silverfish
#define MODULE__INITIALIZE_MEMORY "populate_memory"   // From Silverfish
#define MODULE__INITIALIZE_TABLE "populate_table"     // From Silverfish
#define MODULE__INITIALIZE_LIBC "wasmf___init_libc"   // From Silverfish
#define MODULE__MAIN "wasmf_main"                     // From Silverfish
#define MODULE__MAX_ARGUMENT_COUNT 16                 // Max number of arguments
#define MODULE__MAX_ARGUMENT_SIZE 64                  // Max size of a single argument
#define MODULE__MAX_MODULE_COUNT 128                  // Max number of modules
#define MODULE__MAX_NAME_LENGTH 32                    // Max module name length
#define MODULE__MAX_PATH_LENGTH 256                   // Max length of path string
#define MODULE__MAX_PENDING_CLIENT_REQUESTS 1000

#define RUNTIME__LOG_FILE "awesome.log"
#define RUNTIME__READ_WRITE_VECTOR_LENGTH 16
#define RUNTIME__MAX_SANDBOX_REQUEST_COUNT (1 << 19) // random!

#define SANDBOX__FILE_DESCRIPTOR_PREOPEN_MAGIC (707707707) // reads lol lol lol upside down
#define SANDBOX__MAX_IO_HANDLE_COUNT 32
#define SANDBOX__PULL_BATCH_SIZE 1 // Max # standboxes pulled onto the local runqueue in a single batch

#define SOFTWARE_INTERRUPT__TIME_TO_START_IN_USEC (10 * 1000)    // start timers 10 ms from now.
#define SOFTWARE_INTERRUPT__INTERVAL_DURATION_IN_USEC (1000 * 5) // and execute every 5ms


// If multicore, use all but the dedicated listener core
// If there are fewer cores than this, main dynamically overrides this and uses all available
#define WORKER_THREAD__CORE_COUNT (NCORES > 1 ? NCORES - 1 : NCORES)


#endif /* SFRT_TYPES_H */
