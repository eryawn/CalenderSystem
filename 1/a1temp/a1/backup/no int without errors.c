/*
 * calprint.c
 *
 * Starter file provided to students for Assignment #1, SENG 265,
 * Summer 2019.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 80
#define MAX_EVENTS 500

typedef struct event_t
{
    char dt_start[MAX_LINE_LEN];
	char  until[MAX_LINE_LEN];
    char summary[MAX_LINE_LEN];
    char location[MAX_LINE_LEN];
	int  hour_start;
	int  hour_end;
	int day;
} event_t;

event_t entry[MAX_EVENTS];
int 	num_entries = 0;

int cmp(const void *a, const void *b)
{
	if(strncmp((*(event_t *)a).dt_start, (*(event_t *)b).dt_start, MAX_LINE_LEN) == 0)
		return strncmp((*(event_t *)a).summary, (*(event_t *)b).summary, 15);
	return strncmp((*(event_t *)a).dt_start, (*(event_t *)b).dt_start, MAX_LINE_LEN); 
}

void dt_format(char *formatted_time, const char *dt_time, const int len)
{
    struct tm temp_time;
    time_t    full_time;
    char      temp[5];
 
    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(dt_time, "%4d%2d%2d",
        &temp_time.tm_year, &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    full_time = mktime(&temp_time);
    strftime(formatted_time, len, "%B %d, %Y (%a)", 
        localtime(&full_time));
}

void dt_increment(char *after, const char *before, int const num_days)
{
    struct tm temp_time, *p_temp_time;
    time_t    full_time;
    char      temp[5];

    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(before, "%4d%2d%2d", &temp_time.tm_year,
        &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    temp_time.tm_mday += num_days;

    full_time = mktime(&temp_time);
    after[0] = '\0';

    strftime(after, 9, "%Y%m%d", localtime(&full_time));
    strncpy(after + 8, before + 8, MAX_LINE_LEN - 8); 
    after[MAX_LINE_LEN - 1] = '\0';
}

void time_format(void *after, int time){
	
    char temp[MAX_LINE_LEN];
	int before;
	
	if(time >= 130000)
		before = (time - 120000) / 100;
	else
		before = time / 100;
	int hour, min;
	hour = before / 100;
	min = before - 100 * hour;
	
	if(min < 10 && hour < 10)
		sprintf(temp," %d:0%d", hour, min);
	else if(hour < 10)
		sprintf(temp," %d:%d", hour, min);
	else if(min < 10)
		sprintf(temp,"%d:0%d", hour, min);
	else
		sprintf(temp,"%d:%d", hour, min);

	if(time >= 120000)
		sprintf(after,"%s pm", temp);
	else
		sprintf(after,"%s am", temp);
}

int main(int argc, char *argv[])
{
    int from_y = 0, from_m = 0, from_d = 0;
    int to_y = 0, to_m = 0, to_d = 0;
    char *filename = NULL;
    int i; 

    for (i = 0; i < argc; i++) {
        if (strncmp(argv[i], "--from=", 7) == 0)
            sscanf(argv[i], "--from=%d/%d/%d", &from_d, &from_m, &from_y);
        else if (strncmp(argv[i], "--to=", 5) == 0)
            sscanf(argv[i], "--to=%d/%d/%d", &to_d, &to_m, &to_y);
        else if (strncmp(argv[i], "--file=", 7) == 0)
            filename = argv[i] + 7;
    }

    if (from_y == 0 || to_y == 0 || filename == NULL) {
        fprintf(stderr, 
            "usage: %s --from=dd/mm/yyyy --to=dd/mm/yyyy --file=icsfile\n",
            argv[0]);
			(1);
    }
	
	FILE *file_pointer;
	file_pointer = fopen(filename,"r");							//filename is a pointer but can be assigned to an char array variable
	if(file_pointer == NULL)
		printf("Failed to open the file!\n");

	char *p = NULL;
	char str[MAX_LINE_LEN];
	int  temp_num;
	char temp_char_a;
	char temp_char_b;
	p = fgets(str, MAX_LINE_LEN,file_pointer);
		while(strncmp(str, "END:VCALENDAR", 13) != 0){	
		
			while(strncmp(str, "END:VEVENT", 10) != 0){	
			
				if (strncmp(str, "DTSTART:", 8) == 0) {
					sscanf(str,"DTSTART:%s", &entry[num_entries].dt_start);
					sscanf(str,"DTSTART:%dT%d", &temp_num, &entry[num_entries].hour_start);
				}
				else if (strncmp(str, "DTEND:", 6) == 0) {
					sscanf(str,"DTEND:%dT%d", &temp_num, &entry[num_entries].hour_end);
				}
				else if (strncmp(str, "LOCATION:", 9) == 0) {
					str[strlen(str) - 1] = '\0';
					char *p_location = str + 9;
					strncpy(entry[num_entries].location, p_location, MAX_LINE_LEN);
				}
				else if (strncmp(str, "SUMMARY:", 8) == 0) {
					str[strlen(str) - 1] = '\0';
					char *p_summary = str + 8;
					strncpy(entry[num_entries].summary, p_summary, MAX_LINE_LEN);
				}
				else if (strncmp(str, "RRULE:", 6) == 0) {
					sscanf(str,"RRULE:FREQ=WEEKLY;WKST=MO;UNTIL=%s;BYDAY=[A-Z][A-Z]", 
											&entry[num_entries].until, &temp_char_a, &temp_char_b);
				}
				p = fgets(str, MAX_LINE_LEN,file_pointer);
			}

			p = fgets(str, MAX_LINE_LEN,file_pointer);
			num_entries += 1;
		}	
	fclose(file_pointer);
	
	char start[MAX_LINE_LEN];
	sprintf(start, "%d%02d%02dT000000", from_y, from_m, from_d);
	//printf(start);
	char end[MAX_LINE_LEN];
	sprintf(end, "%d%02d%02dT235959", to_y, to_m, to_d);	
	
	char temp[MAX_LINE_LEN];
  	for(i = 0; i < num_entries; i++){
		if(strncmp(entry[i].until, "2019", 4) == 0){
			
			while(strncmp(entry[i].dt_start, start, MAX_LINE_LEN) < 0){
				dt_increment(temp, entry[i].dt_start, 7);
				strncpy(entry[i].dt_start, temp, MAX_LINE_LEN);
			}
		}
	} 
	
 	int k;
	char date[MAX_LINE_LEN];
	char underline[MAX_LINE_LEN];
	char time_str_start[MAX_LINE_LEN];
	char time_str_end[MAX_LINE_LEN];
	int first_in_all = 1;
	char temp1[MAX_LINE_LEN];
	while (strncmp(start, end, 8) <= 0){		
	
		int first_in_day = 1;
		qsort(entry, num_entries, sizeof(event_t), cmp);
		
		for(k = 0; k < num_entries; k++){	
					
			if(strncmp(entry[k].dt_start, start, 8) == 0){
				
				dt_format(date, entry[k].dt_start, MAX_LINE_LEN);
				int j;
				
				for(j = 0; j < strlen(date) + 1; j++){
					if(j == strlen(date))
						underline[j] = '\0';
					else
						underline[j] = '-';
				}

				if(strncmp(entry[k].until, "2019", 4) != 0){
					if(first_in_all == 1 && first_in_day == 1)
						printf("%s\n%s\n", date, underline);
					else{
						if(first_in_day == 1)
							printf("\n%s\n%s\n", date, underline);
					}
						
					first_in_all = 0;
					first_in_day = 0;
					time_format(time_str_start, entry[k].hour_start);
					time_format(time_str_end, entry[k].hour_end);
					printf("%s to %s: %s [%s]\n", &time_str_start, &time_str_end, &entry[k].summary, &entry[k].location);
				}
		
				else if(strncmp(start, entry[k].until, MAX_LINE_LEN) <= 0){	

					if(first_in_all == 1 && first_in_day == 1)
						printf("%s\n%s\n", date, underline);
					else{
						if(first_in_day == 1)
							printf("\n%s\n%s\n", date, underline);
					}
						
					first_in_all = 0;
					first_in_day = 0;

					time_format(time_str_start, entry[k].hour_start);
					time_format(time_str_end, entry[k].hour_end);

					printf("%s to %s: %s [%s]\n", &time_str_start, &time_str_end, &entry[k].summary, &entry[k].location);
					
					dt_increment(temp1, entry[k].dt_start, 7);
					strncpy(entry[k].dt_start, temp1, MAX_LINE_LEN);
					sscanf(entry[k].dt_start,"%dT%d", &temp_num, &entry[k].hour_start);
				}
			}	
		}
		dt_increment(temp1, start, 1);
		strncpy(start, temp1, MAX_LINE_LEN);
	}
	
    exit(0);
}
