#include <time.h>

#define DAY 86400
#define TZ 60*60*10

struct event {
    time_t start;
    int duration;
    char *title;
};

struct calendar {
    struct event *events[100];
    int size;
};


int collect_events(struct event *events[100], time_t start);
void trim(char *string);

char *substring(char *str, int start, int end);
void get_event_uid(struct event *event, char *uid);
void write_event(FILE *out, struct event *event);
void write_cal_foot(FILE *out);
void write_cal_head(FILE *out);
time_t get_start(void);
int ind_of(char *str, char a);
struct event *init_event(void);
void set_start(struct event *e, time_t start);
void set_dur(struct event *e, int mins);
void set_title(struct event *e, char *title);
struct calendar *new_cal(void);
void write_cal(struct calendar *calendar);
void print_intro(void);

void free_cal(struct calendar *calendar);
void free_event(struct event *event);

char * get_tz(time_t t);
int get_month(char *str);

void print_event(struct event *event);
void write_cal_head(FILE *out);
