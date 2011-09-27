/*++

Copyright (c) 1994-1995  Microsoft Corporation

Module Name: prperf.c


Abstract: This Sample Windows Socket Application demonstrates use of
          the AppleTalk PAP Protocol. Both a server and a client
          application are included in this source file. The client
          app reads a file and sends the contents to the server. The
          server writes out the received bytes to another file.


--*/

#include <stdlib.h>
#include <string.h>
#include <windef.h>
#include <windows.h>
#include <winbase.h>
#include <winsock.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

//
// the following header contains appletalk specific definitions.
// should be included for AppleTalk Windows Socket Applications
//
#include <atalkwsh.h>
#include "prsamp.h"

#define SOCKET_COUNT                    32

#define DEF_SERVER_NAME                 "Windows Adsp"
#define DEF_SERVER_TYPE                 "Windows Sockets"
#define DEF_SERVER_ZONE                 "*"
#define BLOCKSIZE                       4096

#define DEF_QUEUE_LEN                   5
#define ENTITY_LEN                      32

CHAR    gServerName[ENTITY_LEN]         = DEF_SERVER_NAME;
CHAR    gServerType[ENTITY_LEN]         = DEF_SERVER_TYPE;
CHAR    gServerZone[ENTITY_LEN]         = DEF_SERVER_ZONE;
CHAR    gProtocolName[10]               = "ADSP";
INT     NumTimes                        = 5;
CHAR    gFileName[256];

LPSTR   pServerName   = gServerName;
LPSTR   pServerType   = gServerType;
LPSTR   pServerZone   = gServerZone;
LPSTR   pProtocolName = gProtocolName;
LPSTR   pFileName     = gFileName;


HANDLE  gFileHandle;
WSADATA WsaData;

SYSTEMTIME    SystemStartTime;
SYSTEMTIME    SystemEndTime;
FILETIME      FileTime;

BOOL fFileSpecified = FALSE;
BOOL fVerify = FALSE;
BOOL fRandom = FALSE;
BOOL fRDM    = FALSE;

int loop = 1, looptot;
double Throughput, AvgThroughPut, MinThroughPut, MaxThroughPut;

CHAR DataString[] = {"0123456789abcdefghijklmnopqrstuvwxyz~!@#$%^&*()_+"};

#define NANOPOWER    1000000000

//#define DEBUG  

typedef struct Sock_Info {
   int     sock_typ;
   int     prototyp;
   SOCKET  nsock;
} *PTEST_INFO, test_info;

_cdecl main (IN SHORT argc, IN PSZ argv[])
{

   register char p;
   register char *ptr;
   int i, r;
   BOOL fClient = FALSE;
   BOOL fServer = FALSE;

   for(i = 1; i < argc; i++)
   {
       p = argv[i][0];

       if (p == '/' || p == '-')          // option string
       {
           p = argv[i][1];

           switch(tolower(p))
           {
               case 'n':
                   ptr = argv[i]+2;
                   if(*ptr == '\0')
                       usage();
                   while(*pServerName++ = *ptr++);
                   break;
               case 't':
                   ptr = argv[i]+2;
                   if(*ptr == '\0')
                       usage();
                   while(*pServerType++ = *ptr++);
                   break;
               case 'z':
                   ptr = argv[i]+2;
                   if(*ptr == '\0')
                       usage();
                   while(*pServerZone++ = *ptr++);
                   break;
               case 's':
                   ptr = argv[i]+2;
                   if(*ptr != '\0')
                       usage();
                   if(fClient == TRUE)
                       usage();
                   fServer = TRUE;
                   break;
               case 'c':
                   ptr = argv[i]+2;
                   if(*ptr != '\0')
                       usage();
                   if(fServer == TRUE)
                       usage();
                   fClient = TRUE;
                   break;
               case 'p':
                   ptr = argv[i]+2;
                   if(*ptr == '\0')
                       usage();
                   while(*pProtocolName++ = *ptr++);
                   break;
               case 'b':
                   ptr=argv[i]+2;
                   if(*ptr == '\0')
                       usage();
                   NumTimes = atoi(ptr);
                   break;
               case 'f':
                   ptr = argv[i]+2;
                   if(*ptr == '\0')
                       usage();
                   while(*pFileName++ = *ptr++);
                   fFileSpecified = TRUE;
                   break;
               case 'l':
                   ptr = argv[i]+2;
                   if(*ptr == '\0')
                       usage();
                   loop = atoi(ptr);
                   break;
               case 'v':
                   ptr = argv[i]+2;
                   if(*ptr != '\0')
                       usage();
                   fVerify = TRUE;
                   break;
               case 'r':
                   ptr = argv[i]+2;
                   if(*ptr != '\0')
                       usage();
                   fRandom = TRUE;
                   break;
               case 'y':
                   ptr = argv[i]+2;
                   if(*ptr != '\0')
                       usage();
                   fRDM = TRUE;
                   break;
               case '?':
               default:
                   usage();

           }
       }
       else
       {
           usage();
       }

   }

#ifdef DEBUG
   printf("Server Name %s\n", gServerName);
   printf("Server Type %s\n", gServerType);
   printf("Server Zone %s\n", gServerZone);
#endif
   printf("%s throughput measurement\n", gProtocolName);

   if(stricmp(gProtocolName, "ADSP") && stricmp(gProtocolName, "PAP"))
   {
       printf("Invalid Protocol Specified\n");
       exit(1);
   }


   if(!(fClient || fServer))
       usage();

   r = WSAStartup(0x0101, &WsaData);
   if (r == SOCKET_ERROR)
   {
       printf("Startup failed!\n");
       WSACleanup();
       return(0);
   }

   if(fClient)
   {
       BOOL DClnt;

       looptot = 0;
       MaxThroughPut = 0;
       AvgThroughPut = 0;
       if(fFileSpecified)
           printf("Ignoring -f option for client.\n");
       while (looptot < loop)
       {
           DClnt = DoClient();
           if(DClnt)
           {
               if ((looptot == 0) || (MinThroughPut > Throughput))
                   MinThroughPut = Throughput;

               if (MaxThroughPut < Throughput)
                   MaxThroughPut = Throughput;

               AvgThroughPut = ((AvgThroughPut * looptot) + Throughput)
                               / (looptot+1);
               printf("Loop %2d/%d Throughput Cur/Min/Max/Ave :", ++looptot, loop);
               printf(" %6.2f/%6.2f/%6.2f/%6.2f\n\n\n", Throughput,
                                                        MinThroughPut,
                                                        MaxThroughPut,
                                                        AvgThroughPut);
               Throughput = 0.0;
           }
           else
               break;
       }
   }

   if(fServer)
       DoServer();

   WSACleanup();
   return(0);
}


/******************************************************************************
 
 Function - DoServer()

 Calling Function - main();

 Comments - This section of code calls the Server side of the application.
            
******************************************************************************/


BOOL DoServer()
{
   BOOL   fRet = FALSE;

   do
   {

//
// register the name specified on the command line  or use the
// defaults.
//

       fRet = OpenSocketRegisterAndListen();


   }while(FALSE);

   return(fRet);

}


/******************************************************************************
 
 Function - OpenSocketRegisterAndListen()

 Calling Function - DoServer();

 Comments - This section of code controls the Server side of the application.
            
******************************************************************************/

BOOL OpenSocketRegisterAndListen()
{

   int                  r = 0;         // return from socket calls
   int                  pass_no = 1;
   BOOL                 fRet = TRUE;
   SOCKET               sock, newsock; // SOCKET descriptor
   SOCKADDR_AT          atalkaddr;     // See atalkwsh.h
   WSH_REGISTER_NAME    nbpname;       // structure for registering NBP name
                                       // see atalkwsh.h
   fd_set               readfds;       // fd_set strutture for select call
   int                  addrlen;
   PTEST_INFO           test;
   int                  sockettype = SOCK_STREAM;
   int                  protocoltype = ATPROTO_ADSP;
   int                  NumBytesRecvd = 0;
   unsigned long        ThreadId;





//
// open a appletalk socket. The protocol family should be AF_APPLETALK,
// the socket type can be SOCK_STREAM or SOCK_RDM, and the ProtocolType
// for PAP socket must be ATPROTO_PAP. Note that opening a socket does
// not yet create an endpoint on the AppleTalk Protocol. A bind must
// happen before this socket can be used with AppleTalk;
//

   do
   {
       if(!stricmp(gProtocolName, "PAP"))
       {
           sockettype = SOCK_RDM;
           protocoltype = ATPROTO_PAP;
       }
       else if (fRDM)
           sockettype = SOCK_RDM;


       sock = socket(AF_APPLETALK, sockettype, protocoltype);

       if(sock == INVALID_SOCKET)
       {
           printf("Open Socket: Error = %ld\n", WSAGetLastError());
           r = -1;
           break;
       }

       atalkaddr.sat_socket = 0;
       atalkaddr.sat_family = AF_APPLETALK;

       r = bind(sock, (struct sockaddr *)&atalkaddr, sizeof(SOCKADDR_AT));

       if(r < 0)
       {
           printf("Bind:Error = %d\n", WSAGetLastError());
           break;
       }

       //
       // end point has now been created on the AppleTalk Protocol
       // now register the name that the client will look up.
       //

       strcpy(nbpname.ObjectName, gServerName);
       nbpname.ObjectNameLen = strlen(gServerName);

       strcpy(nbpname.TypeName, gServerType);
       nbpname.TypeNameLen = strlen(gServerType);

       strcpy(nbpname.ZoneName, "*");
       nbpname.ZoneNameLen = strlen("*");

       printf("Registering %s:%s@%s\n", nbpname.ObjectName,
                                        nbpname.TypeName,
                                        nbpname.ZoneName);


       r = setsockopt(sock,                         // socket descriptor
                     SOL_APPLETALK,                 // socket level - always SOL_APPLETALK
                     SO_REGISTER_NAME,              // socket option
                     (char *)&nbpname,              // nbp name structure
                     sizeof(WSH_NBP_NAME));         // size of nbp name structure


       if(r < 0)
       {
           printf("Register Name: Error = %d\n", WSAGetLastError());
           break;
       }

   }while(FALSE);

   if(r == SOCKET_ERROR)
   {
       closesocket(sock);
       return(FALSE);
   }

//
// Post a listen on this socket. The default queue length is 5
//

   r =  listen(sock, DEF_QUEUE_LEN);

   if(r < 0)
   {
       printf("Listen:Error = %d\n", WSAGetLastError());
       return FALSE;
   }


//
// listen is successful. select the socket for reading
//

   do
   {

       FD_ZERO(&readfds);            // clear set
       FD_SET(sock, &readfds);       // add sock to set

//
// this is a blocking select. Select will complete when
// a client connects.
//

       r = select(0, &readfds, NULL, NULL, NULL);

       if(r == SOCKET_ERROR)
       {
           printf("Select: Error = %d\n", WSAGetLastError());
           fRet = FALSE;
           break;
       }

       if ( r == 0)
       {
           printf("Select: no sockets available\n");
           fRet = FALSE;
           break;
       }

//
// Accept an incoming request.
//

       addrlen = sizeof(SOCKADDR_AT);
       newsock = accept(sock, (struct sockaddr *)&atalkaddr, &addrlen);

       if(newsock == INVALID_SOCKET)
       {
           printf("Accept: Socket Error = %d\n", WSAGetLastError());

           fRet = FALSE;
           break;
       }

#ifdef DEBUG
//       printf("Accept Succeeded\n");
#endif


       if(r == SOCKET_ERROR)
       {
           fRet = FALSE;
           break;
       }


       test = (PTEST_INFO) LocalAlloc(LPTR, sizeof(test_info));  
       test->sock_typ = sockettype;
       test->prototyp = protocoltype;
       test->nsock = newsock;
         
       if( ! CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PerfThread,
                            (PTEST_INFO) test, 0, & ThreadId ) )
       {
           fprintf(stderr,"CreateThread NULL 0 PerfThread %d 0 %d 0 %lx",
                        test->sock_typ, test->prototyp, (unsigned long) & ThreadId);
           exit(1);
        }

       if (fRet == FALSE)
           break;

   }while (TRUE);


   closesocket(sock);

   //
   // send all done
   //

   return(TRUE);

}


/******************************************************************************
 
 Function - PerfThread()

 Calling Function - OpenSocketRegisterAndListen();

 Comments - This section of code controls the Server side Recieve and Verification
            for the application.
            
******************************************************************************/


unsigned long PerfThread( PTEST_INFO lpvTest )
{
   int     r;                          // return from socket calls
   int     recvflags = 0;              // see WSARecvEx call
   CHAR    recvbuf[4096];              // recv buffer
   long    blockmode = 1;              // see ioctlsocket call
   BOOL    fRet = TRUE;
   int     NumBytesRecvd = 0, pass_no = 1;
   SOCKET  newsock = lpvTest->nsock;   // SOCKET descriptor
   int     sockettype = lpvTest->sock_typ;
   int     protocoltype = lpvTest->prototyp;

   //
   // make socket non blocking. As far as possible, use non
   // blocking sockets to improve performance of app.
   //
   //

   r = ioctlsocket(newsock, FIONBIO, &blockmode);

   if( r == SOCKET_ERROR)
   {
       printf("ioctlsocket: error = %d\n", WSAGetLastError());
       fRet = FALSE;
       return(FALSE);
   }

   do
   {


       //
       // Prime a Read here. This will enable the receive to complete
       // This is PAP specific. For other AppleTalk protocols -
       // ADSP and DDP a recv can be posted directly
       //

       if(!stricmp(gProtocolName, "PAP"))
       {

           r = setsockopt(newsock,
                          SOL_APPLETALK,
                          SO_PAP_PRIME_READ,
                          recvbuf,
                          sizeof(recvbuf));

           if( r < 0)
           {
               printf("primeread: error = %d\n", WSAGetLastError());
               fRet = FALSE;
               break;

           }
       }

       fRet = CheckForRecv(newsock);

       if(fRet == FALSE)
           break;

       r = WSARecvEx(newsock,recvbuf,sizeof(recvbuf), &recvflags);


       if ((sockettype == SOCK_RDM) && (r < sizeof(recvbuf)))
       {
           printf("Did not receive entire message\n");
           fRet = FALSE;
           break;
       }

       if( r == SOCKET_ERROR)
       {

           if (WSAGetLastError() == WSAEWOULDBLOCK)
               continue;
           else
           {
               if(WSAGetLastError() == WSAENOTCONN || WSAGetLastError() == WSAEFAULT)
                   break;
               printf("recv: error = %d\n", WSAGetLastError());
               fRet = FALSE;
               break;
           }

       }

#ifdef DEBUG
//       printf("Recvd %d bytes\n", r);
#endif

       if (fVerify)
       {
           int rr, pos = 0;


           do
           {
               fRet = CheckForSend(newsock);

               if (fRet == FALSE)
               {
                   printf("send select error\n");
                   break;
               }

               rr = send(newsock, &recvbuf[pos], r-pos, 0);   
               
               if ( rr < 0 )
               {
                   if(WSAGetLastError() == WSAEWOULDBLOCK)
                   {  
                       printf("Send: error = WSAEWOULDBLOCK\n");
                       continue;
                   }
                   else
                   {
                       printf("send: error = %d\n", WSAGetLastError());
                       break;
                   }

               }

#ifdef DEBUG
//	           printf("Bytes Sent %d\n", rr);
#endif

               pos += rr;

               if (pos >= r)
                   break;

           } while(TRUE);

       }  // endif Verify


       NumBytesRecvd += r;

       //
       // reset the recv flags for the next WSARecvEx
       //
       recvflags = 0;

   }while(TRUE);

   printf("Total Number of Bytes Received = %d \n",NumBytesRecvd);

   closesocket(newsock);
   LocalFree(lpvTest);

   return(TRUE);
}

/******************************************************************************
 
 Function - DoClient()

 Calling Function - main();

 Comments - This section of code controls the Client side of the application.
            
******************************************************************************/

BOOL DoClient()
{
   int                  r = 0;             // return code
   BOOL                 fRet = FALSE;      //
   int                  charcnt;           // count of bytes read from file
   CHAR                 LookupBuffer[512]; // LookUp results return buffer
   PCHAR                ptupleBuffer;
   PWSH_LOOKUP_NAME     pLookup;
   PWSH_NBP_TUPLE       pTempTuple;
   SOCKADDR_AT          ataddress;
   DWORD                WrittenSize;
   SOCKET               clntsock;
   long                 blockmode = 1;          // for ioctlsocket
   int                  BytesTransferred = 0;
   HLOCAL               memhandle;
   LPSTR                DataPtr, DataStartPtr, DataEndPtr;
   DWORD                hourdiff;          //, minutediff, seconddiff;
   double               StartSecond, EndSecond,ElapsedSeconds;
   int                  sockettype = SOCK_STREAM;
   int                  prototype = ATPROTO_ADSP;
   int                  i, rndbuf;

   if(!stricmp(gProtocolName, "PAP"))
   {
       sockettype = SOCK_RDM;
       prototype = ATPROTO_PAP;
   }
   else if (fRDM)
       sockettype = SOCK_RDM;

   if (fRandom)
       rndbuf = rand() % strlen(DataString) + 1;
   else
       rndbuf = strlen(DataString);

   do
   {
       //
       // See socket call in OpenSocketRegisterAndListen for desc
       //
       clntsock = socket(AF_APPLETALK, sockettype, prototype);

       if(clntsock == INVALID_SOCKET)
       {
           printf("Open Socket: Error = %ld\n", WSAGetLastError());
           r = -1;
           break;

       }

       ataddress.sat_socket = 0;
       ataddress.sat_family = AF_APPLETALK;


       r = bind(clntsock, (struct sockaddr *)&ataddress, sizeof(SOCKADDR_AT));

       if(r < 0)
       {
           printf("Bind:Error = %d\n", WSAGetLastError());
           break;
       }

       //
       // end point has now been created on the AppleTalk Protocol.
       // lookup the server name.
       //


       pLookup = (PWSH_LOOKUP_NAME)LookupBuffer;

       strcpy(pLookup->LookupTuple.NbpName.ObjectName, gServerName);
       pLookup->LookupTuple.NbpName.ObjectNameLen = strlen(gServerName);

       strcpy(pLookup->LookupTuple.NbpName.TypeName,  gServerType);
       pLookup->LookupTuple.NbpName.TypeNameLen = strlen(gServerType);

       strcpy(pLookup->LookupTuple.NbpName.ZoneName, gServerZone);
       pLookup->LookupTuple.NbpName.ZoneNameLen = strlen(gServerZone);

#ifdef DEBUG
       printf("Looking up %s:%s@%s\n",pLookup->LookupTuple.NbpName.ObjectName,
                                      pLookup->LookupTuple.NbpName.TypeName,
                                      pLookup->LookupTuple.NbpName.ZoneName);
#endif

       WrittenSize = sizeof(LookupBuffer);

       r = getsockopt(clntsock, SOL_APPLETALK, SO_LOOKUP_NAME,
                      (char*)LookupBuffer,
                      &WrittenSize);

       if(r != NO_ERROR)
       {
           printf("getsockopt:error = %d\n", WSAGetLastError());
           break;

       }

       if (pLookup->NoTuples <= 0)
       {
           printf("LookupName failed - no tuples found\n");
           r=-1;
           break;
       }

       ptupleBuffer = (char *)pLookup+sizeof(WSH_LOOKUP_NAME);
       pTempTuple = (PWSH_NBP_TUPLE)ptupleBuffer;

       ataddress.sat_family = AF_APPLETALK;
       ataddress.sat_net = pTempTuple[0].Address.Network;
       ataddress.sat_node = pTempTuple[0].Address.Node;
       ataddress.sat_socket = pTempTuple[0].Address.Socket;

#ifdef DEBUG
       printf("server address = %lx.%lx.%lx.\n", ataddress.sat_net,
                                                 ataddress.sat_node,
                                                 ataddress.sat_socket);
#endif


       //
       // lookup succeeded. Use the address in ataddress to connect to the
       // server
       //

       r =  connect(clntsock, (struct sockaddr *)&ataddress, sizeof(SOCKADDR_AT));

       if(r < 0)
       {
           printf("connect: error = %d\n", WSAGetLastError());
           break;
       }

#ifdef DEBUG
//       printf("Connect Succeeded\n");
#endif


   }while(FALSE);

   if(r < 0)
   {
       closesocket(clntsock);
       return(FALSE);
   }

   //
   //  Set Socket to non blocking mode
   //

   r = ioctlsocket(clntsock, FIONBIO, &blockmode);

#ifdef DEBUG
//       printf("ioctlsocket Succeeded\n");
#endif

   if( r == SOCKET_ERROR)
   {
       printf("ioctlsocket: error = %d\n", WSAGetLastError());
       return FALSE;
   }

   //
   // Fill Up Send Buffer with Data
   //
   memhandle = LocalAlloc(LPTR, BLOCKSIZE);
   if(memhandle == NULL)
   {
       printf("LocalAlloc Failed %d\n", GetLastError());
       return(FALSE);
   }
   DataPtr = LocalLock(memhandle);
   DataStartPtr = DataPtr;
   DataEndPtr = DataPtr + LocalSize(memhandle);

   //
   // fill the buffer with Data
   //
   while((DataStartPtr + rndbuf) <= DataEndPtr)
   {
       memcpy(DataStartPtr, DataString,rndbuf);
       DataStartPtr+= rndbuf;
   }

   if(DataStartPtr != DataEndPtr)
       memcpy(DataStartPtr, DataString, DataEndPtr-DataStartPtr-1);

   *DataEndPtr = '\0';

   fRet = CheckForSend(clntsock);

   if(fRet == FALSE)
       return FALSE;

   GetLocalTime(&SystemStartTime);

#ifdef DEBUG
   printf("Data Size = %d\n", lstrlen(DataPtr));
//   printf("Base Data sent: %s\n",DataStartPtr);
   printf("Start Time:%d:%2d:%2d:%2d\t", SystemStartTime.wHour,
                                         SystemStartTime.wMinute,
                                         SystemStartTime.wSecond,
                                         SystemStartTime.wMilliseconds);
#endif

   for( i= 0; i < NumTimes; i++)
   {
       //
       // Can I send - CheckForSend calls select to find if we can send
       // without blocking
       //
       DataStartPtr = DataPtr;

       do
       {
           fRet = CheckForSend(clntsock);

           if(fRet == FALSE)
           {
               printf("send select error\n");
               break;
           }

           charcnt = DataEndPtr - DataStartPtr;

           if(charcnt == 0)
               break;

           r = send(clntsock, DataStartPtr, charcnt, 0);


           if ( r < 0 )
           {

               if(WSAGetLastError() == WSAEWOULDBLOCK)
               {
                   printf("send: wouldblock\n");
                   continue;
               }
               else
               {
                   printf("send: error = %d\n", WSAGetLastError());
                   fRet = FALSE;
                   break;
               }

           }
           else
           { 
           
#ifdef DEBUG
//               printf("Sent %d bytes\n",r);
#endif

               if ((fVerify) && (DataStartPtr + r == DataEndPtr))
               {
                   CHAR buffer[4096];
                   int recvflag = 0;
                   int rr, pos = 0;

           //
           // Prime a Read here. This will enable the receive to complete
           // This is PAP specific. For other AppleTalk protocols -
           // ADSP and DDP a recv can be posted directly
           //

                   if(!stricmp(gProtocolName, "PAP"))
                   {

                       rr = setsockopt(clntsock,
                                      SOL_APPLETALK,
                                      SO_PAP_PRIME_READ,
                                      buffer,
                                      sizeof(buffer));

                       if( rr < 0)
                       {
                           printf("primeread: error = %d\n", WSAGetLastError());
                           fRet = FALSE;
                           break;
                       }
                   }

                   do
                   {

                       fRet = CheckForRecv(clntsock);

                       if(fRet == FALSE)
                           break;

                       rr = WSARecvEx(clntsock, &buffer[pos], sizeof(buffer)-pos, &recvflag);

                       if( rr == SOCKET_ERROR)
                       {
                           if (WSAGetLastError() == WSAEWOULDBLOCK)
                               continue;
                           else
                           {
                               printf("recv: error = %d\n", WSAGetLastError());
                               fRet = FALSE;
                               break;
                           }
                       }

                       pos += rr;

#ifdef DEBUG
//                       printf("Recv'd %d bytes\n",rr);
#endif

                       if ((recvflag == 0) || (sockettype == SOCK_RDM) || 
                           (pos == sizeof(buffer)))
                           break;
                       recvflag = 0;

                   } while(TRUE);


                   if ((memcmp(DataPtr, buffer, 4096) != 0) && (fRet != FALSE))
                   {
                       printf("Strings do not compare\n");
#ifdef DEBUG
//                       printf("Data sent: \n%s\n",DataPtr);
//                       printf("Data recv'd: \n%s\n",buffer);
#endif

                   }


               } // IF (fVerify)

               BytesTransferred += r;
               DataStartPtr += r;

               if(DataStartPtr == DataEndPtr)
                   break;

           } // IF send() PASSED

       }while(TRUE);

       if(fRet == FALSE)
           break;

   } // FOR

   GetLocalTime(&SystemEndTime);

#ifdef DEBUG
   printf("End Time:%d:%2d:%2d:%2d\t", SystemEndTime.wHour,
                                       SystemEndTime.wMinute,
                                       SystemEndTime.wSecond,
                                       SystemEndTime.wMilliseconds);
#endif

   //
   // calculate the difference
   //

   hourdiff = SystemEndTime.wHour - SystemStartTime.wHour;

   StartSecond = (SystemStartTime.wHour * 3600) +
                 (SystemStartTime.wMinute * 60) +
                 SystemStartTime.wSecond +
                 (SystemStartTime.wMilliseconds * 0.001);

   EndSecond = 0;

   if(SystemEndTime.wMonth != SystemStartTime.wMonth)
   {
       EndSecond = (SystemEndTime.wDay * 24) * 3600;
       switch (SystemStartTime.wMonth)
       {
           case 1:
           case 3:
           case 5:
           case 7:
           case 8:
           case 10:
           case 12:
                   EndSecond = ((31 - SystemStartTime.wDay) * 24) + EndSecond;
                   break;
           case 4:
           case 6:
           case 9:
           case 11:
                   EndSecond = ((30 - SystemStartTime.wDay) * 24) + EndSecond;
                   break;
           case 2:
                   if((SystemStartTime.wYear % 400 == 0) || 
                      ((SystemStartTime.wYear % 4 == 0) &&
                       (SystemStartTime.wYear % 100 != 0)))
                       EndSecond = ((29 - SystemStartTime.wDay) *
                                     24) + EndSecond;
                   else
                       EndSecond = ((28 - SystemStartTime.wDay) *
                                     24) + EndSecond;
                   break;
       }
   }
   else
       if(SystemEndTime.wDay > SystemStartTime.wDay)
       {
           EndSecond = ((SystemEndTime.wDay - SystemStartTime.wDay) * 24) * 3600;
       }

   EndSecond = EndSecond + (SystemEndTime.wHour * 3600) +
               (SystemEndTime.wMinute * 60) + SystemEndTime.wSecond +
               (SystemEndTime.wMilliseconds * 0.001);

   ElapsedSeconds = EndSecond-StartSecond;

#ifdef DEBUG
   printf("Elapsed Time (secs) = %6.3f\n", ElapsedSeconds);
   printf("Bytes Transferred = %ld\n", BytesTransferred);
   printf("Send All Done\n");
#endif


   if(ElapsedSeconds !=0)
   {
       Throughput = (BytesTransferred)/(ElapsedSeconds) ;

#ifdef DEBUG
       printf("Throughput (bytes/sec) = %6.2f\n", Throughput);
#endif

   }

   printf("\n");

   closesocket(clntsock);

   LocalFree(memhandle);
   return(TRUE);

}  



BOOL CheckForSend(SOCKET s)
{

   fd_set writefds;
   int r;


#ifdef DEBUG
   printf("\t\tChecking for Send..\n");
#endif

   FD_ZERO(&writefds);
   FD_SET(s, &writefds);

   //
   // indefinite wait select
   //
   r =  select(0, NULL, &writefds, NULL, NULL);

   if( r != 1)
   {
       printf("select:error = %d\n", WSAGetLastError());
       return(FALSE);
   }
   return(TRUE);
}
  
  
  
BOOL CheckForRecv(SOCKET s)
{

   fd_set readfds;
   int r;

   FD_ZERO(&readfds);
   FD_SET(s, &readfds);

#ifdef DEBUG
   printf("\t\tChecking for Recv..\n");
#endif

   r =  select(0, &readfds, NULL, NULL, NULL);

   if( r != 1)
   {
       printf("select:error = %d\n", WSAGetLastError());
       return(FALSE);
   }
   return(TRUE);
}

void usage()
{

   printf("prperf -s/c <options>\n");
   printf("\t-s/c\t Server/Client\n");
   printf("\t-n\t Server Name\n");
   printf("\t-t\t Server Type \n");
   printf("\t-z\t Server Zone \n");
   printf("\t-p\t Protocol Name (ADSP/PAP)\n");
   printf("\t-f\t File Name for data to be saved (server only) \n");
   printf("\t-b\t Number of times to send a 4k buffer (Default = 5) \n");
   printf("\t-l\t Number of times to re-start prperf (Default = 1) (Client only)\n");
   printf("\t-v\t Verify Integraty of data transmission. \n");
   printf("\t-r\t Randomize the size of the base data string. \n");
   printf("\t-y\t Change ADSP to use SOCK_RDM instead of SOCK_STREAM. \n");
   printf("\n");
   exit(1);

}
