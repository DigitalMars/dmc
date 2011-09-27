/********************************************************************
MODULE: Net.H
AUTHOR: JohnMil
DATE:   7/3/92

This module contains the definitions required for network packets, including
specialized structures, network message numbers, and so on.
********************************************************************/

#ifndef _NET_H_INC_
#define _NET_H_INC_

#define NP_NEWPLAYER        1
#define NP_REQUESTIDENTITY  2
#define NP_MOVETO           3
#define NP_SHOTFIRED        4
#define NP_LEAVINGGAME      5
#define NP_HITCONFIRM       6
#define NP_SCORE            7
#define NP_INTONE           8

#define MAX_PACKET_SIZE                400
#ifndef MAX_COMPUTERNAME_LENGTH
#define MAX_COMPUTERNAME_LENGTH         32
#endif
#define MAX_USERNAME_LENGTH             32

                /* A network packet is all crunched together, leading directly
                   from the header on into data. We make cData the last element
                   of the header, then take a pointer to it, and viola!
                   beginning of message-specific information.
                 */
struct _s_NetPacketType {
  unsigned long ulSender;
  unsigned long ulDest;
  DWORD dwPacketType;
  char cData;           // We'll take a pointer to this for start of info.
  };
typedef struct _s_NetPacketType NetPacketType;


struct _s_PlayerInfoType {
  unsigned long ulID;
  DWORD dwPID;
  char cUserName[MAX_USERNAME_LENGTH];
  char cComputerName[MAX_COMPUTERNAME_LENGTH];
  int ix, iy;
  BYTE bFacing;
  int iScore;
  int iPicNum;
  int iGridNum;
  POINT pGridLoc;
  };

typedef struct _s_PlayerInfoType PlayerInfoType;


#endif
