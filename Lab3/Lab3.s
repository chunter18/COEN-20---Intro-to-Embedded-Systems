        .syntax     unified
        .cpu        cortex-m4
        .text
        .thumb_func
        .align       2

// void UseLDRB(void *dst, void *src) ;
        .global     UseLDRB
UseLDRB:
        .rept   512                     // 512 bytes / 1 byte per loop = 512 repititions
        LDRB    R2, [R1], 1             // Load (unsigned) byte pointed to by pointer in r1 into r2
                                        // update the contents of r1 to next byte address using post indexing
        STRB    R2, [R0], 1             // store the byte in r2 in the mem address held in r1.
                                        // update r1s contents to the next adress by post indexing
        .endr                           // end code repitition
        BX      LR                      // return

// void UseLDRH(void *dst, void *src) ;
        .global     UseLDRH
UseLDRH:
        .rept   256                     // 512 bytes / 2 bytes per loop = 256 repititions
        LDRH    R2, [R1], 2             // Load (unsigned) half word pointed to by pointer in r1 into r2
                                        // update the contents of r1 by shifting 16 bits (2 bytes) to next half word address using post indexing
        STRH    R2, [R0], 2             // store the byte in r2 in the mem address held in r1.
                                        // update r1s contents to the next adress by post indexing
        .endr                           // end code repitition
        BX      LR                      // return

// void UseLDR(void *dst, void *src) ;
        .global     UseLDR
UseLDR:
        .rept   128                     // 512 bytes / 4 bytes per loop = 128 repititions
        LDR     R2, [R1], 4             // Load (unsigned) word pointed to by pointer in r1 into r2
                                        // update the contents of r1 by shifting 32 bits (4 bytes) to next word address using post indexing
        STR     R2, [R0], 4             // store the byte in r2 in the mem address held in r1.
                                        // update r1s contents to the next adress by post indexing
        .endr                           // end code repitition
        BX      LR                      // return

// void UseLDRD(void *dst, void *src) ;
        .global     UseLDRD
UseLDRD:
        .rept   64                      // 512 bytes / 8 bytes per loop = 64 repititions
        LDRD    R2, R3, [R1], 8         // Load (unsigned) double word pointed to by pointer in r1 into r2 and r3
                                        // update the contents of r1 by shifting 64 bits (8 bytes) to next  double word address using post indexing
        STRD    R2, R3, [R0], 8         // store the bytes in r2 and r3 in the mem address held in r1.
        .endr                           // end code repitition
        BX      LR                      // return

// void UseLDMIA(void *dst, void *src) ;
// not sure why this is more efficient
        .global     UseLDMIA
UseLDMIA:
        PUSH    {R4-R11}                // we will need 11 registers for all 44 bytes the LDMIA gives us, so we preserve their contents for later
                                        // (we will use R12 but remember it is essentially a scratch register)
        .rept   11                      // 44*11 = 484 bytes. any more repitions and LDMIA wil grab some stuff we dont want.
                                        // this leaves 28 bytes which we will do sperately
        LDMIA   R1!, {R2-R12}           // load 44 bytes into registers r2-r12 (44 bytes / 4 bytes per register = 11 registers)
                                        // ! is the writeback flag which tells the processor to update the contents of r1 to the mem address next after the read
        STMIA   R0!, {R2-R12}           // store the 44 bytes in r2-r12 into the mem adress held in r0
                                        // ! writeback flag updates adress in r1 to be the next subsequent store adress.
        .endr                           // end code repition

        //grab remaining  28 bytes that didnt fit nicely into 44 bytes segments
        LDMIA   R1, {R2-R8}             //same process as above, but less registers used because less of them are needed (28 bytes / 4 bytes per reg = 7 registers)
        STMIA   R0, {R2-R8}             //same process as above, again less registers used
        POP     {R4-R11}                // pop the saved contents off the stack
        BX      LR                      //return

        // INTERESTING NOTE
        // by using BX LR for the return instead of PUSHing the LR at the top of the function and POPing it to the
        // PC as the last instruction, i save 4 clock cycles of execution time.

        .end                            //only put .end once at the end of ALL the functions
