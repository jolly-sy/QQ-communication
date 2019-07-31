#include "func.h"

struct msgbuf
{
	long mtype;
	char mtext[128];
};
int msgid;

void final(int signum)
{
	
	msgctl(msgid,IPC_RMID,NULL);
	printf("No.%d signal is coming, msg will be deleted!\n",signum);
	exit(0);
}

int main()
{//process B1
	signal(SIGINT,final);
         
	msgid=msgget(1000,IPC_CREAT|0600);
        struct msgbuf buf1;
	while(1)
	{

	memset(&buf1,0,sizeof(buf1));
	msgrcv(msgid,&buf1,sizeof(buf1.mtext),0,0);
	printf("%s\n",buf1.mtext);
	
	}
	exit(0);
}
