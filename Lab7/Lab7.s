        .syntax     unified
        .cpu        cortex-m4
        .text
        .thumb_func
        .align       2


// unsigned CallReturnOverhead(unsigned dummy) ;
        .global     CallReturnOverhead
CallReturnOverhead:
        BX      LR                      //return
                                        //this function just returns, to test how many clock cyles a call and return takes

// int SDIVby13(int dividend) ;
        .global     SDIVby13
SDIVby13:
        MOV     R1, 13                  // move the constant 13 into r1
        SDIV    R0, R0, R1              // perform a signed division of R0 by 13
        BX      LR                      // result was placed in R0, return

// int MySDIVby13(int dividend) ;
        .global     MySDIVby13
MySDIVby13:
        LDR     R1, =0x4EC4EC4F         //load the contant 1321528399 into R1 (constant came from other program)
        SMMUL   R1, R1, R0              //multiply the dividend by the constant, and put the most significant 32 bits
                                        //of the 64 bit double length product in r1
        ASR     R1, R1, 2               //becuase we used a larger constant than (2^32)/19, we need to shift to adjust back
        SUB     R0, R1, R0, ASR 31      //fill r0 with the sign bit of the divident, and subtract it from our result in r1
        BX      LR                      //return


// unsigned UDIVby13(unsigned dividend) ;
        .global     UDIVby13
UDIVby13:
        MOV     R1, 13                  // move the constant 13 into R1
        UDIV    R0, R0, R1              // perform an unsigned division of Ro by 13
        BX      LR                      // result was placed in R0, return


// unsigned MyUDIVby13(unsigned dividend) ;
        .global     MyUDIVby13
MyUDIVby13:
        LDR     R1, =0x4EC4EC4F         //load the contant 1321528399 into R1 (constant came from other program)
        UMULL   R2, R1, R1, R0          //multiply R1 by R0, placing the most significant bits in R1 and the rest in r2
        LSR     R0, R1, 2               //adjust for the constant being bigger than (2^32)/9 with a shift putting result in R0
        BX      LR                      //done, return

        .end
