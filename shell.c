/**
 * MATHEW RINNE
 * CHAPTER 3 PROJECT 1
 * CS300
 **/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <assert.h>


#define MAX_LINE 80 /* maximum length command */


/**********functions**********/
typedef struct node1 node1;
typedef struct dll DLL;
extern DLL* newDLL();
extern void insertDLL(DLL *items, int index, char value[]);
extern void *getDLL(DLL *items, int index);
extern int sizeDLL(DLL *items);
void display_history(DLL *history);
void add_to_history(char input_buffer[], DLL *history);
int clean(char buffer[], char *args[]);
/*****************************/


struct node1
{
  struct node1 *prev;
  struct node1 *next;
  char data[MAX_LINE];
} ;

struct dll
{
  void *head;
  void *tail;
  int size;
} ;

extern DLL *
newDLL()  //constructor of a new Doubly Linked List
{
  DLL *items = malloc(sizeof(DLL));
  assert(items != 0);
  items->head = 0;
  items->tail = 0;
  items->size = 0;
  return items;
}

extern void
insertDLL(DLL *items,int index,char value[])  //insert a node to list
{
  assert(index >= 0 && index <= items->size);
  struct node1 *temp = malloc(sizeof(node1));
  for (int i=0; i<strlen(value); i++)
  {
    temp->data[i] = value[i];
  }
  temp->prev = NULL;
  temp->next = NULL;

  if(items->size == 0) //add to empty list
  {
    items->head = temp;
    items->tail = temp;
    items->size = 1;
    return;
  }
  else
  {
    struct node1 *curr = items->tail;
    curr->next = temp;
    temp->prev = curr;
    temp->next = NULL;
    items->tail = temp;
    items->size++;
  }

  return;
}

extern void *
getDLL(DLL *items,int index)  //returns the data at any given node
{
  assert(index >= 0 && index <items->size);
  struct node1 *get = items->head;
  if (index == 0) return get->data;
  else if (index == items->size-1)
  {
    get = items->tail;
    return get->data;
  }
  else if (index > items->size/2)
  {
    get = items->tail;
    int ind = items->size-1;
    while (index != ind)
    {
      get = get->prev;
      ind--;
    }
    return get->data;
  }
  else
  {
    for (int i=0; i<index; i++)
    {
      get = get->next;
    }
    return get->data;
  }
}

extern int
sizeDLL(DLL *items)  //returns the size of list
{
  return items->size;
}

void display_history(DLL *history)
{
  printf("\nShell Command History:\n");
  if (sizeDLL(history) < 11)
  {
    for (int i=sizeDLL(history); i>0; i--)
    {
      printf("%d. %s\n", i, getDLL(history,i-1));
    }
  }
  else
  {
    for (int i=sizeDLL(history); i>sizeDLL(history)-10; i--)
    {
      printf("%d. %s\n", i, getDLL(history,i-1));
    }
  }
  return;
}

void add_to_history(char input_buffer[], DLL *history)
{
  insertDLL(history, history->size, input_buffer);
  return;
}

int clean(char buffer[], char *args[])
{
  int len = strlen(buffer);
  if (buffer[len-1] == '\n') buffer[len-1] = 0;

  int flag = 1;
  int arg_count = 0;
  char *token = strtok(buffer, " ");

  while (token)
  {
    if (strcmp(token, "&") == 0)
    {
      flag = 0;
      token = NULL;
    }
    args[arg_count] = token;
    arg_count++;
    token = strtok(NULL, " ");
  }

  args[arg_count] = NULL;

  return flag;
}

int main(void)
{
  char *args[MAX_LINE/2 + 1];
  int should_run = 1;
  char buffer[MAX_LINE];
  int flag;
  DLL *history = newDLL();

  pid_t pid;

  while (should_run)
  {
    printf("osh>");
    fflush(stdout);

    fgets(buffer, MAX_LINE, stdin);

    if (strcmp(buffer, "exit\n") == 0)
    {
      should_run = 0;
      break;
    }

    char input_buffer[MAX_LINE];
    for (int i=0; i<MAX_LINE; i++)
    {
      input_buffer[i] = buffer[i];
    }
    input_buffer[strlen(input_buffer)-1] = 0;

    if (strcmp(buffer, "history\n") == 0)
    {
      if (sizeDLL(history) > 0) display_history(history);
      else printf("\nNo History\n\n");

    }
    else if (strlen(buffer) == 1)
    {
      continue;
    }
    else
    {
      if (strlen(buffer) > 1)
      {
        flag = clean(buffer, args);
      }

      int history_flag = 0;
      int skip = 0;

      if (buffer[0] == '!')
      {
        if (buffer[1] == '!')
        {
          if (sizeDLL(history) == 0)
          {
            printf("No commands in history\n");
            skip = 1;
            continue;
          }
          char temp[MAX_LINE];
          strcpy(temp, getDLL(history,sizeDLL(history)-1));
          add_to_history(temp, history);
          flag = clean(temp, args);
          history_flag = 1;
        }
        else
        {
          if (buffer[2] == 0)
          {
            char *dig = &input_buffer[1];
            int x = atoi(dig);
            if (x > sizeDLL(history) || x == 0)
            {
              printf("No such command in history\n");
              skip = 1;
              continue;
            }
            char temp[MAX_LINE];
            strcpy(temp, getDLL(history,x-1));
            add_to_history(temp, history);
            flag = clean(temp, args);
            history_flag = 1;
          }
          else
          {
            char dig[strlen(input_buffer)-2];
            for(int i=0; i<strlen(input_buffer)-1; i++)
            {
              dig[i] = input_buffer[i+1];
            }
            int x = atoi(dig);
            if (x > sizeDLL(history) || x == 0)
            {
              printf("No such command in history\n");
              skip = 1;
              continue;
            }
            char temp[MAX_LINE];
            strcpy(temp, getDLL(history,x-1));
            add_to_history(temp, history);
            flag = clean(temp, args);
            history_flag = 1;
          }
        }
      }

      if (!history_flag) add_to_history(input_buffer, history);

      if (!skip)
      {
        pid = fork();

        if (pid < 0)
        {
          printf("fork failed\n");
          return 0;
        }
        else if (pid == 0)
        {
          execvp(args[0], args);
          should_run = 0;
        }
        else
        {
          if (flag) wait(NULL);
          else sleep(1);
        }
      }
    }
  }
}
