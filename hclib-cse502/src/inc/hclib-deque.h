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
 * hclib-deque.h
 *  
 *      Acknowledgments: https://wiki.rice.edu/confluence/display/HABANERO/People
 */

#ifndef HCLIB_DEQUE_H_
#define HCLIB_DEQUE_H_

namespace hclib {

/****************************************************/
/* DEQUE API                                        */
/****************************************************/

#define INIT_DEQUE_CAPACITY 8096

typedef struct deque_t {
    volatile int head;
    volatile int tail;
    volatile task_t* data[INIT_DEQUE_CAPACITY];
} deque_t;

void dequeInit(deque_t * deq, void * initValue);
bool dequePush(deque_t* deq, void* entry);
task_t* dequePop(deque_t * deq);
task_t* dequeSteal(deque_t * deq);
void dequeDestroy(deque_t* deq);

/****************************************************/
/* Semi Concurrent DEQUE API                        */
/****************************************************/
typedef struct {
    deque_t deque;
    volatile int lock;
} semiConcDeque_t;

void semiConcDequeInit(semiConcDeque_t* deq, void * initValue);
void semiConcDequeLockedPush(semiConcDeque_t* deq, void* entry);
task_t* semiConcDequeNonLockedPop(semiConcDeque_t * deq);
void semiConcDequeDestroy(semiConcDeque_t * deq);

}

#endif /* HCLIB_DEQUE_H_ */
