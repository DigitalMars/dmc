'Constants and error IDs for OLE 2.0 Client Control
'Created 1/19/93 - BOBM
'Updated 3/16/93 - BOBM

'Actions
Global Const OLE_CREATE_EMBED = 0
Global Const OLE_CREATE_NEW = 0		  'from ole1 control
Global Const OLE_CREATE_LINK = 1
Global Const OLE_CREATE_FROM_FILE = 1     'from ole1 control
Global Const OLE_COPY = 4
Global Const OLE_PASTE = 5
Global Const OLE_UPDATE = 6
Global Const OLE_ACTIVATE = 7
Global Const OLE_CLOSE = 9
Global Const OLE_DELETE = 10
Global Const OLE_SAVE_TO_FILE = 11
Global Const OLE_READ_FROM_FILE = 12
Global Const OLE_INSERT_OBJ_DLG = 14
Global Const OLE_PASTE_SPECIAL_DLG = 15
Global Const OLE_FETCH_VERBS = 17
Global Const OLE_SAVE_TO_OLE1FILE = 18

'OLEType
Global Const OLE_LINKED = 0
Global Const OLE_EMBEDDED = 1
Global Const OLE_NONE = 3

'OLETypeAllowed
Global Const OLE_EITHER = 2

'UpdateOptions
Global Const OLE_AUTOMATIC = 0
Global Const OLE_FROZEN = 1
Global Const OLE_MANUAL = 2

'AutoActivate modes
'Note that OLE_ACTIVATE_GETFOCUS only applies to objects that
'support "inside-out" activatino.  See related Verb notes below.
Global Const OLE_ACTIVATE_MANUAL = 0
Global Const OLE_ACTIVATE_GETFOCUS = 1
Global Const OLE_ACTIVATE_DOUBLECLICK = 2

'SizeModes
Global Const OLE_SIZE_CLIP = 0
Global Const OLE_SIZE_STRETCH = 1
Global Const OLE_SIZE_AUTOSIZE = 2

'DisplayTypes
Global Const OLE_DISPLAY_CONTENT = 0
Global Const OLE_DISPLAY_ICON = 1

'Update Event Constants
Global Const OLE_CHANGED = 0
Global Const OLE_SAVED = 1
Global Const OLE_CLOSED = 2
Global Const OLE_RENAMED = 3

'Special Verb Values
Global Const VERB_PRIMARY = 0
Global Const VERB_SHOW = -1
Global Const VERB_OPEN = -2
Global Const VERB_HIDE = -3
Global Const VERB_INPLACEUIACTIVATE = -4
Global Const VERB_INPLACEACTIVATE = -5
'The last two verbs are for objects that support "inside-out" activation,
'meaning they can be edited in-place, and that they support being left
'inplace-active even when the input focus moves to another control or form.
'These objects actually have 2 levels of being active.  "InPlace Active"
'means that the object is ready for the user to click inside it and start
'working with it.  "InPlace UI-Active" means that, in addition, if the object
'has any other UI associated with it, such as floating pallette windows,
'that those windows are visible and ready for use.  Any number of objects
'can be "InPlace Active" at a time, although only one can be 
'"InPlace UI-Active".  

'You can cause an object to move to either one of states programmatically by 
'setting the Verb property to the appropriate verb and setting 
'Action=OLE_ACTIVATE.  

'Also, if you set AutoActivate = OLE_ACTIVATE_GETFOCUS, the server will 
'automatically be put into "InPlace UI-Active" state when the user clicks
'on or tabs into the control.

'VerbFlag Bit Masks 
Global Const VERBFLAG_GRAYED = &H1
Global Const VERBFLAG_DISABLED = &H2
Global Const VERBFLAG_CHECKED = &H8
Global Const VERBFLAG_SEPARATOR = &H800

'MiscFlag Bits - Or these together as desired for special behaviors

'MEMSTORAGE causes the control to use memory to store the object while
'           it is loaded.  This is faster than the default (disk-tempfile),
'	    but can consume a lot of memory for objects whose data takes
'	    up a lot of space, such as the bitmap for a paint program.
Global Const OLE_MISCFLAG_MEMSTORAGE = &H1

'DISABLEINPLACE overrides the control's default behavior of allowing 
'	    in-place activation for objects that support it.  If you
'	    are having problems activating an object inplace, you can
'	    force it to always activate in a separate window by setting this
'	    bit
Global Const OLE_MISCFLAG_DISABLEINPLACE = &H2

'OLE Client Error IDs
'This first set are carried over from previous control
Global Const OLEERR_OutOfMem = 31001
Global Const OLEERR_CantOpenClipboard = 31003
Global Const OLEERR_NoObject = 31004
Global Const OLEERR_CantClose = 31006
Global Const OLEERR_CantPaste = 31007
Global Const OLEERR_InvProp = 31008
Global Const OLEERR_CantCopy = 31009
Global Const OLEERR_InvFormat = 31017
Global Const OLEERR_NoClass = 31018
Global Const OLEERR_NoSourceDoc = 31019
' InvAction is our first new error for the OLE2 control
Global Const OLEERR_InvAction = 31021
Global Const OLEERR_OleInitFailed  = 31022
Global Const OLEERR_InvClass = 31023
Global Const OLEERR_CantLink = 31024
Global Const OLEERR_SourceTooLong = 31026
Global Const OLEERR_CantActivate = 31027
Global Const OLEERR_NotRunning = 31028
Global Const OLEERR_DialogBusy = 31029
Global Const OLEERR_InvalidSource = 31031
Global Const OLEERR_CantEmbed = 31032
Global Const OLEERR_CantFetchLinkSrc = 31033
Global Const OLEERR_InvalidVerb	= 31034
Global Const OLEERR_NoCompatClipFmt = 31035
Global Const OLEERR_ErrorSavingFile = 31036
Global Const OLEERR_ErrorLoadingFile = 31037
Global Const OLEERR_BadVBVersion = 31038
Global Const OLEERR_CantAccessSource = 31039
