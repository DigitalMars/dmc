/****************************************************************************\
*
*               Microsoft Developer Support
*               Copyright (c) 1992-1995 Microsoft Corporation
*
* MODULE:       winnet.c
*
* PURPOSE:      Demonstrate the WNetXXX Win32 api(s) (10 of the 11 - all but
*                 WNetConnectionDialog are demonstrated)
*
* TO RUN:       Ensure 1 or more network disk shares can be used by the
*                 machine/user-id you run winnet on, and that these 1 or more
*                 network disk shares aren't already connected to the
*                 machine/user-id you run winnet on.  These net shares must
*                 not require a password
*
*               IMPORTANT:  Ensure W: is not in use when you type "WINNET"
*
* OVERALL APPROACH: Find a connectable disk resource on the network, connect
*                 to it, and disconnect
*
*               The first time you run this sample, it's probably best to
*                 ensure there's a shared disk directory accessable to the
*                 machine on which you run this program, so you can see the
*                 success paths in this code exercised first.  Ensure this
*                 shared disk is NOT already connected
*
*               We recursively open containers returned by the EnumResources
*                 calls (recursively using OpenEnum), but will bail out of the
*                 recursion without fully exploring all containers and enum's
*                 if we hit the arbitrary limit placed on how many connectable
*                 disk resources we enter into our list.  Since there may
*                 possibly be many more enumerable resources on a net than
*                 there are connectable disk resources, we also place another
*                 arbitrary limit on the total number of resources we'll
*                 enumerate
*
*               Both limits can easily be removed or raised by anyone working
*                 with this sample code
*
*               After the container exploration is completed, we walk down the
*                 list of connectable disk resources, arbitrarily picking one
*                 to connect to and then disconnect from
*
* FUNCTIONS:  Open_Do_Close_1_Enum
*
*               Called by main with a NULL container to start the enumeration
*                 of the top-level container.  Called recursively from
*                 EnumResources
*
*             OpenEnum
*
*               Returns an enumeration handle by opening a container
*
*             EnumResources
*
*               Enumerates the connectable resources and containers, given an
*                 enumeration handle.  Recusively calls Open_Do_Close_1_Enum
*
*             AddGetGetCancel
*             AddGetGetCancel2
*
*               Identical logic in both functions adds a connection, gets the
*                 user for the connection, gets information about the
*                 connection and then cancels the connection.  The difference
*                 is that AddGetGetCancel2 uses the new Win32 forms of the Add
*                 and Cancel api's
*
*             PrintAPIError
*
*               Simple routine to print error message info
*
* GLOBAL VARS:
*             ConnectablesList
*
*               List of disk resources we can connect to.  Filled in while
*                 enumeration resources, walked after all enumeration is
*                 complete
*
*             dwResourcesEnumedSoFar
*
*               Counter of resources enumerated so far, includes count of both
*                 disk resources and containers enumerated.  If many
*                 containers are enumerated compared to disk resources, this
*                 counter will limit the search.  If many disk resources are
*                 found compared to containers, the size of ConnectablesList
*                 will limit the search
*
*             dwDiskListI
*
*               Index into ConnectablesList.  At all times, except when adding
*                 a list entry, dwDiskListI is 1 greater than the number of
*                 entries we have entered into ConnectablesList
*
\****************************************************************************/


/****************************************************************************\
*  INCLUDES, DEFINES, TYPEDEFS
\****************************************************************************/
#define STRICT
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winnetwk.h>

#define PERR(api) printf("%s: Error %d from %s on line %d\n",  \
    __FILE__, GetLastError(), api, __LINE__);
#define PMSG(msg) printf("%s line %d: %s\n",  \
    __FILE__, __LINE__, msg);

#define PrintAppStyleAPIError(ApiTxt,MsgTxt) {                 \
  PrintAPIError( #ApiTxt , #MsgTxt , __LINE__ , __FILE__ ); \
  }


/****************************************************************************\
*
* These next two search limits are completely tunable.  On one net
*   MAX_NUMBER_OF_RESOURCES_TO_ENUM had to be large to see 3 or 4 sharable disk
*   resources.  This could vary widely depending on the number of servers on
*   the net that have no shares up
*
\****************************************************************************/

#define MAX_NUMBER_OF_DISK_RESOURCES_TO_LIST 3
#define MAX_NUMBER_OF_RESOURCES_TO_ENUM      480000

#define LOCAL_DEVICE_NAME                    "W:"
#define WHICH_TO_USE_FROM_LIST               0

typedef struct _DISK_RESOURCES_CONNECTABLE
{
  LPTSTR lpRemoteName;
  LPTSTR lpComment;
  LPTSTR lpProvider;
} DISK_RESOURCES_CONNECTABLE, *LPDISK_RESOURCES_CONNECTABLE;

/****************************************************************************\
* GLOBAL VARIABLES
\****************************************************************************/

DISK_RESOURCES_CONNECTABLE
               ConnectablesList[MAX_NUMBER_OF_DISK_RESOURCES_TO_LIST];
DWORD    dwResourcesEnumedSoFar = 0;
DWORD    dwDiskListI            = 0;

/****************************************************************************\
* FUNCTION PROTOTYPES
\****************************************************************************/

VOID   Open_Do_Close_1_Enum(LPNETRESOURCE lpNetContainerToOpen);
HANDLE OpenEnum            (LPNETRESOURCE lpNetContainerToOpen);
UINT   EnumResources(HANDLE hNetEnum);
VOID   AddGetGetCancel(VOID);
VOID   AddGetGetCancel2(VOID);
VOID   PrintAPIError(LPTSTR ApiTxt,
                     LPTSTR MsgTxt,
                     UINT   Line,
                     LPTSTR File);

/****************************************************************************\
*
* FUNCTION: Main
*
\****************************************************************************/

UINT main(UINT argc, char *argv[])
{
    UNREFERENCED_PARAMETER(argv[1]);

    // check if Win32s, if so, display notice and terminate
    if( GetVersion() & 0x80000000 )
    {
        MessageBox(NULL,
                   "This application cannot run on Windows 3.1.\n"
                   "This application will now terminate.",
                   "WinNet",
                   MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
        return( 1 );
    }

    if (argc > 1)
    {
        printf("\nTo run type 'winnet', with no parameters\n");
        printf("\n  IMPORTANT: Ensure W: is not in use when you type WINNET\n");
        return(1);
    }

    /**************************************************************************\
    *
    * Pass NULL as container argument to get top level container.  This call
    *   starts the recursive exploration of the top level network resource
    *   container
    *
    \**************************************************************************/

    Open_Do_Close_1_Enum(NULL);

    /**************************************************************************\
    *
    * We now have a list of connectable resources in our table, but some error
    *   conditions may have caused the list to be 0 length
    *
    \**************************************************************************/

    if (0<dwDiskListI)
    { 
        AddGetGetCancel();
        AddGetGetCancel2();
    }
    else
    { 
        printf("\nError: enumerated 0 connectable disk resources!\n");
    }

    /**************************************************************************\
    *
    * Free the list of connectable resources, zero length or not
    *
    \**************************************************************************/

    { 
        DWORD i;
        for (i=0; i<dwDiskListI; i++)
        {
            free(ConnectablesList[i].lpRemoteName);
            free(ConnectablesList[i].lpComment   );
            free(ConnectablesList[i].lpProvider  );
        }
    }

    return(0);
}


/****************************************************************************\
*
* FUNCTION: Open_Do_Close_1_Enum
*
* COMMENTS: There's little reason this is in a separate function in this
*             sample other than the code is used in two places, so putting it
*             in a separate function saves repetition in the source code
*
\****************************************************************************/

VOID Open_Do_Close_1_Enum(LPNETRESOURCE lpNetContainerToOpen)
{
    HANDLE hNetEnum;

    hNetEnum = OpenEnum(lpNetContainerToOpen);

    if (!hNetEnum)
        return;

    if (!EnumResources(hNetEnum))
        return;

    if (NO_ERROR != WNetCloseEnum(hNetEnum))
    { 
        PERR("WNetCloseEnum");
        return;
    }
}

/****************************************************************************\
*
* FUNCTION: OpenEnum
*
\****************************************************************************/

HANDLE OpenEnum(LPNETRESOURCE lpNetContainerToOpen)
{
    HANDLE hNetEnum;

    if (NO_ERROR != WNetOpenEnum(RESOURCE_GLOBALNET,
                                 RESOURCETYPE_DISK,
                                 (RESOURCEUSAGE_CONNECTABLE |
                                 RESOURCEUSAGE_CONTAINER),
                                 lpNetContainerToOpen,
                                 (LPHANDLE)&hNetEnum))
    { 
        if (GetLastError() != NO_ERROR)
        { 
            PrintAppStyleAPIError(WNetOpenEnum,"OpenEnum");
            return(NULL);
        }
    }

    return(hNetEnum);
}

/****************************************************************************\
*
* FUNCTION: EnumResources
*
\****************************************************************************/

UINT EnumResources(HANDLE hNetEnum)
{
    #define                    RESOURCE_BUF_ENTRIES 5000
    NETRESOURCE ResourceBuffer[RESOURCE_BUF_ENTRIES];
    DWORD       dwSzResourceBuf;
    DWORD       dwEntriesToGet;

    /**************************************************************************\
    *
    * It's possible that not all entries will fit in a RESOURCE_BUF_ENTRIES slot
    *   buffer, so we call WNetEnumResource in a loop, asking for
    *   RESOURCE_BUF_ENTRIES at a time, and continuing to call WNetEnumResource
    *   until it returns no more entries
    *
    \**************************************************************************/

    for (;;)
    {
        dwSzResourceBuf = sizeof(ResourceBuffer);
        dwEntriesToGet  = RESOURCE_BUF_ENTRIES;

        printf("Getting %d entries\n",dwEntriesToGet);

        if (NO_ERROR != WNetEnumResource(hNetEnum,
                                         (LPDWORD)&dwEntriesToGet,
                                         (LPVOID)ResourceBuffer,
                                         (LPDWORD)&dwSzResourceBuf))
        { 
            switch (GetLastError())
            { 
                case NO_ERROR:
                    // Drop out of the switch, walk the buffer
                    break;
        
                case ERROR_NO_MORE_ITEMS :
                    // Return with 0 code because this only happens when we got
                    //   RESOURCE_BUF_ENTRIES entries on the previous call to
                    //   WNetEnumResource, and there were coincidentally exactly
                    //   RESOURCE_BUF_ENTRIES entries total in the enum at the time of
                    //   that previous call
                    return(0);

                default                  :
                    PrintAppStyleAPIError(WNetEnumResource,"EnumResources");
                    return(1);
            }
        }

        /************************************************************************\
        *
        * Walk the returned buffer.  Now dwEntriesToGet has been set by the api
        *   call to be the number of entries actually returned
        *
        \************************************************************************/

        {
            DWORD i;

            printf("Got %d entries\n",dwEntriesToGet);

            for (i=0; i<dwEntriesToGet; i++)
            {
                printf("\nResourceBuffer[%d].dwScope      = 0x%x \n",i,
                        ResourceBuffer[i].dwScope);
                printf("ResourceBuffer[%d].dwType       = 0x%x \n",i,
                        ResourceBuffer[i].dwType);
                printf("ResourceBuffer[%d].dwUsage      = 0x%x \n",i,
                        ResourceBuffer[i].dwUsage);
                printf("ResourceBuffer[%d].lpLocalName  = %s   \n",i,
                        ResourceBuffer[i].lpLocalName);
                printf("ResourceBuffer[%d].lpRemoteName = %s   \n",i,
                        ResourceBuffer[i].lpRemoteName);
                printf("ResourceBuffer[%d].lpComment    = %s   \n",i,
                        ResourceBuffer[i].lpComment);
                printf("ResourceBuffer[%d].lpProvider   = %s   \n",i,
                        ResourceBuffer[i].lpProvider);

                dwResourcesEnumedSoFar += dwEntriesToGet;

                printf("dwResourcesEnumedSoFar = %d \n",dwResourcesEnumedSoFar);

                if (dwDiskListI >= MAX_NUMBER_OF_DISK_RESOURCES_TO_LIST)
                    return(0);  // List full, stop enumerating

                switch (ResourceBuffer[i].dwUsage)
                { 
                    case RESOURCEUSAGE_CONNECTABLE :
                        ConnectablesList[dwDiskListI].lpRemoteName =
                            malloc(1+strlen(ResourceBuffer[i].lpRemoteName));
                        ConnectablesList[dwDiskListI].lpComment    =
                            malloc(1+strlen(ResourceBuffer[i].lpComment   ));
                        ConnectablesList[dwDiskListI].lpProvider   =
                            malloc(1+strlen(ResourceBuffer[i].lpProvider  ));

                        if ((NULL == ConnectablesList[dwDiskListI].lpRemoteName) ||
                            (NULL == ConnectablesList[dwDiskListI].lpComment   ) ||
                            (NULL == ConnectablesList[dwDiskListI].lpProvider  ))
                            PERR("EnumResource - ran out of heap space");

                        strcpy(ConnectablesList[dwDiskListI].lpRemoteName,
                               ResourceBuffer[i].lpRemoteName);
                        strcpy(ConnectablesList[dwDiskListI].lpComment   ,
                               ResourceBuffer[i].lpComment   );
                        strcpy(ConnectablesList[dwDiskListI].lpProvider  ,
                               ResourceBuffer[i].lpProvider  );

                        dwDiskListI++;

                        break;

                    case  RESOURCEUSAGE_CONTAINER:      // Regular container
                    case (RESOURCEUSAGE_CONTAINER | RESOURCEUSAGE_RESERVED): 
                        //Top-level container

                        /****************************************************************\
                        *
                        * This next test is here to filter out servers we don't wish to
                        *   enumerate while running the sample.  A real app would be more
                        *   likely to look at all servers, but for running a sample, it's
                        *   worthwhile to think about which servers should actually be
                        *   enumerated, to avoid annoying people who have other servers on
                        *   the net they may not wish to be disturbed
                        *
                        * So, you may want to modify this test to filter out whatever
                        *   servers make sense to filter out in your environment
                        *
                        * The first clause shows how to include in the filter the top
                        *   level Windows NT network provider container, the second clause
                        *   shows how to include in the filter a domain name, and the
                        *   third and fourth clauses show how to include any servers whose
                        *   names start with particular characters
                        *
                        \****************************************************************/

                        if (!(  (0==_strnicmp("Microsoft Windows Network", ResourceBuffer[i].lpRemoteName,25))
                              ||(0==_strnicmp("DMAIN1",ResourceBuffer[i].lpRemoteName,6))
                              ||(0==_strnicmp("\\\\BLUMPR2",ResourceBuffer[i].lpRemoteName,9))
                              ||(0==_strnicmp("\\\\BLAMTR2",ResourceBuffer[i].lpRemoteName,9))))
                            break;

                        if (dwResourcesEnumedSoFar < MAX_NUMBER_OF_RESOURCES_TO_ENUM)
                            Open_Do_Close_1_Enum(&ResourceBuffer[i]);
                            break;

                    case RESOURCEUSAGE_RESERVED:   // Ignore these
                        break;

                    default:
                    { 
                        UCHAR ucMsgBuf[1500];

                        sprintf(ucMsgBuf,"WNetEnumResources bad .dwUsage value = 0x%X,\n",
                        ResourceBuffer[i].dwUsage);
                        PERR(ucMsgBuf);
                        return(1);
                    }
                }
            }
        }

        /************************************************************************\
        *
        * We have walked the returned buffer.  Since dwEntriesToGet has been set
        *   by the api call to be the number of entries actually returned, if how
        *   many were returned is less than what we asked for, return out of the
        *   loop because this enum is done
        *
        \************************************************************************/

        if (dwEntriesToGet < RESOURCE_BUF_ENTRIES)
            return(0);

    }

    // Infinite loop never exits this way.
}


/****************************************************************************\
*
* FUNCTION: AddGetGetCancel
*
* COMMENTS: Connects to first disk resource on our global list
*
\****************************************************************************/

VOID AddGetGetCancel(VOID)
{
    UCHAR ucLocalName[sizeof(LOCAL_DEVICE_NAME)+1] = LOCAL_DEVICE_NAME;
    UCHAR ucPassword[] = "Passw00";
    #define            SZ_BUF 1000
    UCHAR ucRemoteName[SZ_BUF];
    UCHAR ucUserName  [SZ_BUF];
    DWORD dwSzBuf    = SZ_BUF;

    if (NO_ERROR != WNetAddConnection(
                  ConnectablesList[WHICH_TO_USE_FROM_LIST].lpRemoteName,
                  (LPTSTR)ucPassword,(LPTSTR)ucLocalName))
    { 
        PrintAppStyleAPIError(WNetAddConnection,"AddGetGetCancel");
        return;
    }

    dwSzBuf = SZ_BUF;

    if (NO_ERROR != WNetGetConnection((LPTSTR)ucLocalName,
                                      (LPTSTR)ucRemoteName,(LPDWORD)&dwSzBuf))
    { 
        PrintAppStyleAPIError(WNetGetConnection,"AddGetGetCancel");
        return;
    }

    dwSzBuf = SZ_BUF;

    if (NO_ERROR != WNetGetUser((LPTSTR)ucLocalName,
                                (LPTSTR)ucUserName,(LPDWORD)&dwSzBuf))
    { 
        PrintAppStyleAPIError(WNetGetUser,"AddGetGetCancel");
        return;
    }

    printf("\nDisk resource AddGetGetCancel connected = [%s], User name logged"
           "on to that resource = [%s]\n", ucRemoteName,ucUserName);

    if (NO_ERROR != WNetCancelConnection((LPTSTR)ucLocalName,FALSE))
    { 
        PrintAppStyleAPIError(WNetCancelConnection,"AddGetGetCancel");
        return;
    }
}


/****************************************************************************\
*
* FUNCTION: AddGetGetCancel2
*
* COMMENTS: If we had chosen to keep our list as a list of NetResource data
*             structures, using the new form of AddConnection2 would have been
*             easier to code.  As it is, declaring and filling in the
*             NetResource takes a few more lines
*
\****************************************************************************/

VOID AddGetGetCancel2(VOID)
{
    UCHAR ucLocalName[sizeof(LOCAL_DEVICE_NAME)+1] = LOCAL_DEVICE_NAME;
    NETRESOURCE ToConnect;
    UCHAR ucPassword[] = "Passw00";
    #define            SZ_BUF 1000
    UCHAR ucRemoteName[SZ_BUF];
    UCHAR ucUserName  [SZ_BUF] = "AUserName";
    DWORD dwSzBuf    = SZ_BUF;

    ToConnect.lpRemoteName = ConnectablesList[WHICH_TO_USE_FROM_LIST].lpRemoteName;
    ToConnect.lpLocalName  = ucLocalName;
    ToConnect.lpProvider   = NULL;
    ToConnect.dwType       = RESOURCETYPE_DISK;

    if (NO_ERROR != WNetAddConnection2(&ToConnect,(LPTSTR)ucPassword,
                                       (LPTSTR)ucUserName,CONNECT_UPDATE_PROFILE))
    { 
        PrintAppStyleAPIError(WNetAddConnection2,"AddGetGetCancel2");
        return;
    }

    dwSzBuf = SZ_BUF;

    if (NO_ERROR != WNetGetConnection((LPTSTR)ucLocalName,
                                      (LPTSTR)ucRemoteName,(LPDWORD)&dwSzBuf))
    { 
        PrintAppStyleAPIError(WNetGetConnection,"AddGetGetCancel2");
        return;
    }

    dwSzBuf = SZ_BUF;

    if (NO_ERROR != WNetGetUser((LPTSTR)ucLocalName,
                                (LPTSTR)ucUserName,(LPDWORD)&dwSzBuf))
    { 
        PrintAppStyleAPIError(WNetGetUser,"AddGetGetCancel2");
        return;
    }

    printf("\nDisk resource AddGetGetCancel2 connected = [%s], "
           "User name logged on to that resource = [%s]\n",
           ucRemoteName,ucUserName);

    if (NO_ERROR != WNetCancelConnection2((LPTSTR)ucLocalName,
                                          CONNECT_UPDATE_PROFILE,FALSE))
    { 
        PrintAppStyleAPIError(WNetCancelConnection2,"AddGetGetCancel2");
        return;
    }
}


/****************************************************************************\
*
* FUNCTION: PrintAPIError
*
\****************************************************************************/

VOID   PrintAPIError(LPTSTR ApiTxt,
                     LPTSTR MsgTxt,
                     UINT   Line,
                     LPTSTR File)
{ 
    DWORD dwLastError;

    dwLastError = GetLastError();
    switch (dwLastError)
    { 
        case NO_ERROR:
            printf("\nNO_ERROR not expected (%s) line %d file %s",
                   MsgTxt,Line,File);
            break;

        case ERROR_EXTENDED_ERROR:
        { 
            DWORD dwErrorCode;

            #define SZ_ERROR_BUF 4000
            UCHAR ucErrorBuf[SZ_ERROR_BUF];
            #define SZ_PROVIDER_NAME_BUF   400
            UCHAR ucProviderNameBuf[SZ_PROVIDER_NAME_BUF];

            if (NO_ERROR != WNetGetLastError(&dwErrorCode,
                                             (LPTSTR)ucErrorBuf,
                                             (DWORD)SZ_ERROR_BUF,
                                             (LPTSTR)ucProviderNameBuf,
                                             (DWORD)SZ_PROVIDER_NAME_BUF))
            { 
                PERR("WNetGetLastError");
            }
            else
            {
                #define szMsgBuf SZ_ERROR_BUF * 2
                UCHAR ucMsgBuf[szMsgBuf];

                sprintf(ucMsgBuf, "WNetGetLastError code = %d, Msg=[%s],"
                        "Provider =[%s] line %d file %s", dwErrorCode,
                        ucErrorBuf, ucProviderNameBuf, Line, File);
                PERR(ucMsgBuf);
            }
        }
        break;

        case ERROR_NOT_SUPPORTED:
            printf("\nERROR_NOT_SUPPORTED (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_UNEXP_NET_ERR:
            printf("\nERROR_UNEXP_NET_ERR (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_MORE_DATA:
            printf("\nERROR_MORE_DATA (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;
    
        case ERROR_INVALID_ADDRESS:
            printf("\nERROR_INVALID_ADDRESS (%s) line %d file %s", MsgTxt,
                   Line, File);
            break;

        case ERROR_INVALID_PARAMETER:
            printf("\nERROR_INVALID_PARAMETER (%s) line %d file %s", MsgTxt,
                   Line, File);
            break;

        case ERROR_INVALID_PASSWORD:
            printf("\nERROR_INVALID_PASSWORD (%s) line %d file %s", MsgTxt,
                   Line, File);
            break;

        case ERROR_ACCESS_DENIED:
            printf("\nERROR_ACCESS_DENIED (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_BUSY:
            printf("\nERROR_BUSY (%s) line %d file %s", MsgTxt, Line, File);
            break;

        case ERROR_BAD_USERNAME:
            printf("\nERROR_BAD_USERNAME (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_NOT_ENOUGH_MEMORY:
            printf("\nERROR_NOT_ENOUGH_MEMORY (%s) line %d file %s", MsgTxt,
                   Line, File);
            break;

        case ERROR_NO_NETWORK:
            printf("\nERROR_NO_NETWORK (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_NOT_CONNECTED:
            printf("\nERROR_NOT_CONNECTED (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_OPEN_FILES:
            printf("\nERROR_OPEN_FILES (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_DEVICE_IN_USE:
            printf("\nERROR_DEVICE_IN_USE (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_BAD_NET_NAME:
            printf("\nERROR_BAD_NET_NAME (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_BAD_DEVICE:
            printf("\nERROR_BAD_DEVICE (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_ALREADY_ASSIGNED:
            printf("\nERROR_ALREADY_ASSIGNED (%s) line %d file %s", MsgTxt,
                   Line, File);
            break;

        case ERROR_GEN_FAILURE:
            printf("\nERROR_GEN_FAILURE (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_CONNECTION_UNAVAIL:
            printf("\nERROR_CONNECTION_UNAVAIL (%s) line %d file %s", MsgTxt,
                   Line, File);
            break;

        case ERROR_NO_NET_OR_BAD_PATH:
            printf("\nERROR_NO_NET_OR_BAD_PATH (%s) line %d file %s", MsgTxt,
                   Line, File);
            break;

        case ERROR_BAD_PROVIDER:
            printf("\nERROR_BAD_PROVIDER (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_CANNOT_OPEN_PROFILE:
            printf("\nERROR_CANNOT_OPEN_PROFILE (%s) line %d file %s", MsgTxt,
                   Line, File);
            break;

        case ERROR_BAD_PROFILE:
            printf("\nERROR_BAD_PROFILE (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_INVALID_HANDLE:
            printf("\nERROR_INVALID_HANDLE (%s) line %d file %s", MsgTxt,
                   Line, File);
            break;

        case ERROR_NO_MORE_ITEMS:
            printf("\nERROR_NO_MORE_ITEMS (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        case ERROR_NOT_CONTAINER:
            printf("\nERROR_NOT_CONTAINER (%s) line %d file %s", MsgTxt, Line,
                   File);
            break;

        default:
            printf("\n%s - unexpected return code=%d (%s) line %d file %s",
                   ApiTxt, dwLastError, MsgTxt, Line, File);
            break;
    }
}


