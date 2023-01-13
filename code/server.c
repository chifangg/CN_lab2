#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <arpa/inet.h> 
#include <sys/wait.h>
#include <math.h>
#include <pthread.h>
#define TIMEOUT 100

/*****************notice**********************
 * 
 * You can follow the comment inside the code.
 * This kind of comment is for basic part.
 * =============== 
 * Some hints...
 * ===============
 * 
 * This kind of comment is for bonus part.
 * ---------------
 * Some hints...
 * ---------------
 * 
 * 
 * 
 *********************************************/


//==============
// Packet header 
//==============
typedef struct header{
	unsigned int seq_num;
	unsigned int ack_num;
	unsigned char isLast;
}Header;

//==================
// Udp packet & data 
//==================
typedef struct udp_pkt{
	Header header;
	char data[1024];
}Udp_pkt;

//=============
// Declaration
//=============
int sockfd;
struct sockaddr_in info, client_info;
Udp_pkt snd_pkt,rcv_pkt;
socklen_t len;
pthread_t th1,th2;
int first_time_create_thread = 0;

//---------------------------------------
// Declare for critical section in bonus. 
//---------------------------------------
/*******************notice*******************************
 * 
 * If you dont need the bonus point, ignore this comment.
 * Use it like following block.
 * 
 * pthread_mutex_lock( &mutex );
 * ...
 * critical section
 * ...
 * pthread_mutex_unlock( &mutex );
 * 
 *********************************************************/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


//------------------------------
// Bonus part for timeout_thread
//------------------------------
/*******************notice***************************
 * 
 * In bonus part, you should use following threads to
 * checking timeout and receive client ack.
 * 
 ***************************************************/
void* receive_thread()
{
	//--------------------------------------
	// Checking timeout & Receive client ack
	//--------------------------------------
    	
	
	
	//------------------------------------------
	// Keep the thread alive not to umcomment it
	//------------------------------------------
	//pthread_exit(NULL);
}

//------------------------------
// Bonus part for timeout_thread
//------------------------------
void* timeout_thread()
{
	//------------------------------------------
	// Keep the thread alive not to umcomment it
	//------------------------------------------
	// pthread_exit(NULL);
}

//==================================
// You should complete this function
//==================================
// Send file function, it call receive_thread function at the first time.



int sendFile(FILE *fd)
{	
    int filesize = ftell(fd);
    int seq_num = 0;
    int bytes_sent;
    int num_bytes;
    int total_bytes_sent = 0;
    int last_ack = 0;
    struct timespec start, end;
	//----------------------------------------------------------------
	// Bonus part for declare timeout threads if you need bonus point,
	// umcomment it and manage the thread by youself
	//----------------------------------------------------------------
	// At the first time, we need to create thread.
	if(!first_time_create_thread)
	{
		first_time_create_thread=1;
		//pthread_create(&th1, NULL, receive_thread, NULL);
		//pthread_create(&th2, NULL, timeout_process, NULL);
	}
	/*******************notice************************
	 * 
	 * In basic part, you should finish this function.
	 * You can try test_clock.c for clock() usage.
	 * checking timeout and receive client ack.
	 * 
	 ************************************************/

	//==========================
	// Send video data to client
	//==========================
	//snd_pkt.header.seq_num = seq_num;
	//if (sendto(sockfd, &filesize, sizeof(filesize), 0, (struct sockaddr *)&client_info, len) < 0) 
	//{
		
		//perror("sendto failed");
		//return 1;
	//}
	
	fseek(fd, 0, SEEK_SET);
    	while (total_bytes_sent < filesize) 
	{
		clock_t t, t2, tf;
		num_bytes = fread(snd_pkt.data, 1, 1024, fd);
		//printf("%d\n", num_bytes);
		snd_pkt.header.seq_num = seq_num;
		//printf("%d\n", seq_num);
		//printf("%d %d %d %d\n", seq_num, total_bytes_sent, bytes_sent, filesize);
		snd_pkt.header.isLast = (total_bytes_sent + bytes_sent) >= filesize ? 1 : 0;
		//printf("%d\n", snd_pkt.header.isLast);
		
		
		t = clock();
		bytes_sent = sendto(sockfd, &snd_pkt, sizeof(snd_pkt), 0, (struct sockaddr *)&client_info, len);
		
		//clock_gettime(CLOCK_MONOTONIC, &start);
		printf("Send %d byte\n", bytes_sent);
		
		if (bytes_sent < 0) 
		{
		    perror("sendto failed");
		    return 1;
		    
        	}
        	


	
		//======================================
		// Checking timeout & Receive client ack
		//======================================	
		
		
		
		
		
		//int f;
		
		//printf ("Calculating...\n");
		//f = frequency_of_primes (999999);
		//printf ("The number of primes lower than 100,000 is: %d\n",f);
		//
		
		
		//printf ("It took me %ld milliseconds.\n",tf);
		//return 0;
		//last_ack = seq_num;
		
		int got = recvfrom(sockfd, &rcv_pkt, sizeof(rcv_pkt), 0, (struct sockaddr *)&client_info, &len);
		t2 = clock();
		tf = (t2-t) * 1000 / CLOCKS_PER_SEC;
		if(tf < 100 && got > 0 && rcv_pkt.header.ack_num != 1000)
		{
			//printf ("It took me %ld milliseconds.\n",tf);
			seq_num++;
        		total_bytes_sent += bytes_sent;
        		printf("Receive a packet ack_num = %d\n", rcv_pkt.header.ack_num);
		}
		else if(rcv_pkt.header.ack_num == 1000)
		{
			printf("Timeout! Resend packet!\n");
			//fseek(fd, total_bytes_sent, SEEK_SET);
			//sendto(sockfd, &snd_pkt, sizeof(snd_pkt), 0, (struct sockaddr *)&client_info, len);
		
		}
		got = 0;
		
		
		
		//clock_gettime(CLOCK_MONOTONIC, &end);
		//while ((end.tv_sec - start.tv_sec) > TIMEOUT) 
		//{
		//	printf("Timeout, resending last packet\n");
		//	fseek(fd, total_bytes_sent, SEEK_SET);
		//	seq_num = last_ack;
		//	clock_gettime(CLOCK_MONOTONIC, &start);
		//}
		
		//if (rcv_pkt.header.ack_num == seq_num) 
		//{
		//	last_ack = rcv_pkt.header.ack_num;
		//	clock_gettime(CLOCK_MONOTONIC, &start);
		//}
		
		
    }

	//=============================================
	// Set is_last flag for the last part of packet
	//=============================================


	printf("send file successfully\n");
	fclose(fd);
	return 0;
}

int main(int argc, char *argv[])
{
	//===========================
	// argv[1] is for server port
	//===========================
	printf("hello\n");
	sockfd = socket(AF_INET , SOCK_DGRAM , 0);

	if (sockfd == -1)
	{
		printf("Fail to create a socket.");
	}
	//=======================
	// input server info
	// IP address = 127.0.0.1
	//=======================
	bzero(&info,sizeof(info));
	info.sin_family = AF_INET;
	int port=atoi(argv[1]);
	info.sin_addr.s_addr = INADDR_ANY ;
	info.sin_port = htons(port);
	//printf("server %s : %d\n", inet_ntoa(info.sin_addr), htons(info.sin_port));

	//================
	// Bind the socket
	//================	
	if(bind(sockfd, (struct sockaddr *)&info, sizeof(info)) == -1)
	{
		perror("server_sockfd bind failed: ");
		return 0;
	}

	//====================================
	// Create send packet & receive packet
	//====================================
	memset(rcv_pkt.data, '\0', sizeof(rcv_pkt.data));

	//====================
	// Use for client info
	//====================
	bzero(&client_info,sizeof(client_info));
	client_info.sin_family = AF_INET;
	len = sizeof(client_info);

	printf("====Parameter====\n");
	printf("Server's IP is 127.0.0.1\n");
	printf("Server is listening on port %d\n",port);
	printf("==============\n");

	while(1)
	{
		//=========================
		// Initialization parameter
		//=========================
		snd_pkt.header.seq_num = 0;
		snd_pkt.header.ack_num = 0;
		snd_pkt.header.isLast = 0;
		FILE *fd;
		
		printf("server waiting.... \n");
		char *str;
		int ret;
		
		while ((ret = recvfrom(sockfd, &rcv_pkt, sizeof(rcv_pkt), 0, (struct sockaddr *)&client_info, (socklen_t *)&len)) != -1)
		{
			printf("%d\n", ret);
			//In client, we set is_last 1 to comfirm server get client's first message.
			if(rcv_pkt.header.isLast == 1)
				break;
		}  
		printf("process command.... \n");
		//printf("%s\n", rcv_pkt.data);
		str = strtok(rcv_pkt.data, " ");
		
		//===============================================================
		// command "download filename": download the file from the server
		// and then check if filename is exist
		//===============================================================
		if(strcmp(str, "download") == 0) 
		{
			str = strtok(NULL, " \n");
			printf("filename is %s\n",str);
			//===================
			// if file not exists 
			//===================
			if((fd = fopen(str, "rb")) == NULL) 
			{    
				//=======================================
				// Send FILE_NOT_EXISTS msg to the client
				//=======================================
				printf("FILE_NOT_EXISTS\n");
                		strcpy(snd_pkt.data, "FILE_NOT_EXISTS");
				int numbytes;
				if ((numbytes = sendto(sockfd, &snd_pkt, sizeof(snd_pkt), 0,(struct sockaddr *)&client_info, len)) == -1) 
				{
					printf("sendto error\n");
					return 0;
				}
				printf("server: sent %d bytes to %s\n", numbytes,inet_ntoa(client_info.sin_addr));
			} 
			//==================
			// else, file exists 
			//==================
			else 
			{
				fseek(fd,0,SEEK_END);
                		printf("FILE_EXISTS\n");
				strcpy(snd_pkt.data, "FILE_EXISTS");
				
				
				//==================================
				// Send FILE_EXIST msg to the client
				//==================================
				int numbytes;
				if ((numbytes = sendto(sockfd, &snd_pkt, sizeof(snd_pkt), 0,(struct sockaddr *)&client_info, len)) == -1) 
				{
					printf("sendto error\n");
					return 0;
				}
				printf("server: sent %d bytes to %s\n", numbytes,inet_ntoa(client_info.sin_addr));

				//==========================================================================
				// Sleep 1 seconds before transmitting data to make sure the client is ready
				//==========================================================================
				sleep(1);
				printf("trasmitting...\n");

				//=====================================
				// Start to send the file to the client
				//=====================================

				sendFile(fd);
				
			}
		}
		else 
		{
			printf("Illegal request!\n");   
		}
	}
}
