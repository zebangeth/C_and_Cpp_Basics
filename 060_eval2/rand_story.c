#include "rand_story.h"
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <ctype.h> 


// check for underscores in the input file and 
// print the file after replacing blank with chosen words
void print_parse_story(FILE * f) {
  char c;
  int in_blank = 0; // flag: 1 after first underscore, and 0 after second
  char * catg_name = NULL; 
  size_t catg_len = 0; 
  size_t idx = 0; 

  while ((c = fgetc(f)) != EOF) {
    // inside the blank
    if (in_blank) {
      catg_name = realloc(catg_name, (catg_len + 1) * sizeof(*catg_name)); 
      if (catg_name == NULL) {
        fprintf(stderr, "Failed to reallocate memory.\n");
        exit(EXIT_FAILURE); 
      }
      catg_len++; 
      if (c == '_') {
        catg_name[idx] = '\0'; 
        const char * chosen_word = chooseWord(catg_name, NULL); 
        printf("%s", chosen_word); 
        in_blank = 0; 
        idx = 0; 
        free(catg_name); 
        catg_name = NULL;
        catg_len = 0;
      } else {
        catg_name[idx++] = c; 
      }
    // outside the blank
    } else {
      if (c == '_') {
        in_blank = 1;
      } else {
        printf("%c", c);
      }
    }
  }

  // check for unmatched blank after reading the file
  if (in_blank) {
    fprintf(stderr, "Found unmatched underscore in the story file.\n");
    exit(EXIT_FAILURE);
  }
}

// create an empty catarray 
catarray_t * create_catarray() {
    catarray_t * catarray = malloc(sizeof(*catarray)); 
    if (catarray == NULL) {
      fprintf(stderr, "Could not allocate memory for catarray.\n"); 
      return NULL;
    }
    catarray->arr = NULL;
    catarray->n = 0;
    return catarray;
}

// check if category already exists, and find the category if exist
category_t * find_category(catarray_t * cats, const char * category) {
  for (size_t i = 0; i < cats->n; i++) {
    if (!strcmp(cats->arr[i].name, category)) {
      return &cats->arr[i];
    }
  }
  return NULL;
}

// add a new category to the catarray
void add_category(catarray_t * cats, const char * cat_name) {
  cats->n++;
  cats->arr = realloc(cats->arr, cats->n * sizeof(*cats->arr)); 
  if (cats->arr == NULL) {
    fprintf(stderr, "Failed to reallocate memory.\n");
    exit(EXIT_FAILURE); 
  }
  cats->arr[cats->n - 1].name = strdup(cat_name);
  cats->arr[cats->n - 1].n_words = 0;
  cats->arr[cats->n - 1].words = NULL;
}

// parse categories from file f to cats
void parse_categories(FILE * f, catarray_t * cats) {
  char * cur = NULL;
  size_t len = 0;

  while ((getline(&cur, &len, f)) != -1) {
    char * div = strchr(cur, ':');
    if (!div) {
      fprintf(stderr, "Missing colon.\n");
      free(cur);
      if (fclose(f) != 0) {
        fprintf(stderr, "Could not close the words file.\n"); 
        free_catarray(cats); 
        exit(EXIT_FAILURE);
      }
      free_catarray(cats); 
      exit(EXIT_FAILURE);
    }

    *div = '\0';
    char * cat_name = cur;
    char * word = div + 1;

    // replace the \n
    size_t word_len = strlen(word); 
    if (word_len > 0 && word[word_len - 1] == '\n') {
      word[word_len - 1] = '\0'; 
    }

    // check if category already exists, and find the category if exist
    category_t * category = find_category(cats, cat_name); 
    if (!category) {
      add_category(cats, cat_name); 
      category = &cats->arr[cats->n - 1]; 
    } 
    // add word to category
    category->n_words++; 
    category->words = realloc(category->words, category->n_words * sizeof(*category->words));
    if (category->words == NULL) {
      fprintf(stderr, "Failed to reallocate memory.\n");
      free(cur);
      if (fclose(f) != 0) {
        fprintf(stderr, "Could not close the words file.\n"); 
        free_catarray(cats); 
        exit(EXIT_FAILURE);
      }
      free_catarray(cats);
      exit(EXIT_FAILURE);
    }
    category->words[category->n_words - 1] = strdup(word); 
  }
  free(cur);
}

// free all the memory spaces used by cats
void free_catarray(catarray_t * cats) {
  for (size_t i = 0; i < cats->n; i++) {
    category_t * category = &cats->arr[i]; 
    for (size_t j = 0; j < category->n_words; j++) { 
      free(category->words[j]); 
    }
    free(category->words); 
    free(category->name); 
  }
  free(cats->arr); 
  free(cats); 
}

// return the int if input is a valid integer, else 0
int get_index(char * str, category_t * prev_words) {
  char * cur = str; 
  while (*cur) {
    if (!isdigit(*cur)) {
      return 0; 
    }
    cur++;
  }
  int index = atoi(str); 
  if (index > 0) return index; 
  return 0;
}

// add the the most recently added word to the prev_word
void add_prev_word(category_t * prev_words, const char * word_to_add) {
  prev_words->n_words++;
  prev_words->words = realloc(prev_words->words, prev_words->n_words * sizeof(*prev_words->words));
  if (prev_words->words == NULL) {
    fprintf(stderr, "Failed to reallocate memory for prev words.\n");
    exit(EXIT_FAILURE);
  }
  prev_words->words[prev_words->n_words - 1] = strdup(word_to_add);
}

// print the file after replacing blank with randomly chosen or prev used words
void print_story_with_catwords(FILE * f, catarray_t * categories) {
  char c;
  int in_blank = 0; // flag: 1 after first underscore, and 0 after second
  char * catg_name = NULL; 
  size_t catg_len = 0; 
  size_t idx = 0; 

  category_t prev_words;
  prev_words.name = "prev_words"; 
  prev_words.n_words = 0;
  prev_words.words = NULL;

  while ((c = fgetc(f)) != EOF) {
    // inside the blank
    if (in_blank) {
      catg_name = realloc(catg_name, (catg_len + 1) * sizeof(*catg_name)); 
      if (catg_name == NULL) {
        fprintf(stderr, "Failed to reallocate memory.\n");
        exit(EXIT_FAILURE); 
      }
      catg_len++; 
      if (c == '_') {
        catg_name[idx] = '\0'; 

        const char * chosen_word = NULL;
        int back_ref_idx = get_index(catg_name, &prev_words); 
        // check if we can find a valid back referenced index
        if (back_ref_idx != 0) {
          if (back_ref_idx > prev_words.n_words) {
            fprintf(stderr, "Back referenced index out of bound.\n");
            exit(EXIT_FAILURE); 
          }
          chosen_word = prev_words.words[prev_words.n_words - back_ref_idx];
        } else {
          chosen_word = chooseWord(catg_name, categories); 
        }
        add_prev_word(&prev_words, chosen_word);
        printf("%s", chosen_word); 
        in_blank = 0; 
        idx = 0; 
        free(catg_name); 
        catg_name = NULL;
        catg_len = 0;
      } else {
        catg_name[idx++] = c; 
      }
    // outside the blank
    } else {
      if (c == '_') {
        in_blank = 1;
      } else {
        printf("%c", c);
      }
    }
  }

  // check for unmatched blank after reading the file
  if (in_blank) {
    fprintf(stderr, "Found unmatched underscore in the story file.\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < prev_words.n_words; i++) {
    free(prev_words.words[i]);
  }
  free(prev_words.words);
}

// remove word from the category
void remove_word(category_t * category, const char * word) {
  for (size_t i = 0; i < category->n_words; i++) {
    if (strcmp(category->words[i], word) == 0) {
      free(category->words[i]);
      for (size_t j = i; j < category->n_words - 1; j++) {
        category->words[j] = category->words[j + 1];
      }
      category->n_words--;
      break;
    }
  }
}

// print the file after replacing blank with randomly chosen or prev used words
// and can take a no_reuse arg to avoid choosing the same word
void print_story_check_reuse(FILE * f, catarray_t * categories, int no_reuse) {
  char c;
  int in_blank = 0; // flag: 1 after first underscore, and 0 after second
  char * catg_name = NULL; 
  size_t catg_len = 0; 
  size_t idx = 0; 

  category_t prev_words;
  prev_words.name = "prev_words"; 
  prev_words.n_words = 0;
  prev_words.words = NULL;

  while ((c = fgetc(f)) != EOF) {
    // inside the blank
    if (in_blank) {
      catg_name = realloc(catg_name, (catg_len + 1) * sizeof(*catg_name)); 
      if (catg_name == NULL) {
        fprintf(stderr, "Failed to reallocate memory.\n");
        exit(EXIT_FAILURE); 
      }
      catg_len++; 
      if (c == '_') {
        catg_name[idx] = '\0'; 

        const char * chosen_word = NULL;
        int back_ref_idx = get_index(catg_name, &prev_words); 
        if (back_ref_idx != 0) {
          if (back_ref_idx > prev_words.n_words) {
            fprintf(stderr, "Back referenced index out of bound.\n");
            exit(EXIT_FAILURE); 
          }
          chosen_word = prev_words.words[prev_words.n_words - back_ref_idx];
        } else {
          chosen_word = chooseWord(catg_name, categories); 
        }

        add_prev_word(&prev_words, chosen_word); 
        printf("%s", chosen_word); 

        // if reuse is not allowed, we can remove the most recent chosen word
        if (no_reuse && back_ref_idx == 0) { 
          category_t * category = find_category(categories, catg_name);
          if (category != NULL) {
            remove_word(category, chosen_word);
          }
        }

        in_blank = 0; 
        idx = 0; 
        free(catg_name); 
        catg_name = NULL;
        catg_len = 0; 
      } else {
        catg_name[idx++] = c; 
      }
    // outside the blank
    } else {
      if (c == '_') {
        in_blank = 1;
      } else {
        printf("%c", c);
      }
    }
  }

  // check for unmatched blank after reading the file
  if (in_blank) {
    fprintf(stderr, "Found unmatched underscore in the story file.\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < prev_words.n_words; i++) {
    free(prev_words.words[i]);
  }
  free(prev_words.words);
}