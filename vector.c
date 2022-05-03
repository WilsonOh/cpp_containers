#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NOVAL INT_MIN

typedef struct {
  int *vals;
  size_t size;
} vector;

vector *new_vector(size_t init_size) {
  vector *v = calloc(init_size, sizeof(int));
  if (v == NULL) {
    fprintf(stderr, "Failed to create new vector\n");
    return NULL;
  }
  v -> size = init_size;
  v -> vals = malloc(sizeof(int) * init_size);
  return v;
}

void vector_free(vector *v) {
  free(v -> vals);
  free(v);
}

int vector_add(vector *v, int val) {
  int *tmp = realloc(v -> vals, v -> size + 1);
  if (tmp == NULL) {
    fprintf(stderr, "Failed to add to vector\n");
    return -1;
  }
  v -> vals = tmp;
  v -> vals[v -> size] = val;
  v -> size++;
  return 0;
}

int vector_set(vector *v, size_t idx, int val) {
  if (idx >= v -> size) {
    fprintf(stderr, "index %zu out of bounds for vector of size %zu\n", idx, v -> size);
    return -1;
  }
  v -> vals[idx] = val;
  return 0;
}

int vector_get(vector *v, size_t idx) {
  if (idx >= v -> size) {
    fprintf(stderr, "index %zu out of bounds for vector of size %zu\n", idx, v -> size);
    return NOVAL;
  }
  return v -> vals[idx];
}

void print_vector(vector *v) {
  printf("[");
  for (size_t i = 0; i < v -> size; i++) {
    printf("%d", v -> vals[i]);
    if (i < v -> size - 1) {
      putchar(' ');
    }
  }
  printf("]\n");
}

int main (void) {
  vector *v = new_vector(3);
  vector_set(v, 0, 1);
  vector_set(v, 1, 2);
  vector_set(v, 2, 3);
  vector_add(v, 4);
  vector_set(v, 0, 3);
  print_vector(v);
  vector_free(v);
}
