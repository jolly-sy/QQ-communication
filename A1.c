#include "func.h"

int shmid;
struct t
{
	char buf3[128];
};

struct t *p;
void final(int signum)
{
	shmdt(p);
	shmctl(shmid,IPC_RMID,NULL);
	printf("No.%d signal is coming, shm will be deleted!\n",signum);
	exit(0);
}

int main()
{//process A1
	shmid=shmget(1234,4096,IPC_CREAT|0600);
	p=(struct t *)shmat(shmid,NULL,0);
	char s[128];
	while(1)
	{
		if(strcmp(s,p->buf3))
		{
			strcpy(s,p->buf3);
			printf("%s\n",s);
		}
	}
	exit(0);
}
