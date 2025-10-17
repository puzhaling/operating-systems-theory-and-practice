#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct __node_t {
    pthread_mutex_t mutex;
    int val;
    struct __node_t *next;
} node_t;

typedef struct __list_t {
    node_t *head;
} list_t;

list_t list;

void list_init(list_t *list) 
{
    list->head = malloc(sizeof(node_t));
    list->head->next = NULL;
    pthread_mutex_init(&list->head->mutex, NULL);
}

void list_insert(list_t *list, int val)
{
    node_t *new_node = malloc(sizeof(node_t)); 
    if (new_node == NULL) {
        perror("malloc");
        return;
    }

    new_node->val = val;
    new_node->next = NULL;
    pthread_mutex_init(&new_node->mutex, NULL);
    
    node_t *prev = list->head;
    pthread_mutex_lock(&prev->mutex);
    
    node_t *curr = prev->next;
    if (curr != NULL) {
        pthread_mutex_lock(&curr->mutex);
    }

    prev->next = new_node;
    new_node->next = curr;
    
    if (curr != NULL) {
        pthread_mutex_unlock(&curr->mutex);
    }
    pthread_mutex_unlock(&prev->mutex);
}

void list_delete(list_t *list, int val)
{
    node_t *prev = list->head;
    pthread_mutex_lock(&prev->mutex);

    node_t *curr = prev->next;
    if (curr != NULL) {
        pthread_mutex_lock(&curr->mutex);
    }

    while (curr != NULL) {
        if (curr->val == val) {
            prev->next = curr->next;
            pthread_mutex_unlock(&prev->mutex);  
            pthread_mutex_destroy(&curr->mutex); 
            free(curr);                          
            return;                             
        }

        pthread_mutex_unlock(&prev->mutex); 
        prev = curr;
        curr = curr->next;
        if (curr != NULL) {
            pthread_mutex_lock(&curr->mutex); 
        }
    }

    pthread_mutex_unlock(&prev->mutex);
    if (curr != NULL) {
        pthread_mutex_unlock(&curr->mutex);
    }
}

void list_print(list_t *list)
{
    node_t *curr = list->head->next;
    while (curr != NULL) {
        pthread_mutex_lock(&curr->mutex);
        printf("%d ", curr->val);
        node_t *temp = curr;
        curr = curr->next;
        pthread_mutex_unlock(&temp->mutex);
    }
    putchar('\n');
}

void list_free(list_t *list)
{
    node_t *curr = list->head;
    while (curr != NULL) {
        node_t *temp = curr;
        curr = curr->next;
        pthread_mutex_destroy(&temp->mutex);
        free(temp);
    }
}



// TESTS DRIVERS:

#define NTHREADS 3

typedef struct __range_t {
    int a;
    int b;
} range_t;

void *insert_list_from_to(void *arg)
{
    range_t *range = arg;
    int i;

    for (i = range->a; i <= range->b; ++i) {
        list_insert(&list, i);                
    }

    return NULL;
}

void test_for_insert(list_t *list)
{
    pthread_t thread[NTHREADS];
    range_t range[NTHREADS];
    int i;

    for (i = 0; i < NTHREADS; ++i) {
        range[i].a = i * 10;
        range[i].b = (i + 1) * 10 - 1;
        pthread_create(&thread[i], NULL, 
             insert_list_from_to, (void *) &range[i]);
    }

    for (i = 0; i < NTHREADS; ++i) {
        pthread_join(thread[i], (void **) NULL);
    }
}

int main(void)
{
    list_init(&list);

    test_for_insert(&list);

    list_print(&list);

    list_free(&list);
}
