/*_ perror.c   Fri May 12 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars  */
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* Modified for Windows 3.0 by G. Eric Engstrom		*/
/* Modified for Unix by David Bustin                    */

#include	<stdio.h>
#include	<errno.h>
#include	<assert.h>
#include	<string.h>
#include	<stdlib.h>
#ifdef _WINDOWS
#include	<stdarg.h>
#include	<windows.h>
#endif

/**************************
 * Print error message associated with the current
 * value of errno.
 */

void perror(const char *s)
{
#if 0
    fputs(s,stderr);
    fputs(": ",stderr);
    fputs(strerror(errno),stderr);
    fputc('\n',stderr);
#else
    if ((s != NULL) && (strlen(s) > 0))
      fprintf (stderr,"%s: ",s);
    fprintf(stderr,"%s\n",strerror(errno));
#endif
}

/* Error messages indexed by error number defined in errno.h */
char *sys_errlist[] =
{
#if M_UNIX || M_XENIX
	"No Error",						/*   0 */
	"Not privileged",				/*   1 */
	"No such file or directory",	/*   2 */
	"No such process",				/*   3 */
	"Interrupted system call",		/*   4 */
	"I/O error",					/*   5 */
	"No such device or address",	/*   6 */
	"Arg list too long",			/*   7 */
	"Exec format error",			/*   8 */
	"Bad file number",				/*   9 */
	"No child processes",			/*  10 */
	"No more processes",			/*  11 */
	"Not enough space",				/*  12 */
	"Permission denied",			/*  13 */
	"Bad address",					/*  14 */
	"Block device required",		/*  15 */
	"Device busy",					/*  16 */
	"File exists",					/*  17 */
	"Cross-device link",			/*  18 */
	"No such device",				/*  19 */
	"Not a directory",				/*  20 */
	"Is a directory",				/*  21 */
	"Invalid argument",				/*  22 */
	"File table overflow",			/*  23 */
	"Too many open files",			/*  24 */
	"Not a typewriter",				/*  25 */
	"Text file busy",				/*  26 */
	"File too large",				/*  27 */
	"No space left on device",		/*  28 */
	"Illegal seek",					/*  29 */
	"Read-only file system",		/*  30 */
	"Too many links",				/*  31 */
	"Broken pipe",					/*  32 */
	"Argument out of domain",		/*  33 */
	"Result too large",				/*  34 */
	"No message of desired type",	/*  35 */
	"Identifier removed",			/*  36 */
	"Channel number out of range",	/*  37 */
	"Level 2 not synchronized",		/*  38 */
	"Level 3 halted",				/*  39 */
	"Level 3 reset",				/*  40 */
	"Link number out of range",		/*  41 */
	"Protocol driver not attached",	/*  42 */
	"No CSI structure available",	/*  43 */
	"Level 2 halted",				/*  44 */
	"Deadlock detected/avoided",	/*  45 */
	"No record locks available",	/*  46 */
	"Error 47",						/*  47 */
	"Error 48",						/*  48 */
	"Error 49",						/*  49 */
	"Bad exchange descriptor",		/*  50 */
	"Bad request descriptor",       /*  51 */
	"Message tables full",			/*  52 */
	"Anode table overflow",			/*  53 */
	"Bad request code",				/*  54 */
	"Invalid slot",					/*  55 */
	"File locking deadlock",		/*  56 */
	"Bad font file format",			/*  57 */
	"Error 58",						/*  58 */
	"Error 59",						/*  59 */
	"Not a stream device",			/*  60 */
	"No data available",			/*  61 */
	"Timer expired",				/*  62 */
	"Out of stream resources",		/*  63 */
	"Machine not on network",		/*  64 */
	"Package not installed",		/*  65 */
	"Object is remote",				/*  66 */
	"Link has been severed",		/*  67 */
	"Advertise error",				/*  68 */
	"Srmount error",				/*  69 */
	"Communication error on send",	/*  70 */
	"Protocol error",				/*  71 */
	"Error 72",						/*  72 */
	"Error 73",						/*  73 */
	"Multihop attempted",			/*  74 */
	"Error 75",						/*  75 */
	"Error 76",						/*  76 */
	"Not a data message",			/*  77 */
	"Filename too long",			/*  78 */
	"Value too big for type",		/*  79 */
	"Name not unique on network",	/*  80 */
	"File descriptor in bad state",	/*  81 */
	"Remote address changed",		/*  82 */
	"Cannot access shared library", /*  83 */
	"Accessing corrupted shared library",			/*  84 */
	".lib section in a.out corrupted",				/*  85 */
	"Attempting to link too many shared libraries",	/*  86 */
	"Can not exec a shared library directly",		/*  87 */
	"Error 88",						/*  88 */
	"Function not implemented",		/*  89 */
	"Operation would block",		/*  90 */
	"Operation now in progress",	/*  91 */
	"Operation already in progress",/*  92 */
	"Socket operation on non-socket",	/* 93 */
	"Destination address required",	/*  94 */
	"Message too long",				/*  95 */
	"Protocol wrong type for socket",	/*  96 */
	"Protocol not supported",		/*  97 */
	"Socket type not supported",	/*  98 */
	"Operation not supported on socket",/*  99 */
	"Protocol family not supported",	/* 100 */
	"Address family not supported by protocol family",	/* 101 */
	"Address already in use",		/* 102 */
	"Can't assign requested address",	/* 103 */
	"Network is down",				/* 104 */
	"Network is unreachable",		/* 105 */
	"Network dropped connection on reset",				/* 106 */
	"Software caused connection abort",	/* 107 */
	"Connection reset by peer",		/* 108 */
	"Error 109",					/* 109 */
	"Socket is already connected",	/* 110 */
	"Socket is not connected",		/* 111 */
	"Can't send after socket shutdown",		/* 112 */
	"Too many references: can't splice",	/* 113 */
	"Connection timed out",			/* 114 */
	"Connection refused",			/* 115 */
	"Host is down",					/* 116 */
	"No route to host",				/* 117 */
	"Protocol not available",		/* 118 */
	"Error 119",					/* 119 */
	"Protocol not supported",		/* 120 */
	"Socket type not supported",	/* 121 */
	"Operation not supported on transport endpoint",	/* 122 */
	"Protocol family not supported",					/* 123 */
	"Address family not supported by protocol family",	/* 124 */
	"Address already in use",		/* 125 */
	"Cannot assign requested address",	/* 126 */
	"Network is down",				/* 127 */
	"Network is unreachable",		/* 128 */
	"Network dropped connection because of reset",	/* 129 */
	"Software caused connection abort",	/* 130 */
	"Connection reset by peer",		/* 131 */
	"No buffer space available",	/* 132 */
	"Transport endpoint is already connected",	/* 133 */
	"Transport endpoint is not connected",		/* 134 */
	"Structure needs cleaning",		/* 135 */
	"Error 136",					/* 136 */
	"Not a name file",				/* 137 */
	"Not available",				/* 138 */
	"Is a name file",				/* 139 */
	"Remote I/O error",				/* 140 */
	"Reserved for future use",		/* 141 */
	"Error 142",					/* 142 */
	"Cannot send after socket shutdown",	/* 143 */
	"Too many references: cannot splice",	/* 144 */
	"Directory not empty",			/* 145 */
	"Connection Refused",			/* 146 */
	"Host is down",					/* 147 */
	"No route to host",				/* 148 */
	"Operation already in progress",		/* 149 */
	"Operation now in progress",	/* 150 */
	"Stale NFS file handle",        /* 151 */
	"Cannot load required kernel module",	/* 152 */
	"Object file error in loading kernel module",	/* 153 */
	"Symbol matching given spec not found",	/* 154 */
	"Error 155",					/* 155 */
	"Version number mismatch for loadable kernel module",	/* 156 */
	"Configured kernel resource exhausted",	/* 157 */
	NULL
#else
	"No error",
	"Operation not permitted",
	"No such file or directory",
	"No such process",
	"Interrupted function call",
	"Input/output error",
	"No such device or address",
	"Arg list too long",
	"Exec format error",
	"Bad file descriptor",
	"No child processes",
	"Resource temporarily unavailable",
	"Not enough space",
	"Permission denied",
	"Bad address",
	"errnum = 15",
	"Resource device",
	"File exists",
	"Improper link",
	"No such device",
	"Not a directory",
	"Is a directory",
	"Invalid argument",
	"Too many open files in system",
	"Too many open files",
	"Inappropriate I/O control operation",
	"errnum = 26",
	"File too large",
	"No space left on device",
	"Invalid seek",
	"Read-only file system",
	"Too many links",
	"Broken pipe",
	"Domain error",
	"Result too large",
	"errnum = 35",
	"Resource deadlock avoided",
	"errnum = 37",
	"Filename too long",
	"No locks available",
	"Function not implemented",
	"Directory not empty",
	"Illegal byte sequence"
#endif
};

/* Number of elements in sys_errlist[]	*/
int sys_nerr = sizeof(sys_errlist) / sizeof(sys_errlist[0]);

