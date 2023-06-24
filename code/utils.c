//algunas macros y declaraciones habituales en mis malos hábitos de programación
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

uint32_t kn[128]; 
double fn[128]; 
double wn[128];

uint32_t jsr=UINT32_MAX/4+15;

//returns an unsigned number between 0 and UINT32_MAX (internal use only)
uint32_t shr3_seeded ( )
{
  uint32_t value;
  value = jsr;
  jsr = ( jsr ^ ( jsr <<   13 ) );
  jsr = ( jsr ^ ( jsr >>   17 ) );
  jsr = ( jsr ^ ( jsr <<    5 ) );
  value = value + jsr;
  return value;
}

//compara dos vectores y mira si son diferentes
int checkDifferentList(int* s1,int* s2,int length)
{
	int i;
	for(i=1;i<length;i++)
	{
		if(s1[i]!=s2[i]) return(YES);
	}
	return(NO);
}

//returns time
double getTime()
{
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return ru.ru_utime.tv_sec +
        ru.ru_utime.tv_usec / 1000000.0;
}

//quicksort ordena vector list usando vector pesos para marcar el orden usa QS_COMPARE para comparaciones
//de primera a ultima posición, por tanto en este caso generalmente de beg=0 end=n-1 (end se cambia!!!)
//el QS_COMPARE actual orden de mayor a menor
void sort(int list[], double pesos[], int beg, int end)
{
	int piv; 
	int tmp;
	int  l,r,p;

	while (beg<end)    // This while loop will avoid the second recursive call
	{
		l = beg; p = beg + (end-beg)/2; r = end;
		piv = list[p];
		//printf("l %d r %d piv: %d (p: %d)\n",l,r,list[p],p);
		//fflush(stdout);
		while (1)
		{
			//printf("l: %d r: %d\t",l,r);
			while ( (l<=r) && ( QS_COMPARE(pesos[list[l]],pesos[piv]) <= 0.0 ) ) l++;
			while ( (l<=r) && ( QS_COMPARE(pesos[list[r]],pesos[piv])  > 0.0 ) ) r--;
			//printf(" -->  l: %d r: %d\n",l,r);
			if (l>r) break;
			tmp=list[l]; list[l]=list[r]; list[r]=tmp;
			if (p==r) p=l;
			l++; r--;
		}
		list[p]=list[r]; list[r]=piv;
		r--;
		//printf("r: %d beg: %d end %d l %d\n",r,beg,end,l);
		//fflush(stdout);
		// Recursion on the shorter side & loop (with new indexes) on the longer
		if ((r-beg)<(end-l))   
		{
			sort(list, pesos, beg, r);
			beg=l;
		}
		else
		{
			sort(list, pesos, l, end);
			end=r;
		}
	}   
}

void swap(int* a,int* b)
{
	int c;
	c=(*a);
	(*a)=(*b);
	(*b)=c;
}

//Allocates int matrix nxm
int** generateIntMatrix(int n, int m)
{
  int i;
  int **matrix;

  if((matrix = (int**)malloc(sizeof(int) * n * m +
		      sizeof(int *) * n	 )) == NULL){
    printf("Out of memory, exit.");
    exit(1);
  }
  for ( i = 0 ; i < n ; i++ ) {
    matrix[i] = (int *)(matrix + n) + i*m;
  }
  return matrix;
}

//allocates double matrix nxm
double ** generateDoubleMatrix( int n, int m)
{

  int i;
  double **matrix;

  if((matrix = (double**)malloc(sizeof(double) * n * m +
		      sizeof(double *) * n	 )) == NULL){
    printf("Out of memory, exit.");
    exit(1);
  }
  for ( i = 0 ; i < n ; i++ ) {
    matrix[i] = (double *)(matrix + n) + i*m;
  }
  return matrix;
}

//Allocates int vector of size n
int * generateIntVector(int n)
{
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

//Allocates double vector of size n
double * generateDoubleVector(int n)
{
	double *vector;
	vector=(double* )malloc(sizeof(double)*n);
	return vector;
}

//deep copies matrix from origin to destination
void deepCopyMatrix(int** o,int** d,int n,int m)
{
	for(int i=0;i<n;i++)
	{
		memcpy(d[i], o[i], m*(sizeof(int)));	
	}
}

//deep copies vector from origin to destination
void deepCopyVector(int* o,int* d,int n)
{
	memcpy(d,o,n*(sizeof(int)));
}

//prints array str is the name displayed
void printArray(int** a,int n,int m,const char* str)
{
	int i,j;
	printf("\n============= %s ====================\n",str);
	for(i=0;i<n;i++)
	{
		printf("(%3d) ",i);
		for(j=0;j<m;j++)
		{
			printf("%3d ",a[i][j]);
		}
		printf("\n");
	}
}

//prints vector. str is the name displayed
void printVector(int* a,int n,const char* str)
{
	int i;
	printf("\n %s : n",str);
	for(i=0;i<n;i++) printf("%3d ",a[i]);
	printf("\n");
}

//sums vector
int sumVector(int* a,int n)
{
	int i,sum=0;
	for(i=0;i<n;i++) sum+=a[i];
	return(sum);
}

//random number between lower and upper (both included)
int r8Int(int lower,int upper)
{
  return (lower+floor(r8Uniform01()*(double)(upper-lower+1)) );
}

//returns a random number drawn from the standard normal distribution
double r8NormalStandarized()
{
  int hz;
  uint32_t iz;
  const double r = 3.442620;
  double value;
  double x;
  double y;

  hz = ( int ) shr3_seeded();
  iz = ( hz & 127 );

  if ( abs ( hz ) < kn[iz] )
  {
    value = ( double ) ( hz ) * wn[iz];
  }
  else
  {
    for ( ; ; )
    {
      if ( iz == 0 )
      {
        for ( ; ; )
        {
          x = - 0.2904764 * log ( r8Uniform01() );
          y = - log ( r8Uniform01() );
          if ( x * x <= y + y )
          {
            break;
          }
        }

        if ( hz <= 0 )
        {
          value = - r - x;
        }
        else
        {
          value = + r + x;
        }
        break;
      }

      x = ( double ) ( hz ) * wn[iz];

      if ( fn[iz] + r8Uniform01() * ( fn[iz-1] - fn[iz] ) 
        < exp ( - 0.5 * x * x ) )
      {
        value = x;
        break;
      }

      hz = ( int ) shr3_seeded();
      iz = ( hz & 127 );

      if ( abs ( hz ) < kn[iz] )
      {
        value = ( double ) ( hz ) * wn[iz];
        break;
      }
    }
  }
  return value;
}

//returns normal distribution with mean and standard deviation
double r8Normal(double mean,double sd)
{
  return(mean+r8NormalStandarized()*sd);
}

//returns normal distribution rounded to the next integer
int r8NormalRounded(double mean,double sd)
{
  return((int)((ceil)(r8Normal(mean,sd))));
}

//return skewed normal distribution (if the number is not within the desired range draw again
int r8NormalRoundedSkewed(double mean,double sd,int m,int M)
{
  int v;
  for(;;)
  {
    v=(int)((ceil)(r8Normal(mean,sd)));
    if((v>=m)&&(v<=M)) break;
  }
  return(v);
}


//sets data needed by R4_NOR.
void initializeNormal()
{
  double dn = 3.442619855899;
  int i;
  const double m1 = 2147483648.0;
  double q;
  double tn = 3.442619855899;
  const double vn = 9.91256303526217E-03;

  q = vn / exp ( - 0.5 * dn * dn );

  kn[0] = ( uint32_t ) ( ( dn / q ) * m1 );
  kn[1] = 0;

  wn[0] = ( double ) ( q / m1 );
  wn[127] = ( double ) ( dn / m1 );

  fn[0] = 1.0;
  fn[127] = ( double ) ( exp ( - 0.5 * dn * dn ) );

  for ( i = 126; 1 <= i; i-- )
  {
    dn = sqrt ( - 2.0 * log ( vn / dn + exp ( - 0.5 * dn * dn ) ) );
    kn[i+1] = ( uint32_t ) ( ( dn / tn ) * m1 );
    tn = dn;
    fn[i] = ( double ) ( exp ( - 0.5 * dn * dn ) );
    wn[i] = ( double ) ( dn / m1 );
  }

  return;
}

//returns random number between 0 and 1
double r8Uniform01 ( )
{
  uint32_t jsr_input;
  double value;

  jsr_input = jsr;
  jsr = ( jsr ^ ( jsr <<   13 ) );
  jsr = ( jsr ^ ( jsr >>   17 ) );
  jsr = ( jsr ^ ( jsr <<    5 ) );
  value = fmod ( 0.5 + ( double ) ( jsr_input + jsr ) / 65536.0 / 65536.0, 1.0 );
  return value;
}

void initializeRandom(int start)
{
  jsr=start;
}

