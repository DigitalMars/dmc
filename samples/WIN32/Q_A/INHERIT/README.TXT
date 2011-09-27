Sample: using anonymous pipe to capture child process output

Summary:

The sample INHERIT demonstrates how to redirect standard
output and standard error to an anonymous pipe using two different
techniques: passing anonymouse pipe handles to the child in the
STARTUPINFO structure, and by setting anonymous pipe handles as the
standard handles with the SetStdHandle API and having the child
process inherit them.

In the sample, a child process is started whose standard
output and standard error handles have been redirected to an
anonymous pipe. The parent reads out of this pipe and puts
the data to both the console and to a log file specified on
the command line.

Additional reference words:

   CloseHandle    CreateProcess  ReadFile       
   CreateFile     GetLastError   CreatePipe     
   WriteFile      
