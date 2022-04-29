/*by Damian Dhesi*/
/*Implementation of a hashtable with partial funcitionality in order to store
 *words read by the main function. Utilizes seperate chaining for collision
 *resolution*/

#include "fw.h"


/*Basic Hash function. Generates a key based on the characters pointed to and
 *the capacity of the HashTable*/
int Hash(char *str, HashPtr hash_table){
	unsigned key = 0;
	
	while(*str != '\0'){
		key = *str + 22 * key;
		str++;
	}
	return key % hash_table->capacity;
}

/*Adds a word to the HashTable or increments its frequency if it is already
 *present. Returns 0 if run succesfully and non-zero if an error occurs*/
void add(char *str, HashPtr hash_table){

	/*index is the index on the Hashtable of the word to be added using
 	 *the hash function*/
	/*i is used in the for-loop*/
	/*ptr is used to loop through the linked list at each array index*/ 
	/*EndPtr is used to point to the last element of the linked list at an
 	 *indice of a HashTable*/ 
	int index, i;
	WordPtr ptr = NULL;
	WordPtr EndPtr = NULL;

	/*if the capcity is zero then the HashTable needs to be created*/
	if(hash_table->capacity == 0){
		/*Create a HashTable of arbitrary size, 10 in this case. Set
 		 *all indices to null and set capacity to 10 if succesful*/  
		hash_table->table = (WordLink**) malloc(sizeof(WordPtr) * 10);
		if(hash_table->table){
			hash_table->capacity = 10;
			for(i = 0; i < hash_table->capacity; i++){
				hash_table->table[i] = NULL;
			}
		}else{
            /*Return an erro is unsuccesful in getting memory space*/
            perror("malloc");
			exit(EXIT_FAILURE);
		}
	}

	/*Get index using hash function and check if there is a word at that
 	 *index*/	 
	index = Hash(str, hash_table);
	if(hash_table->table[index]){
		/*Since there is a word at the index loop through the linked
 		 *list until the word to be added, str, if found in the list
		 *or the end of the list is reached. If str is in the list
		 *then increment it's frequency*/ 
		for(ptr = hash_table->table[index]; ptr != NULL;
		    ptr = ptr->nxt){
			if(strcmp(ptr->word, str) == 0){
				ptr->freq++;
				/*free str if it is already in the table*/
				free(str); 
				return;
			}

			/*Set the end pointer to the second to last pointer in
 			 *the linked list*/ 
			if(ptr->nxt == NULL){
				EndPtr = ptr;
			}
		}
	
		/*Attempt to add the word to be added, str, to the end of the
 		 *list and increment size. Return an error message 
         *if unsuccessful*/
		EndPtr->nxt = (WordPtr) malloc(sizeof(WordLink));
		if(EndPtr->nxt){
			EndPtr->nxt->word = str;
			EndPtr->nxt->freq = 1;
			EndPtr->nxt->nxt = NULL;
			hash_table->size++;
		} else{
			perror("malloc");
            exit(EXIT_FAILURE);
		}		
	} else{
		/*Since this index was empty place the word, str, here and
 		 *increment size if there is memory space*/
		hash_table->table[index] = (WordPtr) malloc(sizeof(WordLink));
		if(hash_table->table[index]){
			hash_table->table[index]->word = str;
			hash_table->table[index]->freq = 1;
			hash_table->table[index]->nxt = NULL;
			hash_table->size++;
		} else{
			perror("malloc");
            exit(EXIT_FAILURE);
		}
	}
	
	/*In order for the hash table to be effiecent the load factor needs
 	 *to stay below 1. Increase the hash table capacity if
	 *load factor = 1*/
	if(LoadFactor(hash_table) == 1){
        /*Attempt to increase the table capacity*/
        UpCapacity(hash_table);
	}

}

/*Increase the capacity of the HashTable. Non-zero value returned if an error
 *occurs*/
void UpCapacity(HashPtr PrevTable){

	/*i is used in the for-loop*/
	/*ptr is used to loop through the linked lists in the hash table*/
	/*temp is used to temperarily hold the value of differenet pointers as
 	 *needed*/
	/*NewPtr is used to hold the new version of the pointer from PrevTable
 	 *that is added to NewTable*/ 	 
	/*NewTable is the new hash table with double the indicies*/
	int i, index;
	WordPtr ptr = NULL;
	WordPtr temp = NULL;
	WordPtr NewPtr = NULL;
	HashTable NewTable = 
		{(WordPtr*) malloc(sizeof(WordPtr) *
			 (2 * PrevTable->capacity)),
		 0,
		 2 * PrevTable->capacity};

	/*Check if memory for the new table was allocated successfully. Return
 	 *an error if not*/	
	if(NewTable.table){
		
		/*Set all pointers to NULL in the hash table to null innitally*/
		for(i = 0;  i < NewTable.capacity; i++){
			NewTable.table[i] = NULL;
		}

		/*Loop through the previous table and add every value to the
 		 *new hash table*/
		for(i = 0; i < PrevTable->capacity; i++){
			for(ptr = PrevTable->table[i]; ptr != NULL;
			    ptr = ptr->nxt){
				
				/*Calculate a new index and attempt to allocate
 				 *space for a new WordPtr*/ 
				index = Hash(ptr->word, &NewTable);
				if((NewPtr = (WordPtr) malloc(sizeof(WordLink)))
                     		    == NULL){
					perror("malloc");
                    			exit(EXIT_FAILURE);
				}
				
				/*Set the NewPtr equal to the contents of the
 				 *pointer from PrevTable and add it to the 
				 *end of the linked list. Add to the head of 
				 *the linked list if there are no values yet.
				 *Once added, set nxt to NULL since there are
				 *no further pointers and increase size*/
				*NewPtr = *ptr;
				if(NewTable.table[index]){
					temp = NewTable.table[index];
					while(temp->nxt != NULL){
						temp = temp->nxt;
					}

					temp->nxt = NewPtr;
					temp->nxt->nxt = NULL;
					NewTable.size++;
				} else{
					NewTable.table[index] = NewPtr;
					NewTable.table[index]->nxt = NULL;
					NewTable.size++;
				}			
			}
			
			/*Free all memory taken by the old pointers for this
 			 *index since they have all been added to the new
			 *table*/ 
			ptr = PrevTable->table[i];
			while(ptr != NULL){
				temp = ptr->nxt;
				free(ptr);
				ptr = temp;
			}	
		}

		/*Free memory taken by the old table and set the old
 		 *table equal to the new hash table*/ 
		free(PrevTable->table);
		*PrevTable = NewTable;		
	} else{
		/*Unable to get memory space for the new hash table*/
        perror("malloc");
        exit(EXIT_FAILURE);
	}
	
}

/*Returns the load factor of the HashTable*/
int LoadFactor(HashPtr hash_table){
	return hash_table->size/hash_table->capacity;
}

/*Finds the word with the highest frequency and removes the word from the table
 *while also returning the word. Highest freq returned via pointer (MaxFreq)*/
char* PopMostFreq(HashPtr hash_table, int *MaxFreq){

	/*ptr is used to loop through the links in the linked list as well as
 	 *remove the word with the greatest frequency from the list*/
	/*start is the starting max frequency word*/
	/*MaxPtr is used to find the word with the highest frequency and record
 	 *values*/
	/*temp is used to temperaily hold a WordPtr value*/
	/*i is used in the for-loop*/
	/*TableIndex tracks the index that the word with highest freq is in*/
	/*MaxStr is used to hold the address of the string with the highest
 	 *freq*/   
	WordPtr ptr = NULL;
	WordLink start = {NULL, 0, NULL};
	WordPtr MaxPtr = &start;
	WordPtr temp = NULL;
	int i, TableIndex;
	char *MaxStr = NULL;

	/*Loop through the hash_table and check for which word has the highest
 	 *freq. If a word has a higher frequency than the previously recorded
	 *one, save it and use it as a comparison from then on. If two words
	 *have equal frequency then ties are broken based on which word comes
	 *later alphabetically*/ 
	for(i = 0; i < hash_table->capacity; i++){
		for(ptr = hash_table->table[i]; ptr != NULL; ptr = ptr->nxt){
			if(ptr->freq > MaxPtr->freq){
				MaxPtr = ptr;
				TableIndex = i;
			} else if(ptr->freq == MaxPtr->freq){
				if(strcmp(ptr->word, MaxPtr->word) > 0){
					MaxPtr = ptr;
					TableIndex = i;
				}
			}
		}
	}

	/*Record the word with the highest frequency in the table*/
	MaxStr = MaxPtr->word;
	*MaxFreq = MaxPtr->freq;

	/*If the pointer with the highest freq is at the head of a linked list
 	 *then free the memory taken by the struct for the word with the 
	 *highest feq. Decrement the size of the table*/
	if(MaxPtr == hash_table->table[TableIndex]){
		temp = hash_table->table[TableIndex]->nxt;
		free(hash_table->table[TableIndex]);
		hash_table->table[TableIndex] = temp;
		hash_table->size--;
		return MaxStr;
	}
	
	for(ptr = hash_table->table[TableIndex]; ptr->nxt != MaxPtr; 
	    ptr = ptr->nxt){
		/*Loop until the pointer before the one with the highest freq
 		 *is found. Purposely empty*/;
	}

	/*Free memory space and set the max freq pointer to point
 	 *to the next word-freq pair in the linked list. Decrement table size
 	 *since an element has been removed*/ 
	temp = ptr->nxt->nxt;
	free(ptr->nxt);
	ptr->nxt = temp;
	hash_table->size--;

	/*Return the string with the highest frequency*/
	return MaxStr;
}
