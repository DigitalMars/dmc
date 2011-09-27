#define IDM_TREEVIEW 5000

// Function prototypes
LRESULT CreateTreeView (HWND hWndParent);
LRESULT InitTreeViewItems (HWND hWndTV, HWND hWndToBrowse);
LRESULT MsgNotifyTreeView(HWND, UINT, WPARAM, LPARAM);


#define WI_ROOT      100
#define WI_NEXT      101
#define WI_PREV      102
#define WI_PARENT    103
#define WI_OWNER     104
#define WI_TASK      105
#define WI_MODULE    106
#define WI_INSTANCE  107
#define WI_WNDPROC   108
#define WI_WNDRECT   109
#define WI_CLTRECT   110
#define WI_STYLES    111
#define WI_EXSTYLES  112


#define CI_ROOT      120
#define CI_STYLES    121
#define CI_HICON     122
#define CI_HCURSOR   123
#define CI_HBACKGRND 124
#define CI_CLSEXTRA  125
#define CI_WNDEXTRA  126
#define CI_MENUNAME  127
#define CI_HICONSM   128


#define IDS_WSMAXIMIZEBOXORTAB        0x3000+ 101
#define IDS_WSTABSTOP                 0x3000+ 102
#define IDS_WSMINIMIZEBOXORGRP        0x3000+ 103
#define IDS_WSGROUP                   0x3000+ 104
#define IDS_WSTHICKFRAME              0x3000+ 105
#define IDS_WSSYSMENU                 0x3000+ 106
#define IDS_WSHSCROLL                 0x3000+ 107
#define IDS_WSVSCROLL                 0x3000+ 108
#define IDS_WSDLGFRAME                0x3000+ 109
#define IDS_WSBORDER                  0x3000+ 110
#define IDS_WSCAPTION                 0x3000+ 111
#define IDS_WSMAXIMIZE                0x3000+ 112
#define IDS_WSCLIPCHILDREN            0x3000+ 113   
#define IDS_WSCLIPSIBLINGS            0x3000+ 114   
#define IDS_WSDISABLED                0x3000+ 115
#define IDS_WSVISIBLE                 0x3000+ 116
#define IDS_WSMINIMIZE                0x3000+ 117
#define IDS_WSCHILD                   0x3000+ 118
#define IDS_WSPOPUP                   0x3000+ 119


#define IDS_WSEXDLGMODALFRAME         0x3000+ 150   
#define IDS_WSEXNOPARENTNOTIFY        0x3000+ 151   
#define IDS_WSEXTOPMOST               0x3000+ 152
#define IDS_WSEXACCEPTFILES           0x3000+ 153
#define IDS_WSEXTRANSPARENT           0x3000+ 154
#define IDS_WSEXMDICHILD              0x3000+ 155
#define IDS_WSEXSMCAPTION             0x3000+ 156

#define IDS_WSEXEDGEMASK              0x3000+ 158
#define IDS_WSEXWINDOWEDGE            0x3000+ 159
#define IDS_WSEXCLIENTEDGE            0x3000+ 160
#define IDS_WSEXRIGHT                 0x3000+ 161
#define IDS_WSEXLEFT                  0x3000+ 162
#define IDS_WSEXRTLREADING            0x3000+ 163
#define IDS_WSEXLTRREADING            0x3000+ 164
#define IDS_WSEXLEFTSCROLLBAR         0x3000+ 165
#define IDS_WSEXRIGHTSCROLLBAR        0x3000+ 166

#define IDS_CSVREDRAW                 0x3000+ 201
#define IDS_CSHREDRAW                 0x3000+ 202
#define IDS_CSKEYCVTWINDOW            0x3000+ 203
#define IDS_CSDBLCLKS                 0x3000+ 204
#define IDS_CSOWNDC                   0x3000+ 205
#define IDS_CSCLASSDC                 0x3000+ 206
#define IDS_CSPARENTDC                0x3000+ 207
#define IDS_CSNOKEYCVT                0x3000+ 208
#define IDS_CSNOCLOSE                 0x3000+ 209
#define IDS_CSSAVEBITS                0x3000+ 210
#define IDS_CSBYTEALIGNCLIENT         0x3000+ 211
#define IDS_CSBYTEALIGNWINDOW         0x3000+ 212
#define IDS_CSGLOBALCLASS             0x3000+ 213


#define IDS_ESLEFT                    0x3000+ 250
#define IDS_ESCENTER                  0x3000+ 251
#define IDS_ESRIGHT                   0x3000+ 252
#define IDS_ESMULTILINE               0x3000+ 253
#define IDS_ESUPPERCASE               0x3000+ 254
#define IDS_ESLOWERCASE               0x3000+ 255
#define IDS_ESPASSWORD                0x3000+ 256
#define IDS_ESAUTOVSCROLL             0x3000+ 257
#define IDS_ESAUTOHSCROLL             0x3000+ 258
#define IDS_ESNOHIDESEL               0x3000+ 259
#define IDS_ESOEMCONVERT              0x3000+ 260
#define IDS_ESREADONLY                0x3000+ 261
#define IDS_ESWANTRETURN              0x3000+ 262


#define IDS_SBSVERT                   0x3000+ 301
#define IDS_SBSTOPALIGNORLEFT         0x3000+ 302
#define IDS_SBSBOTTOMALIGNORRIGHT     0x3000+ 303
#define IDS_SBSSIZEBOXTOPLEFTALIGN    0x3000+ 304
#define IDS_SBSSIZEBOXBOTTOMRIGHTALIGN 0x3000+305
#define IDS_SBSSIZEBOX                0x3000+ 306


#define IDS_LBSNOTIFY                 0x3000+ 350
#define IDS_LBSSORT                   0x3000+ 351
#define IDS_LBSNOREDRAW               0x3000+ 352
#define IDS_LBSMULTIPLESEL            0x3000+ 353
#define IDS_LBSOWNERDRAWFIXED         0x3000+ 354
#define IDS_LBSOWNERDRAWVARIABLE      0x3000+ 355
#define IDS_LBSHASSTRINGS             0x3000+ 356
#define IDS_LBSUSETABSTOPS            0x3000+ 357
#define IDS_LBSNOINTEGRALHEIGHT       0x3000+ 358
#define IDS_LBSMULTICOLUMN            0x3000+ 359
#define IDS_LBSWANTKEYBOARDINPUT      0x3000+ 360
#define IDS_LBSEXTENDEDSEL            0x3000+ 361
#define IDS_LBSDISABLENOSCROLL        0x3000+ 362
#define IDS_LBSNODATA                 0x3000+ 363

#define IDS_CBSSIMPLE                 0x3000+ 401
#define IDS_CBSDROPDOWN               0x3000+ 402
#define IDS_CBSDROPDOWNLIST           0x3000+ 403
#define IDS_CBSOWNERDRAWFIXED         0x3000+ 404
#define IDS_CBSOWNERDRAWVARIABLE      0x3000+ 405
#define IDS_CBSAUTOHSCROLL            0x3000+ 406
#define IDS_CBSOEMCONVERT             0x3000+ 407
#define IDS_CBSSORT                   0x3000+ 408
#define IDS_CBSHASSTRINGS             0x3000+ 409
#define IDS_CBSNOINTEGRALHEIGHT       0x3000+ 410
#define IDS_CBSDISABLENOSCROLL        0x3000+ 411
#define IDS_CBSUPPERCASE              0x3000+ 412
#define IDS_CBSLOWERCASE              0x3000+ 413

#define IDS_BSPUSHBUTTON              0x3000+ 450
#define IDS_BSDEFPUSHBUTTON           0x3000+ 451
#define IDS_BSCHECKBOX                0x3000+ 452
#define IDS_BSAUTOCHECKBOX            0x3000+ 453
#define IDS_BSRADIOBUTTON             0x3000+ 454
#define IDS_BS3STATE                  0x3000+ 455
#define IDS_BSAUTO3STATE              0x3000+ 456
#define IDS_BSGROUPBOX                0x3000+ 457
#define IDS_BSUSERBUTTON              0x3000+ 458
#define IDS_BSAUTORADIOBUTTON         0x3000+ 459
#define IDS_BSOWNERDRAW               0x3000+ 460
#define IDS_BSMONO                    0x3000+ 461
#define IDS_BSRIGHTBUTTON             0x3000+ 462
#define IDS_BSICON                    0x3000+ 463
#define IDS_BSBITMAP                  0x3000+ 464
#define IDS_BSPUSHLIKE                0x3000+ 465
#define IDS_BSMULTILINE               0x3000+ 466
#define IDS_BSNOTIFY                  0x3000+ 467
#define IDS_BSLEFT                    0x3000+ 468
#define IDS_BSRIGHT                   0x3000+ 469
#define IDS_BSCENTER                  0x3000+ 470
#define IDS_BSTOP                     0x3000+ 471
#define IDS_BSBOTTOM                  0x3000+ 472
#define IDS_BSVCENTER                 0x3000+ 473
                                                
                                                
// Static Styles                                
                                                
                                                
