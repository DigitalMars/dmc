Global Chat Windows Sockets Sample Application using the new
Name Registration and Resolution APIs

This application allows two clients to carry on a chat dialog using
a server as the intermediary.  Through the miricle of the new Name
Resolution and Registration APIs the clients need not even have the
same transport protocols loaded.  The server acts as a gateway between
the two clients, forwarding messages over the appropriate Windows
Sockets interface without really knowing what protocols it is using.

To run Global Chat:
1) Load the server side, GLOBCHAT.EXE, on a Windows NT 3.5 server.  You
will want this machine to be running every protocol that a potential 
client might run.

2) On a client machine, run GLOBCL.EXE.  Select Options->Connect to get
to the dialog where you will be able to select the protocol of your 
choice and give yourself a name to be know by.

3) Once you enter this information, the Global Chat Client will find 
the server and then give you a list of other users who you can chat with. 
Pick one, wait for them to accept your offer of a chat, and start typing.

The Global Chat Server displays the protocols it is using, the number of 
client machines connected using each protocol, a list of all the client 
names currently connected, and the status of each.

Please note that some of the supported protocols will simply use the first
Global Chat Server that they find.  This means that if more than one Global
Chat Server is running on your network, you may not connect to the one you 
expect to.
