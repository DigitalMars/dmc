Sample: Spy	[NT only]

Summary:

The SPY SDK sample provides a demonstration for monitoring system messages sent to a specified window.  SPY records the messages and displays them on the specified device.  SPY is also useful in debugging for verifying that the messages you think a window is receiving are being received.    

SPY demonstrates the following techniques:

1. Using a system message hook.
2. Using the new WM_COPYDATA message to pass data to another application.
3. Reading and writing the Registry.
4. Creating a thread.
5. Creating a DLL for the hook.

More Information:

To use SPY to monitor messages, do the following:

1. Choose the Options menu to select the following:

- Messages - to select the messages you wish to monitor.
- Font - to select the font you wish to have displayed.
- Output - to select the output device to which you want messages to go (currently COM: is not enabled).

2. Select the window whose messages you want to watch by choosing the Select window command from the Spy menu.  This will bring up a dialog box listing the current windows to watch.  The default is all windows.

3. The Edit menu it provided to allow you to cut/copy/paste to the clipboard from the SPY window.

4. Choose Start! to start monitoring.  Once this option is chosen, the menu item will change to Stop!.  Choose Stop! to cease monitoring messages.



