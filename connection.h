#ifndef CONNECTION_H
#define CONNECTION_H

#define USB_CONNECTION  (1)
#define LAN_CONNECTION  (2)

extern int connection_type;

int scope_connect(void);
int scope_disconnect(void);
int scope_send_message(char* data, int length);
int scope_get_bmp(void);

#endif
