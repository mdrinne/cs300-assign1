#include <stdio.h>
#include <unistd.h>
// #include <sys/types.h>
#include <sys/wait.h>


void collatz(int val)
{
  printf("%d", val);
  while (val != 1)
  {
    if (val%2 == 1)
    {
      val = 3*val + 1;
    }
    else
    {
      val = val/2;
    }
    printf(", %d", val);
  }
  printf("\n");
}


int main(int argc, char const *argv[])
{

  int val;
  pid_t pid;

  printf("Enter a positive integer: ");
  scanf("%d", &val);

  if (val < 0)
  {
    printf("Invalid input, integer must be postivie.\n");
    printf("Exiting...\n");
    return 0;
  }

  pid = fork();

  if (pid < 0) /* error occured */
  {
    printf("Fork Failed\n");
    return 0;
  }

  else if (pid == 0) /* child process */
  {
    collatz(val);
  }

  else /* parent process */
  {
    /* wait for child to complete */
    wait(NULL);
  }

  return 0;
}
