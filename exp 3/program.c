#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
//colour codes for applying colours to the terminal
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KNRM  "\x1B[0m"

//structure for each file or directory
struct file {
	char name[30],location[30],type[10],created[30],permissions[5];
	int count,size;
	struct file* child;
	struct file* sibling;
}*root, *temp, *new, *stack[100], *prev;
//root stores the first file/dir in the system
//stack[] maintains list of traversed directories to make reverse traversal possible and to find parent of a child node when required


FILE *fp;

//level shows depth into the file structure, count is only used for searching to count no of occurences
int level = 0,count=0;
int Menu(int);
void Create(int);
void Search(struct file*, char*,char*);
void Change();
void View(struct file*, int);
void Delete(struct file*);



int main() {
	fp = fopen ("output.txt","w");
	int choice,flag=1;
	root = malloc(sizeof(struct file));
	root = NULL;
	//recieves type of organization as input
	printf("%s\n\nSelect file organization technique :\n1-Single Level\n2-Two Level\n3-Hierarchical\n4-Exit\nEnter choice : ",KNRM);
	scanf("%d",&choice);
	if(choice < 1 || choice > 4)
		printf("%s\nInvalid choice",KRED);
	else if(choice !=4) {
		switch(choice) {
			case 1:	printf("%s\n\nSelected Technique : Single Level\n",KGRN);
				fprintf(fp,"\n\nSelected Technique : Single Level\n\n");
				break;
			case 2:	printf("%s\n\nSelected Technique : Two Level\n",KGRN);
				fprintf(fp,"\n\nSelected Technique : Two Level\n\n");
				break;
			case 3:	printf("%s\n\nSelected Technique : Hierarchical\n",KGRN);
				fprintf(fp,"\n\nSelected Technique : Hierarchical\n\n");
				break;
		}
		//infinite loop to sustain each organization method
		while(flag)
			flag = Menu(choice);
	}
	printf("%s\nExiting\n\n",KRED);
	fclose(fp);
	return 0;
}


//this function displays all the menus based on the user choice and restrictions imposed by various techniques
//different menus may have different ordering of options, which is then standardized to a particular form and implemented
int Menu(int choice) {
	int ch;
	char name[30], dir[30];
	printf("%s\n\nMenu\n",KNRM);
	//menu for starting case only
	if(root == NULL) {
		printf("%s1-Create Directory\n2-View\n3-Exit\nEnter Choice : ",KNRM);
		scanf("%d", &ch);
		switch(ch) {
			case 1: break;
			case 2: ch = 5;
				break;
			case 3: ch = 7;
				break;
			default:printf("%s\nInvalid Choice\n",KRED);
				ch = 7;;
		}		
	}
	else {
		//menu for single level when at root level => dir creation not allowed
		if(choice == 1 && level == 0) {
			printf("%s1-Search\n2-Change Directory\n3-View\n4-Delete\n5-Exit\nEnter Choice : ",KNRM);
			scanf("%d", &ch);
			switch(ch) {
				case 1: case 2:	case 3: case 4:	
				case 5: ch = ch+2;
					break;
				default:printf("%s\nInvalid Choice\n",KRED);
					ch = 7;
			}
		}	
		//menu for single level and two level at inside first dir => level = 1, here dir creation not allowed
		else if((choice == 1 && level == 1) || (choice == 2 && level == 1)) {
			printf("%s1-Create File\n2-Search\n3-Change Directory\n4-View\n5-Delete\n6-Exit\nEnter Choice : ",KNRM);
			scanf("%d", &ch);
			switch(ch) {
				case 1: case 2:	case 3: case 4:	case 5:
				case 6: ch = ch+1;
					break;
				default:printf("%s\nInvalid Choice\n",KRED);
					ch = 7;
			}
		}
		//menu for root level for all other cases, file creation not allowed here
		else if(level == 0){
			printf("%s1-Create Directory\n2-Search\n3-Change Directory\n4-View\n5-Delete\n6-Exit\nEnter Choice : ",KNRM);
			scanf("%d", &ch);
			switch(ch) {
				case 1: break;
				case 2:	case 3: case 4:	case 5:
				case 6: ch = ch+1;
					break;
				default:printf("%s\nInvalid Choice\n",KRED);
					ch= 7;
			}
		}
		//menu for all other cases
		else {
			printf("%s1-Create Directory\n2-Create File\n3-Search\n4-Change Directory\n5-View\n6-Delete",KNRM);
			printf("%s\n7-Exit\nEnter Choice : ",KNRM);
			scanf("%d", &ch);
		}
	}
	//switch to call all the functions after bringing choices to a common pattern 		
	switch(ch) {
		case 1: //creating of dir
			Create(1);
			break;	
		case 2:	//creation of file
			Create(0); 
			break;
		case 3:	//searching
			count = 0;
			printf("%s\nEnter file/directory to find : ",KNRM);
			scanf(" %[^\n]",name);
			strcpy(dir,"Root");
			printf("%s\n\nSearching for %s\n",KGRN,name);
			fprintf(fp,"\n\nSearching for %s\n ",name);
			Search(root,name,dir);
			if(count == 0) {
				printf("%s\n%s not found",KRED,name);
				fprintf(fp,"\n%s not found",name);
			}
			printf("%sTotal occurences : %d\n",KGRN,count);
			fprintf(fp,"Total occurences : %d\n\n",count);
			break;
		case 4: //change current dir
			Change();
			break;
		case 5: //view entire file system
			if(root == NULL) {
				printf("%s\nNothing to show\n",KGRN);
				fprintf(fp,"\nNothing to show\n\n");
			}
			else if(level == 0) {
				printf("%s\nCurrently in Directory - Root\n",KGRN);
				fprintf(fp,"\n\n\nCurrently in Directory - Root\n");
			}
			else {
				printf("%s\nCurrently in Directory - %s\n",KGRN,stack[level-1]->name);
				fprintf(fp,"\n\n\nCurrently in Directory - %s\n",stack[level-1]->name);
			}
			View(root,0);
			break;
		case 6: //delete file or dir
			printf("%s\nEnter file/directory to delete : ",KNRM);
			scanf(" %[^\n]",name);
			int flag =0;
			if(level == 0)
				temp = root;	
			else
				temp = stack[level-1]->child;	
			prev = temp;
			while(temp != NULL) {
				if(strcmp(temp->name,name)==0) {
					//if name is found
					//if file => delete file and join the other nodes
					//if dir => call delete() and free all the spaces occupied by children nodes 
					flag = 1;
					if(level != 0) {
						stack[level-1]->size -= temp->size;
						stack[level-1]->count -= 1;
					}
					if(temp == prev) {
						if(temp == root)
							root = temp->sibling;
						else
							stack[level-1]->child = temp->sibling;
					}
					else
						prev->sibling = temp->sibling;
					if(strcmp(temp->type, "Directory")==0) {
						Delete(temp->child);
						printf("%sDeleted directory %s\n",KGRN,name);
						fprintf(fp,"\n\nDeleted directory %s",name);
					}
					else {
						printf("%sDeleted file %s\n",KGRN,name);
						fprintf(fp,"\n\nDeleted file %s",name);
					}
					printf("\tlocation\t: %s\n\ttype\t\t: %s\n",temp->location,temp->type);
		       		     	printf("\tsize\t\t: %f KB\n\tcount\t\t: %d\n",(float)temp->size/1024,temp->count);
					printf("\tcreated\t\t: %s\tpermissions\t: %s\n",temp->created,temp->permissions);
					fprintf(fp,"\n\tlocation\t: %s\n\ttype\t\t: %s\n",temp->location,temp->type);
		       		     	fprintf(fp,"\tsize\t\t: %f KB\n\tcount\t\t: %d\n",(float)temp->size/1024,temp->count);
					fprintf(fp,"\tcreated\t\t: %s\tpermissions\t: %s\n",temp->created,temp->permissions);
					free(temp);
					
				}
				if(flag)
					break;
				prev = temp;
				temp = temp->sibling;	
			}
			if(!flag) {
				printf("%s\nfile/document %s not found\n",KRED,name);
				fprintf(fp,"\n\nfile/document %s not found\n\n",name);
			}
			break;
		case 7: return 0;
		default:printf("%s\nInvalid Choice\n",KRED);
	}
	return 1;
}


//fn to create file or dir based on type value, type=1=> dir, type=0=>file
void Create(int type) {
	new = malloc(sizeof(struct file));
	if(type)
		printf("%s\nEnter directory details\nName : ",KNRM);
	else
		printf("%s\nEnter file details\nName : ",KNRM);
	scanf(" %[^\n]", new->name);
	if(level == 0) {
		temp = root;
		strcpy(new->location,"Root");
	}
	else {
		temp = stack[level-1]->child;
		strcpy(new->location,stack[level-1]->name);
	}
	while(temp != NULL) {
		if(strcmp(temp->name,new->name)==0) {
			if(type) {
				printf("%s\nDirectory with name %s already exists\n",KRED,new->name);
				fprintf(fp,"\n\nDirectory with name %s already exists\n",new->name);
			}
			else {
				printf("%s\nFile with name %s already exists\n",KRED,new->name);
				fprintf(fp,"\n\nFile with name %s already exists\n",new->name);
			}
			new = NULL;
			return;
		}
		if(temp->sibling == NULL) {
			temp->sibling = new;
			break;
		}
		temp=temp->sibling;
	}
	if(level != 0 && stack[level-1]->child==NULL)
		stack[level-1]->child = new;
	if(root == NULL)
		root = new;
	if(type) {
		new->count = 1;
		new->size = 1024;
		strcpy(new->type,"Directory");
	}
	else {
		new->count = 1;
		strcpy(new->type,"File");
		printf("Size in bytes : ");
		scanf("%d",&new->size);
	}
	if(level !=0) {
		stack[level-1]->size += new->size;
		stack[level-1]->count +=1;
	}
	new->child = NULL;
	new->sibling = NULL;
	printf("%sPermissions : ",KNRM);
	scanf("%s", new->permissions);
	time_t t;
    	time(&t);
    	strcpy(new->created,ctime(&t));
    	if(type) {
    		printf("%s\n\nNew Directory %s Created\n",KGRN,new->name);
    		fprintf(fp,"\n\nNew Directory %s Created\n",new->name);
    	}
    	else {
    		printf("%s\n\nNew File %s Created\n",KGRN,new->name);
		fprintf(fp,"\n\nNew File %s Created\n",new->name);
	}
	printf("\tlocation\t: %s\n\ttype\t\t: %s\n",new->location,new->type);
	printf("\tsize\t\t: %f KB\n\tcount\t\t: %d\n",(float)new->size/1024,new->count);
	printf("\tcreated\t\t: %s\tpermissions\t: %s\n",new->created,new->permissions);
	fprintf(fp,"\tlocation\t: %s\n\ttype\t\t: %s\n",new->location,new->type);
	fprintf(fp,"\tsize\t\t: %f KB\n\tcount\t\t: %d\n",(float)new->size/1024,new->count);
	fprintf(fp,"\tcreated\t\t: %s\tpermissions\t: %s\n\n",new->created,new->permissions);
	return;
}


//fn to search for a name in entire system
//recursive fn
//first select a node, traverse into all its child nodes, by backtracking get sibling nodes as well, continue till no nodes are left
void Search(struct file* node, char* name, char* dir) {
	if(node == NULL)
		return;
	else {
		if(strcmp(node->name,name) == 0) {
			printf("%s%d-\tlocation\t: %s\n\ttype\t\t: %s\n",KGRN,count+1,dir,node->type);
		       	printf("%s\tsize\t\t: %f KB\n",KGRN,(float)node->size/1024);
		       	printf("\tcreated\t\t: %s\tpermissions\t: %s\n",node->created,node->permissions);
		       	fprintf(fp,"%d-\tlocation\t: %s\n\ttype\t\t: %s\n",count+1,dir,node->type);
		       	fprintf(fp,"\tsize\t\t: %f KB\n",(float)node->size/1024);
		       	fprintf(fp,"\tcreated\t\t: %s\tpermissions\t: %s\n",node->created,node->permissions);
			char name[30],location[30],type[10],created[30],permissions[5]; 
			count ++;
		}
		char tempName[100];
		strcpy(tempName,dir);
		strcat(tempName,"/");
		strcat(tempName,node->name);
		Search(node->child,name,tempName);
		Search(node->sibling,name,dir);
	}
}



//fn to change current working directory
void Change(){
	char name[30];
	printf("%s\nEnter Directory name (type 'back' to go back 1 level) : ",KNRM);
	scanf(" %[^\n]",name);
	if(level == 0 && strcmp(name,"back")==0) {
		printf("%sNo previous levels\n",KRED);
		fprintf(fp,"\n\nNo previous levels\n");
		return;
	}
	else if(level == 0)
		temp = root;
	else if(strcmp(name,"back")==0) {
		stack[level-1]=NULL;
		level--;
		if(level == 0) {
			printf("%sChanged Directory to ROOT\n",KGRN);
			fprintf(fp,"\n\nChanged Directory to ROOT\n\n");
		}
		else {
			printf("%sChanged Directory to %s\n",KGRN,stack[level-1]->name);
			fprintf(fp,"\n\nChanged Directory to %s\n\n",stack[level-1]->name);
		}
		return;
	}
	else
		temp = stack[level-1]->child;
	while(temp != NULL) {
		if(strcmp(temp->name,name)==0) {
			if(strcmp(temp->type,"Directory")==0) {
				stack[level]=temp;
				level++;
				printf("%sChanged Directory to %s\n",KGRN,name);
				fprintf(fp,"\n\nChanged Directory to %s\n\n",name);
				return;
			}
		}
		temp = temp->sibling;
	}
	printf("%sDirectory %s not found\n",KRED,name);
	fprintf(fp,"\n\nDirectory %s not found\n\n",name);
	return;
}



//recursive function that displays the entire file system, similar to search functions control flow
void View(struct file* node,int lvl) {
	if(node == NULL)
		return;
	else {
		for(int i=0;i<lvl;i++) {
			printf("-");
			fprintf(fp,"-");
		}
		if(node == stack[level-1] && level != 0) {
			printf("%s%s*\n",KGRN,node->name);
			fprintf(fp,"%s*\n",node->name);
		}
		else {
			printf("%s%s\n",KGRN,node->name);
			fprintf(fp,"%s\n",node->name);	
		}
		View(node->child,lvl+1);
		View(node->sibling,lvl);
	}
}


//fn to delete all child nodes of the parameter node, by recursion
void Delete(struct file* node) {
	if(node == NULL)
		return;
	else {
		Delete(node->child);
		Delete(node->sibling);
		free(node);
	}	
}
