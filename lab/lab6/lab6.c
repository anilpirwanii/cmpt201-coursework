#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT_EX1(expr)                                                       \
  {                                                                            \
    if (!(expr)) {                                                             \
      fprintf(stderr, "Example 1 Assertion failed: %s\n", #expr);              \
      exit(1);                                                                 \
    }                                                                          \
  }

#define TEST(expr)                                                             \
  {                                                                            \
    if (!(expr)) {                                                             \
      fprintf(stderr, "Test failed: %s at %s:%d\n", #expr, __FILE__,           \
              __LINE__);                                                       \
      exit(1);                                                                 \
    } else {                                                                   \
      /*printf("Test passed: %s\n", #expr);*/                                  \
    }                                                                          \
  }

typedef struct node_ex1 {
  uint64_t data;
  struct node_ex1 *next;
} node_t_ex1;

node_t_ex1 *head_ex1 = NULL;

void insert_sorted_ex1(uint64_t data) {
  node_t_ex1 *new_node = malloc(sizeof(node_t_ex1));
  if (new_node == NULL) {
    perror("malloc failed");
    exit(1);
  }
  new_node->data = data;
  new_node->next = NULL;

  if (head_ex1 == NULL || data < head_ex1->data) {
    new_node->next = head_ex1;
    head_ex1 = new_node;
    return;
  }

  node_t_ex1 *curr = head_ex1;
  while (curr->next != NULL && data >= curr->next->data) {
    curr = curr->next;
  }

  new_node->next = curr->next;
  curr->next = new_node;
}

int index_of_ex1(uint64_t data) {
  node_t_ex1 *curr = head_ex1;
  int index = 0;

  while (curr != NULL) {
    if (curr->data == data) {
      return index;
    }

    curr = curr->next;
    index++;
  }

  return -1;
}

int main_ex1() {
  printf("--- Running Example 1 Logic ---\n");
  insert_sorted_ex1(1);
  insert_sorted_ex1(2);
  insert_sorted_ex1(5);
  insert_sorted_ex1(3);

  TEST(index_of_ex1(3) == 2);

  insert_sorted_ex1(0);
  insert_sorted_ex1(4);

  TEST(index_of_ex1(4) == 4);
  printf("--- Example 1 Tests Passed ---\n");

  node_t_ex1 *current = head_ex1;
  node_t_ex1 *next_node = NULL;
  while (current != NULL) {
    next_node = current->next;
    free(current);
    current = next_node;
  }
  head_ex1 = NULL;

  return 0;
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(expr)                                                           \
  {                                                                            \
    if (!(expr)) {                                                             \
      fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", #expr,       \
              __FILE__, __LINE__);                                             \
      exit(1);                                                                 \
    }                                                                          \
  }

typedef struct node_ex2 {
  uint64_t data;
  struct node_ex2 *next;
} node_t_ex2;

typedef struct info {
  uint64_t sum;
} info_t;

node_t_ex2 *head_ex2 = NULL;
info_t info_ex2 = {0};

uint64_t sum_list() {
  uint64_t current_sum = 0;
  node_t_ex2 *curr = head_ex2;
  while (curr != NULL) {
    current_sum += curr->data;
    curr = curr->next;
  }
  return current_sum;
}

void insert_sorted_ex2(uint64_t data) {
  node_t_ex2 *new_node = malloc(sizeof(node_t_ex2));
  if (new_node == NULL) {
    perror("malloc failed");
    exit(1);
  }
  new_node->data = data;
  new_node->next = NULL;

  if (head_ex2 == NULL) {
    head_ex2 = new_node;
  } else if (data < head_ex2->data) {
    new_node->next = head_ex2;
    head_ex2 = new_node;
  } else {
    node_t_ex2 *curr = head_ex2;
    node_t_ex2 *prev = NULL;

    while (curr != NULL) {
      if (data < curr->data) {
        break;
      }
      prev = curr;
      curr = curr->next;
    }

    prev->next = new_node;

    if (curr != NULL) {
      new_node->next = curr->next;
    }
  }

  info_ex2.sum += data;
}

int index_of_ex2(uint64_t data) {
  node_t_ex2 *curr = head_ex2;
  int index = 0;

  while (curr != NULL) {
    if (curr->data == data) {
      return index;
    }

    curr = curr->next;
    index++;
  }

  return -1;
}

int main() {
  printf("\n--- Running Example 2 Logic ---\n");
  insert_sorted_ex2(1);
  ASSERT(info_ex2.sum == sum_list());

  insert_sorted_ex2(3);
  ASSERT(info_ex2.sum == sum_list());

  insert_sorted_ex2(5);
  ASSERT(info_ex2.sum == sum_list());

  insert_sorted_ex2(2);
  ASSERT(info_ex2.sum == sum_list());

  TEST(info_ex2.sum == 1 + 3 + 5 + 2);
  TEST(index_of_ex2(2) == 1);

  node_t_ex2 *current_ex2 = head_ex2;
  node_t_ex2 *next_node_ex2 = NULL;
  while (current_ex2 != NULL) {
    next_node_ex2 = current_ex2->next;
    free(current_ex2);
    current_ex2 = next_node_ex2;
  }
  head_ex2 = NULL;

  return 0;
}
