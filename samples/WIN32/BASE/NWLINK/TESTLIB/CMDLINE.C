/****************************************************************************\
*  cmdline.c -- sample program library demonstrating NWLink.
*
*       Microsoft Developer Support
*       Copyright (c) 1992, 1993 Microsoft Corporation
*
*  Demonstrates basic sockets programming with the Windows Sockets API
*  using the NWLink transport.
* 
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "externs.h"
#include <wsipx.h>
#include "testlib.h"

/*
*   Global variables that can be set on the command line 
*/

int     verbose = 0;
int     Socket_Type = SOCK_DGRAM;
int     Protocol = NSPROTO_IPX;
int     Backlog = 1;
int     No_Broadcast = 0;
int     No_Loop = 0;
int     Sleep_Time = 250;
int     Send_Length = 1024;
int     Receive_Length = 1024;
int     Local_Packet_Type = 0;
int     Send_Packet_Type = 9;
int     Filter_Packet_Type = 0;
int     Local_Address_Family = AF_NS;
int     Remote_Address_Family = AF_NS;
char    Local_Network_Number[4]  = {0x00, 0x00, 0x00, 0x00};
char    Local_Node_Number[6]     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
char    Local_Socket_Number[2]   = {0x00, 0x00};
char    Remote_Network_Number[4] = {0x00, 0x00, 0x00, 0x00};
char    Remote_Node_Number[6]    = "\xFF\xFF\xFF\xFF\xFF\xFF";
char    Remote_Socket_Number[2]  = {0x12, 0x34};

/*
*   Function prototypes for this file 
*/

void  usage(LPSTR);
void  dump_defaults(LPSTR);
void  get_hex_string(LPSTR, LPSTR, int);
UCHAR get_hex_byte(char);

/****************************************************************************
*
*    FUNCTION:  parse_cmd_line( int argc, char **argv )
*
*    DESCRIPTION:  Routine used by all of the WinSock test programs for NWLink.
*  	      Parses the command line and sets the respective global variables. 
*
*    ARGUMENTS:	 char **     => array of command line arguments
*                int         number of command line arguments
*
*	 RETURNS:  nothing
*
*\***************************************************************************/
void parse_cmd_line(int argc, char **argv)
{
    LPSTR p;
    int num = 1;

    argc--;

    /*
    *   Parse each command line parameter 
    */

    while (num <= argc) {
        /*
        *   If option starts with '-' or '/', skip it 
        */

        p = strupr(argv[num++]);
        if (*p == '-' || *p == '/')
            p++;

        /*
        *   Help ? 
        */

        printf("***> command: %s\n:",p);
        if (*p == 'h' || *p == 'H' || *p == '?' || !stricmp(p, "help"))
	    usage(argv[0]);

        /*
        *   Verbose option? 
        */

        if (!stricmp(p, "v") || !stricmp(p, "verbose")) {
            verbose++;

            printf("verbose is ON\n");
            continue;
        }

        /*
        *   Display default values ? 
        */

        if (!strnicmp(p, "default", 7)) {
            p+= 7;

            dump_defaults(argv[0]);
        }

        /*
        *   Local address family ? 
        */

        if (!strnicmp(p, "laf:", 4)) {
            p+= 4;

            /*
            *   Get the address family from the option 
            */

            Local_Address_Family = atoi(p);

            if (verbose)
                printf("Local_Address_Family = %d\n", Local_Address_Family);
            continue;
        }

        /*
        *   Remote address family ? 
        */

        if (!strnicmp(p, "raf:", 4)) {
            p+= 4;

            /*
            *   Get the address family from the option 
            */

            Remote_Address_Family = atoi(p);

            if (verbose)
                printf("Remote_Address_Family = %d\n", Remote_Address_Family);
            continue;
        }

        /*
        *   Socket type ? 
        */

        if (!strnicmp(p, "st:", 3)) {
            p+= 3;

            /*
            *   Get the socket type from the option 
            */

            Socket_Type = atoi(p);
            if (verbose)
		printf("Socket_Type = %d\n", Socket_Type);
            continue;
        }
#if(0)
        /*
        *   Socket family ? 
        */

        if (!strnicmp(p, "sf:", 3)) {
            p+= 3;

            /*
            *   Get the socket family from the option 
            */

            Socket_Family = get_socket_family(p);
            continue;
        }
#endif
        /*
        *   Protocol ? 
        */

        if (!strnicmp(p, "proto:", 6)) {
            p+= 6;

            /*
            *   Get the protocol from the option 
            */

            Protocol = atoi(p);

            if (verbose)
        	printf("Protocol = %d\n", Protocol);
            continue;
        }

        /*
        *   Local network number ? 
        */

        if (!strnicmp(p, "lnet:", 5)) {
            p+= 5;

            /*
            *   Get the local network number from the option 
            */

            memcpy(Local_Network_Number, get_network_number(p), 4);
            if (verbose) {
                printf("Local_Network_Number = ");
                print_network_num(Local_Network_Number);
                printf("\n");
            }
            continue;
        }


        /*
        *   Local node number ? 
        */

        if (!strnicmp(p, "lnode:", 6)) {
            p+= 6;

            /*
            *   Get the local network number from the option 
            */

            memcpy(Local_Node_Number, get_node_number(p), 6);
            if (verbose) {
                printf("Local_Node_Number = ");
 		print_node_num(Local_Node_Number);
                printf("\n");
            }
            continue;
        }

        /*
        *   Remote network number ? 
        */

        if (!strnicmp(p, "rnet:", 5)) {
            p+= 5;

            /*
            *   Get the remote network number from the option 
            */

            memcpy(Remote_Network_Number, get_network_number(p), 4);
            if (verbose) {
                printf("Remote_Network_Number = ");
                print_network_num(Remote_Network_Number);
                printf("\n");
            }
            continue;
        }

        /*
        *   Remote node number ? 
        */

        if (!strnicmp(p, "rnode:", 6)) {
            p+= 6;

            /*
            *   Get the remote network number from the option 
            */

            memcpy(Remote_Node_Number, get_node_number(p), 6);
            if (verbose) {
                printf("Remote_Node_Number = ");
 		print_node_num(Remote_Node_Number);
                printf("\n");
            }
            continue;
        }

        /*
        *   Local socket number ? 
        */

        if (!strnicmp(p, "lsock:", 6)) {
            p+= 6;

            /*
            *   Get the local socket number from the command line 
            */

            memcpy(Local_Socket_Number, get_socket_number(p), 2);

	    if (verbose) {
                printf("Local_Socket_Number = ");
                print_socket_num(Local_Socket_Number);
                printf("\n");
            }
            continue;
        }

        /*
        *   Remote socket number ? 
        */

        if (!strnicmp(p, "rsock:", 6)) {
            p+= 6;

            /*
            *   Get the remote socket number from the command line 
            */

            memcpy(Remote_Socket_Number, get_socket_number(p), 2);

	    if (verbose) {
                printf("Remote_Socket_Number = ");
                print_socket_num(Remote_Socket_Number);
                printf("\n");
            }
            continue;
        }


        /*
        *   Send length ? 
        */

        if (!strnicmp(p, "sendlen:", 8)) {
            p+= 8;

            /*
            *   Get the amount of data to send from the command line 
            */

            Send_Length = atoi(p);
            if (verbose)
                printf("Send length = %d\n", Send_Length);

            continue;
        }

        /*
        *   Receive length ? 
        */

        if (!strnicmp(p, "recvlen:", 8)) {
            p+= 8;

            /*
            *   Get the amount of data to send from the command line 
            */

            Receive_Length = atoi(p);
            if (verbose)
                printf("Receive length = %d\n", Receive_Length);

            continue;
        }

        /*
        *   Send packet type ? 
        */

        if (!strnicmp(p, "sptype:", 7)) {
	    p+= 7;

            /*
            *   Get the packet type from the command line 
            */

            Send_Packet_Type = atoi(p);

  	    if (verbose)
		printf("Send_Packet_Type = %d\n", Send_Packet_Type);
            continue;
        }

        /*
        *   Local packet type ? 
        */

        if (!strnicmp(p, "lptype:", 7)) {
	    p+= 7;

            /*
            *   Get the packet type from the command line 
            */

            Local_Packet_Type = atoi(p);

  	    if (verbose)
		printf("Send_Packet_Type = %d\n", Send_Packet_Type);
            continue;
        }

        /*
        *   Filter packet type ? 
        */

        if (!strnicmp(p, "fptype:", 7)) {
	    p+= 7;

            /*
            *   Get the packet type from the command line 
            */

            Filter_Packet_Type = atoi(p);

  	    if (verbose)
		printf("Filter_Packet_Type = %d\n", Send_Packet_Type);
            continue;
        }

        /*
        *   Backlog size ? 
        */

        if (!strnicmp(p, "backlog:", 8)) {
 	    p+= 8;

            Backlog = atoi(p);

            if (verbose)
                printf("Backlog = %d\n", Backlog);
            continue;
        }

        /*
        *   No broadcast flag ? 
        */

        if (!strnicmp(p, "nobcast", 7)) {
            p+= 7;

            No_Broadcast++;

            if (verbose)
                printf("No broadcast flag is set\n");

            continue;
        }

        /*
        *   No loop flag ? 
        */

        if (!strnicmp(p, "noloop", 6)) {
            p+= 6;

            No_Loop++;

            if (verbose)
                printf("No loop flag is set\n");

            continue;
        }

        /*
        *   Sleep time ? 
        */

        if (!strnicmp(p, "sleep:", 6)) {
            p+= 6;

            Sleep_Time = atoi(p);

            if (verbose)
                printf("Sleep time = %d\n", Sleep_Time);

            continue;
        }


        if (*p)
            printf("Unknown command line parameter: %s\n", p);

    } /* while */

    if (verbose)
        printf("\n\n");
}

/****************************************************************************
*
*    FUNCTION:  usage( LPSTR progname )
*
*    PURPOSE:   Displays all allowable command line parameters.  (All may not be used
*   		    by the program linking to this library, however).
*
*    ARGUMENTS:	LPSTR	=> program name
*
*	 RETURNS:   exits with errorlevel 0
*
*\***************************************************************************/
void usage(LPSTR progname)
{
    printf("\n\nUsage: %s [options]\n", progname);
    printf("\n");
    printf("Valid options are:\n");
    printf("-v               Toggles verbose mode ON\n");
    printf("-laf:x           Sets local address family to x\n");
    printf("-raf:x           Sets remote address family to x\n");
    printf("-st:x            Sets socket type to x\n");
    printf("-proto:x         Sets protocol to x\n");
    printf("-sleep:x         Sets sleep time between sends to x milliseconds\n");
    printf("-backlog:x       Sets listen backlog size to x indications\n");
    printf("-nobcast         Sets flag so program won't set sockopt to broadcast\n");
    printf("-noloop          Sets flag so program won't loop\n");
    printf("-sptype:x        Sets send packet type to x\n");
    printf("-lptype:x        Sets local packet type to x\n");
    printf("-fptype:x        Sets filter for incoming packet type x\n");
    printf("-sendlen:x       Sets amount of data to send to x bytes\n");
    printf("-recvlen:x       Sets amount of data to receive to x bytes\n");
    printf("-lnet:xxxx       Sets local network number to xxxx (1)\n");
    printf("-lnode:xxxxxx    Sets local node number to xxxxxx (1)\n");
    printf("-rnet:xxxx       Sets remote network number to xxxx (1)\n");
    printf("-rnode:xxxxxx    Sets remote node number to xxxxxx (1)\n");
    printf("-rsock:xx        Sets remote socket number to xx (1)\n");
    printf("-default         Displays programs default values\n");
    printf("-?, -help, or -h Displays this screen\n");
    printf("\n");
    printf("Notes:  (1) Network numbers (network, node, and socket) must\n");
    printf("            be specified as a series of hexadecimal numbers,\n");
    printf("            for example: -lnet:04003BFF. \n");
    printf("\n");
    printf("        All values must come immediately after the ':'\n");
    printf("        -default and/or -v should be first\n");
    printf("        Not all programs make use of all the above parameters\n");

    exit(0);
}

/****************************************************************************
*
*    FUNCTION:  dump_defaults( LPSTR progname )
*
*    PURPOSE:   Prints out the current values of the command line options
*   		    
*
*    ARGUMENTS:	LPSTR	=> program name
*
*	 RETURNS:   nothing
*
*\***************************************************************************/
void dump_defaults(LPSTR progname)
{
    printf("Default option values for %s:\n\n", progname);

    printf("verbose =                %d\n", verbose);
    printf("socket type =            %d\n", Socket_Type);
    printf("protocol =               %d\n", Protocol);
    printf("sleep time =             %d\n", Sleep_Time);
    printf("backlog =                %d\n", Backlog);
    printf("nobcast =                %d\n", No_Broadcast);
    printf("noloop =                 %d\n", No_Loop);
    printf("send length =            %d\n", Send_Length);
    printf("receive length =         %d\n", Receive_Length);
    printf("send packet type =       %d\n", Send_Packet_Type);
    printf("local packet type =      %d\n", Local_Packet_Type);
    printf("filter packet type =     %d\n", Filter_Packet_Type);
    printf("local address family =   %d\n", Local_Address_Family);
    printf("remote address family =  %d\n", Remote_Address_Family);
    printf("local network number =   ");
    print_network_num(Local_Network_Number);
    printf("\nlocal node number =      ");
    print_node_num(Local_Node_Number);
    printf("\nlocal socket number =    ");
    print_socket_num(Local_Socket_Number);
    printf("\nremote network number =  ");
    print_network_num(Remote_Network_Number);
    printf("\nremote node number =     ");
    print_node_num(Remote_Node_Number);
    printf("\nremote socket number =   ");
    print_socket_num(Remote_Socket_Number);
    printf("\n\n");
}


/****************************************************************************
*
*    FUNCTION:  get_network_number( LPSTR cmd )
*
*    PURPOSE:   Reads a network number from the given string
*
*
*    ARGUMENTS:	LPSTR  => string to read from
*
*	 RETURNS:   LPSTR  => hex network number
*
*\***************************************************************************/
LPSTR get_network_number(LPSTR cmd)
{
    static char hex_num[4];

    memset(hex_num, 0, 4);

    if (strlen(cmd) != 8) {
        printf("Incorrect format for network number.\n");
        exit(1);
    }

    get_hex_string(cmd, hex_num, 4);

    return hex_num;
}

/****************************************************************************
*
*    FUNCTION:  get_node_number( LPSTR cmd )
*
*    PURPOSE:   Reads a node number from the given string
*
*
*    ARGUMENTS:	LPSTR  => string to read from
*
*	 RETURNS:   LPSTR  => hex network number
*
*\***************************************************************************/
LPSTR get_node_number(LPSTR cmd)
{
    static char hex_num[6];

    memset(hex_num, 0, 6);

    if (strlen(cmd) != 12) {
        printf("Incorrect format for node number.\n");
        exit(1);
    }

    get_hex_string(cmd, hex_num, 6);

    return hex_num;
}

/****************************************************************************
*
*    FUNCTION:  get_socket_number( LPSTR cmd )
*
*    PURPOSE:   Reads a socket number from the given string
*
*
*    ARGUMENTS:	LPSTR  => string to read from
*
*	 RETURNS:   LPSTR  => hex network number
*
*\***************************************************************************/
LPSTR get_socket_number(LPSTR cmd)
{
    static char hex_num[2];

    memset(hex_num, 0, 2);

    if (strlen(cmd) != 4) {
        printf("Incorrect format for socket number.\n");
        exit(1);
    }

    get_hex_string(cmd, hex_num, 2);

    return hex_num;
}

/****************************************************************************
*
*    FUNCTION:  get_hex_string( LPSTR src, LPSTR dest, int num )
*
*    PURPOSE:   Reads in a character string containing hex digits and converts
*				it to a hexadecimal number.
*
*    ARGUMENTS:	LPSTR  => source string
*				LPSTR  => destination for hex number
*				int    number of bytes to convert	
*
*	 RETURNS:   nothing
*
*\***************************************************************************/
void get_hex_string(LPSTR src, LPSTR dest, int num)
{
    LPSTR q = src;

    while (num--)
        *dest++ = (get_hex_byte(*q++) << 4) + get_hex_byte(*q++);

}

/****************************************************************************
*
*    FUNCTION:  get_hex_byte( char ch )
*
*    PURPOSE:   Converts the character passed in to a hexadecimal nibble.
*
*    ARGUMENTS:	char    character to convert
*
*	 RETURNS:   UCHAR   hex nibble
*
*\***************************************************************************/
UCHAR get_hex_byte(char ch)
{
    if (ch >= '0' && ch <= '9')
        return (ch - '0');

    if (ch >= 'A' && ch <= 'F')
        return ((ch - 'A') + 0x0A);

    printf("Illegal character %c in hex string\n", ch);
    exit(1);
}
