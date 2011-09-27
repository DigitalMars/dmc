/*
*   Globals for test programs 
*/

extern int     verbose;    
extern int     Socket_Type;
extern int     Protocol;
extern int     Sleep_Time;
extern int     Backlog;
extern int     No_Broadcast;
extern int     No_Loop;
extern int     Send_Length;
extern int     Receive_Length;
extern int     Local_Packet_Type;
extern int     Send_Packet_Type;
extern int     Filter_Packet_Type;
extern int     Local_Address_Family;
extern int     Remote_Address_Family;
extern char    Local_Network_Number[];
extern char    Local_Node_Number[];
extern char    Local_Socket_Number[];
extern char    Remote_Network_Number[];
extern char    Remote_Node_Number[];
extern char    Remote_Socket_Number[];
 
/* 
*    Command line and network address print functions
*/

 /*   Parses command line and fills in above global variables  */
extern void parse_cmd_line(int, char **);

 /*   Prints full network address (network, node, and socket)  */
extern void print_netaddr(LPSTR, LPSTR, LPSTR);

 /*   Prints first 4 bytes of address passed in  */
extern void print_network_num(LPSTR);

 /*   Prints first 6 bytes of address passed in */
extern void print_node_num(LPSTR);

 /*   Prints first 2 bytes of address passed in */
extern void print_socket_num(LPSTR);
 
 /*   Prints contents of a SOCKADDR_IPX struct */
extern void print_saddr(PSOCKADDR_IPX);

 /*   Prints contents of a WSAData struct */
extern void print_wsa(LPWSADATA);


/* 
*    Error print functions 
*/

 /*   Returns string with passed in string + description of last h_errno */
extern LPCSTR dos_net_strerror(LPSTR);

 /* Prints given string followed by description of last h_errno to stdout */
extern void  dos_net_perror(LPSTR);

 /* Displays message box with given string followed by description of
 *   last h_errno.  Allows user to specify if messagebox should be 
 *   task modal (last param should be 1 if so)
 */
extern void win_net_perror(HWND, LPSTR, int);

 /* Given h_errno, returns descriptive text */
extern LPCSTR get_error_text(int);

 /* Makes sure that a buffer is filled with only the character specified. */
extern int mem_check(LPSTR, UCHAR, int);
