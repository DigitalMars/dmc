
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* locale.h - header file for the locale sample. */


/* function prototypes.  Window procedures first. */
LRESULT CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);


#define MAXTCHAR 100


/* the control ID's from the dialog box. */
#define DID_LISTBOX     100
#define DID_COMBOBOX    101
#define DID_TEXT        102


/* Define a value for the LOGFONT.lfCharSet
 *  This should be included in wingdi.h, but it
 *  was removed because the font mapper is not
 *  using it anyway in version 1.0.  Currently
 *  scheduled to be included in NT ver 1.1.
 */
#define UNICODE_CHARSET  1


/* structure for the lookup tables.  */
typedef struct tagLookupEntry{
    int     Value;
    TCHAR    String[MAXTCHAR];
} LookupEntry;



/* from WINNLS.H */
#define NLANG 39
LookupEntry  langstrings[NLANG] =
{{ MAKELANGID (LANG_NEUTRAL       , SUBLANG_NEUTRAL)                , TEXT("NEUTRAL                ")},
 { MAKELANGID (LANG_CHINESE       , SUBLANG_CHINESE_SIMPLIFIED )    , TEXT("CHINESE,SIMPLIFIED     ")},
 { MAKELANGID (LANG_CHINESE       , SUBLANG_CHINESE_TRADITIONAL)    , TEXT("CHINESE,TRADITIONAL    ")},
 { MAKELANGID (LANG_CZECH         , SUBLANG_NEUTRAL)                , TEXT("CZECH                  ")},
 { MAKELANGID (LANG_DANISH        , SUBLANG_NEUTRAL)                , TEXT("DANISH                 ")},
 { MAKELANGID (LANG_DUTCH         , SUBLANG_DUTCH        )          , TEXT("DUTCH                  ")},
 { MAKELANGID (LANG_DUTCH         , SUBLANG_DUTCH_BELGIAN)          , TEXT("DUTCH,BELGIAN          ")},
 { MAKELANGID (LANG_ENGLISH       , SUBLANG_ENGLISH_US )            , TEXT("ENGLISH,US             ")},
 { MAKELANGID (LANG_ENGLISH       , SUBLANG_ENGLISH_UK )            , TEXT("ENGLISH,UK             ")},
 { MAKELANGID (LANG_ENGLISH       , SUBLANG_ENGLISH_AUS)            , TEXT("ENGLISH,AUS            ")},
 { MAKELANGID (LANG_ENGLISH       , SUBLANG_ENGLISH_CAN)            , TEXT("ENGLISH,CAN            ")},
 { MAKELANGID (LANG_ENGLISH       , SUBLANG_ENGLISH_NZ )            , TEXT("ENGLISH,NZ             ")},
 { MAKELANGID (LANG_FINNISH       , SUBLANG_NEUTRAL)                , TEXT("FINNISH                ")},
 { MAKELANGID (LANG_FRENCH        , SUBLANG_FRENCH         )        , TEXT("FRENCH                 ")},
 { MAKELANGID (LANG_FRENCH        , SUBLANG_FRENCH_BELGIAN )        , TEXT("FRENCH,BELGIAN         ")},
 { MAKELANGID (LANG_FRENCH        , SUBLANG_FRENCH_CANADIAN)        , TEXT("FRENCH,CANADIAN        ")},
 { MAKELANGID (LANG_FRENCH        , SUBLANG_FRENCH_SWISS   )        , TEXT("FRENCH,SWISS           ")},
 { MAKELANGID (LANG_GERMAN        , SUBLANG_GERMAN         )        , TEXT("GERMAN                 ")},
 { MAKELANGID (LANG_GERMAN        , SUBLANG_GERMAN_SWISS   )        , TEXT("GERMAN,SWISS           ")},
 { MAKELANGID (LANG_GERMAN        , SUBLANG_GERMAN_AUSTRIAN)        , TEXT("GERMAN,AUSTRIAN        ")},
 { MAKELANGID (LANG_GREEK         , SUBLANG_NEUTRAL)                , TEXT("GREEK                  ")},
 { MAKELANGID (LANG_HUNGARIAN     , SUBLANG_NEUTRAL)                , TEXT("HUNGARIAN              ")},
 { MAKELANGID (LANG_ICELANDIC     , SUBLANG_NEUTRAL)                , TEXT("ICELANDIC              ")},
 { MAKELANGID (LANG_ITALIAN       , SUBLANG_ITALIAN      )          , TEXT("ITALIAN                ")},
 { MAKELANGID (LANG_ITALIAN       , SUBLANG_ITALIAN_SWISS)          , TEXT("ITALIAN,SWISS          ")},
 { MAKELANGID (LANG_JAPANESE      , SUBLANG_NEUTRAL)                , TEXT("JAPANESE               ")},
 { MAKELANGID (LANG_KOREAN        , SUBLANG_NEUTRAL)                , TEXT("KOREAN                 ")},
 { MAKELANGID (LANG_NORWEGIAN     , SUBLANG_NORWEGIAN_BOKMAL )      , TEXT("NORWEGIAN,BOKMAL       ")},
 { MAKELANGID (LANG_NORWEGIAN     , SUBLANG_NORWEGIAN_NYNORSK)      , TEXT("NORWEGIAN,NYNORSK      ")},
 { MAKELANGID (LANG_POLISH        , SUBLANG_NEUTRAL)                , TEXT("POLISH                 ")},
 { MAKELANGID (LANG_PORTUGUESE    , SUBLANG_PORTUGUESE          )   , TEXT("PORTUGUESE             ")},
 { MAKELANGID (LANG_PORTUGUESE    , SUBLANG_PORTUGUESE_BRAZILIAN)   , TEXT("PORTUGUESE,BRAZILIAN   ")},
 { MAKELANGID (LANG_RUSSIAN       , SUBLANG_NEUTRAL)                , TEXT("RUSSIAN                ")},
 { MAKELANGID (LANG_SLOVAK        , SUBLANG_NEUTRAL)                , TEXT("SLOVAK                 ")},
 { MAKELANGID (LANG_SPANISH       , SUBLANG_SPANISH        )        , TEXT("SPANISH                ")},
 { MAKELANGID (LANG_SPANISH       , SUBLANG_SPANISH_MEXICAN)        , TEXT("SPANISH,MEXICAN        ")},
 { MAKELANGID (LANG_SPANISH       , SUBLANG_SPANISH_MODERN )        , TEXT("SPANISH,MODERN         ")},
 { MAKELANGID (LANG_SWEDISH       , SUBLANG_NEUTRAL)                , TEXT("SWEDISH                ")},
 { MAKELANGID (LANG_TURKISH		 , SUBLANG_NEUTRAL)					  , TEXT("TURKISH                ")}};



#define NLOCALEENTRIES 89
LookupEntry  localeentry[NLOCALEENTRIES] =
{{  LOCALE_ILANGUAGE         , TEXT("ILANGUAGE         ")},
 {  LOCALE_SLANGUAGE         , TEXT("SLANGUAGE         ")},
 {  LOCALE_SENGLANGUAGE      , TEXT("SENGLANGUAGE      ")},
 {  LOCALE_SABBREVLANGNAME   , TEXT("SABBREVLANGNAME   ")},
 {  LOCALE_SNATIVELANGNAME   , TEXT("SNATIVELANGNAME   ")},
 {  LOCALE_ICOUNTRY          , TEXT("ICOUNTRY          ")},
 {  LOCALE_SCOUNTRY          , TEXT("SCOUNTRY          ")},
 {  LOCALE_SENGCOUNTRY       , TEXT("SENGCOUNTRY       ")},
 {  LOCALE_SABBREVCTRYNAME   , TEXT("SABBREVCTRYNAME   ")},
 {  LOCALE_SNATIVECTRYNAME   , TEXT("SNATIVECTRYNAME   ")},
 {  LOCALE_IDEFAULTLANGUAGE  , TEXT("IDEFAULTLANGUAGE  ")},
 {  LOCALE_IDEFAULTCOUNTRY   , TEXT("IDEFAULTCOUNTRY   ")},
 {  LOCALE_IDEFAULTCODEPAGE  , TEXT("IDEFAULTCODEPAGE  ")},
 {  LOCALE_SLIST             , TEXT("SLIST             ")},
 {  LOCALE_IMEASURE          , TEXT("IMEASURE          ")},
 {  LOCALE_SDECIMAL          , TEXT("SDECIMAL          ")},
 {  LOCALE_STHOUSAND         , TEXT("STHOUSAND         ")},
 {  LOCALE_SGROUPING         , TEXT("SGROUPING         ")},
 {  LOCALE_IDIGITS           , TEXT("IDIGITS           ")},
 {  LOCALE_ILZERO            , TEXT("ILZERO            ")},
 {  LOCALE_SNATIVEDIGITS     , TEXT("SNATIVEDIGITS     ")},
 {  LOCALE_SCURRENCY         , TEXT("SCURRENCY         ")},
 {  LOCALE_SINTLSYMBOL       , TEXT("SINTLSYMBOL       ")},
 {  LOCALE_SMONDECIMALSEP    , TEXT("SMONDECIMALSEP    ")},
 {  LOCALE_SMONTHOUSANDSEP   , TEXT("SMONTHOUSANDSEP   ")},
 {  LOCALE_SMONGROUPING      , TEXT("SMONGROUPING      ")},
 {  LOCALE_ICURRDIGITS       , TEXT("ICURRDIGITS       ")},
 {  LOCALE_IINTLCURRDIGITS   , TEXT("IINTLCURRDIGITS   ")},
 {  LOCALE_ICURRENCY         , TEXT("ICURRENCY         ")},
 {  LOCALE_INEGCURR          , TEXT("INEGCURR          ")},
 {  LOCALE_SDATE             , TEXT("SDATE             ")},
 {  LOCALE_STIME             , TEXT("STIME             ")},
 {  LOCALE_SSHORTDATE        , TEXT("SSHORTDATE        ")},
 {  LOCALE_SLONGDATE         , TEXT("SLONGDATE         ")},
 {  LOCALE_IDATE             , TEXT("IDATE             ")},
 {  LOCALE_ILDATE            , TEXT("ILDATE            ")},
 {  LOCALE_ITIME             , TEXT("ITIME             ")},
 {  LOCALE_ICENTURY          , TEXT("ICENTURY          ")},
 {  LOCALE_ITLZERO           , TEXT("ITLZERO           ")},
 {  LOCALE_IDAYLZERO         , TEXT("IDAYLZERO         ")},
 {  LOCALE_IMONLZERO         , TEXT("IMONLZERO         ")},
 {  LOCALE_S1159             , TEXT("S1159             ")},
 {  LOCALE_S2359             , TEXT("S2359             ")},
 {  LOCALE_SDAYNAME1         , TEXT("SDAYNAME1         ")},
 {  LOCALE_SDAYNAME2         , TEXT("SDAYNAME2         ")},
 {  LOCALE_SDAYNAME3         , TEXT("SDAYNAME3         ")},
 {  LOCALE_SDAYNAME4         , TEXT("SDAYNAME4         ")},
 {  LOCALE_SDAYNAME5         , TEXT("SDAYNAME5         ")},
 {  LOCALE_SDAYNAME6         , TEXT("SDAYNAME6         ")},
 {  LOCALE_SDAYNAME7         , TEXT("SDAYNAME7         ")},
 {  LOCALE_SABBREVDAYNAME1   , TEXT("SABBREVDAYNAME1   ")},
 {  LOCALE_SABBREVDAYNAME2   , TEXT("SABBREVDAYNAME2   ")},
 {  LOCALE_SABBREVDAYNAME3   , TEXT("SABBREVDAYNAME3   ")},
 {  LOCALE_SABBREVDAYNAME4   , TEXT("SABBREVDAYNAME4   ")},
 {  LOCALE_SABBREVDAYNAME5   , TEXT("SABBREVDAYNAME5   ")},
 {  LOCALE_SABBREVDAYNAME6   , TEXT("SABBREVDAYNAME6   ")},
 {  LOCALE_SABBREVDAYNAME7   , TEXT("SABBREVDAYNAME7   ")},
 {  LOCALE_SMONTHNAME1       , TEXT("SMONTHNAME1       ")},
 {  LOCALE_SMONTHNAME2       , TEXT("SMONTHNAME2       ")},
 {  LOCALE_SMONTHNAME3       , TEXT("SMONTHNAME3       ")},
 {  LOCALE_SMONTHNAME4       , TEXT("SMONTHNAME4       ")},
 {  LOCALE_SMONTHNAME5       , TEXT("SMONTHNAME5       ")},
 {  LOCALE_SMONTHNAME6       , TEXT("SMONTHNAME6       ")},
 {  LOCALE_SMONTHNAME7       , TEXT("SMONTHNAME7       ")},
 {  LOCALE_SMONTHNAME8       , TEXT("SMONTHNAME8       ")},
 {  LOCALE_SMONTHNAME9       , TEXT("SMONTHNAME9       ")},
 {  LOCALE_SMONTHNAME10      , TEXT("SMONTHNAME10      ")},
 {  LOCALE_SMONTHNAME11      , TEXT("SMONTHNAME11      ")},
 {  LOCALE_SMONTHNAME12      , TEXT("SMONTHNAME12      ")},
 {  LOCALE_SABBREVMONTHNAME1 , TEXT("SABBREVMONTHNAME1 ")},
 {  LOCALE_SABBREVMONTHNAME2 , TEXT("SABBREVMONTHNAME2 ")},
 {  LOCALE_SABBREVMONTHNAME3 , TEXT("SABBREVMONTHNAME3 ")},
 {  LOCALE_SABBREVMONTHNAME4 , TEXT("SABBREVMONTHNAME4 ")},
 {  LOCALE_SABBREVMONTHNAME5 , TEXT("SABBREVMONTHNAME5 ")},
 {  LOCALE_SABBREVMONTHNAME6 , TEXT("SABBREVMONTHNAME6 ")},
 {  LOCALE_SABBREVMONTHNAME7 , TEXT("SABBREVMONTHNAME7 ")},
 {  LOCALE_SABBREVMONTHNAME8 , TEXT("SABBREVMONTHNAME8 ")},
 {  LOCALE_SABBREVMONTHNAME9 , TEXT("SABBREVMONTHNAME9 ")},
 {  LOCALE_SABBREVMONTHNAME10, TEXT("SABBREVMONTHNAME10")},
 {  LOCALE_SABBREVMONTHNAME11, TEXT("SABBREVMONTHNAME11")},
 {  LOCALE_SABBREVMONTHNAME12, TEXT("SABBREVMONTHNAME12")},
 {  LOCALE_SPOSITIVESIGN     , TEXT("SPOSITIVESIGN     ")},
 {  LOCALE_SNEGATIVESIGN     , TEXT("SNEGATIVESIGN     ")},
 {  LOCALE_IPOSSIGNPOSN      , TEXT("IPOSSIGNPOSN      ")},
 {  LOCALE_INEGSIGNPOSN      , TEXT("INEGSIGNPOSN      ")},
 {  LOCALE_IPOSSYMPRECEDES   , TEXT("IPOSSYMPRECEDES   ")},
 {  LOCALE_IPOSSEPBYSPACE    , TEXT("IPOSSEPBYSPACE    ")},
 {  LOCALE_INEGSYMPRECEDES   , TEXT("INEGSYMPRECEDES   ")},
 {  LOCALE_INEGSEPBYSPACE    , TEXT("INEGSEPBYSPACE    ")}} ;
