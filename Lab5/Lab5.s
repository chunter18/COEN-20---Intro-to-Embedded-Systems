        .syntax     unified
        .cpu        cortex-m4
        .text
        .thumb_func
        .align       2



// void DeleteItem(int data[], int items, int index) ;
        .global     DeleteItem
DeleteItem:
        SUB     R1, R1, 1               // computing items-1
loopbody:
        CMP     R2, R1                  //compare i and (items-1) where r2 is i
        BGE     done                    //if i >= (items-1) the loop should be done, break by branching
        ADD     R2, R2, 1               //finding i+1
        LDR     R3, [R0, R2, LSL 2]     //loading data[i+1] into r3
        SUB     R2, R2, 1               //retruning our index back to i for storage (ie data[i], the index is always r2)
        STR     R3, [R0, R2, LSL 2]     //storing data[i+1] into data[i]
        ADD     R2, R2, 1               //update step of loop, equivalent to i++
        B       loopbody                //branch to next iteration
done:
        BX      LR                      //no return value, just return


// void InsertItem(int data[], int items, int index, int value) ;
        .global     InsertItem
InsertItem:
        PUSH    {R4, R5, LR}            //beacues all scratch reg's are full, push to get some extra space
                                        //we dont need to push lr, but saves an instruction at the end
        MOV     R5, R2                  //make a copy of index for later use
        SUB     R1, R1, 1               //compute items - 1
        SUB     R2, R2, 1               //compute index - 1
loopbody2:
        CMP    R1, R2                   //compare (items-1) to (index-1)
        BLE    done2                    //if (items-1) <= (index-1) break;
        SUB    R1, R1, 1                //compute i-1 for next acess of data[i-1]
        LDR    R4, [R0, R1, LSL 2]      //put data[i-1] in r4
        ADD    R1, R1, 1                //return i-1 to i for store in data[i] (same index is always used)
        STR    R4, [R0, R1, LSL 2]      //store data[i-1] in data[i]
        SUB    R1, R1, 1                //update step, equivalent to i--
        B      loopbody2                //branch to next loop iteration
done2:
        STR     R3, [R0, R5, LSL 2]     //we are done so store value in now empy slot data[index]
        POP     {R4, R5, PC}            //pop registers and return.


        .end
