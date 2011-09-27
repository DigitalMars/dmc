HttpSvr Installation and Tips
=============================

HttpSvr is an MFC sample web server application. It operates as a single-document
interface application, configured with an HttpSvr Configuration file (.HSC). You
create HSC files by selecting "Save" from the "Server" menu. The application
registers the HSC files with the system so that double-clicking on an HSC file 
will start HttpSvr. Multiple instances of HttpSvr can be running at any time, but
only one can be listening on any particular port (80 being the default).

Setup Instructions
==================
Step 1: Build the HttpSvr.exe project.
Step 2: Create a root HTTP directory named "WebPages" in the root folder of the 
        same drive as the executable (see below).
Step 3: Copy the folder "SvrAdmin" and all it's contents into the root HTTP folder.
Step 4: Run the copy of HttpSvr.exe on your hard drive.

Your are now ready to serve up your web pages. Keep in mind that if you make any
changes to the default configuration, you'll have to save those settings into an
HttpSvr Configuration file (.HSC) via the "Save" item on the "Server" menu, and 
launch future HttpSvr sessions from the resulting .HSC file in order to re-use 
those settings.


Root HTTP Folder
================
By default, the server will attempt to serve-up files from the folder "WebPages" 
on the same drive as the executable. When you first run HttpSvr, if that folder 
does not exist, it will ask you if you want to create it. You do not have to use
that folder; you can specify another. Just remember that a different root folder
is a change in configuration and should be saved as a new HttpSvr Configuration 
file (.HSC). 
It is highly recommended that you copy the SvrAdmin subfolder from the source 
directory to this folder so that the directory listing icons can be used by your 
server when a default document file is not available. For example, I put my root 
at C:\WebPages, and the tree looks something like:
	C:\
	  WebPages\
	     SvrAdmin\
	     RonLo\
	     Cmd95\
	     HttpSvr\
Everything in and under "WebPages" will be available for viewing over the web (as long
as the hidden attribute is not set). ***Don't forget to copy "SvrAdmin"!!***

Tips
====
Unless you want people to be able to browse all the files in a directory
FTP-like, include an HTML file named "default.html" that will get loaded
automatically. HttpSvr is smart enough to look for default files with names 
like default.html, default.htm, index.html and index.htm. 
If you do want people to be able to browse (like if you have a directory of files 
that can be downloaded), files with the "hidden" attribute set will not be available 
for download but will still be listed. If the SvrAdmin folder was not installed as 
a subfolder of your main root HTTP directory, directory listings will be quite ugly 
due to the server not finding the icons that should be there.

The most important directory to have a default.html file is the root HTTP folder.
This is the one people type in when they're not sure of any specific page names. 
For example, my server's name is "RonLo." When someone types a URL of "http://RonLo" 
or just "RonLo", the default.html file in the root HTTP file gets loaded, and it tells 
them what all is on my server.

If none of the default HTML files are present, it will look for a CGI app named (you 
gessed it) default.exe. If it finds one, it will run it as a GET operation without 
any parameters.

