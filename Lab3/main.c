#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "library.h"
#include "graphics.h"

extern void UseLDRB(void *dst, void *src) ;
extern void UseLDRH(void *dst, void *src) ;
extern void UseLDR(void *dst, void *src) ;
extern void UseLDRD(void *dst, void *src) ;
extern void UseLDMIA(void *dst, void *src) ;

void Setup(uint8_t *src, uint8_t *dst) ;
int Check(uint8_t *src, uint8_t *dst) ;
void ShowResult(int which, int index[], unsigned cycles[], unsigned maxCycles) ;

#define	VERSIONS			5
#define	BAR_OFFSET			70
#define	BAR_WIDTH			30
#define	MAX_HEIGHT			210

#define ENTRIES(a) (sizeof(a)/sizeof(a[0]))

uint8_t src[512] __attribute__ ((aligned (8))) ; // Place on a double word boundary
uint8_t dst[512] __attribute__ ((aligned (8))) ; // Place on a double word boundary
uint32_t overhead ;

int main(void)
	{
	unsigned strt, stop, cycles[VERSIONS], maxCycles ;
	int which, index[VERSIONS], srcErr, dstErr ;

	InitializeHardware(HEADER, "Lab #3 (Height = clock cycles)") ;

	srcErr = (((unsigned) src) & 0x3) != 0 ;
	dstErr = (((unsigned) src) & 0x3) != 0 ;
	if (srcErr) printf("src array is NOT word-aligned\n") ;
	if (dstErr) printf("dst array is NOT word-aligned\n") ;
	if (srcErr || dstErr) while (1) ;

	for (which = 0; which < VERSIONS; which++)
		{
		index[which] = -1 ;
		cycles[which] = 0 ;
		}

	strt = GetClockCycleCount() ;
	stop = GetClockCycleCount() ;
	overhead = stop - strt ;

	Setup(src, dst) ;
	strt = GetClockCycleCount() ;
	UseLDRB(dst, src) ;
	stop  = GetClockCycleCount() ;
	cycles[0] = stop - strt - overhead ;
	index[0]  = Check(src, dst) ;

	Setup(src, dst) ;
	strt = GetClockCycleCount() ;
	UseLDRH(dst, src) ;
	stop  = GetClockCycleCount() ;
	cycles[1] = stop - strt - overhead ;
	index[1]  = Check(src, dst) ;

	Setup(src, dst) ;
	strt = GetClockCycleCount() ;
	UseLDR(dst, src) ;
	stop  = GetClockCycleCount() ;
	cycles[2] = stop - strt - overhead ;
	index[2]  = Check(src, dst) ;

	Setup(src, dst) ;
	strt = GetClockCycleCount() ;
	UseLDRD(dst, src) ;
	stop  = GetClockCycleCount() ;
	cycles[3] = stop - strt - overhead ;
	index[3]  = Check(src, dst) ;

	Setup(src, dst) ;
	strt = GetClockCycleCount() ;
	UseLDMIA(dst, src) ;
	stop  = GetClockCycleCount() ;
	cycles[4] = stop - strt - overhead ;
	index[4]  = Check(src, dst) ;

	maxCycles = 0 ;
	for (which = 0; which < VERSIONS; which++)
		{
		if (cycles[which] > maxCycles) maxCycles = cycles[which] ;
		}

	for (which = 0; which < VERSIONS; which++)
		{
		ShowResult(which, index, cycles, maxCycles) ;
		}

	SetColor(COLOR_BLACK) ;
	DrawHLine(0, BAR_OFFSET + MAX_HEIGHT, XPIXELS) ;

	while (1) ;
	return 0 ;
	}

void Setup(uint8_t *src, uint8_t *dst)
	{
	int k ;

	for (k = 0; k < 512; k++)
		{
		*src++ = rand() ;
		*dst++ = rand() ;
		}
	}

int Check(uint8_t *src, uint8_t *dst)
	{
	int k ;

	for (k = 0; k < 512; k++)
		{
		if (*src++ != *dst++) return k ;
		}
	return -1 ;
	}

void ShowResult(int which, int index[], unsigned cycles[], unsigned maxCycles)
	{
	static char *opcode[] = {"LDRB", "LDRH", "LDR", "LDRD", "LDMIA"} ;
	float percent = ((float) cycles[which]) / maxCycles ;
	char text[100] ;
	int x, y ;

	x = (XPIXELS / VERSIONS)*which + XPIXELS / (2*VERSIONS) - BAR_WIDTH / 2 ;
	y = MAX_HEIGHT*(1 - percent) + BAR_OFFSET ;

	SetColor(index[which] >= 0 ? COLOR_RED : COLOR_GREEN) ;
	FillRect(x, y, BAR_WIDTH, (unsigned) (percent*MAX_HEIGHT)) ;

	SetColor(COLOR_BLACK) ;
	sprintf(text, "%4u", cycles[which]) ;
	DisplayStringAt(x, y - 12, (uint8_t *) text) ;
	DisplayStringAt(x, BAR_OFFSET + MAX_HEIGHT + 5, (uint8_t *) opcode[which]) ;

	if (index[which] < 0) return ;
	sprintf(text, "Copy512%c: Failed at Index %d!\n", "ABC"[which], index[which]) ;
	SetColor(COLOR_BLACK) ;
	DisplayStringAt(15, YPIXELS/3 + 15*which, (uint8_t *) text) ;
	}

