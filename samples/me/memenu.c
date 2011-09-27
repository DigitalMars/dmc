/*_ memenu.c   Mon Apr 17 1989 */
/* $Header$ */

#include        <stdio.h>
#include	"ed.h"
#include	<disp.h>
#include	"menu.h"

/***********************
 * Menus for MicroEmacs
 */

static menu_item_t f1items[] =
{
    {   "F2  Undel Line",	0x803E,	0x3C00,	MENU_enabled },
    {   "F4  Search",		0x803F,	0x3E00,	MENU_enabled },
    {   "B   List buffers",	0x8016,	'B',	MENU_enabled },
    {   "D   Insert Date",	0x8037,	'D',	MENU_enabled },
    {   "F   File name",	0x8017,	'F',	MENU_enabled },
    {   "I   Insert File",	0x8038,	'I',	MENU_enabled },
    {   "M   Save mods",	0x8018,	'M',	MENU_enabled },
    {   "N   Next File",	0x803B,	'N',	MENU_enabled },
    {   "Q   Quit",		0x803C,	'Q',	MENU_enabled },
    {   "R   Read File",	0x801C,	'R',	MENU_enabled },
    {   "S   Save File",	0x801D,	'S',	MENU_enabled },
    {   "T   Set Hard Tab",	0x8046,	'T',	MENU_enabled },
    {   "U   Unmodify File",	0x803A,	'U',	MENU_enabled },
    {   "W   Write File",	0x8020,	'W',	MENU_enabled },
    {   "V   File Visit",	0x801F,	'V',	MENU_enabled },
    {   "X   Save & Exit",	0x803D,	'X',	MENU_enabled },
};

static menu_t f1menu = { -1,17,sizeof(f1items)/sizeof(f1items[0]),f1items };

int memenu_gold(int i,unsigned row,unsigned col)
{
    return menu_enter(&f1menu,row+i,col+2);
}

static menu_item_t metaitems[] =
{
    {   ".   Set mark",		0x8025,	'.',	MENU_enabled },
    {   ">   End of buffer",	0x8026,	'>',	MENU_enabled },
    {   "<   Beg of buffer",	0x8027,	'<',	MENU_enabled },
    {   "8   Region copy",	0x8028,	'8',	MENU_enabled },
    {   "9   Region cut",	0x8029,	'9',	MENU_enabled },
    {   "B   Back word",	0x802A,	'B',	MENU_enabled },
    {   "C   Capitalize",	0x802B,	'C',	MENU_enabled },
    {   "D   Del next word",	0x802C,	'D',	MENU_enabled },
    {   "F   Forward word",	0x802D,	'F',	MENU_enabled },
    {   "H   Del prev word",	0x8023,	'H',	MENU_enabled },
    {   "I   Spaces to tab",	0x8024,	'I',	MENU_enabled },
    {	"J   Undel line",	0x803E,	'J',	MENU_enabled },
    {   "L   Lower case",	0x802E,	'L',	MENU_enabled },
    {   "N   Down window",	0x8019,	'N',	MENU_enabled },
    {   "P   Up window",	0x801B,	'P',	MENU_enabled },
    {   "Q   Query-replace",	0x802F,	'Q',	MENU_enabled },
    {   "R   Replace",		0x8030,	'R',	MENU_enabled },
    {	"T   Tog col mode",	0x8044, 'T',	MENU_enabled },
    {   "U   Upper case",	0x8031,	'U',	MENU_enabled },
    {   "V   Back page",	0x8032,	'V',	MENU_enabled },
    {   "W   Word select",	0x8033,	'W',	MENU_enabled },
    {   "X   Swap mark",	0x8021,	'X',	MENU_enabled },
    {   "Z   Shrink window",	0x8022,	'Z',	MENU_enabled },
};

static menu_t metamenu = { -1,17,sizeof(metaitems)/sizeof(metaitems[0]),metaitems };

int memenu_meta(int i,unsigned row,unsigned col)
{
    return menu_enter(&metamenu,row+i,col+2);
}

static menu_item_t morectlxitems[] =
{
    {   "=  Show cur pos",	0x8003,	'=',		MENU_enabled },
    {   "(  Start macro",	0x8004,	'(',		MENU_enabled },
    {   ")  End macro",		0x8005,	')',		MENU_enabled },
    {   "[  Inc indent",	0x8006,	'[',		MENU_enabled },
    {   "]  Dec indent",	0x8007,	']',		MENU_enabled },
    {   ".  Remove mark",	0x8009,	'.',		MENU_enabled },
    {   "!  DOS command",	0x800A,	'!',		MENU_enabled },
    {   "@  Pipe command",	0x8001,	'@',		MENU_enabled },
    {   "#  Filter buffer",	0x8002,	'#',		MENU_enabled },
    {   "1  One window",	0x8008,	'1',		MENU_enabled },
    {   "2  Split window",	0x800B,	'2',		MENU_enabled },
};

static menu_t morectlxmenu =
    {   -1,17,sizeof(morectlxitems)/sizeof(morectlxitems[0]),morectlxitems };

int memenu_morectlx(int i,unsigned row,unsigned col)
{
    return menu_enter(&morectlxmenu,row+i,col+2);
}

static menu_item_t ctlxitems[] =
{
    {   "B  Use buffer",	0x800C,	'B',	MENU_enabled },
    {   "D  Del window",	0x800D,	'D',	MENU_enabled },
    {   "E  Execute macro",	0x800E,	'E',	MENU_enabled },
    {   "F  Set fill col",	0x800F,	'F',	MENU_enabled },
    {   "K  Kill buffer",	0x8010,	'K',	MENU_enabled },
    {   "L  Goto Line",		0x8039,	'L',	MENU_enabled },
    {   "N  Next window",	0x8011,	'N',	MENU_enabled },
    {   "O  Deblank",		0x801A,	'O',	MENU_enabled },
    {   "P  Prev window",	0x8012,	'P',	MENU_enabled },
    {   "Q  Quote next",	0x8013,	'Q',	MENU_enabled },
    {   "T  Reposition",	0x801E,	'T',	MENU_enabled },
    {   "W  Next buffer",	0x8014,	'W',	MENU_enabled },
    {   "Z  Grow window",	0x8015,	'Z',	MENU_enabled },
    {   "X  more...",		0,	'X',	MENU_enabled, memenu_morectlx, 2 },
};

static menu_t ctlxmenu = { -1,17,sizeof(ctlxitems)/sizeof(ctlxitems[0]),ctlxitems };

int memenu_ctlx(int i,unsigned row,unsigned col)
{
    return menu_enter(&ctlxmenu,row+i,col+2);
}

static menu_item_t topitems[] =
{
    {   "ESC  META",		0,		META,	MENU_enabled, memenu_meta, 3 },
    {   "^U   Enter Arg",	CTRL('U'),	CTRL('U'),	MENU_enabled },
    {   "^W   Tog Parens",	CTRL('W'),	0x17,	MENU_enabled },
    {   "^X   CTLX",		0,		CTLX,	MENU_enabled, memenu_ctlx, 4 },
    {	"^Y   Paste",		CTRL('Y'),	0x19,	MENU_enabled },
    {	"Alt B Next Buffer",	ALTB,		ALTB,	MENU_enabled },
    {	"Alt C Save Config",	ALTC,		ALTC,	MENU_enabled },
    {   "Alt F GOLD",		ALTF,		GOLD,	MENU_enabled, memenu_gold, 2 },
    {	"Alt H This Menu",	MENU_BUTTON,	MENU_BUTTON,	MENU_enabled },
    {	"Alt Z DOS Cmd",	ALTZ,		ALTZ,	MENU_enabled },
};

static menu_t topmenu = { -1,15,sizeof(topitems)/sizeof(topitems[0]),topitems };

/***********************
 * This is called when the menu command is issued from the keyboard.
 */

int memenu_button()
{
    return menu_enter(&topmenu,disp_cursorrow + 1,disp_cursorcol + 1);
}

/***********************
 * This is called when the menu command is issued from the mouse.
 */

memenu_mouse(row,col)
{
	return menu_enter(&topmenu,row,col);
}
