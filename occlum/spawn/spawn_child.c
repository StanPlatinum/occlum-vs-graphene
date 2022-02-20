#include <spawn.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <errno.h>

extern char ** environ;
int NTRIES;

int main(int argc, char **argv) {
  /* printf("this is child\n"); */
  if(argc < 2){
    NTRIES = 10000;
  }else {
    NTRIES = atoi(argv[1]);
    if(NTRIES == 0) NTRIES = 10000;
  }
  int choice = 0;
  if (argc == 3) {
    choice = atoi(argv[2]);
  }

  char byte;

  struct timeval timevals[2];
  /* posix_spawn_file_actions_t file_actions; */
  /* posix_spawn_file_actions_init(&file_actions); */

  char *const new_argv0[] = {"spawn_victim",  NULL};
  char *const new_argv1[] = {"busybox","true",  NULL};
  char *const new_argv2[] = {"cc1", "-occlumquiet", "-quiet", NULL};
  char *const *new_argv = new_argv0;
  if(choice == 1)  
    new_argv = new_argv1;
  if(choice == 2) 
    new_argv = new_argv2;
  int child;
  // warm up
  for(int count =0; count < 3; count++){
    int ret = posix_spawn(&child, new_argv[0], NULL, NULL, new_argv, environ);
    waitpid(child, NULL, 0);
  }

  gettimeofday(&timevals[0], NULL);
  for (int count = 0; count < NTRIES; count++) {
    int ret = posix_spawn(&child, new_argv[0], NULL, NULL, new_argv, environ);
    if(ret != 0) {
        perror("ERROR: failed to spawn\n");
	      return -1;
    }
    waitpid(child, NULL, 0);
  }

  gettimeofday(&timevals[1], NULL);
  unsigned long long start_time = 0;
  unsigned long long end_time = 0;
  unsigned long long total_time = 0;

  start_time = timevals[0].tv_sec * 1000000ULL + timevals[0].tv_usec;
  end_time = timevals[1].tv_sec * 1000000ULL + timevals[1].tv_usec;
  double st = timevals[0].tv_sec + 1.0 * timevals[0].tv_usec / 1000000ULL ;
  double et = timevals[1].tv_sec + 1.0 * timevals[1].tv_usec / 1000000ULL ;

  printf("fork %d children: throughput = %lf procs/second, "
         "latency = %lf microseconds %lf seconds\n",
         NTRIES,
         1.0 * NTRIES* 1000000 / (end_time - start_time),
         1.0 * (end_time- start_time)/ (NTRIES),
         1.0 * (et-st)/(NTRIES));
  return 0;
}
