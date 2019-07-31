#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/msg.h>

#define args_check(m, num){if(m!=num){printf("error args\n");return -1;}}
