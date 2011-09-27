/***********************************************************************
File:   Network.c
Author: JohnMil
Date:   7-2-92


Abstract:

    This is the network communications module for MazeLords. It contains functions
    to implement mailslots, as well as the various protocols and so-on required
    for communications.


Contents:

    MemCRC() -- Generates a 'unique' LONG for different data structures
    SendNetMessage() -- Transmit a pre-made structure to other players
    MailWatchThread() -- Thread process to watch for new mail
    InitNetwork() -- Create mailslot, Send out ID message to network.
    ProcessNewMail() -- Process newly recieved data packets


Revision History:

10-30-92 (JohnMil):
    Modified to new personal comment format
************************************************************************/

#include <windows.h>
#include <mmsystem.h>
#include "winmaze.h"
#include "mazproto.h"
#include "net.h"
#include "crctable.h"



/*=====================================================================
Function:   MemCRC()

Inputs:     Buffer & buffer length to CRC

Outputs:    Returns single Long corresponding to CRC for structure

Abstract:
    The CRC should provide a unique ID for each player, provided ANY
    information in their data structures is unique. Each player is
    responsible for generating their ID & transmitting it to the other
    players.
======================================================================*/

unsigned long MemCRC(
    LPSTR buffer,
    int bytes
    )
{
    unsigned long crc = 0xFFFFFFFF;
    LPSTR p;

    for ( p = buffer; bytes > 0; bytes-- ) {
        crc = ( crc >> 8 ) ^ CRCtable[(unsigned char) ((unsigned char) crc ^ *p++ ) ];
        }

    return ( crc ^ 0xFFFFFFFF );
}


/*=====================================================================
Function:   SendNetMessage()

Inputs:     Source, destination, structure to send, packet type

Outputs:    Returns success/failure

Abstract:
    SendNetMessage will send a message to all mazelords mailslots in the
    current domain.  Note that this message is broadcast once. It is up
    to the individual recipients to decide whether it is addressed to them,
    or a general message (sent to -1).
======================================================================*/

BOOL SendNetMessage(
    unsigned long ulSender,
    unsigned long ulDest,
    LPVOID lpStruct,
    DWORD dwPacketType
    )
{
    BOOL bResult=TRUE;
    HANDLE hFile;
    DWORD dwScratch,dwLength;
    char cBuff[MAX_PACKET_SIZE];
    LPSTR lpStr;
    NetPacketType FAR *lpN;
    PlayerInfoType FAR *lppiInfo,FAR *lppiInfo2;
    PositionType FAR *lpptPos,FAR *lpptPos2;
    int FAR *iScore;
    IntoneType FAR *itIntone;

    if (!bNetworked) {
        return(TRUE);
        }

    lpN = (NetPacketType FAR *) cBuff;
    lpN->ulSender = (ulSender)? ulSender : ptSelf.ulID;
    lpN->ulDest = ulDest;
    lpN->dwPacketType = dwPacketType;
    dwLength = sizeof(NetPacketType);

    //
    // determine size, type of packet and fill out info accordingly.
    //
    lpStr = (LPSTR) &lpN->cData;

    switch(dwPacketType) {

        case NP_NEWPLAYER:
            lppiInfo = (PlayerInfoType FAR *) lpStr;
            lppiInfo2 = (PlayerInfoType FAR *) lpStruct;
            *lppiInfo = *lppiInfo2;
            dwLength += sizeof(PlayerInfoType);
            break;

        case NP_REQUESTIDENTITY:
        case NP_LEAVINGGAME:
        case NP_HITCONFIRM:
            break;

        case NP_MOVETO:
        case NP_SHOTFIRED:
            lpptPos = (PositionType FAR *) lpStr;
            lpptPos2 = (PositionType FAR *) lpStruct;
            *lpptPos = *lpptPos2;
            dwLength += sizeof(PositionType);
            break;

        case NP_SCORE:
            iScore = (int FAR *) lpStr;
            *iScore = ptSelf.iScore;
            dwLength += sizeof(int);
            break;

        case NP_INTONE:
            itIntone = (IntoneType FAR *) lpStr;
            *itIntone = *((IntoneType FAR *) lpStruct);
            dwLength += sizeof(IntoneType);
            dwLength -= (131 - itIntone->dwLen);
            break;

        default:
            MessageBox((HWND)NULL,"Unknown packet ID -- Unable to transmit","SendNetMessage",
                       MB_ICONEXCLAMATION|MB_APPLMODAL);
            break;
        }

#ifdef WIN32
    hFile = CreateFile("\\\\*\\mailslot\\mazemslt",
                     GENERIC_WRITE,
                     FILE_SHARE_READ,
                     NULL,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL,
                     NULL);

    bResult = (hFile != INVALID_HANDLE_VALUE);

    bResult = bResult &&
              WriteFile(hFile,cBuff,dwLength+1,&dwScratch,NULL);

    return(bResult && CloseHandle(hFile));
#else // else it's any operating system except WIN32
    return(TRUE);
#endif
}



/*=====================================================================
Function:   MailWatchThread()

Inputs:     Handle to mailslot to watch

Outputs:    none

Abstract:
    This is a thread which is spawned. As soon as it sees a mail message
    is queued up, it sends a message to the main window notifying Maze
    Lords that new mail has been received.  This is done 5 times a second.
======================================================================*/

void MailWatchThread(
    HANDLE hMailSlot
    )
{
    DWORD cbMaxMsg,cbNextMsg,cMsg,dwReadTimeout;
#ifdef WIN32
    while(TRUE) {

        Sleep(200);
        if (GetMailslotInfo(hMailSlot,&cbMaxMsg,&cbNextMsg,&cMsg,&dwReadTimeout)) {
            if (cMsg > 0) {
                PostMessage(hWndMain,WM_COMMAND,IDT_NEWMAIL,0);
                }
            }
        }
#endif

    //
    // We shouldn't ever get here.
    //
    return;
}



/*=====================================================================
Function:   InitNetwork()

Inputs:     none

Outputs:    returns success/failure

Abstract:
    InitNetwork takes care of creating a mailslot for this user, and
    sending out a packet saying that we're joining the game.
    FALSE indicates that there was a fatal error starting the network.
======================================================================*/

BOOL InitNetwork(
    void
    )
{
    DWORD dwScratch;
    BOOL bRet;
    PlayerInfoType ptPlayScr;

    bRet = TRUE;

    //
    // initialize player list
    //
    ptPlayers.next = (PlayerType FAR *)NULL;

    if (!bNetworked) {
        hMailSlot = NULL;
        return(TRUE);
        }

#ifdef WIN32
    hMailSlot = CreateMailslot("\\\\.\\mailslot\\mazemslt",0,0,NULL);
    if (hMailSlot == INVALID_HANDLE_VALUE) {
        MessageBox(NULL,"Unable to create MailSlot","Net Initialization",
                   MB_ICONEXCLAMATION|MB_APPLMODAL);
        bRet = FALSE;
        }

    if (! CreateThread((LPSECURITY_ATTRIBUTES) NULL,
                       0,
                       (LPTHREAD_START_ROUTINE)MailWatchThread,
                       (LPVOID)hMailSlot,
                       0,&dwScratch)) {
        MessageBox(NULL,"Unable to create Mail Watch Thread","Net Initialization",
                   MB_ICONEXCLAMATION|MB_APPLMODAL);
        bRet = FALSE;
        }
#endif
    ptPlayScr.ulID = ptSelf.ulID;
    ptPlayScr.dwPID = ptSelf.dwPID;
    lstrcpy(ptPlayScr.cUserName,ptSelf.cUserName);
    lstrcpy(ptPlayScr.cComputerName,ptSelf.cComputerName);
    ptPlayScr.ix = ptSelf.Pos.ix;
    ptPlayScr.iy = ptSelf.Pos.iy;
    ptPlayScr.bFacing = ptSelf.Pos.Facing;
    ptPlayScr.iScore = ptSelf.iScore;
    ptPlayScr.iPicNum = ptSelf.iPicNum;
    ptPlayScr.iGridNum = ptSelf.iGridNum;
    ptPlayScr.pGridLoc = ptSelf.pGridLoc;

    return(bRet&&SendNetMessage(0,0,(LPVOID) &ptPlayScr,NP_NEWPLAYER));
}


/*=====================================================================
Function:   ProcessNewMail()

Inputs:     none

Outputs:    none

Abstract:
    ProcessNewMail is responsible for updating character positions, asking
    for more information, etc.
======================================================================*/

void ProcessNewMail(
    void
    )
{
    DWORD cbMaxMsg,cbNextMsg,cMsg,dwReadTimeout;
    NetPacketType FAR *NPkt;
    PlayerInfoType FAR *piPlayerPacket,piScratch;
    PlayerType FAR *ptTrav;
    PositionType FAR *ptPosPacket;
    BOOL bFound,bDrawMessageSent;
    int ix,iy;
    BYTE bSquare1, bSquare2,Facing;
    char cBuff[257],cNetMessBuff[400];
    int FAR *iScore;
    IntoneType FAR *itIntone;

#ifdef WIN32
    GetMailslotInfo(hMailSlot,&cbMaxMsg,&cbNextMsg,&cMsg,&dwReadTimeout);
    bDrawMessageSent = FALSE;

    while (cMsg) {
        if (!ReadFile(hMailSlot,cNetMessBuff,cbNextMsg,&cbMaxMsg,(LPOVERLAPPED) NULL)) {
            MessageBox(NULL,"Error reading from mailslot","ProcessNewMail",
                       MB_ICONEXCLAMATION|MB_APPLMODAL);
            }

        NPkt = (NetPacketType FAR *) cNetMessBuff;

        //
        // Don't process the message if it's one of our own
        //
        if ((NPkt->ulSender != ptSelf.ulID)&&
            ((NPkt->ulDest == ptSelf.ulID)||(NPkt->ulDest == 0))
           ) {

            piPlayerPacket = (PlayerInfoType FAR *) &(NPkt->cData);
            ptPosPacket = (PositionType FAR *) &(NPkt->cData);
            iScore = (int FAR *) &(NPkt->cData);
            itIntone = (IntoneType FAR *) &(NPkt->cData);

            bFound = FALSE;
            ptTrav = &ptPlayers;

            while (ptTrav->next != NULL) {
                if (ptTrav->next->ulID == NPkt->ulSender) {
                    bFound = TRUE;
                    break;
                    }
                ptTrav = ptTrav->next;
                }

            if ((!bFound)&&(NPkt->dwPacketType != NP_NEWPLAYER)) {
                if(!SendNetMessage(0,NPkt->ulSender,NULL,NP_REQUESTIDENTITY)) {
                    MessageBox(NULL,"Unable to send Packet","ProcessNewMail",
                               MB_ICONEXCLAMATION|MB_APPLMODAL);
                    }
                }

            switch (NPkt->dwPacketType) {

                case (NP_NEWPLAYER):
                    if (!bFound) {
                        ptTrav->next = NewPlayer(piPlayerPacket->ulID,
                                     piPlayerPacket->dwPID,
                                     piPlayerPacket->cUserName,
                                     piPlayerPacket->cComputerName,
                                     piPlayerPacket->ix,
                                     piPlayerPacket->iy,
                                     piPlayerPacket->bFacing,
                                     piPlayerPacket->iScore,
                                     piPlayerPacket->iPicNum,
                                     piPlayerPacket->iGridNum,
                                     piPlayerPacket->pGridLoc.x,
                                     piPlayerPacket->pGridLoc.y,
                                     ptTrav->next);
                        if (!AddPic(piPlayerPacket->iPicNum)) {
                            ptTrav->next->iPicNum = PIC_DEFAULT;
                            sprintf(cBuff,"Unknown Pic #'%d' for %s: Using default.",
                                    piPlayerPacket->iPicNum,piPlayerPacket->cComputerName);
                            PrintTextLine(cBuff);
                            AddPic(PIC_DEFAULT);
                            }

                        //
                        // After we add the user to our own database,
                        // let him know we exist.
                        //
                        sprintf(cBuff,"%s has joined the game.",ptTrav->next->cUserName);
                        PrintTextLine(cBuff);

                        piScratch.ulID = ptSelf.ulID;
                        piScratch.dwPID = ptSelf.dwPID;
                        strcpy(piScratch.cUserName,ptSelf.cUserName);
                        strcpy(piScratch.cComputerName,ptSelf.cComputerName);
                        piScratch.ix = ptSelf.Pos.ix;
                        piScratch.iy = ptSelf.Pos.iy;
                        piScratch.bFacing = ptSelf.Pos.Facing;
                        piScratch.iScore = ptSelf.iScore;
                        piScratch.iPicNum = ptSelf.iPicNum;
                        piScratch.iGridNum = ptSelf.iGridNum;
                        piScratch.pGridLoc = ptSelf.pGridLoc;
                        if (!SendNetMessage(0,NPkt->ulSender,&piScratch,NP_NEWPLAYER)) {
                            MessageBox(NULL,"Unable to send Packet","ProcessNewMail",
                                             MB_ICONEXCLAMATION|MB_APPLMODAL);
                            }

                        //
                        // Finally, do a redraw to make sure if he's in
                        // line of sight we see him. */
                        //
                        PostMessage(hWndMaze,WM_COMMAND,IDM_DRAWPLAYERS,0);
                        PostMessage(hWndScore,WM_COMMAND,WC_ADDSCORE,ptTrav->next->ulID);
                        }
                    break;

                case (NP_REQUESTIDENTITY):
                    piScratch.ulID = ptSelf.ulID;
                    piScratch.dwPID = ptSelf.dwPID;
                    strcpy(piScratch.cUserName,ptSelf.cUserName);
                    strcpy(piScratch.cComputerName,ptSelf.cComputerName);
                    piScratch.ix = ptSelf.Pos.ix;
                    piScratch.iy = ptSelf.Pos.iy;
                    piScratch.bFacing = ptSelf.Pos.Facing;
                    piScratch.iScore = ptSelf.iScore;
                    piScratch.iPicNum = ptSelf.iPicNum;
                    piScratch.iGridNum = ptSelf.iGridNum;
                    piScratch.pGridLoc = ptSelf.pGridLoc;
                    if (!SendNetMessage(0,NPkt->ulSender,&piScratch,NP_NEWPLAYER)) {
                        MessageBox(NULL,"Unable to send Packet","ProcessNewMail",
                                             MB_ICONEXCLAMATION|MB_APPLMODAL);
                        }

                    break;

                case (NP_MOVETO):
                    if (bFound) {
                        ptTrav->next->Pos = *ptPosPacket;
                        if (ptTrav->next->Drawn) {
                            if (!bBitmapDraw) {
                                InvalidateRect(hWndMaze,&ptTrav->next->rDrawn,TRUE);
                                }
//
//BUGBUG -- drawing problem for bitmaps
//
                            ptTrav->next->Drawn=FALSE;
                            }
                        else {
                            if (!bDrawMessageSent) {
                                PostMessage(hWndMaze,WM_COMMAND,IDM_DRAWPLAYERS,0);
                                bDrawMessageSent = TRUE;
                                }
                            }
                        PostMessage(hWndScore,WM_COMMAND,WC_UPDATEDIRECTION,ptTrav->next->ulID);
                        }
                    break;

                case (NP_SHOTFIRED):
                    if (bFound) {

                        PlaySound("othrlasr.wav",NULL,SND_FILENAME|SND_ASYNC);

                        //
                        // if the shot was fired in the same 'x' or 'y'
                        // plane, we may have been hit!
                        //
                        if ((ptPosPacket->ix == ptSelf.Pos.ix)||
                            (ptPosPacket->iy == ptSelf.Pos.iy)
                           ) {

                            ix = ptPosPacket->ix;
                            iy = ptPosPacket->iy;
                            Facing = ptPosPacket->Facing;

                            bSquare2 = bMaze[ix][iy];

                            do {
                                //
                                // if we're hit, handle it!
                                //
                                if ((!InSanctuary(&ptSelf.Pos))&&(ptSelf.Pos.ix == ix)&&(ptSelf.Pos.iy == iy)) {
                                    if (!SendNetMessage(0,NPkt->ulSender,NULL,NP_HITCONFIRM)) {
                                        MessageBox(NULL,"Unable to send Packet","MazeWndProc",
                                             MB_ICONEXCLAMATION|MB_APPLMODAL);
                                        }
                                    sprintf(cBuff,"%s Zapped you!",ptTrav->next->cUserName);
                                    PrintTextLine(cBuff);
                                    KillSelf();
                                    break;
                                    }
                                //
                                // otherwise, keep checking.
                                //
                                bSquare1 = bSquare2;
                                bSquare2 = bMaze[ix = ADJ_X(ix,Facing)][iy = ADJ_Y(iy,Facing)];
                                } while (!((bSquare1 & Facing)||(bSquare2 & BACK_TO_ABS(Facing))));
                            }
                        }

                    break;

                case (NP_LEAVINGGAME):
//
// ***BUGBUG*** Might cause problems if character leaving is on screen
//
                    if (bFound) {
                        sprintf(cBuff,"%s has left the game.",ptTrav->next->cUserName);
                        PrintTextLine(cBuff);
                        PostMessage(hWndScore,WM_COMMAND,WC_DELETESCORE,ptTrav->next->ulID);
                        ptTrav->next = DeletePlayer(ptTrav->next);
                        PostMessage(hWndMaze,WM_COMMAND,IDM_DRAWPLAYERS,0);
                        }
                    break;

                case (NP_HITCONFIRM):
                    PlaySound("squish.wav",NULL,SND_FILENAME|SND_ASYNC);
                    if (bFound) {
                        FadePic(ptTrav->next->iPicNum,
                                ptTrav->next->Pos.Facing,
                                hMazeDC,
                                &ptTrav->next->rFrom,
                                &ptTrav->next->rDrawn);
                        sprintf(cBuff,"You hit Player %s!",ptTrav->next->cUserName);
                        PrintTextLine(cBuff);
                        }
                    ptSelf.iScore += 50;
                    iPlayersKilled++;

                    SendNetMessage(0,0,NULL,NP_SCORE);
                    PostMessage(hWndScore,WM_COMMAND,WC_UPDATESCORE,ptSelf.ulID);
                    break;

                case (NP_SCORE):
                    if (bFound) {
                        ptTrav->next->iScore = *iScore;
                        PostMessage(hWndScore,WM_COMMAND,WC_UPDATESCORE,ptTrav->next->ulID);
                        }
                    break;

                case NP_INTONE:
                    if (bFound) {
                        ix = ptSelf.Pos.ix - itIntone->Pos.ix;
                        iy = ptSelf.Pos.iy - itIntone->Pos.iy;
                        if ((ix*ix + iy*iy) <= (itIntone->iLoudness * itIntone->iLoudness)) {
                            sprintf(cBuff,"[%s] - \"%s\"",ptTrav->next->cUserName,
                                    itIntone->cBuff);
                            PrintTextLine(cBuff);
                            }
                        }
                    break;

                default:
                    MessageBox(NULL,"Received Unknown message ID","ProcessNewMail",
                                MB_ICONEXCLAMATION|MB_APPLMODAL);
                    break;
                }
            }

        GetMailslotInfo(hMailSlot,&cbMaxMsg,&cbNextMsg,&cMsg,&dwReadTimeout);
        }

#endif // end if WIN32. This whole function is not present for WIN16.

    return;

}
