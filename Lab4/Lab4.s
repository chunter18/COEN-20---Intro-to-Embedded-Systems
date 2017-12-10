        .syntax     unified
        .cpu        cortex-m4
        .text
        .thumb_func
        .align       2


// int Discriminant(int32_t a, int32_t b, int32_t c) ;
        .global     Discriminant
Discriminant:
        MUL     R1,R1,R1                // find b^2, leave in R1
        LDR     R3,=4                   // put constant of 4 in R3 because MUL only operated on registers
        MUL     R3,R3,R0                // multiply 4 and a
        MLS     R0,R3,R2,R1             // multiply previous 4*a by c, and subtract the result from b^2 left in r1
        BX      LR                      // the return value is in R0 already, return


// int Root1(int32_t a, int32_t b, int32_t sqrt_d) ;
        .global     Root1
Root1:
        LDR     R3,=-1                  // Load -1 in R3
        MLA     R1,R3,R1,R2             // compute -b, add it to sqrt_d in r2, place result in r1
        LDR     R3,=2                   // load constant of 2 in r3 for 2*a
        MUL     R0,R0,R3                // compute 2*a
        SDIV    R0,R1,R0                // divide (-b + sqrt_d) in r1 by 2*a (in r0)
        BX      LR                      // resut in r0 from previous instruction, return

// int Root2(int32_t a, int32_t b, int32_t sqrt_d) ;
        .global     Root2
Root2:
        LDR     R3,=-1                  // load -1 in r3
        MUL     R1,R3,R1                // compute -b by doing -1*b
        SUB     R1,R1,R2                // subtract sqrt_d from -b
        LDR     R3,=2                   // load 2 into r3
        MUL     R0,R0,R3                // compute 2*a
        SDIV    R0,R1,R0                // divide (-b - sqrt_d) in r1 by 2*a (in r0)
        BX      LR                      // resut in r0 from previous instruction, return

        .end
