#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <arpa/inet.h> 

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
	unsigned char is_last;
}Header;

//==================
// Udp packet & data 
//==================
typedef struct udp_pkt{
	Header header;
	char data[1024];
}Udp_pkt; 

//============
// Declaration
//============
int sockfd = 0;
Udp_pkt snd_pkt,rcv_pkt;
struct sockaddr_in info, client_info;
socklen_t len;
time_t t1, t2;

//=====================
// Simulate packet loss
//=====================
int isLoss(double prob)
{
    double thres = prob * RAND_MAX;
    
    if(prob >= 1)
        return 1;//loss
    return (rand() <= thres);//not loss
}

//==================================
// You should complete this cunction
//==================================
int recvFile(FILE *fd)
{
	printf("FILE_EXISTS\n");

	char* str;
	char fileName[30];

	//==================================================================
	// Split the command into "download" & "filename", just get filename
	//==================================================================
	str = strtok(snd_pkt.data, " \n");	//parse pkt data buffer and suparate string into download&file name
	str = strtok(NULL, " \n");			
	
	sprintf(fileName, "download_");
	strcat(fileName, str);
	
	//FILE *fd;
	fd = fopen(fileName, "wb");
	//printf("(%s@%d)", __FUNCTION__, __LINE__);
	printf("Receiving...\n");
	//char buffer[123431];
	int index=0;
	int receive_packet=0;
	//printf("(%s@%d)", __FUNCTION__, __LINE__);
	memset(snd_pkt.data, 0, sizeof(snd_pkt.data));
	while(1) 
	{
		//printf("(%s@%d)", __FUNCTION__, __LINE__);
		//=======================
		// Simulation packet loss
		//=======================
		if(isLoss(0.5))
		{
			Udp_pkt buf;
			//printf("(%s@%d)", __FUNCTION__, __LINE__);
			printf("\tOops! Packet loss!\n");
			recvfrom(sockfd, &buf, sizeof(Udp_pkt), 0, (struct sockaddr *) &client_info, &len);
			usleep(300000);
			buf.header.ack_num = 1000;
			sendto(sockfd, &buf, sizeof(Udp_pkt), 0, (struct sockaddr *) &client_info, sizeof(client_info));
			//break;
		}
		
		//==============================================
		// Actually receive packet and write into buffer
		//==============================================
		//printf("(%s@%d)", __FUNCTION__, __LINE__);
		int recv_len = recvfrom(sockfd, &rcv_pkt, sizeof(Udp_pkt), 0, (struct sockaddr *) &client_info, &len);
		//receive pkt from socket into pkt buffer, return len
        	if(recv_len > 0)
        	{
            		printf("\tReceived a packet seq_num = %d\n",  rcv_pkt.header.seq_num);
		    	if(rcv_pkt.header.is_last == 1)//if the last flag set -> copy data into buffer and break, else copy
		    	{
		        //memcpy(buffer + index, rcv_pkt.data, recv_len - sizeof(Header));
		        	index += recv_len - sizeof(Header);
		        	//break;
		    	}
		    	else
			{
			//memcpy(buffer + index, rcv_pkt.data, recv_len - sizeof(Header));
				index += recv_len - sizeof(Header);
			}
        	}
		else
		{
			//printf("(%s@%d)", __FUNCTION__, __LINE__);
		    printf("Error in recvfrom()");
		    //break;
		}
		
		
		//==============================================
		// Write buffer into file if is_last flag is set
		//==============================================
		if(rcv_pkt.header.is_last == 1)
		{
		    printf("Client received finished\n");
		    //nd_pkt.header.
		    //fwrite(buffer, 1, index, fd);//set ack num of snd to seq of rcv
		    fclose(fd);
		    //break;
		}


		//====================
		// Reply ack to server
		//====================
		snd_pkt.header.ack_num = rcv_pkt.header.seq_num;
		//printf("%d ", snd_pkt.header.ack_num);
		sendto(sockfd, &snd_pkt, sizeof(Udp_pkt), 0, (struct sockaddr *) &client_info, sizeof(client_info));
		//printf("(%s@%d)\n", __FUNCTION__, __LINE__);
		
		if(rcv_pkt.header.is_last == 1)
			break;




	}
	return 0;
}

int main(int argc, char *argv[])
{
	//==============
	// Create socket
	//==============
	//int sockfd = 0;
	sockfd = socket(AF_INET , SOCK_DGRAM , 0);
	if (sockfd == -1)
	{
		printf("Fail to create a socket.");
	}

	//==================
	// Input server info
	//==================
	//struct sockaddr_in info;
	bzero(&info,sizeof(info));
	info.sin_family = AF_INET;

	char* server_ip=(char *)malloc(sizeof(char)*30);
	int server_port;

	printf("give me an IP to send: ");
	scanf("%s",server_ip);
	printf("server's's port? ");
	scanf("%d",&server_port);

	//==================================
	// Just test how to convert the type
	//==================================	
	info.sin_addr.s_addr = inet_addr(server_ip);
	info.sin_port = htons(server_port);

	server_port=htons(info.sin_port);
	server_ip=inet_ntoa(info.sin_addr);
	//printf("server %s : %d\n", server_ip, server_port);


	//====================================
	// Create send packet & receive packet
	//====================================
	memset(snd_pkt.data, '\0', sizeof(snd_pkt.data));
	len = sizeof(info);

	printf("Waiting for a commands...\n");
	getchar();
	while(fgets(snd_pkt.data, 30, stdin)) 
	{
        
        // ================================
        // command "exit": close the client
        // ================================
		if(strncmp(snd_pkt.data, "exit", 4) == 0) 
		{
            break;
        // ==============================================================
        // command "download filename": download the file from the server
        // ==============================================================
        }
		else if(strncmp(snd_pkt.data, "download", 8) == 0) 
		{
			snd_pkt.header.seq_num = 0;
			snd_pkt.header.ack_num = 0;
			snd_pkt.header.is_last = 1;
			//We first set is_last to 1 so that server know its our first message.
			int numbytes;
			FILE* fd;
			//========================
			// Send filename to server
			//========================
			if ((numbytes = sendto(sockfd, &snd_pkt, sizeof(snd_pkt), 0,(struct sockaddr *)&info, len)) == -1) 
			{
				perror("error");
				return 0;
			}
			printf("client: sent %d bytes to %s\n", numbytes,inet_ntoa(info.sin_addr));
			//=========================================
			// Get server response if file exist or not
			//=========================================
			if ((numbytes = recvfrom(sockfd, &rcv_pkt, sizeof(rcv_pkt), 0, (struct sockaddr *)&info, (socklen_t *)&len)) == -1)
			{
				printf("recvfrom error\n");
				return 0;
			}	
			printf("client: receive %d bytes from %s\n", numbytes,inet_ntoa(info.sin_addr));
			//printf("%s", buf);
			
			//====================
			// File does not exist
			//====================
			if(strcmp(rcv_pkt.data,"FILE_NOT_EXISTS") == 0) 
			{
				printf("FILE_NOT_EXISTS\n");
			} 
			//==========================
			// File exists, receive file
			//==========================
			else if(strcmp(rcv_pkt.data,"FILE_EXISTS") == 0) 
			{
			    t1=time(NULL);
				recvFile(fd);
				t2=time(NULL);
				printf("Total cost %ld secs\n",t2-t1);
			}	
			
		}
		else 
		{
			printf("Illegal command\n");	
		}
        
        printf("Waiting for a commands...\n");
    }

}


