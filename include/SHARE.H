/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SHARE_H
#define __SHARE_H 1

#ifndef __STDC__
#define SH_COMPAT	0x00
#define SH_DENYRW	0x10
#define SH_DENYWR	0x20
#define SH_DENYRD	0x30
#define SH_DENYNO	0x40
#endif

#define _SH_COMPAT	0x00
#define _SH_DENYRW	0x10
#define _SH_DENYWR	0x20
#define _SH_DENYRD	0x30
#define _SH_DENYNO	0x40

#endif
