/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __TSR_H
#define __TSR_H 1

#define POPONLY   0
#define TIMESLICE 1
#define TSR_DEBUG 2

#define NO_SCAN 0
#define SCAN_Q 16
#define SCAN_W 17
#define SCAN_E 18
#define SCAN_R 19
#define SCAN_T 20
#define SCAN_Y 21
#define SCAN_U 22
#define SCAN_I 23
#define SCAN_O 24
#define SCAN_P 25
#define SCAN_A 30
#define SCAN_S 31
#define SCAN_D 32
#define SCAN_F 33
#define SCAN_G 34
#define SCAN_H 35
#define SCAN_J 36
#define SCAN_K 37
#define SCAN_L 38
#define SCAN_Z 44
#define SCAN_X 45
#define SCAN_C 46
#define SCAN_V 47
#define SCAN_B 48
#define SCAN_N 49
#define SCAN_M 50

#define SCAN_F1 59
#define SCAN_F2 60
#define SCAN_F3 61
#define SCAN_F4 62
#define SCAN_F5 63
#define SCAN_F6 64
#define SCAN_F7 65
#define SCAN_F8 66
#define SCAN_F9 67
#define SCAN_F10 68

#define RSHIFT 1
#define LSHIFT 2
#define CTRL   4
#define ALT    8

/* For backward compatability */
#define TSR_HOTSHIFT HOTSHIFT
#define TSR_HOTSCAN HOTSCAN

#if __cplusplus
	extern "C" {
#endif

int __cdecl tsr_install(int);
int __cdecl tsr_uninstall(void);
void __cdecl tsr_service(void);
void __cdecl popmain(void);
extern int __cdecl _tsr_debug;

#if __cplusplus
	}
#endif

int __cdecl _okbigbuf;

#endif
