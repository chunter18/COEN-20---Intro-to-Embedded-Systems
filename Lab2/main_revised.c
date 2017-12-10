#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "library.h"

extern unsigned Ten32(void) ;
extern unsigned long long Ten64(void) ;
extern unsigned Incr(unsigned n) ;
extern unsigned Nested1(void) ;
extern unsigned Nested2(void) ;
extern void PrintTwo(char *format, unsigned number) ;

#define	pRCC_AHB2ENR	((uint32_t *)			0x40023834)
#define	pRNG_CR			((uint32_t *)			0x50060800)
#define	pRNG_SR			((volatile uint32_t *)	0x50060804)
#define	pRNG_DR			((volatile uint32_t *)	0x50060808)

void RNG_Initialize(void) ;
uint32_t RNG_ReadValue(void) ;

#define	MS32BITS(a)	((unsigned *) &a)[1]
#define	LS32BITS(a)	((unsigned *) &a)[0]

int main(void)
	{
	unsigned seed, a32, b32 ;
	uint64_t c64 ;

	InitializeHardware(HEADER, "LAB #2: Calling Functions") ;

	printf("\n") ;

	a32 = Ten32() ;
	printf("Ten32() =         0x%08X", a32) ;
	printf("  %s\n", (a32 == 10) ? "OK " : "ERR") ;
	printf("\n") ;

	c64 = Ten64() ;
	printf("Ten64() = 0x%08X%08X", MS32BITS(c64), LS32BITS(c64)) ;
	printf("  %s\n", (c64 == 10ULL) ? "OK " : "ERR") ;
	printf("\n") ;

	RNG_Initialize() ;
	seed = RNG_ReadValue() ;
	srand(seed) ;

	a32 = rand() % 1000000000 ;
	b32 = Incr(a32) ;
	printf("Incr(%09u) =  %09u", a32, b32) ;
	printf("  %s\n", (b32 == a32 + 1) ? "OK " : "ERR") ;
	printf("\n") ;

	seed = RNG_ReadValue() ;
	srand(seed) ;
	a32 = rand() + 1 ;
	srand(seed) ;
	b32 = Nested1() ;
	printf("Nested1() =       %10u", b32) ;
	printf("  %s\n", (b32 == a32) ? "OK " : "ERR") ;
	printf("\n") ;

	seed = RNG_ReadValue() ;
	srand(seed) ;
	a32 = rand() + rand() ;
	srand(seed) ;
	b32 = Nested2() ;
	printf("Nested2() =       %10u", b32) ;
	printf("  %s\n", (b32 == a32) ? "OK " : "ERR") ;
	printf("\n") ;

	printf("\n") ;

	printf("Next numbers must be consecutive:\n\n") ;
	a32 = RNG_ReadValue() % 10000000 ;
	PrintTwo("number = %u\n", a32) ;

	printf("\n--- FINISHED! ---\n") ;
	while (1) ;
	return 0 ;
	}

void RNG_Initialize(void)
	{
	*pRCC_AHB2ENR |= (1 << 6) ;
	*pRNG_CR	  |= (1 << 2) ;
	}

uint32_t RNG_ReadValue(void)
	{
	while ((*pRNG_SR & 1) == 0) ;
	return *pRNG_DR ;
	}

