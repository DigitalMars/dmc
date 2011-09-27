// RTF.H

int  RTF_GetCharAttribute(HWND hRTFWnd, DWORD dwMask, DWORD dwEffects);
void RTF_ChangeCharAttribute(HWND hRTFWnd, DWORD dwMask, DWORD dwEffects);
void RTF_ChangeSizeAttribute(HWND hRTFWnd, int iPointChange);
void RTF_ChangeFont(HWND hWnd, HWND hRTFWnd);
BOOL RTF_Open(LPSTR szFileName, HWND hRTFWnd);
BOOL RTF_Save(LPSTR szFileName, HWND hRTFWnd);
void RTF_Print(LPSTR szFileName, HDC hPrinterDC, HWND hRTFWnd);
void RTF_GetCurrentFont(HWND hRTFWnd, LPSTR szFont);
void RTF_SetCurrentFont(HWND hRTFWnd, LPSTR szFont);
void RTF_ChangeMargins(HWND hwndRuler,
				       UINT dxStartIndent,
					   UINT dxRightIndent,
					   UINT dxOffset);
void RTF_ShowMargins(HWND   hwndRuler);
void RTF_ShowCharAttributes(void);

// These are app-specific codes to determine the state of text.
// YES means the text has the attribure requested. No means it
// doesn't. WAFFLE means it is neither yes or no (has characters
// with and without the attribute requested).

#define CHAR_ATTRIB_YES    0
#define CHAR_ATTRIB_NO     1
#define CHAR_ATTRIB_WAFFLE 2


