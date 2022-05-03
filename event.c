#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"

#define DAY 86400
#define TZ 60*60*10

int main(void) {
    print_intro();
    struct calendar *cal = new_cal();
    write_cal(cal);
    free_cal(cal);
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
        hour_buf[i+1] = '\0';
    }

    hours = atoi(hour_buf);

    time_t t = time(NULL);
    t -= t % DAY + TZ;
    t += DAY + hours * 60 * 60 + mins * 60;
    printf("\n");
    return t;
}

/*
 * Find the index of a particular character in a string. Return
 * -1 if string does not contain a.
 */
int ind_of(char *str, char a) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == a) return i;
    }
    return -1;
}

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
 * Collects user events from stdin
 * Returns the number of events added to the array,
 * takes in an array of struct events to be filled
 */
int collect_events(struct event *events[100], time_t start) {
    char buf[100];

    // We will collect a max of 100 events for the day
    int i = 0;
    while(i < 100) {
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
            if (strlen(buf) > 0) valid_arg = 1;
            else printf("Invalid arg\n");
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
            if (dur > 0) valid_arg = 1;
            else printf("Invalid duration\n");
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
    if (string[len - 1] == '\n') string[len - 1] = '\0';
}

/*
 * Initialise a new event
 */
struct event *init_event(void) {
    struct event *new = malloc(sizeof(struct event));
    if (new == NULL) return NULL;
    new->start = 0;
    new->duration = 0;
    new->title = NULL;
    return new;
}

/*
 * Set the start time of an event
 */
void set_start(struct event *e, time_t start) {
    e->start = start;
}

/*
 * Set the duration of an event. Durations stored as seconds
 * but must be passed to this function as minutes
 */
void set_dur(struct event *e, int mins) {
    e->duration = mins * 60;
}

/*
 * Set the title of an event
 */
void set_title(struct event *e, char *title) {
    e->title = strdup(title);
}

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
    char * result = malloc(sizeof(char) * 17);
    sprintf(result, "%s%02d%02dT%sZ", year, month, day, time);

    return result;
}

/*
 * Convert a month string into a number, Jan = 1 and so on
 */
int get_month(char *str) {
    if (strcmp(str, "Jan") == 0) return 1;
    if (strcmp(str, "Feb") == 0) return 2;
    if (strcmp(str, "Mar") == 0) return 3;
    if (strcmp(str, "Apr") == 0) return 4;
    if (strcmp(str, "May") == 0) return 5;
    if (strcmp(str, "Jun") == 0) return 6;
    if (strcmp(str, "Jul") == 0) return 7;
    if (strcmp(str, "Aug") == 0) return 8;
    if (strcmp(str, "Sep") == 0) return 9;
    if (strcmp(str, "Oct") == 0) return 10;
    if (strcmp(str, "Nov") == 0) return 11;
    if (strcmp(str, "Dec") == 0) return 12;
    return 0;
}

struct calendar *new_cal(void) {
    struct calendar *new_cal = malloc(sizeof(struct calendar));
    new_cal->size = 0;
    return new_cal;
}

void write_cal(struct calendar *calendar) {
    srand((unsigned) time(NULL));
    char cal_name[50];
    snprintf(cal_name, 25, "calendar%d.ics", rand() % DAY);
    FILE *f = fopen(cal_name, "w");
    time_t t = get_start();
    int len = collect_events(calendar->events, t);
    if (len == 0) {
        printf("No events written.\n");
        exit(1);
    }
    calendar->size = len;
    printf("Writing %d events to %s...\n", calendar->size, cal_name);
    write_cal_head(f);
    for (int i = 0; i < len; i++) {
        write_event(f, calendar->events[i]);
    }
    write_cal_foot(f);

    printf("Finished writing events.\n");
    fclose(f);
}

void free_event(struct event *event) {
    free(event->title);
    free(event);
}

void free_cal(struct calendar *calendar) {
    for (int i = 0; i < calendar->size; i++) {
        free_event(calendar->events[i]);
    }
    free(calendar);
}

/*
 * Write header info for the calendar in ics format.
 * Also specifies timezone.
 */
void write_cal_head(FILE *out) {
    fprintf(out, "BEGIN:VCALENDAR\nMETHOD:PUBLISH\nVERSION:2.0\nX-WR-CALNAME:Planner\nPRODID:-//Apple Inc.//macOS 12.3.1//EN\nX-APPLE-CALENDAR-COLOR:#25FFA8\nX-WR-TIMEZONE:Australia/Sydney\nCALSCALE:GREGORIAN\nBEGIN:VTIMEZONE\nTZID:Australia/Sydney\nBEGIN:STANDARD\nTZOFFSETFROM:+1100\nRRULE:FREQ=YEARLY;BYMONTH=4;BYDAY=1SU\nDTSTART:20080406T030000\nTZNAME:AEST\nTZOFFSETTO:+1000\nEND:STANDARD\nBEGIN:DAYLIGHT\nTZOFFSETFROM:+1000\nRRULE:FREQ=YEARLY;BYMONTH=10;BYDAY=1SU\nDTSTART:20081005T020000\nTZNAME:AEDT\nTZOFFSETTO:+1100\nEND:DAYLIGHT\nEND:VTIMEZONE\n");
}

/*
 * Write the footer of a calendar, ends the calendar
 * in ics format
 */
void write_cal_foot(FILE *out) {
    fprintf(out, "END:VCALENDAR\n");
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
    srand((unsigned) time(NULL));
    t += rand() % event->duration;
    snprintf(uid, 25, "%d", (int)t);
}

void print_intro(void) {
    printf("Welcome to planter\nThe goal of this project is to make it easy to plan your day.\n\nEvents are created in sequence, you simply specify a title and duration.\n\nType 'q' in the event title to finish entering events.\n\n");
}
