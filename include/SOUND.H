/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SOUND_H
#define __SOUND_H 1

#if __cplusplus
extern "C" {
#endif

#if __OS2__ || __NT__
void __cdecl sound_note(int frequency,int duration);
#else
void __cdecl sound_tone(int cycles,int uptime,int dntime);
#endif

void __cdecl sound_beep(int freq);
void __cdecl sound_click(void);

#if __cplusplus
}
#endif


#endif
