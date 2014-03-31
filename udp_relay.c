#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int to_sockaddr_in(char *ip, char *port, struct sockaddr_in *a) {
	a->sin_family = AF_INET;
	a->sin_port = htons(atoi(port));
	a->sin_addr.s_addr = inet_addr(ip);
	return 0;
}

int parse_address(char *s, struct sockaddr_in *a) {
	int ret = 0;
	char *c = strrchr(s,':');
	if (c) {
		*c = 0;
		ret = to_sockaddr_in(s,c+1,a);
		*c = ':';
	} else {
		ret = 1;
	}
	return ret;
}


int main(int argc, char **argv) {
	if ( argc < 3 ) {
		printf("usage: udp_relay <source> <dest1> [ <dest2> ... ]\n");
		exit(1);
	}

	struct sockaddr_in s;
	struct sockaddr_in d[10];
	int dcount = 0, i;

	if ( parse_address(argv[1],&s) ) {
		printf("error: could not resolve source address: %s\n",argv[1]);
		return 1;
	}

	for (i=2;i<argc&&dcount<10;i++) {
		if ( parse_address(argv[i],&d[dcount]) ) {
			printf("error: could not resolve destination address: %s\n",argv[i]);
			return 1;
		}
		dcount++;
	}

	int sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_IP);
	if ( sock < 0 ) {
		perror("socket()");
		return 1;
	}
	if ( bind(sock,(struct sockaddr*)&s,sizeof(s)) ) {
		perror("bind()");
		return 1;
	}


	char buffer[65536];
	while (1) {
		int n = recv(sock,buffer,sizeof(buffer),0);
		if ( n < 0 ) continue;
		for (i=0;i<dcount;i++) {
			sendto(sock,buffer,n,0,(struct sockaddr*)&d[i],sizeof(d[i]));
		}
	}

	return 0;	
}
