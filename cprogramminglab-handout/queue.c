/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q;

    q = (queue_t *) malloc(sizeof (queue_t ));
    if(!q)
        return NULL;

    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Remember to free the queue structue and list elements */
    if (!q)
        exit(0);
    while (q->head)
    {
            list_ele_t *temp;
            temp = q->head;
            q->head = q->head->next;
            free(temp);
    }
    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_head(queue_t *q, int v)
{
    /* What should you do if the q is NULL? */
    /* What if malloc returned NULL? */
    if (!q)
        return false;
    else {
        list_ele_t *p = (list_ele_t *) malloc(sizeof (list_ele_t));
        if (!p)
            return false;
        p->value = v;
        p->next = q->head;
        q->head = p;
        if (q->size == 0)
            q->tail = p;
        (q->size)++;
    }
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_tail(queue_t *q, int v)
{
    /* Remember: It should operate in O(1) time */
    if (!q)
        return false;
    else if (q->size == 0)
        q_insert_head(q, v);
    else {
        list_ele_t *p = (list_ele_t *) malloc(sizeof(list_ele_t));
        if (!p)
            return false;
        p->value = v;
        p->next = NULL;
        q->tail->next = p;
        q->tail = p;
        (q->size)++;
    }
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If vp non-NULL and element removed, store removed value at *vp.
  Any unused storage should be freed
*/
bool q_remove_head(queue_t *q, int *vp)
{
    list_ele_t *temp;

    if (q == NULL)
        return false;
    if (q->head == NULL)
        return false;
    if (q->size == 1) {
        if (vp != NULL)
            *vp = q->head->value;
        free(q->head);
        q->head = q->tail = NULL;
        (q->size)--;
    }
    else {
        temp = q->head;
        q->head = q->head->next;
        temp->next = NULL;
        if (vp != NULL)
            *vp = temp->value;
        (q->size)--;
        free(temp);
    }
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* Remember: It should operate in O(1) time */
    if (q)
        return q->size;
    else
        return 0;
}

/*
  Reverse elements in queue.

  Your implementation must not allocate or free any elements (e.g., by
  calling q_insert_head or q_remove_head).  Instead, it should modify
  the pointers in the existing data structure.
 */
void q_reverse(queue_t *q)
{
    list_ele_t *a, *b;
    list_ele_t *flag;

    if (q == NULL)
        return;
    if (q->size <= 1)
        return;
    else {
        a = q->head;
        b = q->head->next;
        flag = q->head->next->next;

        q->tail = q->head;
        q->tail->next = NULL;
        while (flag != NULL) {
            b->next = a;
            a = b;
            b = flag;
            flag = flag->next;
        }
        b->next = a;
        q->head = b;
    }
}