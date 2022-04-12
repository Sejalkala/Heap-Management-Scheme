/*
    SEJAL VIVEK KALA
    BT20CSE057
    CPL ASSIGNMENT 3
    HEAP MANAGEMENT SCHEME
    VISVESVARAYA NATIONAL INSTITUTE OF TECHNOLOGY,NAGPUR.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct free_list_tag
{
    struct free_list_tag* next;
    struct free_list_tag* prev;
    int size;
    int start;
}free_list;


typedef struct allocate_tag
{
    struct allocate_tag* next;
    struct allocate_tag* prev;
    int size;
    int start;
    char name[50];
}alloc_list;

int SIZE = 2000;

free_list *head_free=NULL;
alloc_list *head_allocate=NULL;

void allocate();
void deallocate(char name[]);

void isTagFound(char str[]);
void merge(free_list* fptr);

void allocated_list_print();
void free_list_print();



void allocate()
{
    int s;
 
    char name[50];
    printf("\n----------\n");
    printf("\nEnter size you want to allocate\n");
    printf("\nNote that the size i.e. memory asked for allocation must be in 'integer' format only.\n");
    printf("\n----------\n");
    scanf("%d",&s);
    printf("\n----------\n");
    printf("\nEnter name for allocated block\n");
    printf("\nIf the entered name already exists,previos memory of that name will be deallocated.\n\nUnique names are preferable.\n");
    printf("\n----------\n");
    scanf("%s",name);
   
    isTagFound(name);
    free_list* free_ptr = head_free;
    while (free_ptr && free_ptr->size < s)             // _ _ _ _ _ _ * * * * - - - - - - - - _ _ * * * _ _ _
    {
        free_ptr = free_ptr->next; //s=8;
    }
   
    if (free_ptr == NULL)
    {
        printf("\nMEMORY CAN'T BE ALLOCATED ,NO SUITABLE FREE BLOACK IS AVAILABLE.\n");
    }
    else
    {
        if (free_ptr->size>s) 
        {
           
            free_list *temp=(free_list*) malloc(sizeof(free_list));
            
            temp->start = free_ptr->start + s;
            temp->next = temp->prev = NULL;
            
            if (free_ptr == head_free)
            {
                temp->next = head_free->next;
                head_free = temp;
            }
            else
            {
                (free_ptr->prev)->next = temp;
                temp->next = free_ptr->next;
                temp->prev = free_ptr->prev;
                if(free_ptr->next)
                {
                    free_ptr->next->prev = temp;
                }
            }
            temp->size = free_ptr->size - s;
        }
        
        else                                                          // _ _ _ _ _ _ * * * *_ _ _ _ _ _ _ _ * * * _ _ _
        {
            if (free_ptr->prev)
            {
                free_ptr->prev->next = free_ptr->next;
                if(free_ptr->next)
                {
                    free_ptr->next->prev = free_ptr->prev;
                }
            }
            else
            {
                head_free = free_ptr->next;
                if(head_free)
                head_free->prev = NULL;
            }
            
        }

       
        alloc_list *alloc_ptr=(alloc_list*) malloc(sizeof(alloc_list));
        alloc_ptr->next = alloc_ptr->prev = NULL;
        alloc_ptr->size = s;
       
        strcpy(alloc_ptr->name,name);
        alloc_ptr->start = free_ptr->start;
        free(free_ptr);
        if (head_allocate == NULL)
        {
            head_allocate = alloc_ptr;
        }
        else                                                                             // _ _ _ _ _ _ * * * * - - - - - - - - _ _ * * * _ _ _
        {
            
            alloc_list* alloc_temp = head_allocate;
            
            while (alloc_temp->next && alloc_temp->start < alloc_ptr->start)
            {
                alloc_temp = alloc_temp->next;
            }
            
            if (alloc_temp == head_allocate && alloc_temp->start > alloc_ptr->start) // * * * * * *  * * * * 
            {
                alloc_ptr->next = alloc_temp;
                alloc_temp->prev = alloc_ptr;
                head_allocate = alloc_ptr;
            }
            else if (alloc_temp->next == NULL && alloc_temp->start < alloc_ptr->start)  // _ _ _ _ _ _ * * * *  * * *  NULL
            {
                alloc_temp->next = alloc_ptr;
                alloc_ptr->prev = alloc_temp;
                alloc_ptr->next = NULL;
            }
            
            else                                                                       // _ _ _ _ _ _ * * * *   * *   _ _ * * * _ _ _
            {
                (alloc_temp->prev)->next = alloc_ptr;
                alloc_ptr->prev = alloc_temp->prev;
                alloc_ptr->next = alloc_temp;
                alloc_temp->prev = alloc_ptr;
            }
        }
        
    
        printf("\n Allocated from index : %d to %d \n",alloc_ptr->start,alloc_ptr->start+alloc_ptr->size);
     
    }
}


void merge(free_list* free_temp)
{
    
    if (free_temp->prev)                                      // _ _  _ _ _ _ * * _
    {
        if (free_temp->prev->start + free_temp->prev->size == free_temp->start)
        {
            
            free_temp->prev->size = free_temp->prev->size + free_temp->size;
            
            free_temp->prev->next = free_temp->next;
            if(free_temp->next)
            {
                free_temp->next->prev = free_temp->prev;
            }
            free_list* temp = free_temp;
            free_temp = free_temp->prev;
            free(temp);
        }
    }
    
    if (free_temp->next)     // _ _ _  _ _ _ _ * * _ _
    {

        if (free_temp->start + free_temp->size == free_temp->next->start)
        {
            
            free_temp->size += free_temp->next->size;
            
            free_temp->next = free_temp->next->next;
            if(free_temp->next)
            {
                free_temp->next->prev = free_temp;
            }
        }
    }
}

void deallocate(char name[])
{
    
    alloc_list* alloc_ptr=head_allocate;
    while (alloc_ptr!=NULL && strcmp(alloc_ptr->name,name))
    {
        alloc_ptr = alloc_ptr->next;
    }
    if(alloc_ptr==NULL)
    {
    	printf("\n block with given name is not there in heap memory\n");
	}
    else if(alloc_ptr!=NULL)                                                        // _ _ _ * * * * _ _  * * * * * _ _ _ _ * * *
    {
        
        free_list *free_ptr=(free_list*) malloc(sizeof(free_list));
        free_ptr->next = free_ptr->prev = NULL;
        free_ptr->size = alloc_ptr->size;
        free_ptr->start = alloc_ptr->start;
        
        if (alloc_ptr == head_allocate)
        {
            head_allocate = alloc_ptr->next;
        }
        
        else
        {
            alloc_ptr->prev->next = alloc_ptr->next;
            if (alloc_ptr->next!=NULL)
            {
                alloc_ptr->next->prev = alloc_ptr->prev;
            }
        }
        
        free_list* free_temp = head_free;
        
        if (head_free == NULL)
        {
            head_free = free_ptr;
        }
        else
        {
            while (free_temp!=NULL && free_temp->next!=NULL && free_temp->start < free_ptr->start) 
            {
                free_temp = free_temp->next;
            }
            if (free_temp && free_temp->next == NULL && free_temp->start < free_ptr->start) // _ _ _ * * * * _ _ * * * * *
            {
                free_temp->next = free_ptr;
                free_ptr->prev = free_temp;
            }
            else
            {
                
                if (free_temp && free_temp == head_free) // _ _ _   _ _
                {
                    free_ptr->next = free_temp;
                    free_temp->prev = free_ptr;
                    head_free = free_ptr;
                }
                else if (free_temp)  // _ _ _ * * * * _ _   _ _ _ _ _   _ _ _ _ * * *
                {
                    free_ptr->prev = free_temp->prev;
                    free_temp->prev->next = free_ptr;
                    free_temp->prev = free_ptr;
                    free_ptr->next = free_temp;
                }
            }
        }
        
        merge(free_ptr);
    }
}

void isTagFound(char name[])
{
    int flag = 1;
    alloc_list* alloc_ptr = head_allocate;
    while (alloc_ptr && flag)
    {
        if (alloc_ptr->name == name)
        {
            printf("\n!!Memory location with same name found!!\nHence deallocating previous one\n");
            deallocate(name);
            flag = 0;
        }
        alloc_ptr = alloc_ptr->next;
    }
}


void allocated_list_print()
{
    alloc_list* alloc_ptr = head_allocate;
    while(alloc_ptr!=NULL)
    {
       
        printf("\n %d to %d \n",alloc_ptr->start,alloc_ptr->start+alloc_ptr->size);
        alloc_ptr = alloc_ptr->next;
    }
}

void free_list_print()
{
    free_list* free_ptr = head_free;
    while (free_ptr!=NULL)
    {
        printf("\n %d to %d \n",free_ptr->start,free_ptr->start+free_ptr->size);
        free_ptr = free_ptr->next;
    }
}

int main()
{
    int option;

    
    head_free=(free_list*) malloc(sizeof(free_list));
    head_free->next = head_free->prev = NULL;
    head_free->size = SIZE;
    head_free->start = 0;
    
    
    head_allocate = NULL;
    printf("\n -- HEAP MANAGEMENT SCHEME -- \n");
    printf("\nMemory of 2000 units available\n");
    int flag=1;
    while (flag!=0)
    {
        printf("\n----------\n");
        printf("\nEnter the correct option number\n");
        printf("\n1. Allocate Memory\n");
        printf("\n2. Deallocate Memory\n");
        printf("\n----------\n");
        scanf("%d",&option);
       
       
        if(option==1)
        {
            allocate();
        }
        else if(option==2)
        {
             char name[50];
                printf("\nEnter the name of memory block you want to delete\n");
       
                scanf("%s",name);
                deallocate(name);
        }
        else
        {
            printf("\nOption entered is incorrect, please select the correct option number.\n");
        }
        printf("\n----------\n");
        printf("\nHeap Memory\n");
        
        printf("\n----------\n");
        printf("\nFree list\n");
        free_list_print();  
        
        printf("\n----------\n");
        printf("\nAllocated list\n");
        allocated_list_print();
        
        printf("\n----------\n");
        printf("\nEnter:\n");
        printf("\n 1 to continue\n");
        printf("\n 0 to stop\n");
        printf("\n----------\n");
        scanf("%d",&flag);

    }
    printf("\nEND\n");
    return 0;
}
