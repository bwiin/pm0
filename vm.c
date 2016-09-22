
text/plain vm.c ( C source, ASCII text )




#include <stdio.h>
#include <string.h>
#include<stdlib.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define OUT "trace.txt"
#define IN "mcode.txt"   //<-- output and input file names. Will be adjusted for final release( To accept arguments for passing in files


//struct as outlined in the instructions.
typedef struct _instruction{

    int op; //opcode
    int l;  // L
    int m;  // M

}instruction;

// global variable declarations
FILE *inputfile, *outputfile;
int stack[MAX_STACK_HEIGHT];// set the stack size( Will be adjusted later since the stack doesn't need to be a global variable
int sp=0, bp = 1, pc = 0, temp = 0, numofinstructions = 0; //numofinstrcutions is used to determine the number of instructions. Temp is not completely necessary.
int i = 0, scrap = 0, ncalls=0;//possibly redundant variables. used for the loops.
instruction ir[MAX_STACK_HEIGHT];//full set of instructions
instruction cur_instr;//the current instruction being worked on

//function declarations.

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

void choose();

void fetch();

void outputstack(FILE* f);








void main(){

	

	
    // While loop that grabs the instruction and places it into the instruction register array
    FILE* grabElements = fopen( IN, "r");
    while(fscanf(grabElements, "%d", &ir[numofinstructions].op)!=EOF){
        
		
	fscanf(grabElements, "%d", &ir[numofinstructions].l);
	fscanf(grabElements, "%d", &ir[numofinstructions].m);
	numofinstructions++;
    }
    fclose(grabElements);
    //finish getting instructions, close file.
	
	//initialize the stack
	stack[0]=0;
        stack[1]=0;
	stack[2]=0;



     outputfile = fopen( OUT, "w"); //open output file to write out to file.




	

 	// print current contents of the instruction array for testing purposes (It prints to stdout
 	printf("\n The following is the current instruction set:\n\n");
 	for(i =0; i<numofinstructions; i++)
        printf(" \n\t Line %d contents: %3d  %2d  %3d ", i+1, ir[i].op,  ir[i].l, ir[i].m); 
	printf(" \n\n\n\n\n"); // print some space for the instructions
	// end of test print		



	// print out the first portion
        fprintf(outputfile,"Line OP   L   M\n");
        for(i = 0; i < numofinstructions; i++){
                fprintf(outputfile," %3d %s %2d %3d\n", i, opname(ir[i].op), ir[i].l, ir[i].m);
        }
        fprintf(outputfile,"\n\n");
	// end of printing the first portion
	

/*
 *
 *
 * The next portion outputs the remaining contents by doing the following in a loop:
 *
 * 1. print the current PC OP L M
 * 2. perform the fetch cycle
 * 3. perform the operation depending on the choice( based off the instruction)
 * 4 output the PC BP SP for the output formatting
 * 5. print the current stack state
 * 
 * The if/else case is for error catching( But I dont think we have to have error test cases) 
 */
	fprintf(outputfile,"                pc  bp  sp  stack\nInitial values  %2d   %d  %2d\n", pc, bp, sp);
	while(bp>0){
	if(pc < numofinstructions){
	
	fprintf(outputfile,"%3d %s %2d %3d ", pc, opname(ir[pc].op), ir[pc].l, ir[pc].m);

	fetch();
	choose();
	fprintf(outputfile,"%3d %3d %3d  ", pc, bp, sp);
	outputstack(outputfile);
	}else{
	printf("\n\ncontrol goes here\n\n");
	exit(1);
	}
	fprintf(outputfile,"\n");
}









}





/*
 * fetch function
 */
void fetch(){
        cur_instr = ir[pc];
        pc++;

}



/*
 *
 *
 * Base
 *
 *  Base function provided in the instructions, modified for global variables.
 */
int base(int base, int L){
    int b1; //find base L levels down
    b1 = base;
    while (L > 0){
        b1 = stack[b1 + 1];
        L--;
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
    switch (cur_instr.op){
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
    sp++;
    stack[sp] = cur_instr.m;
}




/*
 *
 *
 * opr
 *
 *  performs the OPR operation as outlined in the homework
 */
void opr(){

    if(cur_instr.m == 0){
        sp = bp - 1;
        bp = stack[sp + 3];
        pc = stack[sp + 4];

    }


    if(cur_instr.m == 1){
        stack[sp] = -stack[sp];
    }

    if(cur_instr.m == 2){
        stack[sp] = stack[sp] + stack[sp + 1];
        sp = sp - 1;
    }

    if(cur_instr.m == 3){
        stack[sp] = stack[sp] - stack[sp + 1];
        sp = sp - 1;
    }


    if(cur_instr.m == 4){
        stack[sp] = stack[sp] * stack[sp + 1];
        sp = sp - 1;
    }

    if(cur_instr.m == 5){
        stack[sp] = stack[sp] / stack[sp + 1];
        sp = sp - 1;
    }

    if(cur_instr.m == 6){
        stack[sp] = stack[sp] % 2;

    }

    if(cur_instr.m == 7){

        stack[sp] = stack[sp] % stack[sp + 1];
        sp = sp - 1;

    }

    if(cur_instr.m == 8){
        stack[sp] =  ((stack[sp] == stack[sp + 1]) ? 1 : 0);
        sp = sp - 1;
    }

    if(cur_instr.m == 9){
        stack[sp] =  ((stack[sp] != stack[sp + 1]) ? 1 : 0);
        sp = sp - 1;
    }

    if(cur_instr.m == 10){
        stack[sp] =  ((stack[sp + 1] - stack[sp])>0 ? 1 : 0);
        sp = sp - 1;

    }
	
    if(cur_instr.m == 11){
        stack[sp] =  ((stack[sp + 1] - stack[sp])>=0 ? 1 : 0);
        sp = sp - 1;
    }

    if(cur_instr.m == 12){
        stack[sp] =  (stack[sp] - (stack[sp + 1])>0 ? 1 : 0);
        sp = sp - 1;
    }

    if(cur_instr.m == 13){
        stack[sp] =  (stack[sp] - (stack[sp + 1])>=0 ? 1 : 0);
        sp = sp - 1;
    }

    if(cur_instr.m != 0)
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
    stack[sp] = stack[ base(bp, cur_instr.l) + cur_instr.m];
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

    stack[ base(bp, cur_instr.l) + cur_instr.m] = stack[sp]; 
    sp--;
}



/*
 *
 *
 * cal
 *
 *  performs the CAL operation as outlined in the homework
 */
void cal(){

    ncalls++;
    stack[sp + 1] = 0; // space to return value (FV)
    stack[sp + 2] = base(bp, cur_instr.l); // static link (SL)
    stack[sp + 3] = bp; // dynamic link (DL)
    stack[sp + 4] = pc; // return address (RA)
    bp = sp + 1;
    pc = cur_instr.m;


}



/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * inc
 *
 *  performs the INC operation as outlined in the homework
 */
void inc(){

    sp+= cur_instr.m;
}



/*
 *
 *
 * jmp
 *
 *  performs the JMP operation as outlined in the homework
 */
void jmp(){

    pc = cur_instr.m;
}


/*
 *
 *
 * jpc
 *
 *  performs the JPC operation as outlined in the homework
 */
void jpc(){

    if (stack[sp] == 0)
        pc = cur_instr.m;
    sp--;
}



/*
 *
 *
 * sio
 *
 *  performs the SIO operation as outlined in the homework
 */
void sio(){

    if(cur_instr.m == 0){
        fprintf(stdout, "\n\n\t Contents: %d", stack[sp]);
        sp = sp - 1;
    }

    if(cur_instr.m == 1){
        fscanf(stdin, "\n\n\t Please enter value: %d", &stack[sp+1]);
	sp++;
    }

    if(cur_instr.m == 2){
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

        default:
            exit(0);
            break;
    }
}


/*
 *
 * Prints the current stack contents with proper formatting
 *
 *
 */
void outputstack(FILE* fileTrace){
        int tempBp = bp;
        int bpCount = 1;
        int levels[MAX_LEXI_LEVELS];
        int j = 1;
        levels[0] = 1;

        while (tempBp > 1){
        levels[j++] = tempBp;
        tempBp = stack[tempBp + 2];
        }
    bpCount = j-1;
    if(bpCount > MAX_LEXI_LEVELS){
        printf("\nError: :: ");
    }

        for(i = j; j <= sp; j++) {
                if (j == levels[bpCount] && j != 1) {
                        fprintf(fileTrace,"| ");
                        bpCount--;
                }
                fprintf(fileTrace,"%d ", stack[j]);
        }
	//fprintf(fileTrace,"\n");
}



