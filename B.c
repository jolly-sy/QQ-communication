#include "func.h"

int msgid;

void final(int signum)
{
	msgctl(msgid,IPC_RMID,NULL);
	printf("No.%d signal is coming, shm will be deleted!\n",signum);
	exit(0);
}

struct msgbuf
{
	long mtype;
	char mtext[128];
};//消息队列的通信内容都输到这个数组里

int main(int argc,char **argv)
{//process B
	args_check(argc,3);
	signal(SIGINT,final);

	msgid=msgget(1000,IPC_CREAT|0600);//创建消息队列
        struct msgbuf buf2;
	int fdw=open(argv[1],O_WRONLY);
	int fdr=open(argv[2],O_RDONLY);
	
        printf("I am process B\n");
	char buf[128];
        int ret,ret1;
	fd_set readset;
	
	while(1)
	{
		FD_ZERO(&readset);
		FD_SET(STDIN_FILENO,&readset);
		FD_SET(fdr,&readset);
		select(fdr+1,&readset,NULL,NULL,NULL);

		if(FD_ISSET(STDIN_FILENO,&readset))
		{
			memset(buf,0,sizeof(buf));
			read(STDIN_FILENO,buf,sizeof(buf));
			write(fdw,buf,strlen(buf)-1);
			strcpy(buf2.mtext,buf);
			buf2.mtype=1;
			ret1=msgsnd(msgid,&buf2,sizeof(buf2.mtext),0);
			if(-1==ret1)
			{
				puts("send msg fail\n");
				system("pause");
				return -1;
			}
			/*if(!strncmp(buf2.mtext,"bye",3))
			{
				buf2.mtype=2;
                                msgsnd(msgid,&buf2,sizeof(buf2.mtext),0);
				break;
			}*/
                        memset(buf2.mtext,0,sizeof(buf2.mtext));
		}

		if(FD_ISSET(fdr,&readset))
		{
			memset(buf,0,sizeof(buf));
			read(fdr,buf,sizeof(buf));//the contend of f2 read to buf
	
			printf("from A: %s\n",buf);
			strcpy(buf2.mtext,buf);
                        buf2.mtype=1;
                        ret1=msgsnd(msgid,&buf2,sizeof(buf2.mtext),0);
                        if(-1==ret1)
                        {
                                puts("send msg fail\n");
                                system("pause");
                                return -1;
                        }
                        
                        memset(buf2.mtext,0,sizeof(buf2.mtext));

		}
		if(strcmp(buf,"bye")==0)
		{
			break;
		}
	}
	exit(0);
}
