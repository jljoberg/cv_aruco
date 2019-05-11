
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstdlib>
#include <stdio.h>
#include <string.h> //memcpy

#include <iostream> //min

#define UDP_BC_PORT 12001
#define MAX_UDP_LEN 60000
static int sock;
static int addr_len;
static struct sockaddr_in addr_bc;

void error(const char* msg)
{
    perror(msg);
    exit(0);
}

int udp_init(int is_listner=0)
{
    if( (sock=socket(AF_INET, SOCK_DGRAM, 0)) < 0  ) error("Socket init error");
    addr_len = sizeof(addr_bc);

    addr_bc.sin_family = AF_INET;
    // addr_bc.sin_addr.s_addr = INADDR_ANY;
    // addr_bc.sin_addr.s_addr = INADDR_BROADCAST;
    inet_pton(AF_INET, "129.241.154.255", &(addr_bc.sin_addr));
    // inet_pton(AF_INET, "127.0.0.1", &(addr_bc.sin_addr));
    addr_bc.sin_port = htons(UDP_BC_PORT);

    if(setsockopt(sock,SOL_SOCKET,SO_BROADCAST,&sock,sizeof(sock)) < 0) error("Opt BC");

    if(is_listner)
    {
        if(  bind(sock, (struct sockaddr*)&addr_bc, addr_len)  <0) error("bind");
    }
    return 1;
}

int udp_bc(char* data, int data_len)
{
    char tmp;
    int n;
    int i=0;
    for(int seg=0; seg<data_len; seg+=MAX_UDP_LEN-1)
    {
        int pck_size = std::min(MAX_UDP_LEN, data_len-seg);
        tmp = data[seg+pck_size];
        data[seg+pck_size-1] = (char)i;
        n = sendto(sock,&(data[seg]),pck_size,0,(struct sockaddr *)&addr_bc,addr_len);
        data[seg+pck_size] = tmp;
        i++;
        printf("Sending seg %d\n", seg);
        if(n<0) printf("==== ERROR SENDING ====");
    }
    printf("Sending %d bytes\ti frames: %d\n", n, i);
    return n;
}

int udp_rcv_bc(char*data, int max_data_len)
{
    int n_bytes_rcvd;
    for(int i=0; i<21; i++)
    {
        char buf[MAX_UDP_LEN];
        if( (n_bytes_rcvd = recvfrom(sock,buf,MAX_UDP_LEN, 0, NULL,NULL) )<0) continue;
        printf("Rcvd %d bytes\n", n_bytes_rcvd);
        int seg = (int)buf[n_bytes_rcvd-1]*(MAX_UDP_LEN-1);
        printf("Seqment: %d\n", (int)buf[n_bytes_rcvd-1]) ;

        if(seg+n_bytes_rcvd > max_data_len ){
            printf("==== SEGMENT OUT OF BOUND === seq+n = %d\n", seg+n_bytes_rcvd);
            continue;
        }
        memcpy(&data[seg], buf, n_bytes_rcvd-1);
    }
}
