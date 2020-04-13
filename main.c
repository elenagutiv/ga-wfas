#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../inc/population.h"
#include "../inc/pcp.h"


int n_repeated = 0;

int MAX_HASH_WORD_SIZE;
int K;
int COMPATIBLE_CROSSOVER_TRIES;

FILE* fp_times;

char* type; // 1: difference; 2: aut

unsigned long long n_matrixMult = 0;
unsigned long long n_potential_matrixMult = 0;
double totalTime_matrixMult = 0.0;

clock_t start_algorithm;

int main(int argc, char** argv)
{
   int i;
   type = argv[2];

   char* input = argv[1];
   char* specification;
   if(!strcmp(type, "difference")){
      specification = argv[7];
   }else{
      specification = NULL;
   }
   char* memo_times;

   double id = strtod(input + 20, NULL); // The beginning of the floating number rep. is at position 20
   double M = strtod(argv[3], NULL);
   MAX_HASH_WORD_SIZE = strtod(argv[4], NULL);
   
   double C = strtod(argv[5], NULL);

   K = (int)M;
   COMPATIBLE_CROSSOVER_TRIES = (int) C;

   // char* init_pop_type = argv[9];
   // if(!strcmp(init_pop_type, "rand")){
   //    INIT_POP_RANDOM=1;
   //    INIT_POP_OPT=0;
   // }else if (!strcmp(init_pop_type, "opt")){
   //    INIT_POP_OPT=1;
   //    INIT_POP_RANDOM=0;
   // }else{
   //    printf("Error: init pop option must be rand or opt.\n");
   // }
   // if(INIT_POP_OPT == 1){
   //    MAX_INIT_WORD_SIZE_OPT = strtod(argv[5], NULL);
   // }else{
   //    MAX_INIT_WORD_SIZE_OPT = -1;
   // }

   automata* aut;

   individual* argmax;
   population* pop;
   population* children;
   int halt = 0;
   int flag_pcp;
   char** f1;
   char** f2;
   char* word1;
   char* word2;
   int diff;
   mpq_t comp;
   mpq_init(comp);


   FILE* fp_log;
   fp_log = fopen(LOG_FILE, "w");

   fp_times = (fopen(argv[6], "w"));

   fprintf(fp_log, "******************************\n******************************\n");
   fprintf(fp_log, "Experiment %s\n", input);
   
   newIndividual(&argmax);
   strcpy(argmax->gens, "$");

   srand(time(NULL));

   clock_t end_algorithm;

   clock_t start_i;
   clock_t end_i;

   clock_t start_runtime;
   clock_t end_runtime;

   if(initialization(&aut, input, specification, fp_log, &f1, &f2) == -1){
      return -1;
   };

   start_i = clock();
   start_runtime = clock();

   hashedWord* hashTable =  initializeHashTable(*aut, &pop);

   end_i = clock();

   start_algorithm = clock();
   if(initPopulation(&pop, *aut, &hashTable) == -1){ return -1;};

   calcFitness(pop, *aut, flag_pcp, &hashTable);

   evaluate(*pop, argmax);
   halt = 0; // # repeated argmax is not considered to halt, otherwise write !halt in AND condition in while

   fprintf(fp_log,"Initial population:\n");
   printPopulation(*pop, fp_log);


   double time_algorithm = ((double)(clock() - start_algorithm) / CLOCKS_PER_SEC);
   while(time_algorithm < TIMEOUT){
      naturalSelection(pop);
      
      generate(pop, &children, *aut, fp_log, &hashTable);
      mutate(pop, *aut, argmax, &hashTable); //Only mutates the old generation (except from the argmax). Children mutate in next generations

      prune(&pop, pop, children, *aut, flag_pcp, fp_log, &hashTable);

      fprintf(fp_log, "Next generation:\n");

      printPopulation(*pop, fp_log);

      fprintf(fp_log,"Generation: %d\n\n", pop->generations);

      evaluate(*pop, argmax);
      halt = 0;
      fprintf(fp_log,"[Gen: %d][n_reps = %d] argmax found is %s and weight: %.25lf", pop->generations, n_repeated, argmax->gens, mpq_get_d(argmax->fitness));
      fflush(fp_log);

      time_algorithm = ((double)(clock() - start_algorithm) / CLOCKS_PER_SEC);

   }
   if(!strcmp(type, "bf-search")){
      fprintf(fp_times, "%.10lf\t%.25lf\t%s\n", time_algorithm, mpq_get_d(argmax->fitness), argmax->gens);
   }

   printf("Finish\n");
   end_runtime = clock();
   
   double total_runtime = ((double) (end_runtime - start_runtime)) / CLOCKS_PER_SEC;

   double total_inithash = ((double) (end_i - start_i)) / CLOCKS_PER_SEC;

   if(!strcmp(type, "memo")){
      fprintf(fp_times, "a\t%llu\n", n_matrixMult);
      fprintf(fp_times, "b\t%llu\n", n_potential_matrixMult);
      fprintf(fp_times, "Time-fraction-matrix-mult\t%.2lf\n", (totalTime_matrixMult/total_runtime));
      fprintf(fp_times, "Time-fraction-init-hash\t%.2lf\n", (total_inithash/total_runtime));
   }

   

   fprintf(fp_log, "******************************\n******************************\n");
   fclose(fp_log);

   fclose(fp_times);
   
   freeIndividual(argmax);
   freePopulation(pop);

   freeHashTable(hashTable, *aut);

   freeAutomata(aut);
   
   return 0;
   
}