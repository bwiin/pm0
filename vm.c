//Group 29
//-Alexander Dossantos
//-Brian Nguyen
//-Heather Connors
//-Ryan Hoeck




#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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
int halt = 0;
instruction ir[MAX_STACK_HEIGHT];//full set of instructions
instruction cur_instr;//the current instruction being worked on
char instructionWord[3];


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
char *opname(instruction op);
void choose();
int commandParser(char *filename);
void printTheStack();
void fetch();
void outputstack();


void main(int argc, char *argv[]){
	int ret;


	//function takes in the instruction file and populates the ir stack;
	ret = commandParser(argv[1]);
	if (ret == -1){
		exit(-1);
	}

	//initialize the stack
	stack[0]=0;
    stack[1]=0;
	stack[2]=0;

 	// print current contents of the instruction array for testing purposes (It prints to stdout
 	//printf("\n The following is the current instruction set:\n\n");
 	//	for(i =0; i<numofinstructions; i++){
    //    printf(" \n\t Line %d contents: %3d  %2d  %3d ", i+1, ir[i].op,  ir[i].l, ir[i].m);
	//}
	//printf(" \n\n\n\n\n"); // print some space for the instructions
	// end of test print

	//outputfile = fopen( OUT, "w"); //open output file to write out to file.

	// print out the first portion
    printf("PL/0 code:\n\n");
	for(i = 0; i < numofinstructions; i++){

		strcpy(instructionWord,opname(ir[i]));

		//if we use STO or CAL, the level must be printed out differntly.
		if (ir[i].op == 2 || ir[i].op == 9) {
			printf("%3d%5s\n",i, instructionWord, ir[i].m);
		} else if (ir[i].op == 4 || ir[i].op == 5 ){
			printf("%3d%5s%5d%5d\n",i, instructionWord, ir[i].l,ir[i].m);
		} else {
			printf("%3d%5s%10d\n",i, instructionWord, ir[i].m);
		}
	}
	printf("\n" );
    //fprintf(outputfile,"\n\n");
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
 	printf("Execution:\n");
 	printf("%24s%5s%5s%8s\n", "pc","bp","sp","stack");
	printf("%24d%5d%5d\n", pc, bp, sp);
	while(bp>0 && halt != 1){
		if(pc < numofinstructions){

			fetch();
			if (cur_instr.op == 5 || cur_instr.op == 4) {
				printf("%3d%5s%5d%5d", pc-1, opname(ir[pc-1]), ir[pc-1].l, ir[pc-1].m);
			} else if (cur_instr.op == 2 || cur_instr.op == 9){
				printf("%3d%5s%10s", pc-1, opname(ir[pc-1]), " ");
			} else {
				printf("%3d%5s%10d", pc-1, opname(ir[pc-1]), ir[pc-1].m);
			}

			choose();
			printf("%6d%5d%5d   ", pc, bp, sp);
			outputstack(outputfile);
		} else {
			printf("\n\ncontrol goes here\n\n");
			exit(1);
		}

	printf("\n");
}




}

//written by Ryan Hoeck
//this function was taken and modified from one of my previous programs
int commandParser(char *filename){
	FILE *fp;
    char *line;
    char *temp;
    char *token;
	int n;

    if (filename == NULL) {
		perror("You did not input a filename\n");
        return -1;
    }

    fp = fopen(filename,"r");
    if (fp == NULL) {
		perror("You did not input a valid filename\n");
        return -1;
    }


    line = malloc(sizeof(char) * MAX_CODE_LENGTH + 1);
    line = fgets(filename, MAX_CODE_LENGTH + 1, fp);


	if ( line == NULL) {
		return -1;
	}

    while (line != NULL) {
		//printf("%s\n", token);
        token = strtok(line," ");
		if (token != NULL) {
			ir[numofinstructions].op = atoi(token);
		}

		token = strtok(NULL," ");
		if (token != NULL) {
			ir[numofinstructions].l = atoi(token);
		}

		token = strtok(NULL," ");
		if (token != NULL) {
			ir[numofinstructions].m = atoi(token);
		}


		line = fgets(filename, MAX_CODE_LENGTH + 1, fp);
		numofinstructions++;
    }

	free(line);
    fclose(fp);

	return 0;
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
			//printf("a\n");
            lit();
            break;

        case 2:
		//	printf("b\n");
            opr();
            break;

        case 3:
			//printf("c\n");
            lod();
            break;

        case 4:
		//	printf("d\n");
            sto();
            break;

        case 5:
			//printf("e\n");
            cal();
            break;

        case 6:
			//printf("f\n");
            inc();
            break;

        case 7:
			//printf("g\n");
            jmp();
            break;

        case 8:
		//	printf("h\n");
            jpc();
            break;

        case 9:
		//	printf("i\n");
            sio();
            break;

        case 10:
			//printf("j\n");
            sio();
            break;

        case 11:
			//printf("k\n");
            sio();
            break;

        default:
			//printf("l\n");
			halt = 1;
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
        printf("\n\n\t Contents: %d", stack[sp]);
        sp = sp - 1;
    }

    if(cur_instr.m == 1){
        scanf("\n\n\t Please enter value: %d", &stack[sp+1]);
	sp++;
    }

    if(cur_instr.m == 2){
        halt = 1;
    }

}// end sio



/*
 *
 *
 * opname
 *
 *  returns a string of the operation name based on it's integer representation.
 */
char *opname(instruction op){

    switch (op.op){
        case 1:
            return "LIT";
            break;

        case 2:
            switch (op.l) {
            	case 0:
					return "RET";
					break;

				case 1:
					return "NEG";
					break;

				case 2:
					return "ADD";
					break;

				case 3:
					return "SUB";
					break;

				case 4:
					return "MUT";
					break;

				case 5:
					return "DIV";
					break;

				case 6:
					return "ODD";
					break;

				case 7:
					return "MOD";
					break;

				case 8:
					return "EQL";
					break;

				case 9:
					return "NEQ";
					break;

				case 10:
					return "LLS";
					break;

				case 11:
					return "LEQ";
					break;

				case 12:
					return "GTR";
					break;

				case 13:
					return "GEQ";
					break;

				default:
					return "HLT";
					break;
            }
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
            return "HLT";
            break;

        default:
            return "HLT";
            break;
    }
}


/*
 *
 * Prints the current stack contents with proper formatting
 *
 *
 */
void outputstack(){
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
                printf("| ");
                bpCount--;
            }
            printf("%-2d", stack[j]);
        }
	//fprintf(fileTrace,"\n");
}
