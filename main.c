/*NAME : Abhishek S 
  Date : 02/01/2026
  project name :  Inverted Search 
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "main.h"

int main(int argc,char * argv[])
{
    

    Slist *head = NULL;
   
    if(argc<2)
    {
        printf("Too few arguments\n");
        return FAILURE;
    }

    if(validate(argc,argv,&head) == SUCCESS)
        print_list(head);

    hash_t arr[27];
    for(int i = 0 ;i < 27;i++)
    {
        arr[i].index = i;
        arr[i].main_node = NULL;
    }
    int choice;
    do
    {
        printf(RED "╔═══════════════════════════════════════════════════════════════════════════════════╗\n");
        printf(CYAN "                                  Inverted Search                                   \n"RESET);
        printf(RED "╚═══════════════════════════════════════════════════════════════════════════════════╝\n\n" RESET);

        printf("\n+--------------------------------------------+\n");
        printf(RED"|                MAIN MENU                   |\n"RESET);
        printf("+--------------------------------------------+\n");
        printf("| 1 |Create Database                         |\n");
        printf("| 2 | Search Database                        |\n");
        printf("| 3 | Display Database                       |\n");
        printf("| 4 | Save Database                          |\n");
        printf("| 5 | Update Database                        |\n");
        printf("| 6 | Exit                                   |\n");
        printf("+--------------------------------------------+\n");

        printf("Enter the choice: ");
        
        //scanf(" %d", &choice);
        if (scanf(" %d", &choice) != 1)
        {
            printf("Invalid input\n");
            while (getchar() != '\n');  
            continue;
        }
        //printf("DEBUG choice = %d\n", choice);
        int flag = 0;
        switch (choice)
        {
            case 1:if(!flag){
                if (create_database(&head, arr) == SUCCESS)
                {
                    printf(CYAN"Database created successfully\n"RESET);
                    flag = 1;
                }
                else
                    printf(RED"Database creation failed\n"RESET);

                }
                break;

            case 2:
                if (search_database(&head, arr) == SUCCESS)
                    printf(CYAN"Search completed\n"RESET);
                else
                    printf(RED"Search failed\n"RESET);
                break;

            case 3:
                display_HT(arr, 27);
                break;
            case 4: save_database(arr,27);
                    break;

            case 5: update_db(&head,arr);
                    break;

            case 6:
                printf(CYAN"Exiting...\n"RESET);
                break;

            default:
                printf(RED"Invalid option, try again\n"RESET);
        }

    } while (choice != 6);
}

int validate(int argc,char * argv[],Slist **head)
{
    
    for(int i = 1;i<argc;i++)
    {
        char *ret1 = strrchr(argv[i], '.');
        if (ret1 == NULL || strcmp(ret1, ".txt") != 0)
        {
            printf("Error:invalid extension for %s\n",argv[i]);
        } 
        else
        {
            FILE *fptr = fopen(argv[i],"r");
            if(fptr == NULL)
            {
                printf("Error:File %s doesn't exist\n",argv[i]);
            }
            else
            {
                fseek(fptr, 0, SEEK_END);
                if (ftell(fptr) == 0)
                {
                    printf("Error:File empty\n");
                }
                else
                {
                    if(check_duplicates(head,argv[i]) == SUCCESS)
                    {
                        insert_at_last(head,argv[i]);
                    }
                }

            }

        }
    }
    return SUCCESS;
}