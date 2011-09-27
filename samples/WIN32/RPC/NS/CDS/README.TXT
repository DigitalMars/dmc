This directory contains the files that describe the gateway
protocol to the DCE Cell Directory Service (CDS):

File               Description

README.TXT         This readme file
NSICLT.IDL[ACF]    Client-side interface
NSISVR.IDL[ACF]    Server-side interface
NSICOM.IDL[ACF]    Interface common to client and server
NSIMGM.IDL[ACF]    Interface for management

The OSF DCE environment uses the DCE Cell Directory Service
(CDS) for its RPC name service. However, the OSF has not yet
released a specification of the protocol used to access the
CDS. To allow interoperation between DCE computers and RPC
client applications that call RPC name service API
functions, you must use a gateway to the DCE CDS.

To help you develop such a gateway, Microsoft defines an
interface for a gateway protocol in the files NSICLT.IDL,
NSSVR.IDL, and NSICOM.IDL, and provides the client-side
implementation of this gateway protocol in RPCNS.DLL. One
such implementation of the server side gateway is currently
available in the Digital Equipment Corporation DCE Starter
Kit.

To use the gateway so that your RPC application can
interoperate with DCE RPC applications:

1) Verify that the gateway is started on the DCE host

2) Verify that the client is running the TCP/IP transport

3) (Microsoft Windows NT and Windows 95 only) Set the registry entry
Software\Microsoft\Rpc\NameService\ServerNetworkAddress to a
string of the form "<ip address of gateway 1>; <ip address
of gateway 2>...", where <ip address of gateway n> is the ip
address of the nth gateway in the list.

4) Set the following registry entries under Software\
Microsoft\Rpc\NameService to the following values:

Entry            Value

Protocol         ncacn_ip_tcp
NetworkAddress   <ip addr gateway 1>; <ip addr gateway 2>...
Endpoint         <empty string>

To change registry entry settings on Microsoft Windows NT or Windows 95,
use the REGEDIT utility. On MS-DOS and Microsoft Windows,
edit the text file RPCREG.DAT.
