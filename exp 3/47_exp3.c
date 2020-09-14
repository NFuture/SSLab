#include<stdio.h>

struct mem_unit 
{
    char name[24];
    int file_count;
    struct mem_unit *file_link;
    struct mem_unit *dir_link;
    char location[24];
    bool file_type;// 0 = file, 1 = directory
    double size;
    int createdOn;
    char permission[2];
};


void main(){
    /*int * p;     
    int a = 12;
    p = &a;
    printf("%d", *p);*/

    struct mem_unit *head, temp;
    head = &temp;
    (*head).name = "Root Directory";
    printf("%s", &head->name));

}