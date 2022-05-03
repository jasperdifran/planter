#ifndef _UTIL_H_
#define _UTIL_H_

#include <time.h>

#define DAY 86400
#define TZ 60 * 60 * 10

/* String */
void trim(char *string);
char *substring(char *str, int start, int end);
int ind_of(char *str, char a);

/* Time */
int get_month(char *str);
time_t get_start(void);
char *get_tz(time_t t);

#endif
