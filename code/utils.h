#include <limits.h>
#include <stdint.h>

//algunas macros y declaraciones habituales en mis malos habitos de programacion
#define MY_INFINITY 1000000000
#define YES 1
#define NO 0
#define OK 1
#define KO 0
#define QS_COMPARE(a,b) ((b)-(a))
#define MAX(a,b)        ((a)>=(b)?(a):(b))
#define MIN(a,b)        ((a)<=(b)?(a):(b))
#define ABS(a) ((a)<0 ? (-(a)) : (a))

/*some bitset macros https://gist.github.com/w-vi/10093608
Example:
 
#include <stdio.h>
#include <string.h>
#define MAX 10000
int main()
{
char bitarray[BITNSLOTS(MAX)];
int i, j;
  memset(bitarray, 0, BITNSLOTS(MAX));
  for(i = 2; i < MAX; i++)
  {
    if(!BITTEST(bitarray, i))
    {
      printf("%d\n", i);
      for(j = i + i; j < MAX; j += i)
      BITSET(bitarray, j);
    }
  }
  return 0;
}
*/

#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)


int checkDifferentList(int* s1,int* s2,int length);
double getTime();
void sort(int list[], double pesos[], int beg, int end);
void swap(int* a,int* b);
void initialize_shr3(int start);
int** generateIntMatrix(int n, int m);
double ** generateDoubleMatrix( int n, int m);
int * generateIntVector(int n);
double * generateDoubleVector(int n);
void deepCopyMatrix(int** o,int** d,int n,int m);
void deepCopyVector(int* o,int* d,int n);
void printArray(int** a,int n,int m,const char* str);
void printVector(int* a,int n,const char* str);
int sumVector(int* a,int n);

void initializeRandom(int start);
double r8NormalStandarized();
double r8Normal(double mean,double sd);
int r8NormalRounded(double mean,double sd);
int r8NormalRoundedSkewed(double mean,double sd,int m,int M);
void initializeNormal();
double r8Uniform01();
int r8Int(int lower,int upper);


