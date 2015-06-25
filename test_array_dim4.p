/** will val and this.val*/
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
   mem[16] = 0;
   mem[17] = 0;
   mem[18] = 0;
   mem[19] = 0;
   mem[FP] = 38;
   mem[SP] = 38; //start of sp after global vars are placed
  bb0:
   mem[HP] = mem[HP] - 20;
   mem[13] = mem[HP];
   mem[18] = 0;
   goto bb1;
  bb1:
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[FP] + 1] = mem[18] <20;
   if(mem[mem[FP] + 1] == 1) goto bb2;
   mem[SP] = mem[SP] - 1;
   goto bb3;
  bb2:
   mem[SP] = mem[SP] - 1;
   mem[HP] = mem[HP] - 1;
   mem[mem[13] + mem[18]] = mem[HP];
   mem[18] = mem[18] +1;
   goto bb1;
  bb3:
   mem[mem[13]] = 34;
   mem[mem[13] + 1] = 7;
   mem[mem[13] + 2] = 4;
   mem[mem[13] + 3] = 312;
   mem[mem[13] + 4] = 12;
   mem[mem[13] + 5] = 2;
   mem[mem[13] + 6] = 33;
   mem[mem[13] + 7] = 98;
   mem[mem[13] + 8] = 16;
   mem[mem[13] + 9] = 14;
   mem[mem[13] + 10] = 89;
   mem[mem[13] + 11] = 10;
   mem[mem[13] + 12] = 44;
   mem[mem[13] + 13] = 76;
   mem[mem[13] + 14] = 11;
   mem[mem[13] + 15] = 15;
   mem[mem[13] + 16] = 17;
   mem[mem[13] + 17] = 9;
   mem[mem[13] + 18] = 2;
   mem[mem[13] + 19] = 1;
   mem[HP] = mem[HP] - 1;
   mem[14] = mem[HP];
   mem[HP] = mem[HP] - 1;
   mem[20] = mem[HP];
   mem[18] = 17;
   printf("%d\n", mem[mem[13] + 5]);
   printf("%d\n", mem[mem[13] + 6]);
   mem[15] = 1;
   printf("%s\n", "False");
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[FP];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 4;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[14];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 6/* This was a constant passed in a pass by var location */;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 5/* This was a constant passed in a pass by var location */;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 17;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[15];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 7;
   mem[FP] = mem[SP];
   /* end of stack setup for call to happy*/
   /* has label bb10*/
   goto bb10;
  bb10:
   /* FUNCTION: happy */
   /*function var section for 5 */
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[SP] = mem[SP] + 20;
   /* end of func var init */
  bb11:
   mem[mem[FP] + 4] = mem[mem[mem[FP] - 3]];
   printf("%s\n", "True");
   printf("%d\n", mem[mem[mem[mem[FP] - 5]]]);
   printf("%s\n", "True");
   mem[mem[mem[FP] - 4]] = 233;
   mem[mem[mem[FP] - 3]] = 343;
   mem[R1] = mem[mem[FP] - 2];
   mem[SP] = mem[FP]; //reset stack for func happy
   /******* ra if stats *******/
   if (mem[mem[FP] - 6] == 0) goto bb0;
   if (mem[mem[FP] - 6] == 1) goto bb1;
   if (mem[mem[FP] - 6] == 2) goto bb2;
   if (mem[mem[FP] - 6] == 3) goto bb3;
   if (mem[mem[FP] - 6] == 4) goto bb4;
   if (mem[mem[FP] - 6] == 5) goto bb5;
   if (mem[mem[FP] - 6] == 6) goto bb6;
   if (mem[mem[FP] - 6] == 7) goto bb7;
   if (mem[mem[FP] - 6] == 8) goto bb8;
   if (mem[mem[FP] - 6] == 9) goto bb9;
   if (mem[mem[FP] - 6] == 10) goto bb10;
   if (mem[mem[FP] - 6] == 11) goto bb11;
   /****************************/
  bb4:
   mem[FP] = mem[mem[SP] - mem[mem[SP]]];
   mem[SP] = mem[SP] - mem[mem[SP]] - 1;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[FP] + 1] = mem[R1]; /*get return val*/
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[FP];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 5;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[14];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[13] + 5;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[13] + 6;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[mem[FP] + 1];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[15];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 7;
   mem[FP] = mem[SP];
   /* end of stack setup for call to happy*/
   /* has label bb10*/
   goto bb10;
   mem[SP] = mem[SP] - 1;
  bb5:
   mem[FP] = mem[mem[SP] - mem[mem[SP]]];
   mem[SP] = mem[SP] - mem[mem[SP]] - 1;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[FP] + 1] = mem[R1]; /*get return val*/
   mem[18] = mem[mem[FP] + 1];
   printf("%d\n", mem[mem[13] + 5]);
   printf("%d\n", mem[mem[13] + 6]);
   mem[18] = 0;
   mem[SP] = mem[SP] - 1;
   printf("%s\n", "False");
   printf("%s\n", "False");
   printf("%s\n", "False");
   mem[16] = 20;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[FP];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 6;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[14];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 13;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 16;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 5;
   mem[FP] = mem[SP];
   /* end of stack setup for call to arraysort*/
   /* has label bb7*/
   goto bb7;
  bb7:
   /* FUNCTION: arraysort */
   /*function var section for 6 */
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[SP]++;
   mem[mem[SP]] = 0;
   /* end of func var init */
  bb8:
   mem[mem[FP] + 6] = 1;
/*class attr*/
   mem[mem[mem[FP] - 3]] = 15;
   printf("%d\n", mem[mem[mem[mem[FP] - 3]]]);
   printf("%s\n", "False");
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[FP];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 9;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[mem[mem[FP] - 3]];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[FP] + 6;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[FP] + 4;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[mem[FP] + 3];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = mem[mem[FP] + 1];
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[SP]] = 7;
   mem[FP] = mem[SP];
   /* end of stack setup for call to happy*/
   /* has label bb10*/
   goto bb10;
  bb9:
   mem[FP] = mem[mem[SP] - mem[mem[SP]]];
   mem[SP] = mem[SP] - mem[mem[SP]] - 1;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[FP] + 7] = mem[R1]; /*get return val*/
   mem[mem[FP] + 6] = mem[mem[FP] + 7];
   mem[R1] = mem[mem[mem[FP] - 2]];
   mem[SP] = mem[SP] - 1;
   mem[SP] = mem[FP]; //reset stack for func arraysort
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
   /****************************/
  bb6:
   mem[FP] = mem[mem[SP] - mem[mem[SP]]];
   mem[SP] = mem[SP] - mem[mem[SP]] - 1;
   mem[SP]++;
   mem[mem[SP]] = 0;
   mem[mem[FP] + 1] = mem[R1]; /*get return val*/
   mem[12] = mem[mem[FP] + 1];
   mem[SP] = mem[SP] - 1;
   goto _ra_1;
  _ra_1: return 0;
}
