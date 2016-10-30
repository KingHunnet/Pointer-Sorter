#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdlib.h>

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
	//wont work until linked list is fixed
	word* current = head;
	while(current->theWord != NULL)
	{
		printf("%s\n", current->theWord);
		current = current->next;
	}
}

//Creates new node with word specified word and pointing to null
word* newNode(char* totokenize)
{
	printf("newNode totokenize:%s\n", totokenize);
	word* theNewNode = (word*)malloc(sizeof(word));
	printf("made it this far\n");
	if (theNewNode == NULL){
		printf("malloc failed  |||||||||||||||||||||||||||||||||||||||||||\n");
	}
	char* wordsMemory = (char*)malloc(strlen(totokenize) * sizeof(char));
	if (wordsMemory == NULL){
		printf("malloc failed  |||||||||||||||||||||||||||||||||||||||||||\n");
	}

	strcpy(wordsMemory, totokenize);
	theNewNode->theWord = wordsMemory;
	theNewNode->next = NULL;
	return theNewNode;
}

void SaveAndSort(char* totokenize, word*** head){
	printf("{\nbeginning of ssave and sort\n");
	printf("totokenize:%s\n", totokenize);
	printf("head:%p\n", (**head));
	//need to fix comparisons in strcasecmp and that's it. Everything else should work properly
	//Special case for changing the head
	// if (head){
	// 	printf("head's word:%s\n", (*head)->theWord);
	// }

	// if ((**head)){
	// 	printf("valid head\n");
	// 	printf("%s\n", (**head)->theWord);
	// }

	// if ((**head)->theWord == NULL){
	// 	printf("--------------------------------------------------------------------------------\n");
	// }

	printf("head's word:%s\n", (**head)->theWord);
	if ((**head)->theWord == NULL|| strcasecmp(totokenize,((**head)->theWord)) < 0)
	{
		// if(**head){
		// 	printf("in if of valid pointer\n");
		// 	int casecmpint = strcasecmp(totokenize,((***head)->theWord));
		// 	// printf("made it past case comp\n");
		// 	printf("strcasecmp=%d\n", casecmpint);
		// }
		printf("in if\n");
		word* nodetoadd = newNode(totokenize);
		// printf("here before head change\n");
		
		nodetoadd->next = **head;
		**head = nodetoadd;
		
		printf("head:%p\n", **head);
		printf("head's word: %s, head's next: %p\n", (**head)->theWord, (**head)->next);
	}
	else
	{
		//Locate the node before point of insertion
		word* current = **head;

		while(!((current)->next) && strcasecmp(totokenize, ((current)->next->theWord)) > 0)
		{
			current = (current)->next;
		}
		printf("Node before: Pointer:%p, Word:%s, NExt: %p\n", current, current->theWord, current->next);
		//check to make sure not the same word first and then add in node
		printf("current->next->theWord:%s\n", current->next->theWord);
		if (((current->next->theWord) == NULL) || strcasecmp(totokenize, (current->next->theWord)) < 0)
		{
			printf("---------------------------------------------------------------------------------------------------------------------------\n");
			word* nodetoadd = newNode(totokenize);
			nodetoadd->next = current->next;
			current->next = nodetoadd;
			printf("Node to add pointer:%p, nodetoadd next, %p\n", nodetoadd, nodetoadd->next);
			printf("current: %p, current next: %p\n", current, current->next);
		}

		



	}

	printf("end of save and sort\n}\n");
}
/*
From Tokenizer. Gets char's until it hits non-alpha char. Appends chars 
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
		// printf("head: %p, before save and sort: %s\n", head, head->theWord);
		SaveAndSort(totokenize, &head);
		return ts;
	}

	return NULL;
}
/*
From Tokenizer. Makes sure no null return in isWord. This method initally
also had isDec, isFloat, isOct, isHex
head refers to the head of the linked list
*/
//Do we even need this method?
// char* TKCreate( char * ts, word* head ) {
// 	char* wordret = isWord(ts, head);
// 	if (wordret != NULL){
// 		return wordret;
// 	}
// 	printf("RETURNED NULLLLLLLLLL!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
// 	return NULL;
// }

/*
Gets to the next starting point of a word. keeps going until it finds an
alpha char or a null. Terminates on null and throws error if need be
*/
char* StartingPt(char *inputptr){
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

	if(moved == false){
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
	char input[] = " 3001.0000e5 spartans4 killed 1000 033333 persians CONQUERED 0x12345DEF";
	char *inputptr;
	inputptr = malloc(strlen(input) * sizeof(char));
	strcpy(inputptr, input);
	printf("%s\n", inputptr);

	word startinghead = {NULL, NULL};
	word* head = &startinghead;
	printf("%p\n", head);

	// word *head = (word*)malloc(sizeof(word));
	head->theWord = NULL;
	head->next = NULL;
	// printf("head:%p\nhead's word:%s\nhead's next:%p\n", head, head->theWord, head->next);

	// if (!head){
	// 	printf("head is null. you done fucked\n");
	// 	return 0;
	// }
	// word headdata = {'\0', 0, NULL};
	// memcpy(head, &headdata, sizeof(headdata));
	// printf("head: : %s, %p\n", head->theWord, head->next);
	// int dumbcounter = 0;

	inputptr = StartingPt(inputptr);

	while(inputptr != NULL)
	{
		inputptr = isWord(inputptr, &head);
		inputptr = StartingPt(inputptr);
		printf("head:%p, head's word:%s\n", head, head->theWord);
	}
	//free input pointer
	free(inputptr);
	//Print linked list
	printf("before PrintLL. head:%p\n", head);
	//Print linked list
	PrintLL(head);

	//free linked list
	FreeLL(head);

	return 0;
}