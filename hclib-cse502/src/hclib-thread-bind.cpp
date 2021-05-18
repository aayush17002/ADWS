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
 * hclib-thread-bind.cpp
 *
 *      Acknowledgments: https://wiki.rice.edu/confluence/display/HABANERO/People
 */

#include <iostream>
#include <stdio.h>
#define _GNU_SOURCE
#define __USE_GNU
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
/** Platform specific thread binding implementations -- > ONLY FOR LINUX **/

namespace hclib {
#ifdef __linux
#include <pthread.h>
static int round_robin = -1;
static int* bind_map = NULL;
static pthread_mutex_t _lock = PTHREAD_MUTEX_INITIALIZER;

int get_nb_cpus() {
    int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
    return numCPU;
}

void bind_thread_with_mask(int *mask, int lg) {
    cpu_set_t cpuset;
    if (mask != NULL) {
        CPU_ZERO(&cpuset);

        /* Copy the mask from the int array to the cpuset */
        int i;
        for (i = 0; i < lg; i++) {
            CPU_SET(mask[i], &cpuset);
        }

        /* Set affinity */
        int res = sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
        if (res != 0) {
            fprintf(stdout,"ERROR: ");
            if (errno == ESRCH) {
                assert("THREADBINDING ERROR: ESRCH: Process not found!\n");
            }
            if (errno == EINVAL) {
                assert("THREADBINDING ERROR: EINVAL: CPU mask does not contain any actual physical processor\n");
            }
            if (errno == EFAULT) {
                assert("THREADBINDING ERROR: EFAULT: memory address was invalid\n");
            }
            if (errno == EPERM) {
                assert("THREADBINDING ERROR: EPERM: process does not have appropriate privileges\n");
            }
        }
    }
}

/* Bind threads in a round-robin fashion */
void bind_thread_rr(int worker_id) {
    /*bind worker_id to cpu_id round-robin fashion*/
    int nbCPU = get_nb_cpus();
    int mask = worker_id % nbCPU;
    bind_thread_with_mask(&mask, 1);
}

/* Bind threads according to bind map */
void bind_thread_map(int worker_id, int bind_map_size) {
    int mask = bind_map[worker_id % bind_map_size];
    fprintf(stdout,"HCLIB_INFO: Binding worker %d to cpu_id %d\n", worker_id, mask);
    fflush(stdout);
    bind_thread_with_mask(&mask, 1);
}

/** Thread binding api to bind a worker thread using a particular binding strategy **/
void bind_thread(int worker_id, int nworkers) {
    assert(pthread_mutex_lock(&_lock) == 0);
    if(round_robin == -1) {
        char* map = getenv("HCLIB_BIND_THREADS");
	assert(map);
	bind_map = (int*) malloc(sizeof(int) * nworkers);
	assert(bind_map);
        int index=0;
	char *token = strtok(map, ",");
        while(token) {
            bind_map[index++]=atoi(token);
	    token = strtok(NULL, ",");
	}
	if(nworkers>1 && index==nworkers) {
            fprintf(stdout,"HCLIB_INFO: Thread Binding as per Bind Map\n");
            fflush(stdout);
            round_robin=0;
	}
	else {
            fprintf(stdout,"HCLIB_INFO: Round Robin Thread Binding\n");
            fflush(stdout);
            round_robin=1;
	    for(index=0; index<nworkers; index++) bind_map[index] = index;
	}
    }
    assert(pthread_mutex_unlock(&_lock) == 0);

    if (round_robin == 1) {
        /* Round robin binding */
        bind_thread_rr(worker_id);
    } else { /*Bind map provided */
        bind_thread_map(worker_id, nworkers);
    }
}

int* get_thread_bind_map() {
    assert(bind_map);
    return bind_map;
}
#else
void bind_thread(int worker_id, int nworkers) {

}
int* get_thread_bind_map() {
    assert(0 && "This API should be called only with HCLIB_BIND_THREADS");
    return NULL;
}
#endif

}
