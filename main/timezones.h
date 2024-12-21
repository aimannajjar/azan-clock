#ifndef TIMEZONES_H
#define TIMEZONES_H

// Structure for timezone entries
typedef struct {
    const char *timezone_id;
    int index;
} TimezoneEntry;

// Function to get the index of a timezone
int get_timezone_index(const char *timezone_id);
const char* get_timezone_posix(const char *timezone_id);

// External declaration of the timezone map array for use in other files if needed
extern TimezoneEntry timezone_map[];

#endif // TIMEZONES_H
