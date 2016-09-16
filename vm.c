/*

 ==================================================
 pm0.c

 Alexander dos Santos

 ==================================================


 There are a couple tidbits to explain:
 
 
 REGARDING THE -999 VALUE
 --------------------------------------
 
 
 there will be a stack value that you will commonly see being assigned(-999). This is my marker for where to stop
 printing in the stack.
 
 I initialize the entire stack to all zeros and place the -999 value at the start of the initial sp+1(stack[4]).
 
 From there, I manipulate it as necessary.
 
 
 REGARDING GLOBAL VARIABLES
 --------------------------------------
 
 There are obviously WAY better ways to go about programming where global variables are only used if necessary.
 
 For this project, I just decided to use them for simplicity with function calls. This allowed me to just use void functions to manipulate them. I was quite under the gun so I just went with what I knew would work.
 
 
 REGARDING MULTIPLE FILE POINTER DECLARATIONS
 ----------------------------------------------------------
 
 At one point I was constantly getting stuck with segmentation fault errors. At first, it was easy to find the 
 issue using strace, but later on, the system calls were too difficult to understand. Never-the-less, I found the issues, I just may have some extra/unnecessary file pointers( they are all being used, it could just be narrowed down to only a few of them).
 
 
 REGARDING POSSIBLE INCORRECT OPR OPERATIONS
 ----------------------------------------------------------
 
While following the outline for the different opcode for OPR, I noticed that there were some discrepancies with 
 which command was to be executed first. This led some of my instructions to output incorrectly. I fixed most of them, but I have not tested all cases, so there may be a case where an operation may be incorrect, but it's just because something like " sp = sp-1 " was placed in the wrong order of that operation.
 
 
 
 
 */

#include <stdio.h>
#include <string.h>
#include<stdlib.h>

// macros used( 2 of them(IN and OUT) are inserted via a shell script)
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define OUT "trace.txt"



//struct as outlined in the instructions.
typedef struct _instruction{
    
    int op; //opcode
    int l;  // L
    int m;  // M
    
}instruction;

// global variable declarations
FILE *inputfile, *outputfile;
// set the stack size
int stack[MAX_STACK_HEIGHT];
// prevpc is used for the output format
//numofinstrcutions is used to determine the number of instructions. Temp is not completely necessary.
int sp=0, bp = 1, pc = 0, prevpc = 0, temp = 0, numofinstructions = 0;
instruction ir;




//function declarations.
void writeLineP1();

void opr();

void lod();

void sto();

void cal();

void inc();

void jmp();

void jpc();

void sio();

void lit();

char *opname(int op);

void printstack();

void choose();

void writeLineP2();




void main(int argc, char *argv[]){
    
    char IN[50];
	
	if(argc != 2)
		strcpy(IN, "./trace.txt");
	//else
		//strcpy(IN, argv[1]); this is actually taking in the file as an argument... silly me :D.... just pass argv[1] to fopen for it to work.
	
	
    int i = 0, scrap = 0;//possibly redundant variables.
    
    for(i = 0; i < MAX_STACK_HEIGHT; i++)// initialize all array members to 0
        stack[i]=0;
    stack[4] = -999;// place the initial stop marker
    
    //get num of instructions. There are WAY better ways to do this, but this works for the time being.
    FILE* grabnumelements = fopen( IN, "r");
    while(fscanf(grabnumelements, "%d %d %d", &scrap, &scrap, &scrap)!=EOF){
        numofinstructions++;
    }
    fclose(grabnumelements);
    //finish getting number of instructions, close file.
    
    
    
    
    int set[3][numofinstructions]; // create 2d array for the instruction set.
    
    
    //open the files
    inputfile = fopen( IN, "r");
    outputfile = fopen( OUT, "w");
    
    
    
    
    //outputs part 1 of the assignment and sets the components of the 2d array to the instruction set.
    fprintf(outputfile, "Line  OP    L    M");
    i=0;
    while(fscanf(inputfile, "%d %d %d", &ir.op, &ir.l, &ir.m)!=EOF){
        writeLineP1();
        set[0][i] = ir.op, set[1][i] = ir.l, set[2][i] = ir.m;
        i++;
    }// end of output p1
    
    
    fclose(inputfile);//close input file pointer
    
    // print line spaces between the outputs.
    fprintf(outputfile, "\n\n\n\n\n\n\n\n");
    
    //output first line of the 2nd part of the output, sans stack print.
    fprintf(outputfile, "                    pc   bp   sp     stack");
    fprintf(outputfile, "\nInitial values      %d    %d    %d    ", pc, bp, sp);
    
    //for loop control to output remaining portion of part 2 of the assignment.
    for(i=0; i< numofinstructions-1; i++){
        
        if(i==0){
            ir.op = set[0][pc], ir.l = set[1][pc], ir.m = set[2][pc];
            choose(stack, ir, bp, sp, pc);
            fprintf(outputfile, "\n\n%d    %s    %d       %d    %d    %d    ", prevpc, opname(ir.op), ir.l, pc, bp, sp);
            prevpc = pc;
        }else{
        ir.op = set[0][pc], ir.l = set[1][pc], ir.m = set[2][pc];
        choose(stack, ir, bp, sp, pc);
        writeLineP2(i);
        }
    }// end of for loop control
    
    
    fclose(outputfile);
    
   
}// end of main




/*
 *
 *
 *  writelineP2
 *
 *  Prints the current contents of the variables concerned with part 2 of the assignment in the format required. Then prints the stack contents and sets prevpc(previous pc) to current pc for the next change.
 */
void writeLineP2(){
    fprintf(outputfile, "\n\n%d    %s    %d       %d    %d    %d    ", prevpc, opname(ir.op), ir.l, pc, bp, sp);
    printstack(outputfile, stack);
    prevpc = pc;
    
}

/*
 *
 *
 * writelineP1
 *
 *  writes a single line of the output for part 1.
 */
void writeLineP1(){
    
    fprintf(outputfile, "\n\n%d    %s    %d     %d",  numofinstructions, opname(ir.op), ir.l, ir.m);
    
}//end writelinep1



/*
 *
 *
 * printstack
 *
 *  Prints content of the stack up to the break point signified by the -999 value.
 */
void printstack(){
    
    int i = 1;
    while(stack[i]!=-999 && i<MAX_STACK_HEIGHT){
        if(i==bp && bp>1)
           fprintf(outputfile, " | ");
        fprintf(outputfile, " %d ", stack[i]);
        i++;
    }
}




/*
 *
 *
 * Base
 *
 *  Base function provided in the instructions, modified for global variables.
 */
int base(int base){
    int b1; //find base L levels down
    b1 = base;
    while (ir.l > 0){
        b1 = stack[b1 + 1];
        ir.l--;
    }
    return b1;
}



/*
 *
 *
 * choose
 *
 *  initiates the appropriate function correlating to the appropriate op code.
 */
void choose(){
    switch (ir.op){
        case 1:
            lit();
            break;
            
        case 2:
            opr();
            break;
            
        case 3:
            lod();
            break;
            
        case 4:
            sto();
            break;
            
        case 5:
            cal();
            break;
            
        case 6:
            inc();
            break;
            
        case 7:
            jmp();
            break;
            
        case 8:
            jpc();
            break;
            
        case 9:
            sio();
            break;
            
        case 10:
            sio();
            break;
            
        case 11:
            sio();
            break;
            
        default:
            exit(0);
            break;
    }
    
    
}



/*
 *
 *
 * lit
 *
 *  performs the LIT operation as outlined in the homework
 */
void lit(){
    sp = sp + 1;
    stack[sp+1] = -999;
    stack[sp] = ir.m;
    pc++;
}




/*
 *
 *
 * opr
 *
 *  performs the OPR operation as outlined in the homework
 */
void opr(){
    
    if(ir.m == 0){
        sp = bp - 1;
        bp = stack[sp + 3];
        pc = stack[sp + 4];
        stack[sp + 1] = -999;
        
    }
    
    
    if(ir.m == 1){
        stack[sp] = -stack[sp];
    }
    
    if(ir.m == 2){
        stack[sp] = stack[sp] + stack[sp + 1];
        sp = sp - 1;
    }
    
    if(ir.m == 3){
        stack[sp] = stack[sp] - stack[sp + 1];
        sp = sp - 1;
    }
    
    
    if(ir.m == 4){
        stack[sp] = stack[sp] * stack[sp + 1];
        sp = sp - 1;
    }
    
    if(ir.m == 5){
        stack[sp] = stack[sp] / stack[sp + 1];
        sp = sp - 1;
    }
    
    if(ir.m == 6){
        stack[sp] = stack[sp] % 2;
        
    }
    
    if(ir.m == 7){
        
        stack[sp] = stack[sp] % stack[sp + 1];
        sp = sp - 1;
        
    }
    
    if(ir.m == 8){
        stack[sp] =  ((stack[sp] == stack[sp + 1]) ? 1 : 0);
        sp = sp - 1;
    }
    
    if(ir.m == 9){
        stack[sp] =  ((stack[sp] != stack[sp + 1]) ? 1 : 0);
        sp = sp - 1;
    }
    
    if(ir.m == 10){
        stack[sp] =  ((stack[sp + 1] - stack[sp])>0 ? 1 : 0);
        sp = sp - 1;
        
    }
    
    if(ir.m == 11){
        stack[sp] =  ((stack[sp + 1] - stack[sp])>=0 ? 1 : 0);
        sp = sp - 1;
    }
    
    if(ir.m == 12){
        stack[sp] =  (stack[sp] - (stack[sp + 1])>0 ? 1 : 0);
        sp = sp - 1;
    }
    
    if(ir.m == 13){
        stack[sp] =  (stack[sp] - (stack[sp + 1])>=0 ? 1 : 0);
        sp = sp - 1;
    }
    
    if(ir.m != 0)
        pc++;
    
}



/*
 *
 *
 * lod
 *
 *  performs the LOD operation as outlined in the homework
 */
void lod(){
    
    sp = sp + 1;
    stack[sp+1] = -999;
    stack[sp] = stack[ base(bp) + ir.m];
    pc++;
}


/*
 *
 *
 * sto
 *
 *  performs the STO operation as outlined in the homework
 */
void sto(){
    
    stack[ base(bp) + ir.m] = stack[sp];
    stack[sp+1] = 0;
    stack[sp] = -999;
    sp = sp - 1;
    
    pc++;
}




/*
 *
 *
 * cal
 *
 *  performs the CAL operation as outlined in the homework
 */
void cal(){
    
    stack[sp + 1] = 5; // space to return value (FV)
    stack[sp + 2] = base(bp); // static link (SL)
    stack[sp + 3] = bp; // dynamic link (DL)
    stack[sp + 4] = pc+1; // return address (RA)
    stack[sp + 7] = -999;//new end index
    bp = sp + 1;
    pc = ir.m;
    
    
}



/*
 *
 *
 * inc
 *
 *  performs the INC operation as outlined in the homework
 */
void inc(){
    
    
    sp = sp + ir.m;
    //for(int i = 0; i<20; i++)
     //printf(" %d ,", stack[i]);
    if(stack[sp+1] != -999){
    stack[sp-2]=0;
    stack[sp+1]=-999;
    }
    //printf("\n\n");
    //for(int i = 0; i<20; i++)
    //printf(" %d ,", stack[i]);
    pc++;
    //printf("\n\n\n\n\n\n\n\n\n\n");
}



/*
 *
 *
 * jmp
 *
 *  performs the JMP operation as outlined in the homework
 */
void jmp(){
    pc = ir.m;
    

}



/*
 *
 *
 * jpc
 *
 *  performs the JPC operation as outlined in the homework
 */
void jpc(){
    
    if (stack[sp] == 0){
        pc = ir.m;
    }else
        pc++;
    
    sp = sp - 1;
    

}



/*
 *
 *
 * sio
 *
 *  performs the SIO operation as outlined in the homework
 */
void sio(){
    
    if(ir.m == 1){
        fprintf(stdout, "\n\n\t Contents: %d", stack[sp]);
        sp = sp - 1;
    }
    
    if(ir.m == 2){
        sp = sp + 1;
        fscanf(stdin, "\n\n\t Please enter value: %d", &stack[sp]);
        
    }
    
    if(ir.m == 3){
        exit(1);
    }
    
    pc++;

}// end sio



/*
 *
 *
 * opname
 *
 *  returns a string of the operation name based on it's integer representation.
 */
char *opname(int op){
    
    switch (op){
        case 1:
            return "LIT";
            break;
            
        case 2:
            return "OPR";
            break;
            
        case 3:
            return "LOD";
            break;
            
        case 4:
            return "STO";
            break;
            
        case 5:
            return "CAL";
            break;
            
        case 6:
            return "INC";
            break;
            
        case 7:
            return "JMP";
            break;
            
        case 8:
            return "JPC";
            break;
            
        case 9:
            return "SIO";
            break;
            
        case 10:
            return "SIO";
            break;
            
        case 11:
            return "SIO";
            break;
            
        default:
            exit(0);
            break;
    }
}



