#ifndef MAIN_H
#define MAIN_H
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define BLUE    "\033[1;34m"
#define YELLOW  "\033[1;33m"

typedef struct node 
{
    char filename[100];
    struct node*link;
}Slist;

typedef struct hash
{
    int index;
    struct main_node *main_node;
} hash_t;


typedef struct main_node
{
    char word[30];
    int file_count;
    struct main_node *main_link;
    struct sub_node  *sub_link;

}main_node;

typedef struct sub_node
{
    char file_name[50];
    int word_count;
    struct sub_node * sub_link;
}sub_node;



#define SUCCESS 0
#define FAILURE -1

int validate(int argc,char * argv[],Slist **head);
int insert_at_last(Slist **head, char str[]);
int check_duplicates(Slist **head, char str[]);
void print_list(Slist *head);
int create_database(Slist**head,hash_t arr[]);
int search_database(Slist**head,hash_t arr[]);
void display_HT(hash_t *arr, int size);
void save_database(hash_t *arr, int size);
int update_db(Slist **head, hash_t *arr);

#endif