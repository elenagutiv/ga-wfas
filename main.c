#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../inc/population.h"
#include "../inc/pcp.h"


int n_repeated = 0;

int MAX_HASH_WORD_SIZE;
int K;

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
      specification = argv[6];
   }else{
      specification = NULL;
   }
   char* memo_times;

   double id = strtod(input + 20, NULL); // The beginning of the floating number rep. is at position 20
   double M = strtod(argv[3], NULL);
   MAX_HASH_WORD_SIZE = strtod(argv[4], NULL);
   
   K = (int)M;
   automata* aut;

   individual* argmax;
   population* pop;
   population* children;
   int halt = 0;

   mpq_t comp;
   mpq_init(comp);


   FILE* fp_log;
   fp_log = fopen(LOG_FILE, "w");

   if(!strcmp(type, "comparison") || !strcmp(type, "difference") || !strcmp(type, "difference")){
      fp_times = (fopen(argv[5], "w"));
   }

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

   if(initialization(&aut, input, specification, fp_log) == -1){
      return -1;
   };

   start_i = clock();
   start_runtime = clock();

   hashedWord* hashTable =  initializeHashTable(*aut, &pop);

   end_i = clock();

   start_algorithm = clock();
   if(initPopulation(&pop, *aut, &hashTable) == -1){ return -1;};

   calcFitness(pop, *aut, &hashTable);

   evaluate(*pop, argmax);
   halt = 0; // # repeated argmax is not considered to halt, otherwise write !halt in AND condition in while

   fprintf(fp_log,"Initial population:\n");
   printPopulation(*pop, fp_log);


   double time_algorithm = ((double)(clock() - start_algorithm) / CLOCKS_PER_SEC);
   while(time_algorithm < TIMEOUT){
      naturalSelection(pop);
      
      generate(pop, &children, *aut, fp_log, &hashTable);
      mutate(pop, *aut, argmax, &hashTable); //Only mutates the old generation (except from the argmax). Children mutate in next generations

      prune(&pop, pop, children, *aut, fp_log, &hashTable);

      fprintf(fp_log, "Next generation:\n");

      printPopulation(*pop, fp_log);

      fprintf(fp_log,"Generation: %d\n\n", pop->generations);

      evaluate(*pop, argmax);
      halt = 0;
      fprintf(fp_log,"Highest-weight word found is %s and weight: %.25lf generation: %d][n_reps = %d] \n", argmax->gens, mpq_get_d(argmax->fitness), pop->generations, n_repeated);
      fflush(fp_log);

      time_algorithm = ((double)(clock() - start_algorithm) / CLOCKS_PER_SEC);

   }
   if(!strcmp(type, "bf-search")){
      fprintf(fp_times, "%.10lf\t%.25lf\t%s\n", time_algorithm, mpq_get_d(argmax->fitness), argmax->gens);
   }

   if(!strcmp(type, "bwmp")){
      printf("Highest-weight word found is %s and weight: %.25lf [# of generations: %d][# of repetitions = %d][Timeout (s): %d] \n",  argmax->gens, mpq_get_d(argmax->fitness),pop->generations, n_repeated, TIMEOUT);
      printf("Finished!\nMore info about the execution might be found at log.txt\n");

   }else{
      printf("Finished!\n");
   }
   
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

   if(!strcmp(type, "comparison") || !strcmp(type, "difference") || !strcmp(type, "difference")){
      fclose(fp_times);
   }
   
   freeIndividual(argmax);
   freePopulation(pop);

   freeHashTable(hashTable, *aut);

   freeAutomata(aut);
   
   return 0;
   
}