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

int main(int argc,char **argv)
{//process A
	args_check(argc,3);
	signal(SIGINT,final);

	shmid=shmget(1234,4096,IPC_CREAT|0600);
        p=(struct t *)shmat(shmid,NULL,0);
	int fdr=open(argv[1],O_RDONLY);
        if(-1==fdr)
	{
		printf("1.pipe open fail\n");
		system("pause");
		return -1;
	}
	int fdw=open(argv[2],O_WRONLY);
	if(-1==fdw)
	{
		 printf("2.pipe open fail\n");
                system("pause");

		return -1;
	}
        printf("I am process A\n");
	char buf[128];
	fd_set readset;
	int ret;
	while(1)
	{
		FD_ZERO(&readset);
		FD_SET(STDIN_FILENO,&readset);
		FD_SET(fdr,&readset);
		ret=select(fdr+1,&readset,NULL,NULL,NULL);
                if(ret<0)
		{
			printf("select() error");
			system("pause");
			return -1;
		}
		if(FD_ISSET(STDIN_FILENO,&readset))
		{
			memset(buf,0,sizeof(buf));
			read(STDIN_FILENO,buf,sizeof(buf));
			write(fdw,buf,strlen(buf)-1);
			strcpy(p->buf3,buf);
                        //printf("from A: %s\n",buf);
		}

		if(FD_ISSET(fdr,&readset))
		{
			memset(buf,0,sizeof(buf));
			read(fdr,buf,sizeof(buf));//the contend of f2 read to buf
			printf("from B:%s\n",buf);
		        strcpy(p->buf3,buf);
			//printf("from B: %s\n",buf);
		}
		if(strcmp(buf,"bye")==0)
		{
			break;
		}
	}
	exit(0);
}
