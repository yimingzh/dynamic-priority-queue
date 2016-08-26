#include "pq.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Name: Yiming Zhong

// implementation:

struct pq
{
  struct pqpair ** pair_arr;// dynamically allocated array of structs
  int len; // length of pqueue
  int max_len; // for realloc doubling strategy
};

struct pqpair
{
  int key; 
  int val;
};


// see interface file for details

struct pq *pq_create(void)
{
  struct pq *new = malloc(sizeof(struct pq)); 
  new->len = 0;
  new->max_len = 1;
  new->pair_arr = malloc(new->max_len * sizeof(struct pqpair *)); 
  
  return new;
}


// see interface file for details

void pq_destroy(struct pq *pqueue)
{
  int length = pqueue->len;  
  for (int i = 0; i < length; ++i)
  {
    free (*(pqueue->pair_arr + i)); // free each information pair separately.
  }
  
  free(pqueue->pair_arr); 
  free(pqueue);
}


// swap_pairs(pqueue, a, b) swaps the item and keys
// stored at indexes a and b of the heap/array
// effects: modifies pqueue
// time: O(1)
                        
static void swap_pairs(struct pq *pqueue, int a, int b)
{
  struct pqpair *temp = *(pqueue->pair_arr + a);
  *(pqueue->pair_arr + a) = *(pqueue->pair_arr + b); 
  *(pqueue->pair_arr + b) = temp;
}
          

// see interface file for details

void pq_add(struct pq *pqueue, int item, int priority)
{
  assert(pqueue); 
  assert(priority > 0); 
  
  ++pqueue->len; 
  // realloc if necessory (with strategy, runtime of O(1)
  if (pqueue->max_len == pqueue->len)
  {
    pqueue->max_len *= 2; 
    pqueue->pair_arr = realloc(pqueue->pair_arr, 
                               pqueue->max_len * sizeof(struct pqpair *));
  }
  
  // now insert into heap 
  struct pqpair *new_pair = malloc(sizeof(struct pqpair)); 
  new_pair->val = item; 
  new_pair->key = priority; 
  
  *(pqueue->pair_arr + pqueue->len - 1) = new_pair;
  
  int parent = (pqueue->len - 2) / 2; //the parents's location in array
  
  // index where new pair resides right now
  int cur_index = pqueue->len - 1; 
  
  // works because C rounds down with division
  while (priority < (*(pqueue->pair_arr + parent))->key
         || parent > 0)
    // the second statements checks if we are at root
  {
    // helper swap function 
    swap_pairs(pqueue, parent, cur_index);
    
    // increment cur_index and parent 
    cur_index = parent; 
    parent = (cur_index - 1) / 2; 
  }
}
                        
                        
// see interface file for details

int pq_remove(struct pq *pqueue)
{
  int value = (*(pqueue->pair_arr))->val; 
  
  (*(pqueue->pair_arr))->key = (*(pqueue->pair_arr + pqueue->len - 1))->key;
  (*(pqueue->pair_arr))->val = (*(pqueue->pair_arr + pqueue->len - 1))->val;
  free(*(pqueue->pair_arr + pqueue->len - 1));
  --pqueue->len; 
  
  int swapping_key = (*(pqueue->pair_arr))->key; 
  int cur_index = 0; 
  int child_left = (2 * cur_index) + 1;
  int child_right = (2 * cur_index) + 2;
  
  while (1)
  {
    if (child_left >= pqueue->len - 1 &&
        child_right >= pqueue->len - 1)
    {
      break;
    }
    else if (child_left < pqueue->len)
    {
      if (swapping_key > (*(pqueue->pair_arr + child_left))->key)
      {
        swap_pairs(pqueue, child_left, cur_index);
      }
      
      break;
    }
    else if (child_right < pqueue->len)
    {
       if (swapping_key > (*(pqueue->pair_arr + child_right))->key)
      {
        swap_pairs(pqueue, child_right, cur_index);
      }
      break;
    }
    else
    {
      if ((*(pqueue->pair_arr + child_left))->key >= 
        (*(pqueue->pair_arr + child_right))->key)
      {
        swap_pairs(pqueue, child_right, cur_index);
        cur_index = child_right; 
        child_left = (2 * cur_index) + 1;
        child_right = (2 * cur_index) + 2;
      }
      else
      {
        swap_pairs(pqueue, child_left, cur_index);
        cur_index = child_left; 
        child_left = (2 * cur_index) + 1;
        child_right = (2 * cur_index) + 2;
      }
    }
    
  }
  return value; 
}


// see interface file for details

bool pq_is_empty(const struct pq *pqueue)
{
  bool empty = (pqueue->len == 0); 
  return empty; 
}


// see interface file for details

void pq_print(const struct pq *pqueue)
{
  assert(pqueue); 
  
  printf("[");
  int length = pqueue->len; 
  
  if (length > 0)
  {
    for (int i = 0; i < length - 1; ++i)
    {
      printf("("); 
      printf("%d,%d", (*(pqueue->pair_arr + i))->val, 
                      (*(pqueue->pair_arr + i))->key);
      printf("),");
    }
  
    // print last item separately
    printf("("); 
    printf("%d,%d", (*(pqueue->pair_arr + length - 1))->val, 
                   (*(pqueue->pair_arr + length - 1))->key);
    printf(")]\n");
  }
  else
  {
    printf("]\n");
  }
}


// see interface file for details

int pq_length(const struct pq *pqueue)
{
  int length = pqueue->len; 
  return length; 
}


// see interface file for details

int pq_front_priority(const struct pq *pqueue)
{
  assert(pqueue); 
  assert(pq_is_empty(pqueue)); 
  
  int pri = (*(pqueue->pair_arr))->key; 
  
  return pri; 
}


// see interface file for details

int pq_front_item(const struct pq *pqueue)
{
  assert(pqueue); 
  assert(pq_is_empty(pqueue)); 
  
  int val = (*(pqueue->pair_arr))->val; 
  
  return val; 
}
