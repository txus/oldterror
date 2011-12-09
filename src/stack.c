#include <stdio.h>
#include "stack.h"

void Stack_print(Object **stack, Object **sp) {
  // Set a peek pointer to the top of the stack
  Object **peek = stack;
  int i=0;

  while(peek < sp) {
    // Print the contents of each stack cell up until the Stack Pointer.
    printf("STACK[%i]\n", i);
    if (*(++peek)) {
      Object_print(*peek);
    } else {
      printf("NULL");
    }
    printf("\n");

    i++;
  }
}
