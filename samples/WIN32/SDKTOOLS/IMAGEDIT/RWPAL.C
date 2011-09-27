    /****************************************************************************/
    /*                                                                          */
    /*                 Copyright (c) Microsoft Corp.  1987, 1991                */
    /*                           All Rights Reserved                            */
    /*                                                                          */
    /****************************************************************************/
    /****************************** Module Header *******************************
    * Module Name: rwpal.c
    *
    * Routines for reading and writing color palette files.
    *
    * History:
    *
    ****************************************************************************/
    
    #include "imagedit.h"
    
    #include <stdio.h>
    #include <fcntl.h>
    #include <io.h>
    #include <sys\types.h>                      // For fstat() types.
    #include <sys\stat.h>                       // For fstat() function.
    
    
    /*
     * The color palette is saved in a .PAL file.  This file consists
     * of a header followed by the colors.
     *
     * The header has the following format:
     *
     * struct {
     *     CHAR tag;                // Always 'C'.
     *     WORD colors;             // Number of colors.  Always COLORSMAX.
     *     CHAR reserved[47];       // Reserved bytes.
     * }
     *
     * Immediately following this is RGB quads for each of the colors in
     * the palette.
     */
    
    
    /*
     * Size in bytes of the header of a color palette file.
     */
    #define CBCOLORHDR          (sizeof(CHAR) + sizeof(WORD) + 47)
    
    /*
     * Size in bytes of the color information in the color file.
     */
    #define CBCOLORINFO         (sizeof(DWORD) * COLORSMAX)
    
    /*
     * Size in bytes of a color palette file.  This includes the
     * size of the header and room for all the colors.
     */
    #define CBCOLORFILE         (CBCOLORHDR + CBCOLORINFO)
    
    
    
    /************************************************************************
    * LoadColorFile
    *
    *
    *
    * Arguments:
    *
    * History:
    *
    ************************************************************************/
    
    VOID LoadColorFile(VOID)
    {
        HFILE hf;
        OFSTRUCT OfStruct;
        struct stat FileStatus;
        DWORD argb[COLORSMAX];
        UINT cbRead;
        INT i;
        CHAR tag;
        CHAR szFileName[CCHMAXPATH];
    
        *szFileName = '\0';
        if (!OpenDlg(szFileName, FT_PALETTE))
            return;
    
		  if ((hf = (HFILE)OpenFile(szFileName, (LPOFSTRUCT)&OfStruct, OF_READ))
                == (HFILE)-1) {
            Message(MSG_CANTOPEN, szFileName);
            return;
        }
    
        fstat((INT)_open_osfhandle((long)(hf), (int)(O_RDONLY)), &FileStatus);
    
        if (FileStatus.st_size != CBCOLORFILE) {
            Message(MSG_BADPALFILE, szFileName);
            goto Error1;
        }
    
        if ((cbRead = _lread((HFILE)hf, &tag, 1)) == -1 || cbRead != 1) {
            Message(MSG_READERROR, szFileName);
            goto Error1;
        }
    
        if (tag != 'C') {
            Message(MSG_BADPALFILE, szFileName);
            goto Error1;
        }
    
        SetFilePointer((HANDLE)hf, CBCOLORHDR, NULL, (DWORD)0);
        if ((cbRead = _lread((HFILE)hf, (LPSTR)argb, CBCOLORINFO)) == -1 ||
                cbRead != CBCOLORINFO) {
            Message(MSG_READERROR, szFileName);
            goto Error1;
        }
    
        for (i = 0; i < COLORSMAX; i++)
            gargbColor[i] = argb[i];
    
        SetColorPalette(16, giType, TRUE);
    
    Error1:
        _lclose((HFILE)hf);
    }
    
    
    
    /************************************************************************
    * SaveColorFile
    *
    *
    *
    * Arguments:
    *
    * History:
    *
    ************************************************************************/
    
    VOID SaveColorFile(VOID)
    {
        INT i;
        HFILE hf;
        OFSTRUCT OfStruct;
        CHAR reserved[47];
        WORD wColors = COLORSMAX;
        CHAR tag = 'C';
        CHAR szFileName[CCHMAXPATH];
    
        *szFileName = '\0';
        if (!SaveAsDlg(szFileName, FT_PALETTE))
            return;
    
        if ((hf = (HFILE)OpenFile(szFileName, &OfStruct,
                OF_CREATE | OF_WRITE)) == (HFILE)-1) {
            Message(MSG_CANTCREATE, szFileName);
            return;
        }
    
        for (i = 0; i < sizeof(reserved); i++)
            reserved[i] = 0;
    
        if (_lwrite((HFILE)hf, (LPSTR)&tag, sizeof(tag)) != sizeof(tag) ||
                _lwrite((HFILE)hf, (LPSTR)&wColors, sizeof(wColors)) !=
                sizeof(wColors) ||
                _lwrite((HFILE)hf, (LPSTR)reserved, sizeof(reserved)) !=
                sizeof(reserved) ||
                _lwrite((HFILE)hf, (LPSTR)gargbColor, CBCOLORINFO) != CBCOLORINFO) {
            Message(MSG_WRITEERROR, szFileName);
        }
    
        _lclose((HFILE)hf);
    }
