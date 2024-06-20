#define GUI_IMPLEMENTATION
#include "gui.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						 DEFINITIONS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define WM_MYHSLOGWINDOW	WSMENUINDEX(0)
#define WM_MYSHUTDOWN		WSMENUINDEX(2)
#define WM_MYNOTIFYICON		0
#define icopath "helloworld.ico"
#define icopathlength 14

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						 MESSAGE HANDLERS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct HELLOWORLDAPP
{
	WSAPP mSuper;
	WSWINDOW mLogWindow;
	WSSCROLLEDWINDOW mLogScrolledWindow;
	WSTEXTCTRL mLogTextCtrl;
	WSMENUACTIONS mIconMenuActions;
	WSMENUBAR mLogMenuBar;
	WSMENU mIconMenu;
	WSNOTIFYICON mNotifyIcon;
	WSWNDCLASS mWc;
	WSICON mWindowIcon;
	wsbool mWillShutdown;
} HELLOWORLDAPP;
HELLOWORLDAPP* gApp;

void HelloWorldApp_ShowHideLogWindow(HELLOWORLDAPP* pThis)
{
	if(WSWindow_IsVisible(&pThis->mLogWindow))
	{
		WSWindow_Hide(&pThis->mLogWindow);
		WSMenu_SetItemTitle(&pThis->mIconMenu, WM_MYHSLOGWINDOW, "S" WSMENUALT "how Log Window");
	}
	else
	{
		WSWindow_Show(&pThis->mLogWindow);
		WSMenu_SetItemTitle(&pThis->mIconMenu, WM_MYHSLOGWINDOW, WSMENUALT "Hide Log Window");
	}
}

WSMENUEVENT_CALLBACK(menuitem_showlog)
{
	WSMENUEVENT ev;
	HELLOWORLDAPP* pApp;
	WSMenuEvent_Construct(&ev);
	pApp = (HELLOWORLDAPP*) WSMenuEvent_GetWindowCon(&ev);
	/*WSLogDebug("GTK ICON LOGSHOW");*/
	HelloWorldApp_ShowHideLogWindow(pApp);
	return WSMENUEVENT_UNHANDLED;
}

WSMENUEVENT_CALLBACK(menuitem_shutdown)
{
	WSMENUEVENT ev;
	HELLOWORLDAPP* pApp;
	WSMenuEvent_Construct(&ev);
	pApp = (HELLOWORLDAPP*) WSMenuEvent_GetWindowCon(&ev);
	pApp->mWillShutdown = TRUE;
	return WSMENUEVENT_UNHANDLED;
}

WSSIZEEVENT_CALLBACK(HelloWorldAppOnSize)
{
	WSSIZEEVENT ev;
	WSWINDOW* pWindow;
	HELLOWORLDAPP* pApp;
	WSSizeEvent_Construct(&ev);
	pWindow = WSSizeEvent_GetWindow(&ev);
	pApp = (HELLOWORLDAPP*)WSSizeEvent_GetWindowCon(&ev);
	if(pApp && WSWindow_GetHandle(pWindow)
				== WSWindow_GetHandle(&pApp->mLogWindow))
	{
		WSTextCtrl_SetSize(&pApp->mLogTextCtrl,
			WSSizeEvent_GetWidth(&ev), WSSizeEvent_GetHeight(&ev));
	}
	return WSSIZEEVENT_UNHANDLED;
}
WSNOTIFYICONEVENT_CALLBACK(HelloWorldAppOnNotifyIcon)
{
	WSNOTIFYICONEVENT ev;
	HELLOWORLDAPP* pApp;
	WSNotifyIconEvent_Construct(&ev);
	pApp = (HELLOWORLDAPP*)
		WSNotifyIconEvent_GetWindowCon(&ev);
#if defined(_WIN32) || defined(WSMACCLASSICORCARBON)
	WSMenu_Popup(&pApp->mIconMenu, WSNotifyIconEvent_GetWindow(&ev));
#endif
	return WSNOTIFYICONEVENT_UNHANDLED;
}

WSPAINTEVENT_CALLBACK(HelloWorldAppOnPaint)
{
	WSPAINTEVENT ev;
	WSWINDOW* pWindow;
	HELLOWORLDAPP* pApp;
	WSPAINTDC dc;
	WSPaintEvent_Construct(&ev);
	pWindow = WSPaintEvent_GetWindow(&ev);
	pApp = (HELLOWORLDAPP*)
		WSPaintEvent_GetWindowCon(&ev);

	WSPaintDC_Create(&dc, pWindow);
	if(WSWindow_GetHandle(pWindow) == WSTextCtrl_GetHandle(&pApp->mLogTextCtrl))
	{
		WSTextCtrl_HandlePaint(&pApp->mLogTextCtrl, &ev, &dc);
	}
	WSPaintDC_Destroy(&dc, pWindow);
	return WSPAINTEVENT_UNHANDLED;
}
WSKEYEVENT_CALLBACK(HelloWorldAppOnKeyDown)
{
	WSKEYEVENT ev;
	HELLOWORLDAPP* pApp;
	WSKeyEvent_Construct(&ev);
	pApp = (HELLOWORLDAPP*)
		WSKeyEvent_GetWindowCon(&ev);
	WSTextCtrl_HandleKeyDown(&pApp->mLogTextCtrl, &ev);
	return WSKEYEVENT_UNHANDLED;
}


WSIDLEEVENT_CALLBACK(HelloWorldAppOnHandleIdle)
{
	HELLOWORLDAPP* pApp;
	WSIDLEEVENT ev;
	WSIdleEvent_Construct(&ev);
	pApp = (HELLOWORLDAPP*) WSEvent_GetWindowCon(&ev);
	WSTextCtrl_HandleIdle(&pApp->mLogTextCtrl);
	return WSIDLEEVENT_UNHANDLED;
}

WSCONTENTEVENT_CALLBACK(HelloWorldAppOnHandleContent)
{
	HELLOWORLDAPP* pApp;
	WSCONTENTEVENT ev;
	WSContentEvent_Construct(&ev);
	pApp = (HELLOWORLDAPP*) WSEvent_GetWindowCon(&ev);
	WSTextCtrl_HandleContentEvent(&pApp->mLogTextCtrl, &ev);
	return WSCONTENTEVENT_UNHANDLED;
}

WSGROWEVENT_CALLBACK(HelloWorldAppOnHandleGrow)
{
	HELLOWORLDAPP* pApp;
	WSGROWEVENT ev;
	WSGrowEvent_Construct(&ev);
	pApp = (HELLOWORLDAPP*) WSEvent_GetWindowCon(&ev);
	WSTextCtrl_HandleGrowEvent(&pApp->mLogTextCtrl, &ev);
	return WSGROWEVENT_UNHANDLED;
}

WSZOOMEVENT_CALLBACK(HelloWorldAppOnHandleZoom)
{
	HELLOWORLDAPP* pApp;
	WSZOOMEVENT ev;
	WSZoomEvent_Construct(&ev);
	pApp = (HELLOWORLDAPP*) WSEvent_GetWindowCon(&ev);
	WSTextCtrl_HandleZoomEvent(&pApp->mLogTextCtrl, &ev);
	return WSZOOMEVENT_UNHANDLED;
}

WSACTIVATEEVENT_CALLBACK(HelloWorldAppOnHandleActivate)
{
	HELLOWORLDAPP* pApp;
	WSACTIVATEEVENT ev;
	WSActivateEvent_Construct(&ev);
	pApp = (HELLOWORLDAPP*) WSEvent_GetWindowCon(&ev);
	WSTextCtrl_HandleActivate(&pApp->mLogTextCtrl);
	return WSACTIVATEEVENT_UNHANDLED;
}

WSDEACTIVATEEVENT_CALLBACK(HelloWorldAppOnHandleDeactivate)
{
	HELLOWORLDAPP* pApp;
	WSDEACTIVATEEVENT ev;
	WSDeactivateEvent_Construct(&ev);
	pApp = (HELLOWORLDAPP*) WSEvent_GetWindowCon(&ev);
	WSTextCtrl_HandleDeactivate(&pApp->mLogTextCtrl);
	return WSDEACTIVATEEVENT_UNHANDLED;
}

WSIBEAMEVENT_CALLBACK(HelloWorldAppOnHandleIBeam)
{
	HELLOWORLDAPP* pApp;
	WSIBEAMEVENT ev;
	WSIBeamEvent_Construct(&ev);
	pApp = (HELLOWORLDAPP*) WSEvent_GetWindowCon(&ev);
	WSTextCtrl_HandleIBeamCursor(&pApp->mLogTextCtrl, &ev);
	return WSIBEAMEVENT_UNHANDLED;
}

WSMOUSEWHEELEVENT_CALLBACK(HelloWorldAppOnHandleMouseWheel)
{
	HELLOWORLDAPP* pApp;
	WSMOUSEWHEELEVENT ev;
	WSMouseWheelEvent_Construct(&ev);
	pApp = (HELLOWORLDAPP*) WSEvent_GetWindowCon(&ev);
	WSTextCtrl_HandleMouseWheel(&pApp->mLogTextCtrl, &ev);
	return WSMOUSEWHEELEVENT_UNHANDLED;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						 MESSAGE MAP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
BEGIN_ALL_MENU_MESSAGES
	BEGIN_MENU_MESSAGES(HelloWorldAppMenuMessages)
		DECLARE_MENU_MESSAGE(WM_MYHSLOGWINDOW, menuitem_showlog)
		DECLARE_MENU_MESSAGE(WM_MYSHUTDOWN, menuitem_shutdown)
	END_MENU_MESSAGES(HelloWorldAppMenuMessages)
END_ALL_MENU_MESSAGES
BEGIN_ALL_WINDOW_MESSAGES
	BEGIN_WINDOW_MESSAGES(HelloWorldAppWindowMessages)
		IMPLEMENT_MENU_WINDOW_MESSAGES(HelloWorldAppMenuMessages)
		IMPLEMENT_SIZE_CHANGED_MESSAGE(HelloWorldAppOnSize)
		IMPLEMENT_NOTIFYICON_MESSAGE(HelloWorldAppOnNotifyIcon,
									 WM_MYNOTIFYICON)
		IMPLEMENT_PAINT_MESSAGE(HelloWorldAppOnPaint)
		IMPLEMENT_KEYDOWN_MESSAGE(HelloWorldAppOnKeyDown)
		IMPLEMENT_HANDLEIDLE_MESSAGE(HelloWorldAppOnHandleIdle)
		IMPLEMENT_HANDLECONTENT_MESSAGE(HelloWorldAppOnHandleContent)
		IMPLEMENT_HANDLEGROW_MESSAGE(HelloWorldAppOnHandleGrow)
		IMPLEMENT_HANDLEZOOM_MESSAGE(HelloWorldAppOnHandleZoom)
		IMPLEMENT_HANDLEACTIVATE_MESSAGE(HelloWorldAppOnHandleActivate)
		IMPLEMENT_HANDLEDEACTIVATE_MESSAGE(HelloWorldAppOnHandleDeactivate)
		IMPLEMENT_HANDLEIBEAM_MESSAGE(HelloWorldAppOnHandleIBeam)
		IMPLEMENT_HANDLEMOUSEWHEEL_MESSAGE(HelloWorldAppOnHandleMouseWheel)
	END_WINDOW_MESSAGES(HelloWorldAppWindowMessages)
END_ALL_WINDOW_MESSAGES

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						 GUI Initialization/Destruction
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void HelloWorldApp_CreateLogWindow(HELLOWORLDAPP* appRef)
{
	WSMENUSECTION section;
	WSRECT bounds = { WSDEFAULTPOSITION, WSDEFAULTPOSITION, 700, 700 }; // top, left, bottom, right

	guipos_t dpix, dpiy;
	WSDisplayGetScaleFactor(&dpix, &dpiy);
	WSDisplayGetDPI(&dpix, &dpiy);
	dpix -= 96;
	while(dpix > 20)
	{
		WSRect_SetWidth(&bounds, WSRect_GetWidth(&bounds) + 150);
		WSRect_SetHeight(&bounds, WSRect_GetHeight(&bounds) + 75);
		dpix -= 20;
	}

	/* WNDCLASS */
	#if defined(_WIN32)
		WSIcon_CreateFromResource(&appRef->mWindowIcon, &appRef->mSuper, "aappicon");
	#else
		WSIcon_CreateFromFile(&appRef->mWindowIcon, icopath, icopathlength);
	#endif
	WSWndClass_Create(&appRef->mWc, &appRef->mSuper, &appRef->mWindowIcon,
		HelloWorldAppWindowMessages, "WS");

	/* Menu */
	WSMenuActions_Create(&appRef->mIconMenuActions);
	WSMenu_Create(&appRef->mIconMenu, &section, WSMENUALT "File");

	WSMenuSection_AppendWithAccelLit(&section,
		"S" WSMENUALT "how Log Window",
		WM_MYHSLOGWINDOW, menuitem_showlog, &appRef->mLogWindow, "S",
		&appRef->mIconMenuActions);

	WSMenu_EndSection(&appRef->mIconMenu, &section);

	WSMenuSection_AppendWithAccelLit(&section,
		WSMENUALT "Quit", WM_MYSHUTDOWN,
		menuitem_shutdown, &appRef->mLogWindow, "Q",
		&appRef->mIconMenuActions);

	WSMenu_Finalize(&appRef->mIconMenu, &section);


	WSMenuBar_Create(&appRef->mLogMenuBar);

	WSMenuBar_Append(&appRef->mLogMenuBar, &appRef->mIconMenu, WSMENUALT "File");


	/* Window */

	WSWindow_Create(&appRef->mLogWindow, &appRef->mSuper, &bounds, "Hello World",
		&appRef->mLogMenuBar, &appRef->mWc);

	WSWindow_SetCon(&appRef->mLogWindow, (void*)appRef);

	WSTextCtrl_CreateScrolled(&appRef->mLogTextCtrl,
		&appRef->mLogScrolledWindow, &appRef->mLogWindow,
		&bounds, &appRef->mWc
	);

	WSTextCtrl_SetText(&appRef->mLogTextCtrl, "Hello World", 11);

	#if defined(_WIN32)
		WSWindow_SetCon((WSWINDOW*)&appRef->mLogTextCtrl, (void*)appRef);
	#endif

	WSWindow_SetMenuBar(&appRef->mLogWindow, &appRef->mLogMenuBar,
		&appRef->mIconMenuActions);

	WSWindow_SetIconFromFilePath(&appRef->mLogWindow, icopath, icopathlength);


	WSApp_SetMenuBar(&appRef->mSuper, &appRef->mLogMenuBar,
		&appRef->mIconMenuActions);

	HelloWorldApp_ShowHideLogWindow(appRef);
}
void HelloWorldApp_CreateNotifyIcon(HELLOWORLDAPP* appRef)
{
	WSNotifyIcon_Create(&appRef->mNotifyIcon, &appRef->mLogWindow,
		"Hello World", 11, &appRef->mWindowIcon,
		icopath, WM_MYNOTIFYICON);
	/* FreeBSD: The notify icon's icon MUST be set before calling
		set tooltip (set_title) or the icon won't show */

	WSNotifyIcon_SetMenu(&appRef->mNotifyIcon, &appRef->mIconMenu);
	WSApp_SetNotifyIconMenu(&appRef->mSuper, &appRef->mIconMenu);

	WSApp_SetNotifyIcon(&appRef->mSuper, icopath, icopathlength);
}
void HelloWorldApp_DestroyLogWindow(HELLOWORLDAPP* appRef)
{
	WSApp_SetNotifyIconMenu(&appRef->mSuper, NULL);
	WSNotifyIcon_Destroy(&appRef->mNotifyIcon);
	WSTextCtrl_Destroy(&appRef->mLogTextCtrl);
	WSScrolledWindow_Destroy(&appRef->mLogScrolledWindow);
	WSWindow_Destroy(&appRef->mLogWindow);
	WSMenu_Destroy(&appRef->mIconMenu);
	WSMenuBar_Destroy(&appRef->mLogMenuBar);
	WSMenuActions_Destroy(&appRef->mIconMenuActions);
	WSWndClass_Destroy(&appRef->mWc, &appRef->mSuper);
	WSIcon_Destroy(&appRef->mWindowIcon);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						 main()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
wsbool HelloWorldApp_OnInit(HELLOWORLDAPP* pThis)
{
	pThis->mWillShutdown = FALSE;
	WSApp_CreateGUI(&pThis->mSuper);
	return TRUE;
}
 int HelloWorldApp_OnExit(HELLOWORLDAPP* pThis)
{
	WSApp_Destroy(&pThis->mSuper);
	return 0;
}
 wsbool HelloWorldApp_OnIdle(HELLOWORLDAPP* pThis)
{
	if(pThis->mWillShutdown == FALSE)
	{
		//WSThreadYield();
		return TRUE;
	}
	else
		return FALSE;
}
 wsbool HelloWorldApp_OnEventLoopBegin(HELLOWORLDAPP* pThis)
{
	HelloWorldApp_CreateLogWindow(pThis);
	HelloWorldApp_CreateNotifyIcon(pThis);
	return TRUE; /* continue processing */
}
 wsbool HelloWorldApp_OnEventLoopEnd(HELLOWORLDAPP* pThis)
{
	HelloWorldApp_DestroyLogWindow(pThis);
	return FALSE; /* exit */
}

IMPLEMENT_WSAPP(HELLOWORLDAPP,
				HelloWorldApp_OnInit, HelloWorldApp_OnIdle,
				HelloWorldApp_OnEventLoopBegin,
				HelloWorldApp_OnEventLoopEnd,
				HelloWorldApp_OnExit)
