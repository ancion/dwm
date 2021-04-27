/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>


/* appearance */
static const unsigned int borderpx      = 1;        /* border pixel of windows */
static const unsigned int gappx         = 4;        /* gaps between windows */
static const unsigned int snap          = 36;       /* snap pixel */
static const int showbar                = 1;        /* 0 means no bar */
static const int topbar                 = 1;        /* 0 means bottom bar */
static const int vertpad                = 5;        /* vertical padding of bar */
static const int sidepad                = 5;        /* horizontal padding of bar */
static const int leftpad                = 0;        /* inner left padding of bar */
static const char *fonts[]              = {
  "FiraCode Nerd Font:size=12",
  "WenQuanYi Micro Hei:size=12",
};
//static const char dmenufont[]           = "monospace:size=14";
static const char dmenufont[]           = "FiraCode Nerd Font:size=14";
static const char col_gray1[]           = "#222222";
static const char col_gray2[]           = "#444444";
static const char col_gray3[]           = "#bbbbbb";
static const char col_gray4[]           = "#eeeeee";
static const char col_cyan[]            = "#5b97f7";
static const char col_base01[]          = "#585858";
static const char col_base02[]          = "#262626";
static const unsigned int baralpha      = 0xa0;
static const unsigned int borderalpha   = 0x80;
static const char *colors[][3]          = {
	/*               fg         bg          border   */
	[SchemeNorm] = { col_gray3, col_base02, col_gray2 },
	[SchemeSel]  = { col_gray4, col_base01, col_cyan  },
	[SchemeHid]  = { col_cyan,  col_base02, col_cyan  },
};
static const unsigned int alphas[][3]   = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
//static const char *tags[] = { "1", "2", "3", "4", "3", "5", "7", "8", "9" };
//static const char *tags[] = { "❶", "❷", "❸",  "➍", "❺", "❻", "❼", "❽", "❾" };
//static const char *tags[] = { "𝟭", "𝟮", "𝟯", "𝟰", "𝟱", "𝟲", "𝟳", "𝟴", "𝟵" };


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance        title       tags mask     isfloating   monitor */
	//{ "term",         "autostart",    NULL,       1,            0,           -1 },
	//{ "Google-chrome",NULL,           NULL,       1 << 1,       0,           -1 },
    { "code-oss",     NULL,           NULL,       1 << 2,       0,           -1 },
	//{ NULL,           "libreoffice",  NULL,       1 << 3,       0,           -1 },
	//{ "zoom",         NULL,           NULL,       1 << 4,       0,           -1 },
	//{ "Virt-viewer",  NULL,           NULL,       1 << 5,       0,           -1 },
	//{ "Gimp",         NULL,           NULL,       1 << 6,       0,           -1 },
	//{ "DBeaver",      NULL,           NULL,       1 << 7,       0,           -1 },
	{ "KeePassXC",    NULL,           NULL,       1 << 8,       0,           -1 },
	{ "trayer",       NULL,           NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.618; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
#include "tcl.c"
#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "| 侀",     dwindle },  /* first entry is default */
	{ "| 禎",     NULL },     /* no layout function means floating behavior */
	{ "| ﬿ ",     tile },     /* nf-mdi-collage */
	{ "|  ",     spiral },   /* ﯩ  mdi-loop */
	{ "| 淋",     monocle },
	{ "|  ",     grid },     /* 﩯 nf-mdi-view_grid 全nf-mdi-view_module */
	{ "| 頻",     tcl },
};
#define LT_DWINDLE    0
#define LT_FLOAT      1
#define LT_TILE       2
#define LT_SPIRAL     3
#define LT_MONOCLE    4
#define LT_GRID       5
#define LT_TCL        6


static const char* ncmodes[] = { "🡶", "🡴" };
//static const char* ncmodes[] = { "溺", "匿" };
static unsigned int ncmode = 1;

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dwmcmd", "launch", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *volupcmd[]  = { "dwmcmd", "volume", "up", NULL };
static const char *voldowncmd[]  = { "dwmcmd", "volume", "down", NULL };
static const char *volmutecmd[]  = { "dwmcmd", "volume", "toggle", NULL };
static const char *toggletrayercmd[]  = { "dwmcmd", "trayer", NULL };
static const char *mednextcmd[]  = { "dwmcmd", "music", "next", NULL };
static const char *medprevcmd[]  = { "dwmcmd", "music", "prev", NULL };
static const char *medplaycmd[]  = { "dwmcmd", "music", "toggle", NULL };
static const char *togglescreenkeycmd[]  = { "dwmcmd", "scrkey", NULL };
static const char *lockcmd[]  = { "loginctl", "lock-session", NULL };
static const char *capturecmd[]  = { "flameshot", "gui", NULL };
static const char *dictcmd[]  = { "dwmcmd", "dict", NULL };
static const char *getaliascmd[]  = { "dwmcmd", "input", "Alias:", NULL };
static const char *bookmarkcmd[]  = { "dwmcmd", "bookmark", NULL };

static Key keys[] = {
	/* modifier                     key                         function        argument */
	{ 0,                            XK_F1,                      spawn,          {.v = capturecmd} },
	{ MODKEY,                       XK_Return,                  spawn,          {.v = termcmd} },
	{ MODKEY,                       XK_comma,	                setalias,       {.v = getaliascmd} },
	{ MODKEY,                       XK_b,                       spawn,			{.v = bookmarkcmd} },
	{ MODKEY,                       XK_c,                       setlayout,      {.v = &layouts[LT_TCL]} },
	{ MODKEY,                       XK_d,                       spawn,          {.v = dictcmd} },
	{ MODKEY|ShiftMask,             XK_d,                       setlayout,      {.v = &layouts[LT_DWINDLE]} },
	{ MODKEY,                       XK_e,                       spawn,          {.v = togglescreenkeycmd} },
	{ MODKEY,                       XK_f,                       togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_f,                       togglefloating, {0} },
	{ MODKEY,                       XK_i,                       focusmon,       {.i = +1} },
	// move window to next monitor, using active tag of the monitor
	{ MODKEY|ShiftMask,             XK_i,                       tagmon,         {.i = +1} },
	// preserve tags while moving window to next monitor
	{ MODKEY|ControlMask,           XK_i,                       tomon,          {.i = +1} },
	{ MODKEY,                       XK_h,                       setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_g,                       setlayout,      {.v = &layouts[LT_GRID]} },
	{ MODKEY,                       XK_j,                       focusstack,     {.i = +1} },
	{ MODKEY|ShiftMask,             XK_j,                       movestack,      {.i = +1 } },
	{ MODKEY,                       XK_k,                       focusstack,     {.i = -1} },
	{ MODKEY|ShiftMask,             XK_k,                       movestack,      {.i = -1 } },
	{ MODKEY,                       XK_l,                       setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_m,                       setlayout,      {.v = &layouts[LT_MONOCLE]} },
	{ MODKEY,                       XK_n,                       incnmaster,     {.i = -1} },
	{ MODKEY|ShiftMask,             XK_n,                       incnmaster,     {.i = +1} },
	{ MODKEY,                       XK_o,                       setlayout,      {.v = &layouts[LT_FLOAT]} },
	{ MODKEY,                       XK_p,                       spawn,          {.v = dmenucmd} },
	{ MODKEY,                       XK_q,                       killclient,     {0} },
	{ MODKEY,                       XK_s,                       setlayout,      {.v = &layouts[LT_SPIRAL]} },
	{ MODKEY,                       XK_t,                       setlayout,      {.v = &layouts[LT_TILE]} },
	{ MODKEY,                       XK_u,                       focusmon,       {.i = -1} },
	{ MODKEY|ShiftMask,             XK_u,                       tagmon,         {.i = -1} },
	{ MODKEY|ControlMask,           XK_u,                       tomon,          {.i = -1} },
	{ MODKEY,                       XK_z,                       togglencmode,   {0} },
	{ MODKEY,                       XK_period,                  zoom,           {0} },
	{ MODKEY,                       XK_Tab,                     view,           {0} },
	{ MODKEY,                       XK_0,                       view,           {.ui = ~0} },
	{ MODKEY|ShiftMask,             XK_equal,                   setgaps,        {.i = +gappx} },
	{ MODKEY|ShiftMask,             XK_minus,                   setgaps,        {.i = -gappx} },
	{ MODKEY|ShiftMask,             XK_0,                       tag,            {.ui = ~0} },
	{ MODKEY,                       XK_Escape,                  quit,           {0} },
	{ MODKEY|ShiftMask,             XK_Escape,                  quit,           {.i = 1} },
	{ MODKEY,                       XK_equal,                   spawn,          {.v = volupcmd} },
	{ MODKEY,                       XK_minus,                   spawn,          {.v = voldowncmd} },
	{ MODKEY,                       XK_slash,                   spawn,          {.v = volmutecmd} },
	{ MODKEY|ShiftMask,             XK_bracketleft,             spawn,          {.v = medprevcmd} },
	{ MODKEY|ShiftMask,             XK_bracketright,            spawn,          {.v = mednextcmd} },
	{ MODKEY,                       XK_backslash,               spawn,          {.v = medplaycmd} },
	{ MODKEY,                       XK_BackSpace,               spawn,          {.v = lockcmd} },
    { 0,                            XF86XK_AudioRaiseVolume,    spawn,          {.v = volupcmd} },
    { 0,                            XF86XK_AudioLowerVolume,    spawn,          {.v = voldowncmd} },
    { 0,                            XF86XK_AudioMute,           spawn,          {.v = volmutecmd} },
    { 0,                            XF86XK_AudioPrev,           spawn,          {.v = medprevcmd} },
    { 0,                            XF86XK_AudioNext,           spawn,          {.v = mednextcmd} },
    { 0,                            XF86XK_AudioPlay,           spawn,          {.v = medplaycmd} },
	TAGKEYS(                        XK_1,                                       0)
	TAGKEYS(                        XK_2,                                       1)
	TAGKEYS(                        XK_3,                                       2)
	TAGKEYS(                        XK_4,                                       3)
	TAGKEYS(                        XK_5,                                       4)
	TAGKEYS(                        XK_6,                                       5)
	TAGKEYS(                        XK_7,                                       6)
	TAGKEYS(                        XK_8,                                       7)
	TAGKEYS(                        XK_9,                                       8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkNcSymbol,          0,              Button1,        togglencmode,   {0} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = toggletrayercmd } },
	// statusbar mouse scroll up
	{ ClkStatusText,        0,              Button4,        spawn,          {.v = volupcmd } },
	// statusbar mouse scroll down
	{ ClkStatusText,        0,              Button5,        spawn,          {.v = voldowncmd } },
	// statusbar mouse middle button
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = medplaycmd } },
	// statusbar mouse backward button
	{ ClkStatusText,        0,              8,              spawn,          {.v = medprevcmd } },
	// statusbar mouse forward button
	{ ClkStatusText,        0,              9,              spawn,          {.v = mednextcmd } },
	{ ClkClientWin,         Mod1Mask,       Button1,        movemouse,      {0} },
	{ ClkClientWin,         Mod1Mask,       Button2,        togglefloating, {0} },
	{ ClkClientWin,         Mod1Mask,       Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            ShiftMask,      Button1,        tag,            {0} },
	{ ClkTagBar,            ShiftMask,      Button3,        toggletag,      {0} },
};

