#include <stdio.h>
#include <string.h>
#include<stdlib.h>

// macros used( 2 of them(IN and OUT) are inserted via a shell script)
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define OUT "trace.txt"
#define IN "mcode.txt"   //<-- deprecated in development. Kept in case


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
int test_set[3][17];
// prevpc is used for the output format
//numofinstrcutions is used to determine the number of instructions. Temp is not completely necessary.
int sp=0, bp = 1, pc = 0, temp = 0, numofinstructions = 0;
int i = 0, scrap = 0;//possibly redundant variables. used for the loops.
instruction ir[MAX_STACK_HEIGHT];


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











void main(){

	

	
    // While loop that grabs the mcode and places it into the instruction register array
    FILE* grabElements = fopen( IN, "r");
    while(fscanf(grabElements, "%d", &ir[numofinstructions].op)!=EOF){
        
		
	fscanf(grabElements, "%d", &ir[numofinstructions].l);
	fscanf(grabElements, "%d", &ir[numofinstructions].m);
	numofinstructions++;
    }
    fclose(grabElements);
    //finish getting instructions, close file.

	for(i = 0; i < numofinstructions; i++)// initialize all stack array members to 0
        stack[i]=0;




     outputfile = fopen( OUT, "w"); // write out to file.




/*
 *
7 0 10
7 0 2
6 0 6
1 0 13
4 0 4
1 0 1
4 1 4
1 0 7
4 0 5                    current contents of the test_set array
2 0 0
6 0 6
1 0 3
4 0 4
1 0 0
4 0 5
5 0 2
2 0 0
 *
 *
 */

	

 	// print contents test
 	printf("\n The following is the current instruction set:\n\n");
 	for(i =0; i<numofinstructions; i++)
             printf(" \n\t Line %d contents: %d  %d  %d ", i+1, ir[i].op,  ir[i].l, ir[i].m); 


//for loop control to output remaining portion of part 2 of the assignment.
    for(i=0; i< numofinstructions; i++){

        if(i==0){
            choose();
            fprintf(outputfile, "\n\n    %s    %d       %d    %d    %d    ", opname(ir[i].op), ir[i].l, pc, bp, sp);
        }else{
        fprintf(outputfile, "\n\n    %s    %d       %d    %d    %d    ", opname(ir[i].op), ir[i].l, pc, bp, sp);
        choose();
        }
    for(temp=0; temp<17; temp++)
    printf(" %d", stack[temp]);
    printf(" \n");
    }// end of for loop control




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
    switch (ir[i].op){
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
    stack[sp] = ir[i].m;
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

    if(ir[i].m == 0){
        sp = bp - 1;
        bp = stack[sp + 3];
        pc = stack[sp + 4];
        stack[sp + 1] = -999;

    }


    if(ir[i].m == 1){
        stack[sp] = -stack[sp];
    }

    if(ir[i].m == 2){
        stack[sp] = stack[sp] + stack[sp + 1];
        sp = sp - 1;
    }

    if(ir[i].m == 3){
        stack[sp] = stack[sp] - stack[sp + 1];
        sp = sp - 1;
    }


    if(ir[i].m == 4){
        stack[sp] = stack[sp] * stack[sp + 1];
        sp = sp - 1;
    }

    if(ir[i].m == 5){
        stack[sp] = stack[sp] / stack[sp + 1];
        sp = sp - 1;
    }

    if(ir[i].m == 6){
        stack[sp] = stack[sp] % 2;

    }

    if(ir[i].m == 7){

        stack[sp] = stack[sp] % stack[sp + 1];
        sp = sp - 1;

    }

    if(ir[i].m == 8){
        stack[sp] =  ((stack[sp] == stack[sp + 1]) ? 1 : 0);
        sp = sp - 1;
    }

    if(ir[i].m == 9){
        stack[sp] =  ((stack[sp] != stack[sp + 1]) ? 1 : 0);
        sp = sp - 1;
    }

    if(ir[i].m == 10){
        stack[sp] =  ((stack[sp + 1] - stack[sp])>0 ? 1 : 0);
        sp = sp - 1;

    }
	
    if(ir[i].m == 11){
        stack[sp] =  ((stack[sp + 1] - stack[sp])>=0 ? 1 : 0);
        sp = sp - 1;
    }

    if(ir[i].m == 12){
        stack[sp] =  (stack[sp] - (stack[sp + 1])>0 ? 1 : 0);
        sp = sp - 1;
    }

    if(ir[i].m == 13){
        stack[sp] =  (stack[sp] - (stack[sp + 1])>=0 ? 1 : 0);
        sp = sp - 1;
    }

    if(ir[i].m != 0)
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
    stack[sp] = stack[ base(bp, ir[i].l) + ir[i].m];
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

    stack[ base(bp, ir[i].l) + ir[i].m] = stack[sp]; // remember that L is a global, so you only pass in bp
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
    stack[sp + 2] = base(bp, ir[i].l); // static link (SL)
    stack[sp + 3] = bp; // dynamic link (DL)
    stack[sp + 4] = pc+1; // return address (RA)
    stack[sp + 7] = -999;//new end index
    bp = sp + 1;
    pc = ir[i].m;


}



/*
 *
 *
 * inc
 *
 *  performs the INC operation as outlined in the homework
 */
void inc(){


    sp = sp + ir[i].m;

    if(stack[sp+1] != -999){

    stack[sp-2]=0;
    stack[sp+1]=-999;
    }

    pc++;
}



/*
 *
 *
 * jmp
 *
 *  performs the JMP operation as outlined in the homework
 */
void jmp(){
    pc = ir[i].m;


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
        pc = ir[i].m;
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

    if(ir[i].m == 1){
        fprintf(stdout, "\n\n\t Contents: %d", stack[sp]);
        sp = sp - 1;
    }

    if(ir[i].m == 2){
        sp = sp + 1;
        fscanf(stdin, "\n\n\t Please enter value: %d", &stack[sp]);

    }

    if(ir[i].m == 3){
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



