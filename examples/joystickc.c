#define WSJOYSTICK_IMPLEMENTATION
#define WSJOYSTICKXINPUTOPT
#include "joystick.h"
#include "stopwatch.h"
#include <stdio.h>
#if defined(_WIN32)
	#include <Windows.h> /* MSG */
#endif
#define WSConsole_Write(x, y) printf("%s", y)
#define WSConsole_WriteUInt(x, y) printf("%u", y)

int main(int argc, char** argv)
{
	size_t curval;
	WSSTOPWATCH sw;
	int xmin;
	int xmax;
	int xmidpoint;
	int xthreshold;
	int ymin;
	int ymax;
	int ymidpoint;
	int ythreshold;
	int zmin;
	int zmax;
	int zmidpoint;
	int zthreshold;
	int umin;
	int umax;
	int umidpoint;
	int uthreshold;
	int vmin;
	int vmax;
	int vmidpoint;
	int vthreshold;
	int rmin;
	int rmax;
	int rmidpoint;
	int rthreshold;
	size_t lastjoyinterval = 0;
	int joyfound = 0;
	WSJOYSTICK joy;
	WSJOYSTICKITERATOR jit;
	WSJOYSTICKCAPS jc;
	const char* joyname;
	const int threshold_up = -24576;   /* Threshold for UP direction */
	const int threshold_right = -8192; /* Threshold for RIGHT direction */
	const int threshold_down = 8192;   /* Threshold for DOWN direction */
	const int threshold_left = 24576;  /* Threshold for LEFT direction */
#if defined(_WIN32) && !defined(WSJOYSTICKXINPUTOPT)
	HINSTANCE hInstance = GetModuleHandleA(NULL);
	/* NB: mmsystem doesn't work unless focusing on window */
	WNDCLASSA wc =
		{ 0, DefWindowProcA, 0, 0,
		hInstance, NULL, NULL,
		NULL, NULL, "WS" };
	ATOM atom = RegisterClassA(&wc); /*!atom == error*/
	HWND hWnd = CreateWindowA("WS", "Test", 0,  0, 0,
		500, 500, NULL, NULL, hInstance, 0);
	ShowWindow(
		hWnd,
		SW_SHOW);
#endif
	WSJoystickBegin(&jit);
	for(; !WSJoystickIsEnd(&jit); WSJoystickIterator_Iterate(&jit))
	{
		WSJoystickIterator_Value(&jit, &joy);
		if(WSJoystick_IsCreated(&joy))
		{
			joyfound = 1;
			break;
		}
	}
	WSJoystickIterator_Destroy(&jit);
	if(!joyfound)
	{
		printf("No joystick found...\n");
		return -1;
	}
	WSJoystick_GetCaps(&joy, &jc);
	joyname = WSJoystickCaps_GetProductName(&jc, &joy);
	printf("Joystick [%s] found!\n", joyname);
	xmin = WSJoystickCaps_GetXMin(&jc);
	xmax = WSJoystickCaps_GetXMax(&jc);
	xmidpoint = (xmin + xmax) / 2;
	xthreshold = (xmax - xmin) / 4;
	ymin = WSJoystickCaps_GetYMin(&jc);
	ymax = WSJoystickCaps_GetYMax(&jc);
	ymidpoint = (ymin + ymax) / 2;
	ythreshold = (ymax - ymin) / 4;
	zmin = WSJoystickCaps_GetZMin(&jc);
	zmax = WSJoystickCaps_GetZMax(&jc);
	zmidpoint = (zmin + zmax) / 2;
	zthreshold = (zmax - zmin) / 4;
	umin = WSJoystickCaps_GetUMin(&jc);
	umax = WSJoystickCaps_GetUMax(&jc);
	umidpoint = (umin + umax) / 2;
	uthreshold = (umax - umin) / 4;
	vmin = WSJoystickCaps_GetVMin(&jc);
	vmax = WSJoystickCaps_GetVMax(&jc);
	vmidpoint = (vmin + vmax) / 2;
	vthreshold = (vmax - vmin) / 4;
	rmin = WSJoystickCaps_GetRudderMin(&jc);
	rmax = WSJoystickCaps_GetRudderMax(&jc);
	rmidpoint = (rmin + rmax) / 2;
	rthreshold = (rmax - rmin) / 4;
	WSStopWatch_Construct(&sw);
	while((curval = WSStopWatch_Value(&sw)) < 30000)
	{
		#if defined(_WIN32) /* process an event for windows */
			MSG msg;
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)
				&& msg.message != WM_QUIT)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if(msg.message == WM_QUIT)
				break; /* quit */
		#endif
		if(curval >= 260 && curval - lastjoyinterval >= 80)
		{
			/* 7000 sensitivity, 260 initial delay, 80 repeat delay
			0 == up, 9000 == right, 18000 == down, 27000 == left, else none
			zaxis triggers:
			128-32766 == right, 32767 == none, 32768-65408 == left
			POV == dpad,
			x/y axis == left stick,
			u/r axis == right stick

			A == 0, B == 1, X == 2, Y == 3, LSHOULDER == 4, RSHOULDER == 5
			SELECT == 6, START == 7
			*/
			WSJOYSTICKSTATE js;
			WSJoystick_PollState(&joy, &js);
			int xpos = WSJoystickState_GetXPosition(&js);
			int ypos = WSJoystickState_GetYPosition(&js);
			int zpos = WSJoystickState_GetZPosition(&js);
			int upos = WSJoystickState_GetUPosition(&js);
			int vpos = WSJoystickState_GetVPosition(&js);
			int rpos = WSJoystickState_GetRudderPosition(&js);
			int pov = WSJoystickState_GetPOVPosition(&js);
			int center_value = WSJoystickCaps_GetPOVCenter(&jc);
			int min_value = WSJoystickCaps_GetPOVMin(&jc);
			int max_value = WSJoystickCaps_GetPOVMax(&jc);
			int scaled_value;
			int ibutton;
			int numbuttons = WSJoystickCaps_GetNumberButtons(&jc);
			for(ibutton = 0; ibutton < numbuttons; ++ibutton)
			{
				if(WSJoystickState_GetButtonState(&js, ibutton))
				{
					WSConsole_Write(&con, "Joystick button #[");
					WSConsole_WriteUInt(&con, ibutton);
					WSConsole_Write(&con, "] is pressed\n");
					lastjoyinterval = curval;
				}
			}

			if (xpos < xmidpoint - xthreshold || xpos > xmidpoint + xthreshold)
			{
				WSConsole_Write(&con, "Joystick Left Stick X [");
				WSConsole_WriteUInt(&con, xpos);
				WSConsole_Write(&con, "]\n");
				lastjoyinterval = curval;
			}
			if (ypos < ymidpoint - ythreshold || ypos > ymidpoint + ythreshold)
			{
				WSConsole_Write(&con, "Joystick Left Stick Y [");
				WSConsole_WriteUInt(&con, ypos);
				WSConsole_Write(&con, "]\n");
				lastjoyinterval = curval;
			}
			if(zmin != 0)
			{
				if (zpos < zmidpoint - zthreshold || zpos > zmidpoint + zthreshold)
				{
					if(zpos < zmidpoint - zthreshold)
						WSConsole_Write(&con, "Joystick RT [");
					else
						WSConsole_Write(&con, "Joystick LT [");
					WSConsole_WriteUInt(&con, zpos);
					WSConsole_Write(&con, "]\n");
					lastjoyinterval = curval;
				}
			}
			else
			{
				if (zpos > zmidpoint + zthreshold)
				{
					WSConsole_Write(&con, "Joystick LT [");
					WSConsole_WriteUInt(&con, zpos);
					WSConsole_Write(&con, "]\n");
					lastjoyinterval = curval;
				}
				if (vpos > vmidpoint + vthreshold)
				{
					WSConsole_Write(&con, "Joystick RT [");
					WSConsole_WriteUInt(&con, vpos);
					WSConsole_Write(&con, "]\n");
					lastjoyinterval = curval;
				}
			}
			if (upos < umidpoint - uthreshold || upos > umidpoint + uthreshold)
			{
				WSConsole_Write(&con, "Joystick Right Stick X [");
				WSConsole_WriteUInt(&con, upos);
				WSConsole_Write(&con, "]\n");
				lastjoyinterval = curval;
			}
			if (rpos < rmidpoint - rthreshold || rpos > rmidpoint + rthreshold)
			{
				WSConsole_Write(&con, "Joystick Right Stick Y [");
				WSConsole_WriteUInt(&con, rpos);
				WSConsole_Write(&con, "]\n");
				lastjoyinterval = curval;
			}
			/* Scale the value to a range between -32767 and 32767 */

			if (center_value < min_value || center_value > max_value)
				center_value = (min_value + max_value) / 2;
			if (pov < min_value || pov > max_value)
				pov = (min_value + max_value) / 2;


			scaled_value =
				(pov - center_value) * 32767 / ((max_value - min_value) / 2);
			if(scaled_value)
			{
				if (scaled_value < threshold_up)
				{
					WSConsole_Write(&con, "DPAD [UP]\n");
					lastjoyinterval = curval;
				}
				else if (scaled_value < threshold_right)
				{
					WSConsole_Write(&con, "DPAD [RIGHT]\n");
					lastjoyinterval = curval;
				}
				else if (scaled_value < threshold_down)
				{
					WSConsole_Write(&con, "DPAD [DOWN]\n");
					lastjoyinterval = curval;
				}
				else if (scaled_value < threshold_left)
				{
					WSConsole_Write(&con, "DPAD [LEFT]\n");
					lastjoyinterval = curval;
				}
			} /* end if scaled */
		} /* end if time threshold */
	} /* end if still running */
	WSStopWatch_Destruct(&sw);
	if(joyfound)
		WSJoystick_Destroy(&joy);
#if defined(_WIN32) && !defined(WSJOYSTICKXINPUTOPT)
	DestroyWindow(hWnd);
	UnregisterClassA("WS", hInstance);
#endif
	return 0;
}