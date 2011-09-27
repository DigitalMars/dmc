Summary:

The WINNET sample finds a connectable disk resource on the
  network, connects to it, then disconnects.  It's purpose is to
  demonstrate these WNet Win32 APIs:

    WNetAddConnection
    WNetAddConnection2
    WNetCancelConnection
    WNetCancelConnection2
    WNetCloseEnum
    WNetEnumResource
    WNetGetConnection
    WNetGetLastError
    WNetGetUser
    WNetOpenEnum

More Information:

Requirements for WINNET:

 - Ensure one or more network disk shares can be used by the
   machine/user-ID you run WINNET on.

 - Ensure that these one or more network disk shares are not already
   connected to the machine/user-ID you run WINNET on.

 - These net shares must not require a password.

IMPORTANT: Ensure net drive W: is not in use when you run WINNET