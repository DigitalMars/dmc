/*_ sound.c   Modified by: Walter Bright
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include	<sound.h>

#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <windows.h>
#endif


/**********************
 * Beep the speaker.
 * Use:
 *	frequency in Hz
 *	duration in milliseconds
 */

void __cdecl sound_note(int frequency,int duration)
{
#if __NT__
    Beep(frequency,duration);
#else
    DosBeep(frequency,duration);
#endif
}

/**********************
 * Beep the speaker.
 * Use:
 *	freq = frequency (1331 = 1000 Hz)
 */

void __cdecl sound_beep(int freq)
{
#if __NT__
    /* Turn frequency into hertz, 500 == half a second	*/
    Beep((freq * 1000) / 1331,500);
#else
    /* Turn frequency into hertz, 500 == half a second	*/
    DosBeep((freq * 1000) / 1331,500);
#endif
}

/**********************
 * Generate a click on the speaker.
 */

void __cdecl sound_click()
{
#if __NT__
    Beep(50,50);
#else
    DosBeep(50,50);
#endif
}
