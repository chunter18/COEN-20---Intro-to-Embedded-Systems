#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "library.h"
#include "graphics.h"

extern void DeleteItem(int data[], int items, int index) ;
extern void InsertItem(int data[], int items, int index, int value) ;

#define ENTRIES(a) (sizeof(a)/sizeof(a[0]))

void InitArray(int data[], int items) ;
void CopyArray(int dst[], int src[], int items) ;
int  DelOK(int orig[], int del[], int row, int index) ;
int  InsOK(int orig[], int del[], int row, int index) ;
void DisplayResults(int orig[], int del[], int ins[], int index) ;

#define	VALUE_INS	0
#define	ITEMS		10


int main(void)
	{
	unsigned int strt, stop, overhead, cycles ;
	int orig[ITEMS], del[ITEMS], ins[ITEMS+1], index ;

	InitializeHardware(HEADER, "Lab 5: Inserting & Deleting") ;

	strt = GetClockCycleCount() ;
	stop = GetClockCycleCount() ;
	overhead = stop - strt ;

	index = 0 ;
	while (1)
		{
		char text[100] ;

		InitArray(orig, ITEMS) ;

		CopyArray(del, orig, ITEMS) ;
		strt = GetClockCycleCount() ;
		DeleteItem(del, ITEMS, index) ;
		stop = GetClockCycleCount() ;
		cycles = stop - strt - overhead ;
		sprintf(text, "Del at index %d: %d Clock Cycles", index, cycles) ;
		SetColor(COLOR_BLACK) ;
		DisplayStringAt(5, 55, (uint8_t *) text) ;

		CopyArray(ins, orig, ITEMS) ;
		ins[ITEMS] = -1 ; // Must not overwrite this!
		strt = GetClockCycleCount() ;
		InsertItem(ins, ITEMS, index, VALUE_INS) ;
		stop = GetClockCycleCount() ;
		cycles = stop - strt - overhead ;
		sprintf(text, "Ins at index %d: %d Clock Cycles", index, cycles) ;
		SetColor(COLOR_BLACK) ;
		DisplayStringAt(5, 70, (uint8_t *) text) ;

		DisplayResults(orig, del, ins, index) ;

		SetColor(COLOR_BLACK) ;
		DisplayStringAt(15, 285, (uint8_t *) "Next index: Press blue button") ;
		WaitForPushButton() ;

		index = (index + 1) % ITEMS ;
		}

	while (1) ;
	return 0 ;
	}

void InitArray(int data[], int items)
	{
	int k ;

	for (k = 0; k < ITEMS; k++)
		{
		int value ;

		do value = rand() % 1000 ;
		while (value < 100) ;
		data[k] = value ;
		}
	}

void CopyArray(int dst[], int src[], int items)
	{
	int k ;

	for (k = 0; k < items; k++)
		{
		dst[k] = src[k] ;
		}
	}

void DisplayResults(int orig[], int del[], int ins[], int index)
	{
	int row, x, y ;
	char text[10] ;

	SetColor(COLOR_BLACK) ;
	DisplayStringAt(5, 100, (uint8_t *) "Index   Orig     Del      Ins") ;

	for (row = 0; row < ITEMS; row++)
		{
		x = 5 ;
		y = 15*row + 115 ;

		sprintf(text, "%3d", row) ;
		SetColor(COLOR_BLACK) ;
		DisplayStringAt(x, y, (uint8_t *) text) ;

		x += XPIXELS/4 ;
		sprintf(text, "%3d", orig[row]) ;
		DisplayStringAt(x, y, (uint8_t *) text) ;

		x += XPIXELS/4 ;
		sprintf(text, "%3d", del[row]) ;
		SetColor(DelOK(orig, del, row, index) ? COLOR_BLACK : COLOR_RED) ;
		DisplayStringAt(x, y, (uint8_t *) text) ;

		x += XPIXELS/4 ;
		sprintf(text, "%3d", ins[row]) ;
		SetColor(InsOK(orig, ins, row, index) ? COLOR_BLACK : COLOR_RED) ;
		DisplayStringAt(x, y, (uint8_t *) text) ;
		}

	if (ins[ITEMS] != -1)
		{
		SetColor(COLOR_RED) ;
		DisplayStringAt(10, 15*ITEMS + 130, (uint8_t *) "Insert wrote beyond the array!") ;
		}
	}

int DelOK(int orig[], int del[], int row, int index)
	{
	if (row < index && del[row]==orig[row]) return 1 ;
	if (row == ITEMS-1 && del[row]==orig[row])  return 1 ;
	return del[row] == orig[row+1] ;
	}

int InsOK(int orig[], int ins[], int row, int index)
	{
	if (row < index && ins[row]==orig[row])  return 1 ;
	if (row == index && ins[row]==VALUE_INS) return 1 ;
	return row < ITEMS && ins[row]==orig[row-1] ;
	}
