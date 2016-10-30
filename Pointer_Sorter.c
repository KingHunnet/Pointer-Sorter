#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdlib.h>

//Created new struct to hold char pointer and a next pointer for the linked list
typedef struct word_ word; 
struct word_{
	char* theWord; //the actual word
	word* next;	//for linked lists. Points at the next node
};

typedef enum {false, true} bool; //quick and easy def for booleans
/*
free up all allocated space in linked list
*/
void FreeLL(word* head)
{
	//won't work until linked list is fixed
	while(head->theWord != NULL)
	{
		word* temp = head;
		free(head->theWord);
		free(head);
		head = temp->next;
	}
}
/*
Prints linked list
*/
void PrintLL(word* head)
{
	word* current = head;
	while(current->theWord != NULL)
	{
		printf("%s\n", current->theWord);
		current = current->next;
	}
}

//Creates new node with word specified word and pointing to null
//Prints out if malloc failed
word* newNode(char* totokenize)
{
	word* theNewNode = (word*)malloc(sizeof(word));
	if (theNewNode == NULL){
		printf("malloc failed. Output may suffer\n");
        return NULL;
	}
	char* wordsMemory = (char*)malloc(strlen(totokenize) * sizeof(char));
	if (wordsMemory == NULL){
		printf("malloc failed. Output may suffer\n");
        return NULL;
	}

	strcpy(wordsMemory, totokenize);
	theNewNode->theWord = wordsMemory;
	theNewNode->next = NULL;
	return theNewNode;
}
/*
Save and Sort method takes the word to be added into the linked list and adds it into the correct location in the linked list
The linked list will remain sorted after the method is complete and no sorting is needed on the linked list
If the word is a duplicate word (case insensitive), the word will not be added into the linked list
*/
void SaveAndSort(char* totokenize, word*** head){
	if ((**head)->theWord == NULL || strcasecmp(totokenize,((**head)->theWord)) <= 0)
	{
		//checking if it's the same word
        	if ((**head)->theWord != NULL && strcasecmp(totokenize,((**head)->theWord)) == 0)
        	{
            		return;
        	}
		//adding the word before head
		word* nodetoadd = newNode(totokenize);
		nodetoadd->next = **head;
		**head = nodetoadd;
	}
	else
	{
		//Locate the node before point of insertion
		word* current = **head;
		while(!((current)->next->theWord == NULL) && strcasecmp(totokenize, ((current)->next->theWord)) > 0)
		{
			current = (current)->next;
			}
        	//Checking if the words are the same
        	if (!((current)->next->theWord == NULL) && strcasecmp(totokenize, ((current)->next->theWord)) == 0){
          	  	return;
       		 }
		//adding new node to the linked list
		if (((current->next->theWord) == NULL) || strcasecmp(totokenize, (current->next->theWord)) < 0)
		{
			word* nodetoadd = newNode(totokenize);
			nodetoadd->next = current->next;
			current->next = nodetoadd;
		}
	}

}
/*
Gets char's until it hits non-alpha char. Appends chars 
to string and adds null at end (makes it easy to strcpy). returns a
pointer of where it left off in the string
*/
char* isWord( char* ts, word** head ) {
	int length = strlen(ts);
	char totokenize[length];
	int i = 0;
	
	if (isalpha(ts[0]) != 0){
		while (isalpha(ts[i]) != 0){
			totokenize[i] = ts[i];
			i++;
		}
		
		ts+=i;	
		totokenize[i] = '\0';
		SaveAndSort(totokenize, &head);
		return ts;
	}

	return NULL;
}
/*
Gets to the next starting point of a word. keeps going until it finds an
alpha char or a null. Terminates on null and throws error if need be
*/
char* StartingPt(char *inputptr, bool firsttime){
	char* finalptr;
	finalptr = inputptr;
	bool moved = false;
	while((finalptr[0] != '\0') && (isalpha(finalptr[0]) == 0)){
		finalptr++;
		moved = true;
	}
	
	if (finalptr[0] == '\0'){
		return '\0';
	}

	if(moved == false && firsttime == false){
		printf("Error. Tokens are improperly seperated\n");
	}
	
	return finalptr;
}
/*
Main driver. Input is for convience of Sublime's build output. Could run 
command line but this is way faster and easier. Just replace with 
char input[] = argv[1]
*/
int main(int argc, char **argv) {
    if (argc <  2)
    {
        printf("not enough arguments\n");
        return 0;
    }
    if (argc>2)
    {
        printf("Too many arguments. You fail\n");
        return 0;
    }
    char *inputarg = argv[1];
    char* inputptr = inputarg;

	inputptr = malloc(strlen(inputarg) * sizeof(char));
	strcpy(inputptr, inputarg);

	word startinghead = {NULL, NULL};
	word* head = &startinghead;
	head->theWord = NULL;
	head->next = NULL;
    bool firststart = true;
	inputptr = StartingPt(inputptr, firststart);

    firststart = false;
	while(inputptr != NULL)
	{
		inputptr = isWord(inputptr, &head);
		inputptr = StartingPt(inputptr, firststart);
	}
	//free input pointers
	free(inputptr);
	
	//Print linked list
	PrintLL(head);

	//free linked list and all other malloced memory
	FreeLL(head);

	return 0;
}
