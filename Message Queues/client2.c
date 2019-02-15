//client 2 for sum of n numbers program

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<error.h>
#include<string.h>

struct message_text {
	long mtype;
	int a[2];
	int c_qid,sum;
}mess;

int main() {
	int s_qid,c_qid,b[2],sum_c2;
	key_t server_key;
	
	server_key = ftok("/bin/bash",1);
	if(server_key == -1) perror("ftok failed\n");	
	else  printf("server key successfully created : %d\n",server_key);
	s_qid = msgget(server_key,IPC_CREAT | 0660);
	if(s_qid == -1)
		perror("msgget failed");
	else
		printf("server message qid = %d\n",s_qid);
	
	c_qid = msgget(IPC_PRIVATE,0660);
	if (c_qid == -1)
		perror("msgget failed");
	else
		printf("client message qid = %d\n",c_qid);
	mess.mtype = 2;
	mess.c_qid = c_qid;
	
	printf("Sending Message from C2....\n");
	if(msgsnd(s_qid,&mess,sizeof(mess),0) == -1)  perror("message send failed\n");
	else  printf("pid = %d, Message on msg queue %d: %d mtype: %ld\n",getpid(),s_qid,mess.c_qid,mess.mtype);

	printf("Receiving Message from Server....\n");
	if(msgrcv(c_qid,&mess,sizeof(mess),2,0) == -1)  perror("message send failed\n");
	else  printf("pid = %d, Message received from msg queue %d: %d\n",getpid(),c_qid,mess.c_qid);
	
	b[0] = mess.a[0]; b[1] = mess.a[1]; sum_c2 = b[0]+b[1];
	printf("Number received by C1: %d , %d and sum = %d\n", b[0],b[1],sum_c2);
	
	mess.mtype = 2;
	mess.sum = sum_c2;

	printf("Sending Message from C2....\n");
	if(msgsnd(s_qid,&mess,sizeof(mess),0) == -1)  perror("message send failed\n");
	else  printf("pid = %d, sum value on msg queue %d: %d mtype: %ld\n",getpid(),s_qid,mess.sum,mess.mtype);

}
