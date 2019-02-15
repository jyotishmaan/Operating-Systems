//server for sum of n numbers programs

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
	int c_qid, sum;
}mess;

int main() {
	int s_qid,c1_qid,c2_qid,sum_c1,sum_c2;
	key_t server_key;
	
	server_key = ftok("/bin/bash",1);
	if(server_key == -1) perror("ftok failed\n");	
	else  printf("server key successfully created : %d\n",server_key);

	s_qid = msgget(server_key,IPC_CREAT | 0660);
	if(s_qid == -1)
		perror("msgget failed");
	else
		printf("server message qid1 = %d\n",s_qid);
	
	printf("Receiving Message from Client 1....\n");
	if(msgrcv(s_qid,&mess,sizeof(mess),1,0) == -1)  perror("message send failed\n");
	else  printf("pid = %d, Message received from msg queue %d: %d\n",getpid(),s_qid,mess.c_qid);
	c1_qid = mess.c_qid;

	printf("Receiving Message from Client 2....\n");
	if(msgrcv(s_qid,&mess,sizeof(mess),2,0) == -1)  perror("message send failed\n");
	else  printf("pid = %d, Message received from msg queue %d: %d\n",getpid(),s_qid,mess.c_qid);
	c2_qid = mess.c_qid;
	
	mess.a[0] = 25; mess.a[1] = 35; mess.mtype = 1; mess.c_qid = c1_qid;
	printf("Sending Message to Client 1....\n");
	if(msgsnd(c1_qid,&mess,sizeof(mess),0) == -1)  perror("message send failed\n");
	else  printf("pid = %d, Message on msg queue %d, type: %ld\n",getpid(),c1_qid,mess.mtype);

		
	mess.a[0] = 45; mess.a[1] = 55; mess.mtype = 2; mess.c_qid = c2_qid;
	printf("Sending Message to Client 2....\n");
	if(msgsnd(c2_qid,&mess,sizeof(mess),0) == -1)  perror("message send failed\n");
	else  printf("pid = %d, Message on msg queue %d, type: %ld\n",getpid(),c2_qid,mess.mtype);

	printf("Receiving Message from Client 1....\n");
	if(msgrcv(s_qid,&mess,sizeof(mess),1,0) == -1)  perror("message send failed\n");
	else  printf("pid = %d, sum value received from msg queue %d: %d\n",getpid(),s_qid,mess.sum);
	sum_c1 = mess.sum;

	printf("Receiving Message from Client 2....\n");
	if(msgrcv(s_qid,&mess,sizeof(mess),2,0) == -1)  perror("message send failed\n");
	else  printf("pid = %d, sum value received from msg queue %d: %d\n",getpid(),s_qid,mess.sum);
	sum_c2 = mess.sum;

	printf("Sum values from C1: %d and from C2: %d, Total sum: %d\n", sum_c1,sum_c2,sum_c1+sum_c2);
	
}


