
#ifndef UDP_H
#define UDP_H

int udp_init(int is_listner=0);
int udp_bc(char* data, int data_len);
int udp_rcv_bc(char*data, int max_data_len);

#endif