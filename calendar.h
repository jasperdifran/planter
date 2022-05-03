#ifndef _CALENDAR_H_
#define _CALENDAR_H_

#include "event.h"

struct calendar {
    struct event *events[100];
    int size;
};

struct calendar *new_cal(void);
void write_cal_foot(FILE *out);
void write_cal_head(FILE *out);
void write_cal(struct calendar *calendar);
void print_intro(void);
void free_cal(struct calendar *calendar);

#endif
