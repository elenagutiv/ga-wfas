#ifndef UTIL_H_   /* Include guard */
#define UTIL_H_

char* generateRandomWord(char* alphabet);
char* generateRandomKWord(char* alphabet, int L);
int cmpfunc (const void * a, const void * b);
int addEspecialCharacterToWord(char** word, char* old_word, char ch);
int removeEspecialCharacterFromWord(char** word, char* old_word, char ch);
int bestRound (double num);
void swapWords(char** w1, char** w2);
int randint(int n);
int mystrcpy(char** str1, char* str2);
void reverse(char str[], int length);
char* itoa(int num, char* str, int base);
int addZeros(char* listNumbers, int l);
int poissonGenerator(double lambda);
double exponentialGenerator(double lambda);
int removeSymbolInWord(char** word, int index);
int replaceSymbolInWord(char* word, int index, char ch);
int addSymbolLeftWord(char** word, int index, char ch);
int translateList(int* list, int length, int n);


#endif // UTIL_H_