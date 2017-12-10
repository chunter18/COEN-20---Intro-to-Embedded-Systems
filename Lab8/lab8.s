        .syntax     unified
        .cpu        cortex-m4
        .text
        .thumb_func
        .align       2

float_zero: .float   0.0
float_one:  .float   1.0

// float			FloatPoly(float x, float coef[], int32_t numb) ;
        .global     FloatPoly
FloatPoly:
        //S0 has x, R0 has ADR coef[0], R1 has n

        VLDR        S1, float_zero	    //ans reg
        VLDR        S2, float_one	    //x^n = initially 1 for x^0

loop:
        CBZ         R1, done            //branch if r1 is zero
        VLDMIA      R0!, {S3}           //load next num fromcoef

        VMLA.F32    S1, S2, S3		    //ans += coef*(x^n)
        VMUL.F32    S2, S0, S2          //prev x^n * x


        SUB         R1, R1, 1           //update loop counter
        B           loop                //re run the loop

done:
        VMOV        S0, S1
        BX          LR                  //return



// Q16				FixedPoly(Q16   x, Q16   coef[], int32_t numb) ;
        .global     FixedPoly
FixedPoly:
        //r0 has x, r1 has adr coef[0], r2 has numb
        PUSH        {R4, R5, R6}

        LDR         R3, =0              //coef
        LDR         R4, =0              //lower half of ans
        LDR         R5, =0              //upper half of ans
        LDR         R6, =65536          //x^0 in q16, will hold x^n after first loop


Loop:
        CBZ         R2, Done            //check loop counter, branch if done
        LDR         R3, [R1], 4         //get Q16 coef into r3

        SMLAL       R4, R5, R3, R6      //multiply coef and X^n and add t0 r4/r5 reg pair of answers

        SMULL       R3, R6, R0, R6      //x^n*x into r3/r6 (both can be reused)
        LSR         R3, R3, 16          //get fractional bits into place
        ORR         R6, R3, R6, LSL 16  //put the whole part in and put in r6 for next loop

        SUB         R2, R2, 1           //decrement loop counter
        B           Loop                //rerun loop

Done:
        LSL         R0, R5, 16          //get the fractional part in r0
        ORR         R0, R0, R4, LSR 16  //put whole ans in r0 return reg
        POP         {R4, R5, R6}        //return registers
        BX          LR                  //return


        .end
