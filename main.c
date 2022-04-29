/*Damian Dhesi*/
/*Returns a sorted list of the n most common words in a file or set of files
 *ordered by frequency. Utilizes hashtable.c to store these words*/

#include "fw.h"

int main(int argc, char *argv[]){

	/*hash_table is the hash table that the words will be stored in*/
	/*i and j are used in for-loops. i is also used to add chars to word*/
	/*c is used to hold a char read from a file or stdin*/
	/*n is used to determine how many words are displayed*/
	/*file is used to hold the file arguments*/
	/*word is used to hold each word to be added to the hash table*/
	/*WordSize is used to decide how much memory to allocate for the word
 	 *arbitrarily starts at 5 since it seemed like a good baseline between
	 *efficent use of memory and optimizing performace*/
	/*WordFound is used to tell if a word was found. 0 = false, 1 = true*/
	/*FreqWord points to the most frequent word once found*/
	/*freq holds the frequency of the most frequent word*/
	/*usage holds the usage error message*/
	/*NoMem holds the error message when the heap has no more space*/
	/*optarg and optind from unistd.h*/	
	HashTable hash_table = {NULL, 0, 0};
	int i, j, c;
	int n = 10;
	FILE *file = NULL;
	char *word = NULL;
	int WordSize = 5;
	int WordFound = 0;
	char *FreqWord = NULL;
	int freq = 0;
	char usage[] = "usage: fw [-n num] [ file1 [ file 2 ...] ]\n";
	extern char *optarg;
	extern int optind;

	/*check if -n is used and if so check if there is a valid arg. If
 	 *the arg is a postive number or zero then save the requested number
	 *of words to display in n*/  
	if(getopt(argc, argv, "n:") != -1){
        
		if(optarg == NULL){
			fprintf(stderr, "%s", usage);
			exit(EXIT_FAILURE);
		}
        
        /*Loop through given argument to ensure that only a number is given*/
		for(i = 0; (c = optarg[i]) != '\0'; i++){
			if(!isdigit(c) && c != ' '){
				fprintf(stderr, "%s", usage);
				exit(EXIT_FAILURE);
			}
		}
		
        /*Save the requested number of words to display if a valid input was
         *given*/
		n = atoi(optarg);
	}

	/*If there are no arguments beside -n #, where # is some valid input,
 	 *then sent the file to be read as the stdin*/ 
	if(argc == 1 || argc == optind){
		file = stdin;
	}

	/*Loop until all files or stdin are read*/
	for(j = optind; file == stdin || j < argc; j++){
		
		/*Set i to zero and check if the file is stdin or a valid file
 		 *that can be read*/
		i = 0;
		if(file == stdin || (file = fopen(argv[j], "r"))){
	
			/*Loop through chars until EOF reached*/
			while((c = fgetc(file)) != -1){
	
				/*Check if a letter has been found*/
				if(isalpha(c)){
					
					/*If a letter is found set WordFound to
 					 *to true and attempt to set space to 
					 *record the new word*/ 		
					WordFound = 1;
					if(word == NULL){
						if((word = malloc(WordSize)) ==
						    NULL){
							perror("malloc");
							exit(EXIT_FAILURE);
						}
					}
					
					/*Add the lowercase letter to the word
 					 *using i and increment i*/ 	
					word[i] = tolower(c);
					i++;

					/*if i = WordSize then attempt to 
 					 *reallocate more space (double the 
					 *previous space)*/ 
					if(i == WordSize){
						word = realloc(word,WordSize*2);
						if(word){
							WordSize *= 2;
						} else{
							perror("realloc");
							exit(EXIT_FAILURE);
						}
					}

				/*After the end of the word is reached set the
 				 *last char in the word as null and attempt to 
				 *add it to the hash table*/
				} else if(WordFound){
					word[i] = '\0';

				    add(word, &hash_table);
 
					/*Reset WordFound, i, word, and
 					 *WordSize to prepare for the next 
					 *word*/ 	
					WordFound = 0;
					i = 0;
					word = NULL;
					WordSize = 5;
				}	
			}
			
			/*if file = stdin then set it as null to indicate that
 			 *stdin has been read. Else attempt to close the file*/
			if(file == stdin){
				file = NULL;
			} else {
				if(fclose(file) != 0){
					perror("fclose");
                    exit(EXIT_FAILURE);
				}
			}

		/*If not able to read the file then check if it was due to
 		 *not having permission. Print relevant message if so*/ 	
		} else if(errno == EACCES){
			fprintf(stderr, "%s: Permission denied\n", argv[j]);

		/*If not a permission error when opening the file then print
 		 *that the file/dir does not exist*/
		} else{
		    fprintf(stderr, "%s: No such file or directory\n", argv[j]);
		}	
	}

	/*Print out the message "The top k words (out of n) are:"*/
	printf("The top %d words (out of %d) are:\n", n, hash_table.size);

	/*If there are less unique words that requested, then set n to the
 	 *number of unique words so that all words are displayed*/ 
	if(hash_table.size < n){
		n = hash_table.size;
	}

	/*Get and display n number of unique words*/
	for(i = 0; i < n; i++){
		
        FreqWord = PopMostFreq(&hash_table, &freq);
		printf("%9d %s\n", freq, FreqWord);	
	}

	/*Succesfuly ran main function*/
	return 0;
}
