        .syntax     unified
        .cpu        cortex-m4
        .text
        .thumb_func
        .align       2

// void PrintTwo(char *format, int number) ;

        .global     PrintTwo
PrintTwo:
                                    // R0/R1 hold the string and number

        PUSH        {R4,R5,LR}      // Push the values of R4, R5, and LR to the stack to be stored becuase we cant lose them.

        MOV         R4, R0          // store the string ptr in R4
        MOV         R5, R1          // Store the number in R5
        // originally i used R2/R3 as temporary storage registers, but i got unpredictable results.
        // im assuming it has to do with printf. instead i just use R4/R5 to make it easy

        BL          printf          // call printf with the valus still in R0/R1

        MOV         R0,R4           // move the string back to its prober place fro next call
        ADD         R1,R5,1         //add 1 to num (in R5) and store it in R1

        BL          printf          //final call to printf

        POP         {R4,R5,PC}      // pop R4/R5 off the stack to restore thier pree function vals, put LR in PC to return.



// unsigned Ten32(void) ;

        .global     Ten32
Ten32:
        LDR         R0, =10         //use LDR pseudo instruction to place the constant 10 in R0 for return
        BX          LR              //return



// unsigned long long Ten64(void) ;

        .global     Ten64
Ten64:
        LDR         R0,=10          //load the constant into the lower register. still 32 bits
        LDR         R1,=0           //accomplish promotion to 64 bits by loading all zeros (for unsigned) into nex register
        BX          LR              //return


// unsigned Incr(unsigned n) ;

        .global     Incr
Incr:
        ADD         R0,R0,1         //add one to the arg in R0 and place it in R0
        BX          LR              //return


// unsigned Nested1(void) ;

        .global     Nested1
Nested1:
        PUSH        {LR}            //remeber return adress because we call rand
        BL          rand            //call rand, result -> R0
        ADD         R0,R0,1         //increment the value from rand and place in R0
        POP         {PC}            //return


// unsigned Nested2(void) ;

        .global     Nested2
Nested2:
        PUSH        {R4,LR}         //store LR and R4, we will need them
        BL          rand            //cal rand, result in R0
        MOV         R4,R0           //Move result into R4 so its not overwritten
        BL          rand            //final call to rand
        ADD         R0,R0,R4        //add up the two results and store in retun register
        POP         {R4,PC}         //restore R4 and return



        .end                //only put .end once at the end of ALL the functions
