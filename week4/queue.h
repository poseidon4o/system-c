#include <stdlib.h>


struct _que_node {
    struct _que_node * _prev;
    void * data; 
};

typedef struct queue_tag {
    struct _que_node * head, * tail;
} queue_t;

void init_queue(queue_t ** que) {
    *que = (queue_t*)malloc(sizeof(queue_t));
    (*que)->head = (*que)->tail = NULL;
}

void * queue_pop(queue_t * que) {
    void * data = NULL;    
    if (que->head) {
        struct _que_node * t = que->head;
        data = t->data;
        if (que->head == que->tail) {
            que->head = que->tail = NULL;
        } else {
            que->head = que->head->_prev;
        }
        
        free(t);
    }
    return data;
}

void queue_push(queue_t * que, void * data) {
    struct _que_node * node = (struct _que_node*)malloc(sizeof(struct _que_node));
    node->data = data;
    node->_prev = NULL;
    if (que->tail) {
        que->tail->_prev = node;
    } else {
        que->head = node;
    }
    
    que->tail = node;
}

