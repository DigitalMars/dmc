/*_ winshell.h	*/

/* A simple way to convert a command line program to a Windows program.
 */

#if _WINDOWS
void _cdecl win_yield(void);
void win_exit(int exitstatus);
int win_spawn(char *cmd,char *args);
#else
#define win_yield()		((void)0)
#define win_exit(exitstatus)	((void)0)
#endif
