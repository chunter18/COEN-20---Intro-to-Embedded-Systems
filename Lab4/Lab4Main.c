#include <stdio.h>
#include <stdint.h>
#include "library.h"
#include "graphics.h"

extern int Discriminant(int32_t a, int32_t b, int32_t c) ;
extern int Root1(int32_t a, int32_t b, int32_t sqrt_d) ;
extern int Root2(int32_t a, int32_t b, int32_t sqrt_d) ;

int isqrt(int x) ;
void PlotQuadratic(int a, int b, int c) ;

typedef struct
	{
	int	a, b, c ;
	int d ;
	int r1, r2 ;
	} TESTCASE ;

#define	ENTRIES(a)	(sizeof(a)/sizeof(a[0]))
#define	GFXCOLS		XPIXELS
#define	GFXROWS		(YPIXELS-60)
#define	GFXCOL1		0
#define	GFXROW1		45

int main(void)
	{
	static TESTCASE testcase[] =
		{
		{1, -2, -15, 64, 5, -3},
		{2, 4, 2, 0, -1, -1},
		{1, 1, 3, -11, 0, 0}
		} ;
	int a, b, c, d, r1, r2 ;
	int my_d, my_r1, my_r2 ;
	char text[100] ;
	int k, x, y ;

	InitializeHardware(HEADER, "LAB #4 (Blue Button: Next Quad)") ;

	k = 0 ;
	x = 10 ;
	while (1)
		{
		SetColor(COLOR_WHITE) ;
		FillRect(GFXCOL1, GFXROW1, GFXCOLS, GFXROWS) ;

		a = testcase[k].a ;
		b = testcase[k].b ;
		c = testcase[k].c ;
		d = testcase[k].d ;
		r1 = testcase[k].r1 ;
		r2 = testcase[k].r2 ;

		y = GFXROW1 + 5 ;
		SetColor(COLOR_BLACK) ;

		sprintf(text, "Quadratic: a=%d, b=%d, c=%d\n", a, b, c) ;
		DisplayStringAt(x, y, (uint8_t *) text) ;
		y += 15 ;

		sprintf(text, "Discriminant(%d,%d,%d) = %d\n", a, b, c, d) ;
		DisplayStringAt(x, y, (uint8_t *) text) ;
		y += 15 ;

		my_d = Discriminant(a, b, c) ;
		if (my_d != d)
			{
			sprintf(text, "Your incorrect discriminant: %d\n", my_d) ;
			SetColor(COLOR_RED) ;
			DisplayStringAt(x, y, (uint8_t *) text) ;
			continue ;
			}

		if (d < 0)
			{
			sprintf(text, "Roots are imaginary!\n") ;
			DisplayStringAt(x, y, (uint8_t *) text) ;
			y += 15 ;
			}
		else if (d > 0)
			{
			sprintf(text, "Roots at x=%d and x=%d\n", r1, r2) ;
			DisplayStringAt(x, y, (uint8_t *) text) ;
			y += 15 ;

			my_r1 = Root1(a, b, isqrt(d)) ;
			my_r2 = Root2(a, b, isqrt(d)) ;
			if ((my_r1 != r1 || my_r2 != r2) && (my_r1 != r2 || my_r2 != r1))
				{
				sprintf(text, "Your incorrect roots: %d and %d\n", my_r1, my_r2) ;
				SetColor(COLOR_RED) ;
				DisplayStringAt(x, y, (uint8_t *) text) ;
				continue ;
				}
			}
		else // if (d == 0)
			{
			r1 = -b / (2 * a) ;
			sprintf(text, "Single root at x=%d\n", r1) ;
			DisplayStringAt(x, y, (uint8_t *) text) ;
			y += 15 ;

			my_r1 = Root1(a, b, isqrt(d)) ;
			if (my_r1 != r1)
				{
				SetColor(COLOR_RED) ;
				sprintf(text, "Your incorrect root: %d\n", my_r1) ;
				DisplayStringAt(x, y, (uint8_t *) text) ;
				continue ;
				}
			}

		PlotQuadratic(a, b, c) ;
		WaitForPushButton() ;
		k = (k + 1) % ENTRIES(testcase) ;
		}

	return 0 ;
	}

// Integer square root - adapted from Warren, "Hacker's Delight", Addison Wesley, 2003.
int isqrt(int x)
	{
	unsigned int m, y, b ;

	y = 0 ;
	m = 0x40000000 ;
	while (m != 0)	// repeats 16 times
		{
		b = y | m ;
		y = y >> 1 ;
		if (b <= x)
			{
			x = x - b ;
			y = y | m ;
			}
		m = m >> 2 ;
		}
	return y ;
	}

void PlotQuadratic(int a, int b, int c)
	{
#	define	XMIN	-10
#	define	XMAX	+10
#	define	YMIN	-20
#	define	YMAX	+20
#	define	XFCTR	(((float) GFXCOLS)/(XMAX-XMIN))
#	define	YFCTR	((float) GFXROWS)/(YMAX-YMIN)

	int row, col ;

	SetColor(COLOR_BLUE) ;
	for (col = 0; col < GFXCOLS; col++)
		{
		float x = (float) col / XFCTR + XMIN ;
		float y = a*x*x + b*x + c ;
		row = (int) (YFCTR * (y - YMIN)) ;
		if (0 <= row && row < GFXROWS) FillCircle(col + GFXCOL1, GFXROW1 + GFXROWS - row - 1, 1) ;
		}

	SetColor(COLOR_BLACK) ;
	DrawLine(GFXCOL1, GFXROW1 + GFXROWS/2, GFXCOL1 + GFXCOLS - 1, GFXROW1 + GFXROWS/2) ;
	DrawLine(GFXCOL1 + GFXCOLS/2, GFXROW1, GFXCOL1 + GFXCOLS/2, GFXROW1 + GFXROWS - 1) ;
	}

