#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/********************************************************************************************************************

Very important: The objective is measured in terms of the number of production stages (hence inst->l is optimal)

********************************************************************************************************************/

typedef struct instance
{
  int nt; //number of tasks
  int c; //cycle time
  int lb; //lower bound
  int* t; //task time of each task
  int** p; // table with predecessors / successors p[i][j] states if i is predecessor of j 
  int** ip; //immediate predecessors ip[i][0] is the number, then for ip[i][1] to ip[i][ip[i][0]] the predecessor number
  int** is; //immediate successors is[i][0] is the number, then for is[i][1] to is[i][is[i][0]] the successor number
  double os;
}instance;

typedef struct characteristics
{
  double mean1;
  double sd1;
  double mean2;
  double sd2;
  int isBimodal;
  double prob1;
  int m; //minimum task time
  int M; //maximum task time
  double OS;
  double delta;
  int withChains;
  int numChains;
  int tasksInChain;
  int withBottlenecks;
  int numBottlenecks;
  int precSucBottlenecks;
  int numLevels;
  int nt;
  int numSolitary;
  int cycle;
}characteristics;

//generate file (save to instanceName)
int generateGraph(instance* inst,characteristics* def);
int generateTaskTimes(instance* inst,characteristics* def);
