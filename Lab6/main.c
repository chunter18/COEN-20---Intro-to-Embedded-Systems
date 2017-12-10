/*
	This code was written to support the book, "ARM Assembly for Embedded Applications",
	by Daniel W. Lewis. Permission is granted to freely share this software provided
	that this notice is not removed. This software is intended to be used with a
	special run-time library written by the author for the EmBitz IDE, using the
	"Sample Program for the 32F429IDISCOVERY Board" as an example, and available for
	download from http://www.engr.scu.edu/~dlewis/book3.
*/

#include <stdio.h>
#include <stdlib.h> //include for my random implementation
#include <stdint.h>
#include <stdarg.h>
#include "library.h"
#include "graphics.h"

#define ENTRIES(a) (sizeof(a)/sizeof(a[0]))

extern unsigned CallReturnOverhead(unsigned word) ;
extern unsigned ReverseBits(unsigned word) ;
extern unsigned ReverseBytes(unsigned word) ;

#define	ROW_HEIGHT		14
#define	FIRST_ROW		60
#define	BYTE_SPACING	3
#define	BIN_COL			(5 - BYTE_SPACING)
#define	LABEL_COL		BIN_COL
#define	PROMPT_COL		(LABEL_COL + 15)

int PutStringAt(int row, int col, char *fmt, ...) ;
int PutBinaryAt(int row, int col, int32_t input, int32_t output, int bits) ;
int32_t ReversedBits(int32_t input) ;
int32_t ReversedBytes(int32_t input) ;
int GetRandomNumber(void);

//time_t t;
//srand((unsigned) time(&t)); //game me errors, proceeding without seed

int main(void)
    {
	unsigned strt, stop, overhead, dummy = 0 ;
	int32_t input, output, verify ;
	int row ;

	InitializeHardware(HEADER, "Lab 6: Reversing Bits & Bytes") ;

    strt = GetClockCycleCount() ;
	dummy = CallReturnOverhead(dummy) ;
    stop = GetClockCycleCount() ;
	overhead = stop - strt ;

	input = 0x87000000 ;
	while (1)
		{
		row = FIRST_ROW ;

        strt   = GetClockCycleCount() ;
		output = ReverseBits(input) ;
        stop   = GetClockCycleCount() ;
		verify = ReversedBits(input) ;

		row = PutStringAt(row, LABEL_COL, "ReverseBits (%d clock cycles)", stop - strt - overhead) ;
		DrawHLine(LABEL_COL, row, XPIXELS - 2*LABEL_COL) ;

		row += 6 ;

		row = PutStringAt(row, LABEL_COL, "Input (0x%08X):", input) ;
		row = PutBinaryAt(row, BIN_COL, input, input, 1) ;

		row += 4 ;

		row = PutStringAt(row, LABEL_COL, "Output (0x%08X):", output) ;
		row = PutBinaryAt(row, BIN_COL, verify, output, 1) ;

        strt   = GetClockCycleCount() ;
		output = ReverseBytes(input) ;
        stop   = GetClockCycleCount() ;
		verify = ReversedBytes(input) ;

		row += 2*ROW_HEIGHT ;

		row = PutStringAt(row, LABEL_COL, "ReverseBytes (%d clock cycles)", stop - strt - overhead) ;
		DrawHLine(LABEL_COL, row, XPIXELS - 2*LABEL_COL) ;

		row += 6 ;

		row = PutStringAt(row, LABEL_COL, "Input (0x%08X):", input) ;
		row = PutBinaryAt(row, BIN_COL, input, input, 8) ;

		row += 4 ;

		row = PutStringAt(row, LABEL_COL, "Output (0x%08X):", output) ;
		row = PutBinaryAt(row, BIN_COL, verify, output, 8) ;

		row += 2*ROW_HEIGHT ;

		PutStringAt(row, PROMPT_COL, "Push button: Random test") ;
		WaitForPushButton() ;
		input = GetRandomNumber() ;
		}

	while (1) ;
	return 0 ;
    }

int PutStringAt(int row, int col, char *fmt, ...)
	{
	va_list args ;
	char text[100] ;

	va_start(args, fmt) ;
	vsprintf(text, fmt, args) ;
	DisplayStringAt(col, row, (uint8_t *) text) ;
	va_end(args) ;
	return row + ROW_HEIGHT ;
	}

int PutBinaryAt(int row, int col, int32_t input, int32_t output, int bits)
	{
	int group, bit ;
	int32_t mask ;

	mask = (bits == 1) ? 0x80000000 : 0xFF000000 ;
	for (group = 0; group < 32/bits; group++)
		{
		if (((input ^ output) & mask) != 0)
			{
			SetForeground(COLOR_WHITE) ;
			SetBackground(COLOR_RED) ;
			}
		for (bit = 0; bit < bits; bit++)
			{
			if (((group*bits + bit) % 8) == 0) col += BYTE_SPACING ;
			DisplayChar(col, row, '0' + (output < 0)) ;
			input  <<= 1 ;
			output <<= 1 ;
			col += 7 ;
			}
		SetForeground(COLOR_BLACK) ;
		SetBackground(COLOR_WHITE) ;
		}

	return row + ROW_HEIGHT ;
	}

int32_t ReversedBits(int32_t input)
	{
	int32_t output ;
	int k ;

	output = 0 ;
	for (k = 0; k < 32; k++)
		{
		output = (output << 1) | (input & 1) ;
		input >>= 1 ;
		}

	return output ;
	}

int32_t ReversedBytes(int32_t input)
	{
	int32_t output ;
	int k ;

	output = 0 ;
	for (k = 0; k < 4; k++)
		{
		output = (output << 8) | (input & 0xFF) ;
		input >>= 8 ;
		}

	return output ;
	}

int GetRandomNumber(void)
{
    int val = rand();
    return val;
}
