#include "event.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Collects user events from stdin
 * Returns the number of events added to the array,
 * takes in an array of struct events to be filled
 */
int collect_events(struct event *events[100], time_t start) {
  char buf[100];

  // We will collect a max of 100 events for the day
  int i = 0;
  while (i < 100) {
    struct event *e = init_event();
    char *e_start = substring(ctime(&start), 11, 19);
    printf("From %s...\n", e_start);
    free(e_start);
    printf("Event title: ");

    int valid_arg = 0;
    while (!valid_arg) {
      fgets(buf, 100, stdin);
      trim(buf);
      if (strlen(buf) == 1 && (buf[0] == 'q' || buf[0] == 'Q')) {
        free(e);
        printf("Exiting events...\n");
        return i;
      }
      if (strlen(buf) > 0)
        valid_arg = 1;
      else
        printf("Invalid arg\n");
    }
    set_title(e, buf);
    set_start(e, start);

    valid_arg = 0;
    int dur = 0;
    while (!valid_arg) {
      printf("Duration (mins): ");
      fgets(buf, 10, stdin);
      trim(buf);
      if (strlen(buf) == 1 && (buf[0] == 'q' || buf[0] == 'Q')) {
        free(e);
        printf("Exiting events...\n");
        return i;
      }
      dur = atoi(buf);
      if (dur > 0)
        valid_arg = 1;
      else
        printf("Invalid duration\n");
    }
    start += dur * 60;
    set_dur(e, dur);
    events[i] = e;
    printf("\n");
    i++;
  }
  return i;
}

/*
 * Trim the newline from the end of a string
 */
void trim(char *string) {
  int len = strlen(string);
  if (string[len - 1] == '\n')
    string[len - 1] = '\0';
}

/*
 * Initialise a new event
 */
struct event *init_event(void) {
  struct event *new = malloc(sizeof(struct event));
  if (new == NULL)
    return NULL;
  new->start = 0;
  new->duration = 0;
  new->title = NULL;
  return new;
}

/*
 * Set the start time of an event
 */
void set_start(struct event *e, time_t start) { e->start = start; }

/*
 * Set the duration of an event. Durations stored as seconds
 * but must be passed to this function as minutes
 */
void set_dur(struct event *e, int mins) { e->duration = mins * 60; }

/*
 * Set the title of an event
 */
void set_title(struct event *e, char *title) { e->title = strdup(title); }

/*
 * Used for debugging, prints the fields contained
 * in an event struct
 */
void print_event(struct event *event) {
  printf("--- EVENT ---\n");
  printf("Name: %s\n", event->title);
  printf("Start time: %s\n", ctime(&event->start));
  printf("Duration: %dmins\n", event->duration / 60);
  printf("-------------\n\n");
}

void free_event(struct event *event) {
  free(event->title);
  free(event);
}

/*
 * Write the details of an event to a file pointer in ics foramt
 */
void write_event(FILE *out, struct event *event) {
  time_t current_time = time(NULL);
  char *created = get_tz(current_time);
  fprintf(out, "BEGIN:VEVENT\n");
  fprintf(out, "CREATED:%s\n", created);
  char uid[30];
  get_event_uid(event, uid);
  fprintf(out, "SUMMARY:%s\n", event->title);
  fprintf(out, "UID:%s\n", uid);
  fprintf(out, "SEQUENCE:1\n");
  fprintf(out, "STATUS:CONFIRMED\n");
  fprintf(out, "DTSTAMP:%s\n", created);
  char *start = get_tz(event->start);
  start[15] = '\0';
  char *end = get_tz(event->start + event->duration);
  end[15] = '\0';
  fprintf(out, "DTSTART;TZID=Australia/Sydney:%s\n", start);
  fprintf(out, "DTEND;TZID=Australia/Sydney:%s\n", end);
  fprintf(out, "END:VEVENT\n");
}

/*
 * Create a pseudo-random uid for an event. Almost certainly will be unique
 */
void get_event_uid(struct event *event, char *uid) {
  long t = event->start;
  srand((unsigned)time(NULL));
  t += rand() % event->duration;
  snprintf(uid, 25, "%d", (int)t);
}
