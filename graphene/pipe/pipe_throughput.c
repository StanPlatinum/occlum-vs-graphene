#include <sys/syscall.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <spawn.h>
#include <string.h>

#define KB              (1024UL)
#define MB              (1024UL * 1024UL)
#define GB              (1024UL * 1024UL * 1024UL)

/* #define SMALL_TOTAL_BYTES (128 * MB) */
#define SMALL_TOTAL_BYTES (4 * MB)
/* #define MID_TOTAL_BYTES (1* GB) */
#define MID_TOTAL_BYTES (32* MB)
/* #define TOTAL_BYTES     (32* GB) */
#define TOTAL_BYTES     (1* GB)

/* #define BUF_SIZE        (128 * KB) */

#define MIN(x, y)       ((x) <= (y) ? (x) : (y))

int main(int argc, const char* argv[]) {
    
    printf("Starting...\n");
    fflush(stdout);

    // Create pipe
    int pipe_fds[2];
    if (pipe(pipe_fds) < 0) {
        printf("ERROR: failed to create a pipe\n");
        return -1;
    }
    if (argc != 2){
      printf("Error, please give buffer size in argv\n");
      exit(-1);
    }
    size_t buf_size = atoi(argv[1]);
    int pipe_rd_fd = pipe_fds[0];
    int pipe_wr_fd = pipe_fds[1];

    printf("initiating...\n");
    fflush(stdout);
    
    // Spawn a child process that reads from the pipe
    posix_spawn_file_actions_t file_actions;
    posix_spawn_file_actions_init(&file_actions);
    posix_spawn_file_actions_adddup2(&file_actions, pipe_rd_fd, STDIN_FILENO);
    posix_spawn_file_actions_addclose(&file_actions, pipe_wr_fd);

    printf("preparing...\n");
    fflush(stdout);

    int child_pid;
    char *spawn_argv[] = {"dev_null", NULL};
    if (posix_spawn(&child_pid, "dev_null", &file_actions, NULL, spawn_argv, NULL) < 0) {
        perror("ERROR: failed to spawn a child process\n");
        return -1;
    }
    //close(pipe_rd_fd);

    // Start the timer
    struct timeval tv_start, tv_end;

    
    size_t total_bytes = SMALL_TOTAL_BYTES;
    
    /*
    if( buf_size <64)
      total_bytes = SMALL_TOTAL_BYTES;
    else if( buf_size <=64)
      total_bytes = MID_TOTAL_BYTES;
    */
    
    
    size_t remain_bytes = total_bytes;
    
    
    gettimeofday(&tv_start, NULL);

    printf("writing...\n");
    fflush(stdout);

    // Tell the reader how many data are to be transfered
    if (write(pipe_wr_fd, &remain_bytes, sizeof(remain_bytes)) != sizeof(remain_bytes)) {
        printf("ERROR: failed to write to pipe\n");
        return -1;
    }

    // Tell the reader the buffer size that it should use
    if (write(pipe_wr_fd, &buf_size, sizeof(buf_size)) != sizeof(buf_size)) {
        printf("ERROR: failed to write to pipe\n");
        return -1;
    }

    printf("mallocing...\n");
    fflush(stdout);
    
    // Write a specified amount of data in a buffer of specified size
    char *write_buf = (char *) malloc(buf_size*sizeof(char));
 
    // Read a specified amount of data in a buffer of specified size
    char *read_buf = (char *) malloc(buf_size*sizeof(char));


    printf("looping...\n");
    fflush(stdout);

    while (remain_bytes > 0) {
        size_t len = MIN(buf_size, remain_bytes);
        
	//printf("  remain: %ld\n", remain_bytes);
	//fflush(stdout);
	
	if ((len = write(pipe_wr_fd, write_buf, len)) < 0) {
            printf("ERROR: failed to write to pipe\n");
            return -1;
        }
   
	remain_bytes -= len;

	//printf("  reading...\n");
	//fflush(stdout);

	if ((read(pipe_rd_fd, read_buf, len)) < 0) {
            printf("ERROR: failed to read to pipe\n");
            return -1;
        }

    }

    /*
    printf("waiting...\n");
    fflush(stdout);
    
    // Wait for the child process to read all data and exit
    int status = 0;
    if (wait4(child_pid, &status, 0, NULL) < 0) {
        printf("ERROR: failed to wait4 the child process\n");
        return -1;
    }
    */

    // Stop the timer
    gettimeofday(&tv_end, NULL);

    printf("counting...\n");
    fflush(stdout);
    
    // Calculate the throughput
    double total_s = (tv_end.tv_sec - tv_start.tv_sec)
                   + (double)(tv_end.tv_usec - tv_start.tv_usec) / 1000000;
    if (total_s < 1.0) {
        printf("WARNING: run long enough to get meaningful results\n");
        if (total_s == 0) { return 0; }
    }
    double total_mb = (double)total_bytes / MB;
    double throughput = total_mb / total_s;
    printf("Throughput of pipe is %.2f MB/s\n", throughput);
    return 0;
}
