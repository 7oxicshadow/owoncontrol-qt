#ifndef LAN_INTERFACE_H
#define LAN_INTERFACE_H

int connect_lan(void);
int disconnect_lan(void);
int send_lan_data(char* data, int length);
int get_lan_data(char* data, int length, int * transferred);
int get_lan_bmp(void);

#endif
