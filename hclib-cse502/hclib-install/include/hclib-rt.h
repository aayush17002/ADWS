/* Copyright (c) 2015, Rice University

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1.  Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.
3.  Neither the name of Rice University
     nor the names of its contributors may be used to endorse or
     promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

/*
 * hclib-rt.h
 *
 *      Acknowledgments: https://wiki.rice.edu/confluence/display/HABANERO/People
 */


namespace hclib {

// -------------------- SOME BUILD TIME CONTROL FLAGS ------------------------> START //

//#define __USE_HC_MM__

// END <-------------------- SOME BUILD TIME CONTROL FLAGS ------------------------ //

// forward declaration
extern pthread_key_t wskey;
struct hc_context;
struct hc_options;
struct hc_workerState;
struct place_t;
struct deque_t;
struct hc_deque_t;
struct finish_t;

typedef struct hc_workerState {
#ifdef __USE_HC_MM__
        hc_mm_bucket buckets [HC_MM_BUCKETS];
#endif
        pthread_t t; /* the pthread associated */
        finish_t*  current_finish;
        deque_t*        deq;
        struct place_t * pl; /* the directly attached place */
        struct place_t ** hpt_path; /* Path from root to worker's leaf place. Array of places. */
        struct hc_context * context;
        struct hc_workerState * nnext; /* the link of other ws in the same place */
        struct hc_deque_t * current; /* the current deque/place worker is on */
        struct hc_deque_t * deques;
        int id; /* The id, identify a worker */
        int did; /* the mapping device id */
} hc_workerState;

#ifdef HC_ASSERTION_CHECK
#define HASSERT(cond) if(!(cond)){ printf("W%d: assertion failure\n", hclib::get_hc_wid()); assert(cond); }
#else
#define HASSERT(cond)       //Do Nothing
#endif
#define current_ws_internal() ((hc_workerState *) pthread_getspecific(wskey))
int get_hc_wid();
hc_workerState* current_ws();
}

#ifdef __USE_HC_MM__
#include "mm.h"
#endif

namespace hclib {
#ifdef __USE_HC_MM__
#define HC_MALLOC(msize) hc_mm_malloc(current_ws_internal(), msize)
#define HC_FREE(p) hc_mm_free(current_ws_internal(), p)
void *hc_mm_malloc(struct hc_workerState * const ws, size_t msize);
void hc_mm_free(struct hc_workerState * const ws, void *ptr);
#else
#define HC_MALLOC(msize)	malloc(msize)
#define HC_FREE(p)			free(p)
#endif
typedef void (*generic_framePtr)(void*);
}

#include "hclib-timer.h"
#include "hclib-ddf.h"
#include "hclib-async.h"
#include "hclib-asyncAwait.h"
#include "hclib-forasync.h"
#include "hclib-place.h"
#include "hclib-hpt.h"

namespace hclib {

void finish(std::function<void()> lambda);
void kernel(std::function<void()> lambda);
int numWorkers();
void init(int * argc, char ** argv);
void finalize();
void start_finish();
void end_finish();


void finish(std::function<void()> lambda, double totalWeight);
void start_finish(double totalWeight);
void check_in_finish(finish_t * finish);
void check_out_finish(finish_t * finish);
hc_context * get_hclib_context();
void execute_task(task_t* task);
}

