
BOOL DisconnectFromProgman(VOID);
BOOL ConnectToProgmanW(VOID);
BOOL ConnectToProgmanA(VOID);
BOOL ProgmanExecuteStringW(LPWSTR pszExec);
BOOL ProgmanExecuteStringA(LPSTR pszExec);

#ifdef UNICODE
#define ConnectToProgman ConnectToProgmanW
#define ProgmanExecuteString ProgmanExecuteStringW
#else
#define ConnectToProgman ConnectToProgmanA
#define ProgmanExecuteString ProgmanExecuteStringA
#endif

