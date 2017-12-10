        .syntax     unified
        .cpu        cortex-m4
        .text
        .thumb_func
        .align       2


// unsigned CallReturnOverhead(unsigned word) ;
        .global     CallReturnOverhead
CallReturnOverhead:
        BX      LR                      //return
                                        //this function just returns, to test how many clock cyles a call and return takes


// unsigned ReverseBits(unsigned word) ;
        .global     ReverseBits
ReverseBits:
//UBFX and BFI need constants not variables for lsb and width, so we cant use them in this case

        MOV     R12, R0                 //we will be using R0 for the result, save val in r12 for use below
        LDR     R0, =0                  //load 0 in R0 to make sure we dont gave any left over values
        //to achieve the reverse, we will be right shifting r12 and placing it into r0 and left shifting it.
        .rept   31                      //loop through next three instructions 31 times in straightline fashion
        RORS    R12, R12, 1             //Rotate r12 right by 1 bit. the lsb goes in to the carry flag.
        ADC     R0, R0, 0               //put the bit shifted off r12 into lsb of R0
        LSL     R0, R0, 1               //shift the bit over so the next bit can go into lsb
        .endr                           //stop looping. we want to avoid a 32nd shift so we have everything but the shift below
        RORS    R12, R12, 1             //take the last bit off r12 (msb)
        ADC     R0, R0, 0               //put it into the lsb spot of r0, and DONT shift.
        BX      LR                      //weve been left shifting the result into r0, we can return becuase its in place.


// unsigned ReverseBytes(unsigned word) ;
        .global     ReverseBytes
ReverseBytes:
        MOV     R12, R0                 //we will be using R0 for the result, save val in r12 for use below

        UBFX    R1, R12, 0, 8           //grab least significant byte from orig word
        BFI     R0, R1, 24, 8           //insert it into most significant byte position in return val

        UBFX    R1, R12, 8, 8           //grab second least significant byte from orig word
        BFI     R0, R1, 16, 8           //insert it into secon most significant byte position in return val

        UBFX    R1, R12, 16, 8          //grab second most significant byte from orig word
        BFI     R0, R1, 8, 8            //insert it into second least significant byte position in return val

        UBFX    R1, R12, 24, 8          //grab most significant byte from orig word
        BFI     R0, R1, 0, 8            //insert it into least significant byte position in return val

        BX      LR                      //byte positions are all set, return

        .end
