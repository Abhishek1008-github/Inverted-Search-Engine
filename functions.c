#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "main.h"


int insert_at_last(Slist **head, char str[])// Insert a filename at the end of the singly linked list
{
    Slist *new = malloc(sizeof(Slist));
    if(new ==NULL) 
    {
        return FAILURE;
    }
    // Copy filename into node
    strcpy(new->filename,str);
    new->link = NULL;
    // If list is empty, new node becomes head
    if(*head == NULL)
    {
        *head = new;
    }
    else
    {
        Slist *temp = *head;
        while(temp->link != NULL)
        {
            temp = temp->link;
        }
        temp->link = new;  // Link new node at the end
    }
    return SUCCESS;
    
    
}
int check_duplicates(Slist **head, char str[])// Check whether filename already exists in list
{
    Slist*temp = *head;
    if(*head == NULL)
    {
        return SUCCESS;
    }

    while(temp != NULL) // Traverse list to compare filenames
    {
        if (strcmp(temp->filename, str) == 0)
        {
            return FAILURE;
        }
        temp = temp->link;
    }
    return SUCCESS;
}

/*void print_list(Slist *head)// Print all filenames in the list
{
	if (head == NULL)
	{
		printf("INFO : List is empty\n");
	}
    else
    {
	    while (head)		
	    {
		    printf("%s -> ", head -> filename);
		    head = head -> link;
	    }

	    printf("NULL\n");
    }
}*/
void print_list(Slist *head)
{
    if (head == NULL)
    {
        printf("\n----------------------------\n");
        printf("|   List is empty          |\n");
        printf("----------------------------\n");
        return;
    }

    printf("\n----------------------------\n");
    printf("|        File List         |\n");
    printf("----------------------------\n");

    while (head)
    {
        printf("| %s\n", head->filename);
        head = head->link;
    }

    printf("----------------------------\n");
}


int create_database(Slist **head, hash_t arr[])// Build inverted index database from file list
{
    Slist *temp = *head;
    char buffer[100];
    int index;

    while (temp != NULL)
    {
        FILE *fp = fopen(temp->filename, "r");
        if (fp == NULL)
            return FAILURE;

        while (fscanf(fp, "%s", buffer) != EOF)
        {
            if (buffer[0] >= 'A' && buffer[0] <= 'Z')   // Calculate hash index based on first character
                index = buffer[0] - 'A';
            else if (buffer[0] >= 'a' && buffer[0] <= 'z')
                index = buffer[0] - 'a';
            else
                index = 26;

             // Case 1: No main node at this index
            if (arr[index].main_node == NULL)
            {
                main_node *new = malloc(sizeof(main_node));
                sub_node *new2 = malloc(sizeof(sub_node));

                strcpy(new->word, buffer);
                new->file_count = 1;
                new->main_link = NULL;
                new->sub_link = new2;

                strcpy(new2->file_name, temp->filename);
                new2->word_count = 1;
                new2->sub_link = NULL;

                arr[index].main_node = new;
            }
            else
            {
                main_node *temp1 = arr[index].main_node;
                main_node *prev1 = NULL;
                int word_found = 0;

                while (temp1 != NULL)
                {
                    if (strcmp(buffer, temp1->word) == 0)
                    {
                        sub_node *temp2 = temp1->sub_link;
                        sub_node *prev2 = NULL;
                        int file_found = 0;

                        while (temp2 != NULL)// Search for filename in sub list
                        {
                            if (strcmp(temp2->file_name, temp->filename) == 0)
                            {
                                temp2->word_count++;
                                file_found = 1;
                                break;
                            }
                            prev2 = temp2;
                            temp2 = temp2->sub_link;
                        }

                        if (!file_found)// New file for existing word
                        {
                            sub_node *new_sub = malloc(sizeof(sub_node));
                            strcpy(new_sub->file_name, temp->filename);
                            new_sub->word_count = 1;
                            new_sub->sub_link = NULL;

                            prev2->sub_link = new_sub;
                            temp1->file_count++;
                        }

                        word_found = 1;
                        break;   // 🔥 VERY IMPORTANT
                    }

                    prev1 = temp1;
                    temp1 = temp1->main_link;
                }

                 // Case 3: Word not found → create new main node
                if (!word_found)
                {
                    main_node *new3 = malloc(sizeof(main_node));
                    sub_node *new4 = malloc(sizeof(sub_node));

                    strcpy(new3->word, buffer);
                    new3->file_count = 1;
                    new3->main_link = NULL;
                    new3->sub_link = new4;

                    strcpy(new4->file_name, temp->filename);
                    new4->word_count = 1;
                    new4->sub_link = NULL;

                    prev1->main_link = new3;
                }
            }
        }
        fclose(fp);
        temp = temp->link;
    }
    return SUCCESS;
}
int search_database(Slist**head,hash_t arr[])// Search for a word in the database
{
    char word[100];
    printf("Enter the word to be searched: ");
    scanf("%s",word);
    int index ;
    if(word[0]>= 'A' && word[0]<='Z')
    {
        index = word[0]- 'A';
    }
    else if(word[0]>= 'a' && word[0]<='z')
    {
        index = word[0] - 'a';
    }
    else
        index = 26;

    if(arr[index].main_node == NULL)
    {
        printf("Word not found\n");
        return SUCCESS;
    }
    else
    {
        main_node * temp = arr[index].main_node;
        while(temp != NULL)
        {
            if(strcmp(word,temp->word)==0)
            {
                printf("word: %s filecount : %d\n",temp->word,temp->file_count);
                sub_node * ptr = temp->sub_link;
                while(ptr != NULL)
                {
                    printf("%s %d\n",ptr->file_name,ptr->word_count);
                    ptr = ptr->sub_link;
                }
                break;
                
            }
            else
            {
                printf("Word not found\n");

            }
        }
        return SUCCESS;
    }

}

void display_HT(hash_t *arr, int size)
{
    
    for(int i = 0; i < size; i++)
    {
        printf("[%d]: ", i);

        main_node *temp1 = arr[i].main_node;
        if(temp1 == NULL)
        {
            printf("NULL\n");
            continue;
        }

        while(temp1)
        {
            printf("%s(file_count=%d) -> ", temp1->word, temp1->file_count);

            sub_node *temp2 = temp1->sub_link;
            while(temp2)
            {
                printf("{%s: %d}  ", temp2->file_name, temp2->word_count);
                temp2 = temp2->sub_link;
            }

            temp1 = temp1->main_link;
        }
        printf("\n");
    }
}

void save_database(hash_t *arr, int size)
{
    char file[100];
    printf("Enter filename: ");
    scanf("%s",file);
    char *ret = strstr(file, ".txt");
    if (ret == NULL || strcmp(ret, ".txt") != 0)
    {
        printf("Invalid extension\n");
        return;
    }
    else
    {
        FILE*ptr = fopen(file,"w");
        if(ptr == NULL)
        {
            return;
        }
        //fprintf(ptr,"%c",'#');
        for(int i = 0; i < size; i++)
        {
            fprintf(ptr,"%c",'#');
            fprintf(ptr,"%d;", i);

            main_node *temp1 = arr[i].main_node;
            if(temp1 == NULL)
            {
                fprintf(ptr,"%s;","NULL");
                fprintf(ptr,"%c",'#');
                fprintf(ptr,"%c",'\n');
                continue;
            }

            while(temp1)
            {
                fprintf(ptr,"%s;%d;", temp1->word, temp1->file_count);

                sub_node *temp2 = temp1->sub_link;
                while(temp2)
                {
                    fprintf(ptr,"%s;%d;", temp2->file_name, temp2->word_count);
                    temp2 = temp2->sub_link;
                }

                temp1 = temp1->main_link;
            }
            fprintf(ptr,"%c",'#');
            fprintf(ptr,"%c",'\n');
        }
        fclose(ptr);
        printf("Database saved sucessfully\n");
        while (getchar() != '\n');

    }
}
int update_db(Slist **head, hash_t *arr)
{
    char fname[100], word[50], filename[50];
    int index, fcount, wcount;

    printf("Enter backup file name: ");
    scanf("%s", fname);

    char *ptr = strstr(fname, ".txt");
    if (!ptr || strcmp(ptr, ".txt") != 0)
    {
        printf("Not a .txt file\n");
        return FAILURE;
    }

    FILE *fptr = fopen(fname, "r");
    if (!fptr)
    {
        printf("File does not exist\n");
        return FAILURE;
    }

    char line[2048];

    while (fgets(line, sizeof(line), fptr))
    {
        if (line[0] != '#') continue;

        char *p = line + 1; // skip first '#'

        // Read index
        index = atoi(p);
        p = strchr(p, ';');
        if (!p) continue;
        p++; // move past ';'

        if (strncmp(p, "NULL", 4) == 0) continue;

        main_node *last_main = NULL;

        while (*p && *p != '\n')
        {
            // Read main word
            char *token = strtok(p, ";");
            if (!token) break;
            strcpy(word, token);

            // Read file count
            token = strtok(NULL, ";");
            if (!token) break;
            fcount = atoi(token);

            main_node *m_new = malloc(sizeof(main_node));
            strcpy(m_new->word, word);
            m_new->file_count = fcount;
            m_new->main_link = NULL;
            m_new->sub_link = NULL;

            sub_node *last_sub = NULL;

            // Read sub-nodes
            for (int i = 0; i < fcount; i++)
            {
                token = strtok(NULL, ";");
                if (!token) break;
                strcpy(filename, token);

                token = strtok(NULL, ";");
                if (!token) break;
                wcount = atoi(token);

                // Remove from Slist if exists
                Slist *temp = *head, *prev = NULL;
                while (temp)
                {
                    if (strcmp(temp->filename, filename) == 0)
                    {
                        if (!prev) *head = temp->link;
                        else prev->link = temp->link;
                        free(temp);
                        break;
                    }
                    prev = temp;
                    temp = temp->link;
                }

                sub_node *s_new = malloc(sizeof(sub_node));
                strcpy(s_new->file_name, filename);
                s_new->word_count = wcount;
                s_new->sub_link = NULL;

                if (!m_new->sub_link)
                    m_new->sub_link = s_new;
                else
                    last_sub->sub_link = s_new;

                last_sub = s_new;
            }

            // Append main_node to hash table
            if (!arr[index].main_node)
                arr[index].main_node = m_new;
            else
                last_main->main_link = m_new;

            last_main = m_new;

            // Move pointer forward
            p = strtok(NULL, "#");
            if (!p) break;
        }
    }

    fclose(fptr);
    printf("\nDatabase updated successfully!\n");
    return SUCCESS;
}

