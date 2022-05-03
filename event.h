#ifndef _EVENT_H_
#define _EVENT_H_

#include <stdio.h>
#include <time.h>

struct event {
  time_t start;
  int duration;
  char *title;
};

int collect_events(struct event *events[100], time_t start);

void write_event(FILE *out, struct event *event);

struct event *init_event(void);
time_t get_start(void);
void set_start(struct event *e, time_t start);
void set_dur(struct event *e, int mins);
void set_title(struct event *e, char *title);

void get_event_uid(struct event *event, char *uid);

void free_event(struct event *event);

void print_event(struct event *event);

#endif
