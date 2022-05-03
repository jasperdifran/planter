#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *substring(char *str, int start, int end) {
  char *new = malloc(sizeof(char) * (end - start + 1));
  int j = 0;
  for (int i = start; i < end; i++) {
    new[j] = str[i];
    j++;
  }
  new[end - start] = '\0';

  return new;
}

/*
 * Find the index of a particular character in a string. Return
 * -1 if string does not contain a.
 */
int ind_of(char *str, char a) {
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == a)
      return i;
  }
  return -1;
}

/*
 * Get a time in the format YYYYMMDDTHHmmss
 */
char *get_tz(time_t t) {
  char *tok;
  const char s[2] = " ";
  char *buf = ctime(&t);
  tok = strtok(buf, s);
  tok = strtok(NULL, s);
  int month = get_month(tok);
  tok = strtok(NULL, s);
  int day = atoi(tok);
  tok = strtok(NULL, s);
  char time[15];
  int j = 0;
  for (int i = 0; i < 9; i++) {
    if (tok[i] != ':') {
      time[j] = tok[i];
      j++;
    }
  }
  tok = strtok(NULL, s);
  char year[6];
  strcpy(year, tok);
  trim(year);
  char *result = malloc(sizeof(char) * 17);
  sprintf(result, "%s%02d%02dT%sZ", year, month, day, time);

  return result;
}

/*
 * Convert a month string into a number, Jan = 1 and so on
 */
int get_month(char *str) {
  if (strcmp(str, "Jan") == 0)
    return 1;
  if (strcmp(str, "Feb") == 0)
    return 2;
  if (strcmp(str, "Mar") == 0)
    return 3;
  if (strcmp(str, "Apr") == 0)
    return 4;
  if (strcmp(str, "May") == 0)
    return 5;
  if (strcmp(str, "Jun") == 0)
    return 6;
  if (strcmp(str, "Jul") == 0)
    return 7;
  if (strcmp(str, "Aug") == 0)
    return 8;
  if (strcmp(str, "Sep") == 0)
    return 9;
  if (strcmp(str, "Oct") == 0)
    return 10;
  if (strcmp(str, "Nov") == 0)
    return 11;
  if (strcmp(str, "Dec") == 0)
    return 12;
  return 0;
}

/*
 * Collects the starting time for a user, i.e. the time they wake up
 */
time_t get_start(void) {
  printf("Wake up at: ");
  char buf[10];
  fgets(buf, 9, stdin);
  trim(buf);
  int len = strlen(buf);

  int mins = 0, hours = 0;
  int colon_ind = ind_of(buf, ':');
  if (colon_ind != -1) {
    char min_buf[10];
    for (int i = colon_ind + 1; i < len; i++) {
      min_buf[i - (colon_ind + 1)] = buf[i];
      min_buf[i - colon_ind] = '\0';
    }
    mins = atoi(min_buf);
  }

  char hour_buf[10];
  for (int i = 0; i < ((colon_ind == -1) ? len : colon_ind); i++) {
    hour_buf[i] = buf[i];
    hour_buf[i + 1] = '\0';
  }

  hours = atoi(hour_buf);

  time_t t = time(NULL);
  t -= t % DAY + TZ;
  t += DAY + hours * 60 * 60 + mins * 60;
  printf("\n");
  return t;
}
