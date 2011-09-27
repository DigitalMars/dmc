/*_ */
#define VERSION "143 digitalmars.com"
#if DOS386
char *EMACSREV = " muEMACSx." VERSION;
#elif _WIN32
char *EMACSREV = " muEMACSn." VERSION;
#else
char *EMACSREV = " muEMACS." VERSION;
#endif
