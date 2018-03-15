#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


enum pstate {ready = 0, running = 1, error = 2, complete = 4};

struct Process 
{
  int pid;
  int arrival;
  int burst;
  int status;

  double wait;
  
};

int process_finished(struct Process * processes, int pcounter)
{
  int status = 0x4;
  for (int i = 0; i < pcounter; i++)
  {
    status = status & processes[i].status;
  }
  return (status == 0x4) ? 1 : 0;
}
void shortest_job_first(struct Process * processes, int pcounter)
{
  int counter = 0;
  int i;
  
  printf("Time ");
  for (i = 0; i < pcounter; i++)
  {
    printf("   P%d   ", processes[i].pid);
  }
  printf("\n-------------------------------------------------------------------------------------\n");

  int run = 0; 
  int burst_limit = 999;

  while(!process_finished(processes, pcounter)){

    for(i = 0; i < pcounter; i++){
      if(processes[i].arrival <= counter && processes[i].status != complete){
	if(processes[i].burst < burst_limit){
	  burst_limit = processes[i].burst;
	  run = processes[i].pid;
	}
      }
    }
    printf("\n%3d", counter);
	
  char* dot = ".";
  char* plus = "+";
  char* space = "";

  
    for(i = 0; i < pcounter; i++){
      if(run == processes[i].pid){
	printf("%7.7s", dot);
	processes[i].burst--;
	if (processes[i].burst <= 0)
	  processes[i].status = complete;
      }
      else if(processes[i].arrival > counter || processes[i].status == complete){
	printf("%7.7s", space);
      }
      else{
	processes[i].wait++;
	printf("%7.7s", plus);
      }
    }
    counter++;
    burst_limit = 999;
  }

  printf("\n");
  
  double wait_time = 0.0;
  for(i = 0; i < pcounter; i++){
    printf(" P%d waited %f seconds\n", processes[i].pid, processes[i].wait);
    wait_time += processes[i].wait;
  }
  printf(" Average waiting time = %f seconds.\n", wait_time/pcounter);
}

void round_robin(struct Process * processes,int pcounter, int limit_rr){
  int counter = 0;

  int q[pcounter];
  int in = 0;
  int out = 0;
  int i;
  int index;
  

  for (i = 0; i < pcounter; i++)
  {
    q[i] = 0;
  }

  printf("Time ");
  
  for (i = 0; i < pcounter; i++)
  {
    printf("   P%d   ", processes[i].pid);
  }
  printf("\n-------------------------------------------------------------------------------------\n");

  while(!process_finished(processes, pcounter))
	  {
		for (int i = pcounter - 1; i >= 0; i--)
		{
			if(processes[i].arrival <= counter && processes[i].status != complete)
			{
				if (q[in] == 0)
				{
				  q[in] = processes[i].pid;				  
				  in++;
				  
				  if (in >= pcounter) 
				  {
					  in = 0;
				  }
				}
		}
    }

  char* plus = "+";
  char* dot = ".";
  char* space = "";
  
    if(q[out] != 0)
	{
      
      for (int i = 0; i < limit_rr; i++)
	  {
		if(processes[q[out] - 1].status == complete) 
		{
			break;
		}
		printf("\n%3d", counter);
		
		for (int j = 0; j < pcounter; j++)
		{
		  if(processes[j].arrival > counter || processes[j].status == complete)
		  {
			printf("%7.7s", space);
		  }
		  else
		  {
			if(q[out] == processes[j].pid)
			{
			  printf("%7.7s", dot);
			}
			else
			{
			  printf("%7.7s", plus);
			  processes[j].wait++;
			}
		  }
		}
		counter++;
		index = q[out] - 1;
		processes[index].burst--;
		
		if(processes[index].burst <= 0) 
		{
		  processes[index].status = complete;
		  break;
		}				   
      }
      q[out] = 0;
      out++;
       if(out >= pcounter) 
	   {
		   out = 0;
	   }
    }
    else 
	{
      printf("\n%3d", counter);
      counter++;
    }
  }
  printf("\n");
   double wait_time = 0.0;
  
  for(i = 0; i < pcounter; i++)
  {
    printf(" P%d waited %f seconds\n", processes[i].pid, processes[i].wait);
    wait_time += processes[i].wait;
  }
  printf(" Average waiting time = %f seconds.\n", wait_time/pcounter);
}


int main(int argc, char ** argv){
  if (argc != 4 && argc != 3)
  {
    printf("error please check your arguments :)");
    exit (-1);
  }

  FILE *file_open;
  file_open = fopen(argv[1], "r");
  
  struct Process processes[30];
  char * mode = argv[2];
  int i = 0;
  
  while (fscanf(file_open, "%d %d", &processes[i].arrival, &processes[i].burst) > 1)
  {
    processes[i].status = error;
    processes[i].pid = i + 1;
    processes[i].wait = 0.0;
    i++;
  }
  
  fclose(file_open);

  if (strcmp("RR",mode) == 0)
  {   
    int limit_rr = atoi(argv[3]); 
    round_robin(processes, i, limit_rr);
  }
  
  else if(strcmp("SJF",mode) == 0)
  {
    shortest_job_first(processes, i);
  }
  
  else
  {
	  printf("Wrong input");
	  exit(-1);
  }
 
  return 0;
}
 