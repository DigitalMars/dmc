

BOOL
DoServer();

BOOL
DoClient();

BOOL
OpenSocketRegisterAndListen();

unsigned long
PerfThread(LPVOID lpvTest);

BOOL
CheckForRecv(SOCKET s);

BOOL
CheckForSend(SOCKET s);

void
usage();
