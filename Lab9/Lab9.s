        .syntax     unified
        .cpu        cortex-m4
        .text
        .thumb_func
        .align       2



//extern void	SIMD_USatAdd(void *pixels, int count, int delta) ;
        .global     SIMD_USatAdd
SIMD_USatAdd:
        PUSH        {R4-R11}                //40 bytes per loop means we need r3-r12, preserve copies
        BFI         R2, R2, 8, 8            //make two 8 bit copies of amount in r2
        BFI         R2, R2, 16, 16          //make two more 8 bit copies in r2
                                            //r2 now holds 4 a bit copies to add to eaxh 8 bit pixel val
loop:
        CMP         R1, 20                  //see if we are done looping
        BLE         done                    //branch if done

        LDMIA       R0, {R3-R12}            //load 40 bytes into r3-12 from R0
        UQADD8      R3, R3, R2              //saturating add delta to bytes 1-4
        UQADD8      R4, R4, R2              //saturating add delta to bytes 5-8
        UQADD8      R5, R5, R2              //saturating add delta to bytes 9-12
        UQADD8      R6, R6, R2              //saturating add delta to bytes 13-16
        UQADD8      R7, R7, R2              //saturating add delta to bytes 17-20
        UQADD8      R8, R8, R2              //saturating add delta to bytes 21-24
        UQADD8      R9, R9, R2              //saturating add delta to bytes 24-28
        UQADD8      R10, R10, R2            //saturating add delta to bytes 29-32
        UQADD8      R11, R11, R2            //saturating add delta to bytes 33-36
        UQADD8      R12, R12, R2            //saturating add delta to bytes 37-40
        STMIA       R0!, {R3-R12}           //store the 40 updated bytes and bump adress in R0 by 40 bytes

        SUB         R1, R1, 40              //decrement the count by the number of bytes we did in that loop
        B           loop                    //re run the loop
done:
        //cleanup - do final 20 bytes that doent fit evenly into main loop
        LDMIA       R0, {R3-R7}             //load 20 bytes into r3-7 from R0
        UQADD8      R3, R3, R2              //saturating add delta to bytes 1-4
        UQADD8      R4, R4, R2              //saturating add delta to bytes 5-8
        UQADD8      R5, R5, R2              //saturating add delta to bytes 9-12
        UQADD8      R6, R6, R2              //saturating add delta to bytes 13-16
        UQADD8      R7, R7, R2              //saturating add delta to bytes 17-20
        STMIA       R0!, {R3-R7}            //store the 20 updated bytes and bump adress in R0 by 20 bytes
        POP         {R4-R11}                //restore the contents of the registers we used
        BX          LR                      //return
        .end
