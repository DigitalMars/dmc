/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */

#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SYS_DIR_H
#define __SYS_DIR_H 1

#ifndef DIRSIZ
#define DIRSIZ 14
#endif

struct	direct
{
  unsigned short int d_ino;
  char d_name[DIRSIZ];
};

#endif

