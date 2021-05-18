#include "hclib-internal.h"
#include "hclib-atomics.h"
#include <stdio.h>

/*
 * NOTE: Following points will be strictly evaluated during
 * final project demo. Failing to comply with any point will
 * lead to mark deduction despite your project working perfectly:
 *
 * 1. All code  related to FPP project should be inside this file only.
 * You should ONLY do bare-minimum (and mandatory) modifications inside
 * other files in HClib. You can easily localize your modifications 
 * inside this file by defining the method here, and only calling that
 * method at other location(s) inside HClib codebase.
 *
 * 2. Although this is a C++ file, but you should ONLY use C-coding. Use
 * of C++ is restricted (e.g., don't use std::vectors, but instead 
 * use only arrays, etc.)
 *
 */

namespace  hclib{

    void finish(std::function<void()> lambda, double totalWeight){
        start_finish(totalWeight);
        lambda();
        end_finish();
    }
    void start_finish(double totalWeight){
        hc_workerState* ws = current_ws_internal();
        finish_t * finish = (finish_t*) HC_MALLOC(sizeof(finish_t));
        finish->counter = 0;
        finish->parent = ws->current_finish;
        finish->totalWeight = totalWeight;
        finish->stealRange = (Node*) HC_MALLOC(sizeof(Node));
        if(finish->parent) {
            check_in_finish(finish->parent);
            if (finish->parent->stealRange) {
                finish->stealRange->left = ws->current_finish->stealRange->left;
                finish->stealRange->right = ws->current_finish->stealRange->right;
                finish->stealRange->pointer = ws->current_finish->stealRange->pointer;
            } else {
                finish->stealRange->left = get_hclib_context()->nworkers;
                finish->stealRange->right = 0;
                finish->stealRange->pointer = get_hclib_context()->nworkers;
            }
        }
//        if(totalWeight>=35){
//            printf("left = %f, right = %f, pointer = %f, Weight = %f\n",finish->stealRange->left,finish->stealRange->right,finish->stealRange->pointer,totalWeight);
//        }
        ws->current_finish = finish;
    }
    void spawn(task_t * task, double weight){
        hc_workerState* ws = current_ws_internal();
//        hc_mfence();
        double pointer = ws->current_finish->stealRange->pointer;
        double right_p = ws->current_finish->stealRange->right;
        double totalWeight = ws->current_finish->totalWeight;
        double value = (pointer - right_p)*(weight/totalWeight);
        double left = pointer;
        double right = left - value;
        int index = (int) floor(right);
        if(index==get_hclib_context()->nworkers){
            index--;
        }
        if (index < 0)
        {
        	index = 0;
        }
        // printf("left = %f, right = %f, worker = %d\n",current_ws_internal()->current_finish->stealRange->left,current_ws_internal()->current_finish->stealRange->right,index);
//        printf("steal range pointer before = %f\n",ws->current_finish->stealRange->pointer);
        ws->current_finish->stealRange->pointer = right;
        ws->current_finish->stealRange->right = right_p;
//        printf("steal range pointer after = %f\n",ws->current_finish->stealRange->pointer);
        Node* stealRange = (Node*) HC_MALLOC(sizeof(Node));
        stealRange->left = left;
        stealRange->right = right;
        stealRange->pointer = left;
        stealRange->parent = ws->current_finish->stealRange;
        ws->current_finish->stealRange->addChild(stealRange);
//        Node stealRange = Node(left, right, left, ws->current_finish->stealRange);

        check_in_finish(ws->current_finish);
        task->set_current_finish(ws->current_finish);
        task->current_finish->stealRange = stealRange;
        if(index==get_current_worker()) {
            if (!dequePush(&(get_hclib_context()->workers[get_current_worker()]->current->deque), (task_t *) task)) {
                // TODO: deque is full, so execute in place
                printf("WARNING: deque full, local execution\n");
                execute_task((task_t *) task);
            }
        }
        else {
            dequePush(&(get_hclib_context()->workers[get_current_worker()]->current->migratory), (task_t *) task);
        }
    }




//    inline void helper(finish_t* finish)
//    {
//        hc_workerState* ws = current_ws_internal();
//        int wid = ws->id;
//        while(finish->counter > 0)
//        {
//            // try to pop
//            task_t* task = dequePop(&(ws->current->deque));
//            if (!task)
//            {
//                task = dequePop(&(ws->current->migratory));
//                if (!task)
//                {
//                    while (finish->counter > 0)
//                    {
//                        // try to steal
//                        task = steal_task(ws);
//                        if (task)
//                        {
//#ifdef HC_COMM_WORKER_STATS
//                            increment_steals_counter(wid);
//#endif
//                            break;
//                        }
//                    }
//                }
//            }
//            if(task)
//            {
//                execute_task(task);
//            }
//        }
//    }
//
//    inline task_t* steal_task(hc_workerState* ws)
//    {
//        MARK_SEARCH(ws->id);
//        finish_t* current = ws->current_finish;
//        stealRange range = current->stealRange;
//        task_t* task = NULL:
//
//        while (range != NULL)
//        {
//            if (!(range.isActive()))
//            {
//                range = range->parent;
//            }
//            task = range.findTask();
//            if (task == NULL)
//            {
//                range.deactivateStealRange();
//            }
//            else
//            {
//                break;
//            }
//        }
//        return task;
//    }

//    void spawn(task_t * task, double weight)
//    {
//        hc_workerState* ws = current_ws_internal();
//        double value = (ws->current_finish->myStealRange->pointer - ws->current_finish->myStealRange->right)*(weight/ws->current_finish->myStealRange->totalWeight);
//        double right = ws->current_finish->myStealRange->pointer;
//        double left = right - value;
//        int index = (int) floor(left);
//        ws->current_finish->myStealRange->pointer = left;//Check first if error found anywhere.
//        check_in_finish(ws->current_finish);
//        stealRange range = new stealRange(left, right, pointer, &(ws->current_finish->stealRange));
//        task->set_current_finish(ws->current_finish);
//        task->current_finish->myStealRange = &range;
//
//        hc_deque_t * current_deque_marker = hclib_context->workers[index]->current;
//
//        if(index==ws->id)
//        {
//            if(!dequePush(&(current_deque_marker->deque), (task_t*) async_task))
//            {
//                printf("WARNING: deque full, local execution\n");
//                execute_task((task_t*) async_task);
//            }
//        }
//        else
//        {
//            if (!dequePush(&(current_deque_marker->migratory), (task_t *) async_task))
//            {
//                printf("WARNING: deque full, local execution\n");
//                execute_task((task_t *) async_task);
//            }
//        }
//    }

}
