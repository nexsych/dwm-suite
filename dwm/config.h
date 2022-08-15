/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "JetBrains Mono:size=12" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x32", NULL };
const char *spcmd2[] = {"st", "-n", "spcalc", "-g", "80x20", "-e", "bc", "-lq", NULL };
const char *spcmd3[] = {"st", "-n", "spnvim", "-g", "120x32", "-e", "nvim", NULL };
static Sp scratchpads[] = {
	/* name       cmd  */
	{"spterm",   spcmd1},
	{"spcalc",   spcmd2},
	{"spnvim",   spcmd3},
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class       instance       title             tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",      NULL,          NULL,             0,         0,          0,           0,        -1 },
	{ "Firefox",   NULL,          NULL,             1 << 8,    0,          0,          -1,        -1 },
	{ "St",        NULL,          NULL,             0,         0,          1,           0,        -1 },
	{ NULL,        NULL,          "Event Tester",   0,         0,          0,           1,        -1 }, /* xev */
	{ NULL,	       "spterm",      NULL,		SPTAG(0),  1,	       1,	    0, 	      -1 },
	{ NULL,	       "spcalc",      NULL,		SPTAG(1),  1,	       1,	    0, 	      -1 },
	{ NULL,	       "spnvim",      NULL,		SPTAG(2),  1,	       1,	    0, 	      -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "st", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,             		XK_Return, spawn,          {.v = termcmd } },

	{ MODKEY,                       XK_t,      togglebar,      {0} },

	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },

	{ MODKEY|ShiftMask,             XK_l,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,      incnmaster,     {.i = -1 } },

	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	
	{ MODKEY,                       XK_v,	   zoom,           {0} },

	{ MODKEY,                       XK_Tab,    view,           {0} },

	{ MODKEY,             		XK_f,      killclient,     {0} },

	{ MODKEY,                       XK_2,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_1,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_3,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_2,      setlayout,      {.v = &layouts[3]} },
/*	{ MODKEY|ShiftMask,             XK_4,      setlayout,      {.v = &layouts[4]} }, */

/*	{ MODKEY,                       XK_space,  setlayout,      {0} },		 */

	{ MODKEY|ShiftMask,             XK_1,	   togglefloating, {0} },

	{ MODKEY|ShiftMask,             XK_3,      togglefullscr,  {0} },

	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },

	{ MODKEY,            		XK_n,  	   togglescratch,  {.ui = 0 } },
	{ MODKEY,            		XK_m,	   togglescratch,  {.ui = 1 } },
	{ MODKEY,            		XK_p,	   togglescratch,  {.ui = 2 } },

	TAGKEYS(                        XK_q,                      0)
	TAGKEYS(                        XK_w,                      1)
	TAGKEYS(                        XK_e,                      2)
	TAGKEYS(                        XK_a,                      3)
	TAGKEYS(                        XK_s,                      4)
	TAGKEYS(                        XK_d,                      5)

	{ Mod1Mask,             	XK_F1,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

