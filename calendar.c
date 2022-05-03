#include "calendar.h"
#include "event.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct calendar *new_cal(void) {
  struct calendar *new_cal = malloc(sizeof(struct calendar));
  new_cal->size = 0;
  return new_cal;
}

/*
 * Write the footer of a calendar, ends the calendar
 * in ics format
 */
void write_cal_foot(FILE *out) { fprintf(out, "END:VCALENDAR\n"); }

/*
 * Write header info for the calendar in ics format.
 * Also specifies timezone.
 */
void write_cal_head(FILE *out) {
  fprintf(
      out,
      "BEGIN:VCALENDAR\nMETHOD:PUBLISH\nVERSION:2.0\nX-WR-CALNAME:"
      "Planner\nPRODID:-//Apple Inc.//macOS "
      "12.3.1//EN\nX-APPLE-CALENDAR-COLOR:#25FFA8\nX-WR-TIMEZONE:Australia/"
      "Sydney\nCALSCALE:GREGORIAN\nBEGIN:VTIMEZONE\nTZID:Australia/"
      "Sydney\nBEGIN:STANDARD\nTZOFFSETFROM:+1100\nRRULE:FREQ=YEARLY;BYMONTH=4;"
      "BYDAY=1SU\nDTSTART:20080406T030000\nTZNAME:AEST\nTZOFFSETTO:+1000\nEND:"
      "STANDARD\nBEGIN:DAYLIGHT\nTZOFFSETFROM:+1000\nRRULE:FREQ=YEARLY;BYMONTH="
      "10;BYDAY=1SU\nDTSTART:20081005T020000\nTZNAME:AEDT\nTZOFFSETTO:+"
      "1100\nEND:DAYLIGHT\nEND:VTIMEZONE\n");
}

void write_cal(struct calendar *calendar) {
  srand((unsigned)time(NULL));
  char cal_name[50];
  snprintf(cal_name, 25, "calendar%d.ics", rand() % DAY);
  time_t t = get_start();
  int len = collect_events(calendar->events, t);
  if (len == 0) {
    printf("No events written.\n");
    exit(1);
  }
  calendar->size = len;
  FILE *f = fopen(cal_name, "w");
  printf("Writing %d events to %s...\n", calendar->size, cal_name);
  write_cal_head(f);
  for (int i = 0; i < len; i++) {
    write_event(f, calendar->events[i]);
  }
  write_cal_foot(f);

  printf("Finished writing events.\n");
  fclose(f);
}

void print_intro(void) {
  printf("Welcome to planter\nThe goal of this project is to make it easy to "
         "plan your day.\n\nEvents are created in sequence, you simply specify "
         "a title and duration.\n\nType 'q' in the event title to finish "
         "entering events.\n\n");
}

void free_cal(struct calendar *calendar) {
  for (int i = 0; i < calendar->size; i++) {
    free_event(calendar->events[i]);
  }
  free(calendar);
}
