#include <string.h>
#include "albp.h"
#include <assert.h>
#include <math.h>

#define MAX_TRIES 100000
#define MAX_TRIES_EXTERNAL 100000

void testLevel(instance* inst,int* level)
{
  int i,j,k;
  int levelTest[inst->nt+1];
  int c=0;

  for(i=1;i<=inst->nt;i++) levelTest[i]=1;
  do
  {
    c=0;
    for(i=1;i<=inst->nt;i++)
    {
      for(k=1;k<=inst->is[i][0];k++)
      {
        j=inst->is[i][k];
        if(levelTest[j]<(levelTest[i]+1))
        {
          levelTest[j]=levelTest[i]+1;
          assert(levelTest[j]<=inst->nt);
          c=1;
        }
      }
    }
  }while(c==1);
  for(i=1;i<=inst->nt;i++) assert(level[i]==levelTest[i]);

  for(i=1;i<=inst->nt;i++) levelTest[i]=1;
  do
  {
    c=0;
    for(i=1;i<=inst->nt;i++)
    {
      for(j=1;j<=inst->nt;j++)
      {
        if(inst->p[i][j]==0) continue;
        if(levelTest[j]<(levelTest[i]+1))
        {
          levelTest[j]=levelTest[i]+1;
          c=1;
        }
      }
    }
  }while(c==1);
  for(i=1;i<=inst->nt;i++) assert(level[i]==levelTest[i]);
}

void printImmediatePrecedenceGraph(instance* inst)
{
  int i,j;

  for(i=1;i<=inst->nt;i++)
  {
    printf("%d\t( ",i);
    for(j=1;j<=inst->ip[i][0];j++)
    {
      printf("%d ",inst->ip[i][j]);
    }
    printf(") (");
    for(j=1;j<=inst->is[i][0];j++)
    {
      printf("%d ",inst->is[i][j]);
    }
    printf(")\n");
  }
}

//returns the number of arcs if we add an arc between pred and succ
//modifies predecessor matrix, immediante predecessors and immediate successors
int addEdge(instance* inst,int pred,int succ,int arcCounter)
{
  int i,j;

  assert(inst->p[pred][succ]==0);
  inst->p[pred][succ]=1;
  arcCounter++;
  for(i=1;i<=inst->nt;i++) //this loop detects i -- pred-succ -- j
  {
    if((i!=pred)&&(i!=succ))
    {
      for(j=1;j<=inst->nt;j++)
      {
        if((i!=j)&&(inst->p[i][j]==0)&&(j!=pred)&&(j!=succ)&&(inst->p[i][pred]==1)&&(inst->p[succ][j]==1))
        {
          arcCounter++;
          inst->p[i][j]=1;
        }
      }
    }
  }
  for(j=1;j<=inst->nt;j++) //this loop detects pred-succ -- j
  {
    if((j!=pred)&&(j!=succ)&&(inst->p[pred][j]==0)&&(inst->p[succ][j]==1))
    {
      arcCounter++;
      inst->p[pred][j]=1;
    }
  } 
  for(i=1;i<=inst->nt;i++) //this loop detects i -- pred-succ
  {
    if((i!=pred)&&(j!=pred)&&(inst->p[i][succ]==0)&&(inst->p[i][pred]==1))
    {
      arcCounter++;
      inst->p[i][succ]=1;
    }
  }
  inst->ip[succ][0]++;
  inst->ip[succ][inst->ip[succ][0]]=pred;
  inst->is[pred][0]++;
  inst->is[pred][inst->is[pred][0]]=succ;
  return(arcCounter);
}

//Counts number of arcs and returns order strength after adding an arc between pred and succ
double evaluateOS(instance* inst,int pred,int succ,int arcCounter)
{
  int i,j;
  double os;

  if((pred!=0)&&(succ!=0)) assert(inst->p[pred][succ]==0);
  if((pred!=0)&&(succ!=0)) arcCounter++;

  for(i=1;i<=inst->nt;i++) //this loop detects i -- pred-succ -- j
  {
    if((i!=pred)&&(i!=succ))
    {
      for(j=1;j<=inst->nt;j++)
      {
        if((i!=j)&&(inst->p[i][j]==0)&&(j!=pred)&&(j!=succ)&&(inst->p[i][pred]==1)&&(inst->p[succ][j]==1))
        {
          arcCounter++;
        }
      }
    }
  }
  for(j=1;j<=inst->nt;j++) //this loop detects pred-succ -- j
  {
    if((j!=pred)&&(j!=succ)&&(inst->p[pred][j]==0)&&(inst->p[succ][j]==1))
    {
      arcCounter++;
    }
  } 
  for(i=1;i<=inst->nt;i++) //this loop detects i -- pred-succ
  {
    if((i!=pred)&&(j!=pred)&&(inst->p[i][succ]==0)&&(inst->p[i][pred]==1))
    {
      arcCounter++;
    }
  }
  assert(arcCounter<=((inst->nt*inst->nt-inst->nt)/2));
  os=(double)(arcCounter)/(double)((inst->nt*inst->nt-inst->nt)/2);
  return os;
}

//calculates transitive closure of the graph from inst->p[][]
//returns the number of arcs
int transitiveClosure(instance* inst)
{
  int i,j,k,c,arcCounter;

  arcCounter=0;
  for(i=1;i<=inst->nt;i++)
  {
    for(j=1;j<=inst->nt;j++)
    {
      if(inst->p[i][j]==1) arcCounter++;
    }
  }
  #if DEBUGLEVEL>0
  for(i=1;i<=inst->nt;i++)
  {
    for(j=1;j<=inst->is[i][0];j++)
    {
      assert(inst->p[i][inst->is[i][j]]==1);
    }
    for(j=1;j<=inst->ip[i][0];j++)
    {
      assert(inst->p[inst->ip[i][j]][i]==1);
    }
  }
  #endif
  do
  {
    c=0;
    for(i=1;i<=inst->nt;i++)
    {
      for(k=1;k<=inst->nt;k++)
      {
        if((i!=k)&&(inst->p[i][k]==0))
        {
          for(j=1;j<=inst->nt;j++)
          {
            if((i!=j)&&(j!=k)&&(inst->p[i][j]==1)&&(inst->p[j][k]==1))
            {
              arcCounter++;
              inst->p[i][k]=1;
              c=1;
              break;
            }
          }
        }
      }
    }
  }while(c==1);
  return(arcCounter);

}

//basic check to obtain order strength from immediante successors from scratch (somewhat expensive)
void checkOS(instance* inst,double currentOS)
{
  int i,j,arcCounter;
  double os;

  for(i=1;i<=inst->nt;i++)
  {
    for(j=1;j<=inst->nt;j++) inst->p[i][j]=0;
    for(j=1;j<=inst->is[i][0];j++) inst->p[i][inst->is[i][j]]=1;
  }
  arcCounter=transitiveClosure(inst);
  os=(double)(arcCounter)/((double)(inst->nt*inst->nt-inst->nt)/2.0);
  //printf("checkOS: arcCounter: %d os: %f\n",arcCounter,os);
  assert(fabs(os-currentOS)<0.000001);
}

//returns the suggested number of levels according to Otto, Otto and Scholl
int setNumberLevels(int nt)
{
  int k;  
  for(k=2;k<nt;k++)
  {
    if(((k*k)*(2*k-1))>(nt*nt)) break;
  }
  //k has the upper bound, we have to choose k or k-1
  if(abs((k*k*(2*k-1))-(nt*nt))<abs(((k-1)*(k-1)*(2*(k-1)-1))-(nt*nt))) return(k);
  else return(k-1);
}

//initialize the instances for generating the graph
void initMemory(instance* inst,instance* outInstance)
{
  //inst.t= generateIntVector(inst->nt+1); //to keep tasks numbered from 1 to nt
  inst->p=generateIntMatrix(inst->nt+1,inst->nt+1); //to keep tasks numbered from 1 to nt
  inst->ip=generateIntMatrix(inst->nt+1,inst->nt+1); //to keep tasks numbered from 1 to nt
  inst->is=generateIntMatrix(inst->nt+1,inst->nt+1); //to keep tasks numbered from 1 to nt
  outInstance->p=generateIntMatrix(inst->nt+1,inst->nt+1); //to keep tasks numbered from 1 to nt
  outInstance->ip=generateIntMatrix(inst->nt+1,inst->nt+1); //to keep tasks numbered from 1 to nt
  outInstance->is=generateIntMatrix(inst->nt+1,inst->nt+1); //to keep tasks numbered from 1 to nt
}

//setup memory
void cleanMemory(int numLevels,instance* inst,int* fixedPredecessors,int* fixedSuccessors,int* level,int* numTasksInLevel,int* acceptsSuccessors,int** acceptsSuccessorsArray,int** acceptsPredecessors)
{
  int i,j;

  for(i=1;i<=inst->nt;i++)
  {
    for(j=1;j<=inst->nt;j++)
    {
      inst->p[i][j]=0;
    }
  }
  for(i=1;i<=inst->nt;i++) inst->ip[i][0]=0;
  for(i=1;i<=inst->nt;i++) inst->is[i][0]=0;
  for(i=1;i<=inst->nt;i++) fixedPredecessors[i]=0;
  for(i=1;i<=inst->nt;i++) fixedSuccessors[i]=0;
  for(i=1;i<=inst->nt;i++) level[i]=0;
  for(i=1;i<=numLevels;i++) numTasksInLevel[i]=0;
  acceptsSuccessors[0]=0;
  for(i=1;i<=inst->nt;i++) acceptsSuccessorsArray[i][0]=0;
  for(i=1;i<=inst->nt;i++) acceptsPredecessors[i][0]=0;
}

//adds a solitary task to the instance. Updates number of tasks
int solitaryTasks(int taskCounter,int* fixedPredecessors,int* fixedSuccessors,int* level)
{
  taskCounter++;
  fixedPredecessors[taskCounter]=1;
  fixedSuccessors[taskCounter]=1;
  level[taskCounter]=1;
  return(taskCounter);
}

//adds a chain of tasks to the instance. Updates number of tasks
int chainTasks(int taskCounter,int numLevels,int tasksInChain,instance* inst,int* fixedPredecessors,int* fixedSuccessors,int** acceptsPredecessors,int* acceptsSuccessors,int** acceptsSuccessorsArray,int* level,int* numTasksInLevel) 
{
  int j,k;

  k=r8Int(1,numLevels+1-tasksInChain);
  //fprintf(stderr,"chain starting in level: %d \t",k);
  //first task of the chain
  taskCounter++;
  fixedSuccessors[taskCounter]=1;
  level[taskCounter]=k;
  if(k==1)
  {
    fixedPredecessors[taskCounter]=1;
  }
  else
  {
    acceptsPredecessors[k][0]++;
    acceptsPredecessors[k][acceptsPredecessors[k][0]]=taskCounter;
  }
  numTasksInLevel[k]++;
  //fprintf(stderr,"tasks: %d ",taskCounter);
  //tasks in the middle
  for(j=k+1;j<(k+tasksInChain-1);j++)
  {
    taskCounter++;
    fixedPredecessors[taskCounter]=1;
    fixedSuccessors[taskCounter]=1;
    level[taskCounter]=j;
    numTasksInLevel[j]++;
    //fprintf(stderr," %d ",taskCounter);
  }
  //last task of the chain
  taskCounter++;
  fixedPredecessors[taskCounter]=1;
  level[taskCounter]=k+tasksInChain-1;
  if(level[taskCounter]==numLevels) 
  {
    fixedSuccessors[taskCounter]=1;
  }
  else
  {
    acceptsSuccessorsArray[level[taskCounter]][0]++;
    acceptsSuccessorsArray[level[taskCounter]][acceptsSuccessorsArray[level[taskCounter]][0]]=taskCounter;
    acceptsSuccessors[0]++;
    acceptsSuccessors[acceptsSuccessors[0]]=taskCounter;
  }
  numTasksInLevel[k+tasksInChain-1]++;
  //fprintf(stderr,"%d\n",taskCounter);
  for(j=taskCounter+1-tasksInChain;j<taskCounter;j++)
  {
    inst->p[j][j+1]=1;
    inst->ip[j+1][0]++;
    inst->ip[j+1][inst->ip[j+1][0]]=j;
    inst->is[j][0]++;
    inst->is[j][inst->is[j][0]]=j+1;
  }
  return(taskCounter);
}

//adds a bottleneck to the instance. Updates number of tasks
int bottleneckTasks(int taskCounter,int numLevels,int precSucBottlenecks,instance* inst,int* fixedPredecessors,int* fixedSuccessors,int** acceptsPredecessors,int* acceptsSuccessors,int** acceptsSuccessorsArray,int* level,int* numTasksInLevel)
{
  int j,k;

  k=r8Int(2,numLevels-1);
  //fprintf(stderr,"bottleneck: %d (level %d)\t\t",taskCounter+1,k);
  for(j=0;j<precSucBottlenecks;j++)
  {
    taskCounter++;
    level[taskCounter]=k-1;
    fixedSuccessors[taskCounter]=1;
    if(k==2) fixedPredecessors[taskCounter]=1;
    acceptsPredecessors[k-1][0]++;
    acceptsPredecessors[k-1][acceptsPredecessors[k-1][0]]=taskCounter;
    //fprintf(stderr,"%d\t",taskCounter);
  }
  taskCounter++;
  fixedPredecessors[taskCounter]=1;
  fixedSuccessors[taskCounter]=1;
  level[taskCounter]=k;
  //fprintf(stderr," -- %d -- \t",taskCounter);
  for(j=taskCounter-precSucBottlenecks;j<taskCounter;j++)
  {
    inst->p[j][taskCounter]=1;
    inst->ip[taskCounter][0]++;
    inst->ip[taskCounter][inst->ip[taskCounter][0]]=j;
    inst->is[j][0]++;
    inst->is[j][inst->is[j][0]]=taskCounter;
  }
  for(j=taskCounter+1;j<=taskCounter+precSucBottlenecks;j++)
  {
    inst->p[taskCounter][j]=1;
    inst->ip[j][0]++;
    inst->ip[j][inst->ip[j][0]]=taskCounter;
    inst->is[taskCounter][0]++;
    inst->is[taskCounter][inst->is[taskCounter][0]]=j;
  }
  for(j=0;j<precSucBottlenecks;j++)
  {
    taskCounter++;
    level[taskCounter]=k+1;
    fixedPredecessors[taskCounter]=1;
    if((k+1)==numLevels) fixedSuccessors[taskCounter]=1;
    if(level[taskCounter]==numLevels) 
    {
      fixedSuccessors[taskCounter]=1;
    }
    else
    {
      acceptsSuccessors[0]++;
      acceptsSuccessors[acceptsSuccessors[0]]=taskCounter;
      acceptsSuccessorsArray[level[taskCounter]][0]++;
      acceptsSuccessorsArray[level[taskCounter]][acceptsSuccessorsArray[level[taskCounter]][0]]=taskCounter;
    }
    //fprintf(stderr,"%d\t",taskCounter);
  }
  //fprintf(stderr,"\n");
  return(taskCounter);
}

//adds a task to a given level
int addTaskToLevel(int taskCounter,int numLevel,int maxLevels,int* level,int* fixedPredecessors,int* fixedSuccessors,int** acceptsPredecessors,int* acceptsSuccessors,int** acceptsSuccessorsArray)
{
  taskCounter++;
  level[taskCounter]=numLevel;
  if(numLevel>1)
  {
    acceptsPredecessors[numLevel][0]++;
    acceptsPredecessors[numLevel][acceptsPredecessors[numLevel][0]]=taskCounter;
  }
  else
  {
    fixedPredecessors[taskCounter]=1;
  }
  if(numLevel<maxLevels)
  {
    acceptsSuccessors[0]++;
    acceptsSuccessors[acceptsSuccessors[0]]=taskCounter;
    acceptsSuccessorsArray[numLevel][0]++;
    acceptsSuccessorsArray[numLevel][acceptsSuccessorsArray[numLevel][0]]=taskCounter;
  }
  else
  {
    fixedSuccessors[taskCounter]=1;
  }
  return(taskCounter);
}


int generateGraph(instance* outInstance,characteristics* def)
{
  instance inst;
  int i,j,k;
  int success;
  int arcCounter;
  int taskCounter=0;
  int* fixedPredecessors;
  int* fixedSuccessors;
  int* level;
  int* numTasksInLevel;
  int* acceptsSuccessors; //used to generate arcs
  int** acceptsSuccessorsArray; //used to generate arcs
  int** acceptsPredecessors; //used to generate random arcs
  int** pp;
  double currentOS;
  int tries,triesExternal=0;
  int origin,destination;

  //initialize levels and number of tasks
  inst.nt=def->nt;
  #if DEBUGLEVEL>0 
  fprintf(stderr,"number of tasks: %d\n",def->nt);
  #endif
  //=== Set the number of levels if needed 
  if(def->numLevels==0) def->numLevels=setNumberLevels(inst.nt);
  #if DEBUGLEVEL>0
  fprintf(stderr,"number of levels: %d\n",def->numLevels);
  #endif
  //=== initialize memory
  initMemory(&inst,outInstance);
  fixedPredecessors=generateIntVector(inst.nt+1);
  fixedSuccessors=generateIntVector(inst.nt+1);
  level=generateIntVector(inst.nt+1);
  numTasksInLevel=generateIntVector(1+def->numLevels);
  acceptsSuccessors=generateIntVector(inst.nt+1);
  acceptsSuccessorsArray=generateIntMatrix(inst.nt+1,inst.nt+1);
  acceptsPredecessors=generateIntMatrix(inst.nt+1,inst.nt+1);
  pp=generateIntMatrix(inst.nt+1,inst.nt+1);
  //repeat until an instance is found or the maximum tries is made
  do
  {
    triesExternal++;
    success=YES;
    taskCounter=0;
    //==== cleaning memory
    cleanMemory(def->numLevels,&inst,fixedPredecessors,fixedSuccessors,level,numTasksInLevel,acceptsSuccessors,acceptsSuccessorsArray,acceptsPredecessors);
    //step 1 solitary tasks
    if(def->numSolitary>0) 
    {
      for(i=0;i<def->numSolitary;i++)
      {
        taskCounter=solitaryTasks(taskCounter,fixedPredecessors,fixedSuccessors,level);
      }
    } 
    //step 2 create chains
    if(def->withChains==YES) 
    {
      assert(def->tasksInChain<=def->numLevels);
      assert((def->tasksInChain*def->numChains+def->numSolitary)<def->nt);
      for(i=0;i<def->numChains;i++)
      {
        taskCounter=chainTasks(taskCounter,def->numLevels,def->tasksInChain,&inst,fixedPredecessors,fixedSuccessors,acceptsPredecessors,acceptsSuccessors,acceptsSuccessorsArray,level,numTasksInLevel);
      }
    }
    //step 3 create bottlenecks
    if(def->withBottlenecks==YES)
    {
      assert((taskCounter+def->numBottlenecks*(def->precSucBottlenecks*2+1))<=inst.nt);
      for(i=0;i<def->numBottlenecks;i++)
      {
        taskCounter=bottleneckTasks(taskCounter,def->numLevels,def->precSucBottlenecks,&inst,fixedPredecessors,fixedSuccessors,acceptsPredecessors,acceptsSuccessors,acceptsSuccessorsArray,level,numTasksInLevel);
      }
    }
    //step 4.0 add a task to each level to ensure that there is always an alternative to create precedence constraints 
    for(i=1;i<=def->numLevels;i++) taskCounter=addTaskToLevel(taskCounter,i,def->numLevels,level,fixedPredecessors,fixedSuccessors,acceptsPredecessors,acceptsSuccessors,acceptsSuccessorsArray);
    //step 4 create additional tasks uniformly distributed among levels 
    for(;taskCounter<inst.nt;) taskCounter=addTaskToLevel(taskCounter,r8Int(1,def->numLevels),def->numLevels,level,fixedPredecessors,fixedSuccessors,acceptsPredecessors,acceptsSuccessors,acceptsSuccessorsArray);
    //step 5 add one precedence to each task that requires a successor
    for(i=1;i<=inst.nt;i++)
    {
      if((fixedPredecessors[i]==0)&&(inst.ip[i][0]==0))
      {
        k=r8Int(1,acceptsSuccessorsArray[level[i]-1][0]);
        j=acceptsSuccessorsArray[level[i]-1][k];
        assert(level[j]==(level[i]-1));
        inst.p[j][i]=1;
        inst.ip[i][0]=1;
        inst.ip[i][1]=j;
        inst.is[j][0]++;
        inst.is[j][inst.is[j][0]]=i;
      }
    }
    arcCounter=transitiveClosure(&inst);
    currentOS=(double)(arcCounter)/((double)(inst.nt*inst.nt-inst.nt)/2.0);

    //move to separate function
    #if DEBUGLEVEL>0 //check level values to make sure that everything was done as expected
    testLevel(&inst,level);
    #endif

    //end move to separate function
    //step 6 add random precedence relations
    //modify acceptsPredecessors to allow arcs between non-consecutive levels
    for(k=def->numLevels-1;k>1;k--)
    {
      for(i=1;i<=acceptsPredecessors[k+1][0];i++)
      {
        acceptsPredecessors[k][0]++;
        acceptsPredecessors[k][acceptsPredecessors[k][0]]=acceptsPredecessors[k+1][i];
      }
    }
    //printf("initialOS: %f vs %f\n",currentOS,def->OS+def->delta);
    if(currentOS<(def->OS+def->delta))
    {
      if(currentOS<(def->OS-def->delta))
      {
        tries=0;
        for(;;)
        {
          tries++;
          i=r8Int(1,acceptsSuccessors[0]);
          origin=acceptsSuccessors[i];
          i=r8Int(1,acceptsPredecessors[level[origin]+1][0]);
          destination=acceptsPredecessors[level[origin]+1][i];
          if(inst.p[origin][destination]==0)
          {
            currentOS=evaluateOS(&inst,origin,destination,arcCounter);
            if(currentOS<(def->OS+def->delta))
            {
              arcCounter=addEdge(&inst,origin,destination,arcCounter);
              tries=0;
            }
            if((currentOS<(def->OS+def->delta))&&(currentOS>(def->OS-def->delta)))
            {
              break;
            }
          }
          if(tries>=MAX_TRIES)
          {
            success=NO;
            break;
          }
        }
      }
    }
    else
    {
      success=NO;
    }
    if(triesExternal>MAX_TRIES_EXTERNAL) break;
  }while(success==NO);
  if(success==YES)
  {
    //printf("currentOS: %f %f\n",currentOS,getTime()-iniT);
    #if DEBUGLEVEL>0 
    checkOS(&inst,currentOS);
    testLevel(&inst,level);
    #endif
    //Before reporting the graph, (1) remove uneeded precedence relations, (2) renumber tasks and (3) copy to outInstance
    //(1) remove precedence relations
    for(i=1;i<=inst.nt;i++)
    {
      for(int c=1;c<=inst.ip[i][0];c++)
      {
        j=inst.ip[i][c];
        for(k=1;k<=inst.nt;k++)
        {
          if((i!=k)&&(j!=k)&&(inst.p[j][k]==1)&&(inst.p[k][i]==1))
          {
            inst.ip[i][c]=0; //remove direct predecessor because it can be induced through other precedence relations
            break;
          }
        }
      }
    }
    //(2) renumber tasks
    int original[inst.nt+1]; //original[i]=j; old task i corresponds to new task j
    for(i=1;i<=inst.nt;i++) original[i]=0;
    int mapped[inst.nt+1]; //mapped[i]=j; new task i corresponds to old task i
    for(i=1;i<=inst.nt;i++) mapped[i]=0;
    i=1;
    for(k=1;k<=def->numLevels;k++)
    {
      for(j=1;j<=inst.nt;j++)
      {
        if(level[j]==k)
        {
          assert(mapped[i]==0);
          mapped[i]=j;
          assert(original[j]==0);
          original[j]=i;
          i++;
        }
      }
    }
    assert(i==(inst.nt+1));
    //(3) copy out
    outInstance->nt=inst.nt;
    outInstance->os=currentOS;
    for(i=1;i<=inst.nt;i++) outInstance->ip[i][0]=0;
    for(i=1;i<=inst.nt;i++) outInstance->is[i][0]=0;
    for(i=1;i<=inst.nt;i++) 
    {
      for(j=1;j<=inst.nt;j++)
      {
        outInstance->p[i][j]=0;
      }
    }
    for(i=1;i<=inst.nt;i++)
    {
      for(int c=1;c<=inst.ip[i][0];c++)
      {
        j=inst.ip[i][c];
        if(j!=0)
        {
          outInstance->p[original[j]][original[i]]=1;
        }
      }
    }
    for(i=1;i<=inst.nt;i++)
    {
      for(j=1;j<=inst.nt;j++)
      {
        if(outInstance->p[i][j]==1)
        {
          outInstance->is[i][0]++;
          outInstance->is[i][outInstance->is[i][0]]=j;
        }
      }
    }
    #if DEBUGLEVEL>0 
    checkOS(outInstance,currentOS);
    #endif
    return(1);
  }
  return(0);
}

int generateTaskTimes(instance* inst,characteristics* def)
{

  initializeNormal();
  inst->c=def->cycle;
  if(def->m<=0) def->m=1;
  if(def->m>=inst->c) def->m=inst->c-1;
  if(def->M<=0) def->M=1;
  if(def->M>=inst->c) def->M=inst->c-1;
  inst->t=generateIntVector(inst->nt+1);
  for(int i=1;i<=inst->nt;i++)
  {
    if(def->isBimodal)
    {
      if(r8Uniform01()<=def->prob1)
      {
        inst->t[i]=r8NormalRoundedSkewed(def->mean1,def->sd1,def->m,def->M);
      }
      else
      {
        inst->t[i]=r8NormalRoundedSkewed(def->mean2,def->sd2,def->m,def->M);
      }
    }
    else
    {
      inst->t[i]=r8NormalRoundedSkewed(def->mean1,def->sd1,def->m,def->M);
    }
  }
  return(0);
}

