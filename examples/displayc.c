#define HAVE_XRANDR
#define WSDISPLAY_IMPLEMENTATION
#include "display.h"
#include <stdio.h>
#define WSConsole_Write(x, y) printf("%s", y)
#define WSConsole_WriteUInt(x, y) printf("%u", y)

void printdisplayinfo(WSDISPLAY* d)
{
	char bufff[5000];
	WSRECT r;
	WSVIDEOMODE* modes;
	WSVIDEOMODE curmode;
	float scalefactorx;
	guipos_t x, y;
	unsigned int modecount;
	WSDisplay_GetName(d, bufff, sizeof(bufff));
	printf("Display Name:%s\n", bufff);
	WSDisplay_GetPPI(d, &x, &y);
	printf("Display PPI:[%u, %u]\n", (unsigned)x, (unsigned)y);
	WSDisplay_GetDepth(d, &x);
	printf("Display Depth:%u\n", (unsigned)x);
	WSDisplay_GetScaleFactor(d, &scalefactorx);
	printf("Display Scale Factor:%f\n", scalefactorx);
	WSDisplay_GetGeometry(d, &r);
	printf("Display Geometry:[L:%u, T:%u, R:%u, B:%u]\n",
		(unsigned)WSRect_GetX(&r), (unsigned)WSRect_GetY(&r),
		(unsigned)(WSRect_GetX(&r) + WSRect_GetWidth(&r)),
		(unsigned)(WSRect_GetHeight(&r) + WSRect_GetY(&r)));
	WSDisplay_GetClientArea(d, &r);
	printf("Display Client Area:[L:%u, T:%u, R:%u, B:%u]\n",
		(unsigned)WSRect_GetX(&r), (unsigned)WSRect_GetY(&r),
		(unsigned)(WSRect_GetX(&r) + WSRect_GetWidth(&r)),
		(unsigned)(WSRect_GetHeight(&r) + WSRect_GetY(&r)));

	WSDisplay_GetCurrentMode(d, &curmode);
	WSConsole_Write(&con, "Current Mode ");
	WSConsole_Write(&con, ": [");
	WSConsole_WriteUInt(&con, curmode.width);
	WSConsole_Write(&con, ", ");
	WSConsole_WriteUInt(&con, curmode.height);
	WSConsole_Write(&con, ", ");
	WSConsole_WriteUInt(&con, curmode.depth);
	WSConsole_Write(&con, ", ");
	WSConsole_WriteUInt(&con, curmode.refreshrate);
	WSConsole_Write(&con, "]\n");
	WSDisplay_GetModes(d, NULL, &modes, &modecount);
	for(unsigned int i = 0; i < modecount; ++i)
	{
		WSConsole_Write(&con, "Mode #");
		WSConsole_WriteUInt(&con, i);
		WSConsole_Write(&con, ": [");
		WSConsole_WriteUInt(&con, modes[i].width);
		WSConsole_Write(&con, ", ");
		WSConsole_WriteUInt(&con, modes[i].height);
		WSConsole_Write(&con, ", ");
		WSConsole_WriteUInt(&con, modes[i].depth);
		WSConsole_Write(&con, ", ");
		WSConsole_WriteUInt(&con, modes[i].refreshrate);
		WSConsole_Write(&con, "]\n");
	}
	WSDisplay_ChangeMode(d, &curmode);
}

int main(int argc, char** argv)
{
	WSDISPLAY d;
	WSDISPLAYITERATOR dit;
	unsigned num = 1;
	/*WSDisplayGetStdPPIValue(&def);*/
	WSDisplayBegin(&dit);
	for(; !WSDisplayIsEnd(&dit); WSDisplayIterator_Iterate(&dit), ++num)
	{
		WSDisplayIterator_Value(&dit, &d);
		printf("----------------Monitor [%u]---------------\n", num);
		printdisplayinfo(&d);
		WSDisplay_Destroy(&d);
	}

	printf("----------------Monitor [Primary]---------------\n");
	WSDisplayGetPrimary(&d);
	printdisplayinfo(&d);
	WSDisplay_Destroy(&d);
	printf("----------------[Done]---------------\n");
	return 0;
}
