#ifndef TIMEZONES_H
#define TIMEZONES_H

typedef struct {
    const char *timezone_id;
    const char *posix_value;
} TimezoneEntry;

// Function to get the index of a timezone
int get_timezone_posix(const char *timezone_id, const char* ret_timezone_posix);
void get_timezone_posix_from_index(int index, const char* ret_timezone_posix);

#endif // TIMEZONES_H
