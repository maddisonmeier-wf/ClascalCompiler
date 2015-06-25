
#include <stdio.h>
#define MEM_MAX 65536
#define NUM_REG 9
#define GLOBAL_REGION (NUM_REG+3)
#define FP NUM_REG
#define SP (NUM_REG + 1)
#define HP (NUM_REG + 2)
#define R1 0
#define R2 1
#define R3 2
#define R4 3
#define R5 4
#define R6 5
#define R7 6
#define R8 7
#define R9 8
int mem[MEM_MAX]; 

int main() {
	mem[HP] = MEM_MAX-1;
	/* end of static initial setup */
	mem[12] = 0;
	mem[13] = 0;
	mem[14] = 0;
	mem[15] = 0;
	mem[FP] = 15;
	mem[SP] = 15; //start of sp after global vars are placed
  bb0:
	mem[HP] = mem[HP] - 20;
	mem[12] = mem[HP];
	mem[15] = 1;
	goto bb1;
  bb1:
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[FP] + 1] = mem[15] <=20;
	if(mem[mem[FP] + 1] == 1) goto bb2;
	mem[SP] = mem[SP] - 1;
	goto bb3;
  bb2:
	mem[SP] = mem[SP] - 1;
	mem[mem[12] + (mem[15] - 1)] = mem[15];
	mem[15] = mem[15] +1;
	goto bb1;
  bb3:
	mem[15] = 1;
	goto bb4;
  bb4:
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[FP] + 1] = mem[15] <=20;
	if(mem[mem[FP] + 1] == 1) goto bb5;
	mem[SP] = mem[SP] - 1;
	goto bb6;
  bb5:
	mem[SP] = mem[SP] - 1;
	mem[13] = mem[13] +mem[mem[12] + (mem[15] - 1)];
	mem[15] = mem[15] +1;
	goto bb4;
  bb6:
	printf("%d\n", mem[13]);
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[SP]] = mem[FP];
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[SP]] = 7;
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[SP]] = mem[12];
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[SP]] = mem[12]; //param cl
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[SP]] = 20; //param nn
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[SP]] = 5;
	mem[FP] = mem[SP];
	/* end of stack setup for call to sum*/
	/* has label bb8*/
	goto bb8;
  bb8:
	/* FUNCTION: sum */
	/*function var section for 2with sum */
/* var  small */
	mem[SP]++;
	mem[mem[SP]] = 0;
/* var  return */
	mem[SP]++;
	mem[mem[SP]] = 0;
	/* end of func var init */
  bb9:
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[FP] + 3] = mem[mem[FP] - 1] ==0;
	if(mem[mem[FP] + 3] == 1) goto bb10;
	mem[SP] = mem[SP] - 1;
	goto bb11;
  bb10:
	mem[SP] = mem[SP] - 1;
	mem[mem[FP] + 2] = 0;
	goto bb13;
  bb13:
	mem[R1] = mem[mem[FP] + 2];
	/******* ra if stats *******/
	if (mem[mem[FP] - 4] == 0) goto bb0;
	if (mem[mem[FP] - 4] == 1) goto bb1;
	if (mem[mem[FP] - 4] == 2) goto bb2;
	if (mem[mem[FP] - 4] == 3) goto bb3;
	if (mem[mem[FP] - 4] == 4) goto bb4;
	if (mem[mem[FP] - 4] == 5) goto bb5;
	if (mem[mem[FP] - 4] == 6) goto bb6;
	if (mem[mem[FP] - 4] == 7) goto bb7;
	if (mem[mem[FP] - 4] == 8) goto bb8;
	if (mem[mem[FP] - 4] == 9) goto bb9;
	if (mem[mem[FP] - 4] == 10) goto bb10;
	if (mem[mem[FP] - 4] == 11) goto bb11;
	if (mem[mem[FP] - 4] == 12) goto bb12;
	if (mem[mem[FP] - 4] == 13) goto bb13;
	/****************************/
  bb11:
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[FP] + 3] = mem[mem[FP] - 1] -1;
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[SP]] = mem[FP];
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[SP]] = 12;
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[SP]] = mem[mem[FP] - 3];
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[SP]] = mem[mem[FP] - 2]; //param cl
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[SP]] = mem[mem[FP] + 3]; //param nn
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[SP]] = 5;
	mem[FP] = mem[SP];
	/* end of stack setup for call to sum*/
	/* has label bb8*/
	goto bb8;
  bb12:
	mem[R2] = mem[FP]; //store size val for SP
 	mem[FP] = mem[mem[FP] - mem[mem[FP]]];
	mem[SP] = mem[R2] - mem[mem[R2]] - 1;
	mem[SP] = mem[SP] - 1;
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[FP] + 3] = mem[R1]; /*get return val*/
	mem[mem[FP] + 1] = mem[mem[FP] + 3];
	mem[mem[FP] + 2] = mem[mem[FP] + 1] +mem[mem[mem[FP] - 2] + (mem[mem[FP] - 1] - 1)];
	mem[SP] = mem[SP] - 1;
	goto bb13;
	mem[SP] = mem[FP]; //reset stack for func sum
  bb7:
	mem[R2] = mem[FP]; //store size val for SP
 	mem[FP] = mem[mem[FP] - mem[mem[FP]]];
	mem[SP] = mem[R2] - mem[mem[R2]] - 1;
	mem[SP]++;
	mem[mem[SP]] = 0;
	mem[mem[FP] + 1] = mem[R1]; /*get return val*/
	mem[13] = mem[mem[FP] + 1];
	printf("%d\n", mem[13]);
	mem[SP] = mem[SP] - 1;
	goto _ra_1;
  _ra_1: return 0;
}
