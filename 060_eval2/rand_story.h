#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include "provided.h"

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <ctype.h> 

//any functions you want your main to use

// check for underscores in the input file and 
// print the file after replacing blank with chosen words
void print_parse_story(FILE * f); 

// create an empty catarray 
catarray_t * create_catarray(); 
// parse categories from file f to cats
void parse_categories(FILE * f, catarray_t * cats); 
// free all the memory spaces used by cats
void free_catarray(catarray_t * cats); 

// print the file after replacing blank with chosen word from category or prev used words
void print_story_with_catwords(FILE * f, catarray_t * categories); 

// print the file after replacing blank with randomly chosen or prev used words
// and can take a no_reuse arg to avoid choosing the same word
void print_story_check_reuse(FILE * f, catarray_t * categories, int no_reuse); 

#endif
