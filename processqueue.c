#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

//Declare a structure for process queue and for memory allocation.
typedef struct process_queue_type
{
	int process_id;
	int k;
	int timestamp;
	int burst_time;
	int memory_required;
	int priority;
	int status; 
	struct process_queue_type *next;
}process_q;

typedef struct memory_block_allocate
{
	int block_id;
	int block_size;
	int allocated_status;
	int process_id;
	struct  memory_block_allocate *next;
}alloc_ptr;

process_q *pq=NULL;
alloc_ptr *a;


//List of functions used.

process_q *makenode(int process_id,int timestamp,int burst_time,int memory_req,int priority,int status);
void initialise_process();
void insert_process(int process_id,int timestamp,int burst_time,int memory_req,int priority,int status);
void delete_process();
void deallocate_process(int processid);
void print_memory_blocks_given();

int firstfit(int memory,int processid);
int bestfit(int memory,int processid);

void display_blocks();


process_q* SortedMerge(process_q* a, process_q* b); 
void FrontBackSplit(process_q* source, process_q** frontRef, process_q** backRef); 




//Initialising the values of block_id,block_size for some processes.

void initialise_process()
{
	int n;
	FILE* fp=fopen("123.txt","r");
	fscanf(fp,"%d",&n);
	printf("Memory blocks are : %d\n",n);
	alloc_ptr *nptr,*ptr;
	int i;
	for(i=0;i<n;i++)
	{
		nptr=(alloc_ptr*)malloc(sizeof(alloc_ptr));
		if(a==NULL)
		{
			nptr->block_id=n-i;
			fscanf(fp,"%d",&nptr->block_size);
			nptr->allocated_status=0;
			a=nptr;
			nptr->next=NULL;
			
			
		}
		else
		{
			nptr->block_id=n-i;
			fscanf(fp,"%d",&nptr->block_size);
			nptr->allocated_status=0;
			nptr->next=a;
			a=nptr;                                            //insert at start.
		}
	}
	fclose(fp);
}

//Choose any of the memory allocation for a particular process.
int firstfit(int memory,int processid)
{
	alloc_ptr *ptr;
	ptr=a;
	int p=0;
	int check;
	int new_memory;
	int ret_val=1;
	
	while(ptr->next!=NULL&&p==0)
	{
		if(ptr->allocated_status==0&&memory<=ptr->block_size)
		{
			ptr->process_id=processid;
			ptr->allocated_status=1;
			p=1;
		}	
		else
		{
			ptr=ptr->next;
		}
	}
	if(p==0)
	{
		printf("No enough space\n");
		printf("Do you want to change memory size?\n1.yes 2.no\n"); 
		//if yes is selected memory is altered so as to use a particular process else memory not allocated as a result process is not inserted
		scanf("%d",&check);
		if(check==1)
		{
			printf("Enter the new memory value that you want to replace with\n");
			scanf("%d",&new_memory);
			ret_val=firstfit(new_memory,processid);
		}
        else
        {
        	printf("Given memory space is not created,hence process with process id %d is not inserted",processid);
        	ret_val=0;
		}
		
	}
	return ret_val;
}
int bestfit(int memory,int processid)
{
	alloc_ptr *ptr,*temp;
	ptr=a;
	int p=0;
	int min=1000;
	int ret_val=1;
	int check;
	int new_memory;
	while(ptr!=NULL)
	{

		
		if(min>=ptr->block_size&&ptr->allocated_status==0&&memory<=ptr->block_size)
		{
		
			temp=ptr;
			min=ptr->block_size;
			p=1;
		}
		ptr=ptr->next;
		
		
	}
	temp->process_id=processid;
	temp->allocated_status=1;
	if(p==0)
	{
		printf("No enough space\n");
		printf("Do you want to change memory size?\n1.yes 2.no\n"); 
		//if yes is selected memory is altered so as to use a particular process else memory not allocated as a result process is not inserted
		scanf("%d",&check);
		if(check==1)
		{
			printf("Enter the new memory value that you want to replace with\n");
			scanf("%d",&new_memory);
			ret_val=bestfit(new_memory,processid);
		}
        else
        {
        	printf("Given memory space is not created,hence process with process id %d is not inserted",processid);
        	ret_val=0;
		}
		
	}
	return ret_val;

}

void deallocate_process(int processid)
{
	alloc_ptr *ptr;
	ptr=a;
	while(ptr!=NULL)
	{
		if(processid==ptr->process_id)
		{
			ptr->allocated_status=0;
		}
		ptr=ptr->next;
	}
}






process_q *makenode(int process_id,int timestamp,int burst_time,int memory_required,int priority,int status)
{
	process_q *nptr;
	nptr=(process_q*)malloc(sizeof(process_q));
	if(nptr!=NULL)
	{
		nptr->process_id=process_id;
		nptr->timestamp=timestamp;
		nptr->burst_time=burst_time;
		nptr->memory_required=memory_required;
		nptr->priority=priority;
		nptr->status=status;
		nptr->k=0;
		nptr->next=NULL;
	}
	else
	{
		
		printf("No node created");
	}
}

void insert_process(int process_id,int timestamp,int burst_time,int memory_required,int priority,int status)
{
	process_q *nptr=makenode(process_id,timestamp,burst_time,memory_required,priority,status);
	process_q *ptr;
	ptr=pq;
	if(pq==NULL)
	{
		pq=nptr;
	}
	else
	{
		while(ptr->next!=NULL)
		{
			ptr=ptr->next;
		}
		ptr->next=nptr;
	}                                            //insert at end.
}
void delete_process()
{
	process_q *ptr;
	ptr=pq;
	pq=pq->next;
	free(ptr);
}
  
//Sorting to get the process that has highest priority to running state.

void MergeSort(process_q** headRef) 
{ 
    process_q* head = *headRef; 
    process_q* a; 
    process_q* b; 
  
    
    if ((head == NULL) || (head->next == NULL)) { 
        return; 
    } 
  
    
    FrontBackSplit(head, &a, &b); 
  
    
    MergeSort(&a); 
    MergeSort(&b); 
  
    
    *headRef = SortedMerge(a, b); 
} 
  

process_q* SortedMerge(process_q* a, process_q* b) 
{ 
    process_q* result = NULL; 
  
    
    if (a == NULL) 
        return (b); 
    else if (b == NULL) 
        return (a); 
  
    
    if ((a->priority <= b->priority)&&(a->process_id<=b->process_id)) { 
        result = a; 
        result->next = SortedMerge(a->next, b); 
    } 
    else { 
        result = b; 
        result->next = SortedMerge(a, b->next); 
    }
    return (result); 
} 
  
void FrontBackSplit(process_q* source, 
                    process_q** frontRef, process_q** backRef) 
{ 
    process_q* fast; 
    process_q* slow; 
    slow = source; 
    fast = source->next; 
  
    
    while (fast != NULL) { 
        fast = fast->next; 
        if (fast != NULL) { 
            slow = slow->next; 
            fast = fast->next; 
        } 
    } 
  
    
    *frontRef = source; 
    *backRef = slow->next; 
    slow->next = NULL; 
} 
  

void printList(process_q* node) 
{ 
    while (node != NULL) { 
        printf("process is %d and priority is %d\n",node->process_id,node->priority); 
        node = node->next; 
    } 
} 

void print_memory_blocks_given()
{
	alloc_ptr *pt;
	pt=a;
	
	while(pt!=NULL)
	{
	   printf("Block size :%d\n",pt->block_size);
	   pt=pt->next;
	}
}



void display_blocks()
{
	alloc_ptr *ptr,*ptr1;
	ptr=a;
	ptr1=a;
	int no_of_freeblocks;
	int a=1,b=1;
	while(ptr!=NULL)
	{
		
		if(ptr->allocated_status==0)
		{
			if(a==1)
			{
			printf("--------------------------------\n");
			printf("Free blocks are :\n");
			a=0;
		    }
			printf("Block id is %d\n",ptr->block_id);
			printf("Block size is %d\n",ptr->block_size);
			no_of_freeblocks++;
			
		}
		ptr=ptr->next;
		
    }
    printf("No of free blocks are %d\n",no_of_freeblocks);
    
    while(ptr1!=NULL)
	  {
	  	if(ptr1->allocated_status==1)
		{
			if(b==1)
			{
			printf("-----------------------------------------------\n");
			printf("Allocated blocks are :\n");
			b=0;
		   }
			printf("Block id %d contains process_id %d\n",ptr1->block_id,ptr1->process_id);
		 
	    }
	    ptr1=ptr1->next;
		
	}
	
}

  

main()
{
	int time=0;
	int total_burst_time=0;
	int n,i,choice,p;
	int set=1;
	int a=1;
	int b,f;
	int prev_priority,curr_priority;
	int process_id,timestamp,burst_time,memory_required,priority,status;
	
	process_q *ptr,*ptr1,*ptr2;
	process_q *temp;
	
	initialise_process();
	print_memory_blocks_given();
	
	FILE* fp=fopen("789.txt","r");
	
		while(time<=total_burst_time)                              //Consider one iteration of the loop equals 1 second of time.
		{                                             //Loop iterates till the time reaches the sum of burst_time of all processes is obtained.
			
			fscanf(fp,"%d",&n);
			if(n!=0)
			{
			printf("No. of processes are %d\n",n);
			for(i=0;i<n;i++)
			{
				
				
				fscanf(fp,"%d",&process_id);
				printf("Process_id is : %d\n",process_id);
				fscanf(fp,"%d",&timestamp);
				printf("Timestamp is : %d\n" ,timestamp);
				fscanf(fp,"%d",&burst_time);
				printf("Burst_time is : %d\n",burst_time);
				fscanf(fp,"%d",&memory_required);
				printf("Memory_required is : %d \n",memory_required);	
				fscanf(fp,"%d",&priority);
				printf("Priority is : %d \n",priority);
				fscanf(fp,"%d",&status);
				printf("Allocated status is : %d\n",status);
				printf("Choose any memory allocation process out of given two\n");
				printf("1.First fit or 2.Best fit\n");
				scanf("%d",&choice);
				if(choice==1)
				{
					f=firstfit(memory_required,process_id);
					
				}
				else
				{
					b=bestfit(memory_required,process_id);
				}
				
				if(f==1||b==1)      //condition check whether to insert the process or not(used to avoid memory blocks which asked to more space than available)
				{
				total_burst_time=total_burst_time+burst_time;
				insert_process(process_id,timestamp,burst_time,memory_required,priority,status);
			    }
				
			}
		
		   	MergeSort(&pq);
		    ptr=pq;
		
		    if(a)
		    {
		    	prev_priority=curr_priority=ptr->priority;
		    	a=0;
			}
		    prev_priority=curr_priority;
			curr_priority=ptr->priority;
	    }
		  printList(pq);
		  ptr=pq;
		  
			if(ptr->status==3)
			{
				printf("Choose any memory allocation process out of given two\n");
				printf("1.First fit or 2.Best fit\n");
				scanf("%d",&choice);
				if(choice==1)
				{
					firstfit(ptr->memory_required,ptr->process_id);
				}
				else
				{
					bestfit(ptr->memory_required,ptr->process_id);
				}
				ptr->status=2;
			}
			
			if(ptr->status==1)
			{
				ptr->status=2;
			}
			
			if(ptr->burst_time!=0)
			{
			     	ptr->burst_time=ptr->burst_time-1;
					printf("Running process at time  %d is %d\n",time,ptr->process_id);
				
				if((prev_priority!=curr_priority)&&n)
				{   
				    ptr2=ptr;
					while(ptr2!=NULL&&set)
					{
					  ptr2=ptr2->next;
					  if(ptr2->priority==prev_priority)	
					  {
					  	ptr2->k=time;
					  	ptr2->status=3;
					  	deallocate_process(ptr2->process_id);
					  	set=0;
					  }
						
					}
				}
				
			
			}
			
			if(ptr->burst_time==0)
			{
				ptr->status=4;
				printf("Completed process at time  %d is %d\n",time,ptr->process_id);
				deallocate_process(ptr->process_id);
				delete_process();	
				ptr=pq;
				if(ptr==NULL)
				{
			       printf("-----------------------------------------------------------------\n");	
				   printf("All processes are completed\n");	
				   time=time+1;
				}
			}
			ptr1=pq;
			while(ptr1!=NULL)
			{
				if(time!=0&&(time-(ptr1->k)+1)%30==0&&ptr1->status==3)
				{
					if(ptr1->priority>1)
					{
		            printf("Priority of process %d updated\n",ptr1->process_id);
					ptr1->priority=ptr1->priority-1;
					MergeSort(&pq);
				    }
				}
				ptr1=ptr1->next;
			}
			display_blocks();
		
			time=time+1;	
			printf("-----------------------------------------------------------------\n");	
		
		
   }
		fclose(fp);
	} 






























