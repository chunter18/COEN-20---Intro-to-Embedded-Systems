#include <stdio.h>
#include <stdint.h>
#include "library.h"
#include "graphics.h"

extern int CallReturnOverhead(unsigned dummy) ;
extern int SDIVby13(int dividend) ;
extern int MySDIVby13(int dividend) ;
extern unsigned UDIVby13(unsigned dividend) ;
extern unsigned MyUDIVby13(unsigned dividend) ;

#define	ENTRIES(a)	(sizeof(a)/sizeof(a[0]))

void DisplayResults(int test, char *label, unsigned works[], unsigned cycles[]) ;

uint32_t dividends[] =
	{
	0xFFFFFF80,	// 15
	0xFFFFF800,	// 16
	0xFFFF8000,	// 17
	0xFFF80000,	// 18
	0xFF800000,	// 19
	0xF8000000,	// 20
	0x80000000,	// 21
	0x08000000,	// 20
	0x00800000,	// 19
	0x00080000,	// 18
	0x00008000,	// 17
	0x00000800	// 16
	} ;

int main(void)
    {
	static unsigned avgs[] = {0, 0, 0, 0} ;
	unsigned strt, stop, overhead, works[4] ;
	uint32_t dummy = 0 ;
	int which, div ;

	InitializeHardware(HEADER, "Lab 7: Division by a Constant") ;

    strt  = GetClockCycleCount() ;
	dummy = CallReturnOverhead(dummy) ;
    stop  = GetClockCycleCount() ;
	overhead = stop - strt ;

	DisplayStringAt(5, 60, (uint8_t *) "Dividend  SDIV MySDIV UDIV MyUDIV") ;
	DrawHLine(5, 72, XPIXELS - 10) ;

	for (which = 0; which < ENTRIES(dividends); which++)
		{
		unsigned cycles[4] ;
		int qSDIV, qMySDIV;
		unsigned qUDIV, qMyUDIV ;
		uint32_t dividend ;
		char label[10] ;

		dividend = dividends[which] ;

        strt = GetClockCycleCount() ;
		qSDIV = SDIVby13((int32_t) dividend) ;
        stop = GetClockCycleCount() ;
		works[0] = (qSDIV == ((int) dividend / 13)) ;
		cycles[0] = stop - strt - overhead ;
		avgs[0] += cycles[0] ;

        strt = GetClockCycleCount() ;
		qMySDIV = MySDIVby13((int32_t) dividend) ;
        stop = GetClockCycleCount() ;
		works[1] = (qMySDIV == ((int) dividend / 13)) ;
		cycles[1] = stop - strt - overhead ;
		avgs[1] += cycles[1] ;

        strt = GetClockCycleCount() ;
		qUDIV = UDIVby13((uint32_t) dividend) ;
        stop = GetClockCycleCount() ;
		works[2] = (qUDIV == ((unsigned) dividend / 13)) ;
		cycles[2] = stop - strt - overhead ;
		avgs[2] += cycles[2] ;

        strt = GetClockCycleCount() ;
		qMyUDIV = MyUDIVby13((unsigned) dividend) ;
        stop = GetClockCycleCount() ;
		works[3] = (qMyUDIV == ((unsigned) dividend / 13)) ;
		cycles[3] = stop - strt - overhead ;
		avgs[3] += cycles[3] ;

		sprintf(label, "%08X", (unsigned) dividend) ;
		DisplayResults(which, label, works, cycles) ;
		}

	for (div = 0; div < 4; div++)
		{
		works[div] = 1 ;
		avgs[div] = (avgs[div] + which/2) / which ;
		}
	DisplayResults(which + 1, "Average", works, avgs) ;

	while (1) ;
	return 0 ;
    }

void DisplayResults(int test, char *label, unsigned works[], unsigned cycles[])
	{
	int x, y, div ;
	char text[10] ;

	x = 5 ;
	y = 15*test + 80 ;

	SetColor(COLOR_BLACK) ;
	DisplayStringAt(x, y, (uint8_t *) label) ;

	x = 78 ;
	for (div = 0; div < 4; div++)
		{
		sprintf(text, "%3d", cycles[div]) ;
		if (!works[div])
			{
			SetForeground(COLOR_WHITE) ;
			SetBackground(COLOR_RED) ;
			}
		DisplayStringAt(x, y, (uint8_t *) text) ;
		SetForeground(COLOR_BLACK) ;
		SetBackground(COLOR_WHITE) ;
		x += 40 ;
		}
	}
