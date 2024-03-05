#include "sunspots.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void charValidator(char * line); 
void dateValidator(char * line);

ss_monthly_t parseLine(char * line) {
  ss_monthly_t ans;

  // Call the validation checker to ensure the input is valid
  // before parsing any data
  charValidator(line);
  dateValidator(line);

  /* Data Parse */
  // Parse year
  char year_temp[5];
  strncpy(year_temp, line, 4);
  year_temp[4] = '\0';
  ans.year = atoi(year_temp);

  //Parse month; need to check if month is in range 01-12
  char month_temp[3];
  strncpy(month_temp, line + 5, 2);
  month_temp[2] = '\0';
  ans.month = atoi(month_temp);
  if (!(ans.month >= 1 && ans.month <= 12)) {
    fprintf(stderr, "Input invalid: Month not in the range of 01-12.\n");
    exit(EXIT_FAILURE);
  }

  //Parse number of sunspots
  char * date_num_div = strchr(line, ',');  // pointer to ','
  ans.num = atof(date_num_div + 1);
  if (ans.num < 0) {
    fprintf(stderr, "Input invalid: Number of sunspots can not be negative.\n");
    exit(EXIT_FAILURE);
  }

  return ans;
}

void meanFilter(ss_monthly_t * data, size_t n, ss_monthly_t * mean, unsigned w) {
  // Validation check for the given window size: can not be odd or larger than n
  if (w % 2 == 0 || w > n) {
    fprintf(stderr,
            "Window size invalid: Window size should be an (unsigned) odd integer and "
            "not larger than the data length.\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < n; i++) {
    // Initialize two pointers for the window, moving from the center to each side
    // while keeping track the total sum of the window using window_sum
    int left = i;
    int right = i;

    double window_sum = data[i].num;
    unsigned actual_window_size = 1;

    for (unsigned j = 0; j < w / 2; j++) {
      if (left > 0) {
        left--;
        window_sum += data[left].num;
        actual_window_size++;
      }

      if (right < n - 1) {
        right++;
        window_sum += data[right].num;
        actual_window_size++;
      }
    }

    mean[i].year = data[i].year;
    mean[i].month = data[i].month;
    // For each data entry, the mean of num is calculated by window_sum / actual_window_size
    mean[i].num = window_sum / actual_window_size;
  }
}

double findLocalMax(ss_monthly_t * data, size_t n) {
  // Validation check for the data array size: can not be 0
  if (n == 0) {
    fprintf(stderr,
            "Data array size invalid: Input array should have more than 0 elements. \n");
    exit(EXIT_FAILURE);
  }

  // Compare current num with the previous max, if current num is larger
  // then we update both local_max and the max_timestamp
  double local_max = data[0].num;
  double max_timestamp;

  for (size_t i = 1; i < n; i++) {
    if (data[i].num > local_max) {
      local_max = data[i].num;
      max_timestamp = data[i].year + (double)data[i].month / 12.0;
    }
  }

  return max_timestamp;
}

double calcSsPeriod(double * timeStamps, size_t n) {
  // Validation check for the data array size: can not be less than 2
  if (n < 2) {
    fprintf(
        stderr,
        "timeStamps array size invalid: Input array should have 2 or more elements. \n");
    exit(EXIT_FAILURE);
  }

  // Use for loop to add up the total difference between each 2 timesatmps iteratively
  double total_difference = 0;

  for (size_t i = 1; i < n; i++) {
    total_difference += (timeStamps[i] - timeStamps[i - 1]);
  }

  // The average difference is calculated by total difference / (n - 1)
  return total_difference / (n - 1);
}

// This is a preliminary character-wise validation checker to check each data entry for:
// 1. Not pointing to Null
// 2. Contains no illegal characters
// 3. Has '-' and ',' as the divider for year, month, num, and no more than one "."
void charValidator(char * line) {
  // Check if input is Null
  if (line == NULL) {
    fprintf(stderr, "Input invalid: Input is NULL\n");
    exit(EXIT_FAILURE);
  }

  // Check no char other than, '-', ',' '.', '\n' and digit
  for (char * cur_char = line; *cur_char != '\0'; cur_char++) {
    if (*cur_char != '-' && *cur_char != ',' && *cur_char != '.' && *cur_char != '\n' &&
        !isdigit(*cur_char)) {
      fprintf(stderr, "Input invalid: Containing illegal character(s).\n");
      exit(EXIT_FAILURE);
    }
  }

  // Check missing or redundant punctuations; check missing digits
  int dash_count = 0;
  int comma_count = 0;
  int decimal_point_count = 0;
  int digit_count = 0;
  char * cur_char = line;

  while (*cur_char != '\0') {
    if (*cur_char == '-') {
      dash_count++;
    }
    else if (*cur_char == ',') {
      comma_count++;
    }
    else if (*cur_char == '.') {
      decimal_point_count++;
    }
    else if (isdigit(*cur_char)) {
      digit_count++;
    }
    cur_char++;
  }

  if (dash_count != 1 || comma_count != 1 || decimal_point_count > 1) {
    fprintf(
        stderr,
        "Input invalid: The input should have a '-', a ',' and no more than one '.'.\n");
    exit(EXIT_FAILURE);
  }
  if (digit_count < 7) {
    fprintf(stderr,
            "Input invalid: The input should have 6 digits for yyyy-mm and at least 1 "
            "digit for num.\n");
    exit(EXIT_FAILURE);
  }
}

// This is a preliminary date validation checker to check each data entry for:
// 1. Year and month have the correct amount of digits
void dateValidator(char * line) {
  char * date_num_div = strchr(line, ',');  // pointer to ','
  char * year_mon_div = strchr(line, '-');  // pointer to '-'

  // Check both year and month have exact amount of characters
  if (year_mon_div - line != 4) {
    fprintf(stderr, "Input invalid: Year must have 4 digits.\n");
    exit(EXIT_FAILURE);
  }
  if (date_num_div - year_mon_div != 3) {
    fprintf(stderr, "Input invalid: Month must have 2 digits.\n");
    exit(EXIT_FAILURE);
  }
}