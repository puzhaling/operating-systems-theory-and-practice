#include <stdio.h>
#include <stdlib.h>

struct vector_t {
  int *data;
  size_t capacity;
  size_t size;
};

void print_vector_t(const struct vector_t v) {
  size_t size;
  size_t i;

  size = v.size;
  for (i = 0; i < size; ++i) {
    printf("%d ", v.data[i]);
  } 
  putchar('\n');
}

void fill_vector_t_with(struct vector_t v, int val) {
  size_t size;
  size_t i;

  size = v.size;
  for (i = 0; i < size; ++i) {
    v.data[i] = val;
  }
}

void print_vector_info(struct vector_t v) {
  printf("size: %ld\n", v.size);
  printf("capacity: %ld\n", v.capacity);
}

void push_back(struct vector_t *v, int val) {
  if (v->size + 1 == v->capacity) {
    int new_capacity = v->capacity * 1.25;
    v->data = (int *) realloc(v->data, new_capacity);
    v->capacity = new_capacity;
  }
  v->data[v->size++] = val;
}

int main(void) {
  struct vector_t v = {
    (int *) malloc(sizeof(int) * 10), 3, 0
  };
  push_back(&v, 10);
  print_vector_info(v);
  push_back(&v, 20);
  print_vector_info(v);
  push_back(&v, 30);
  print_vector_info(v);
  push_back(&v, 40);
  print_vector_info(v);
  push_back(&v, 50);
  print_vector_info(v);
  free(v.data);
  return 0;
}
