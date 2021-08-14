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
    char dt_end[MAX_LINE_LEN];
    char summary[MAX_LINE_LEN];
    char location[MAX_LINE_LEN];
    char dt_repeat_until[MAX_LINE_LEN];
	int  until;
	int  day;
	int  hour_start;
	int  hour_end;
} event_t;

event_t entry[MAX_EVENTS];
int 	num_entries = 0;

int cmp(const void *a, const void *b){
	if((*(event_t *)a).day == (*(event_t *)b).day){
		if((*(event_t *)a).hour_start == (*(event_t *)b).hour_start){
			return strncmp((*(event_t *)a).summary, (*(event_t *)b).summary, 15);
		}
		return (*(event_t *)a).hour_start - (*(event_t *)b).hour_start;
	}	
	return (*(event_t *)a).day - (*(event_t *)b).day;
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

void time_format(void *b, int time){
	
    char a[100];
	int time_a;
	
	if(time >= 130000)
		time_a = (time - 120000) / 100;
	else
		time_a = time / 100;
	int hour, min;
	hour = time_a / 100;
	min = time_a - 100 * hour;
	
	if(min < 10 && hour < 10)
		sprintf(a," %d:0%d", hour, min);
	else if(hour < 10)
		sprintf(a," %d:%d", hour, min);
	else if(min < 10)
		sprintf(a,"%d:0%d", hour, min);
	else
		sprintf(a,"%d:%d", hour, min);

	if(time >= 120000)
		sprintf(b,"%s pm", a);
	else
		sprintf(b,"%s am", a);
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
	
	FILE *filepoint;
	filepoint = fopen(filename,"r");							//filename is a pointer but can be assigned to an char array variable
	if(filepoint == NULL)
		printf("Failed to open the file!\n");

	char *p;
	char str1[MAX_LINE_LEN];
	
	p = fgets(str1, MAX_LINE_LEN,filepoint);
		while(strncmp(str1, "END:VCALENDAR", 13) != 0){	
		
			while(strncmp(str1, "END:VEVENT", 10) != 0){	
			
				if (strncmp(str1, "RRULE:", 6) == 0) {
					int num;
					char word[MAX_LINE_LEN];
					char word1[MAX_LINE_LEN];
					strncpy(entry[num_entries].dt_repeat_until, str1, MAX_LINE_LEN);
					sscanf(str1,"RRULE:FREQ=WEEKLY;WKST=MO;UNTIL=%dT%d;BYDAY=[A-Z][A-Z]", &entry[num_entries].until, &num, &word, &word1);
					entry[num_entries].until += 1;
				}
				else if (strncmp(str1, "DTSTART:", 8) == 0) {
					sscanf(str1,"DTSTART:%s", &entry[num_entries].dt_start);
					sscanf(str1,"DTSTART:%dT%d", &entry[num_entries].day, &entry[num_entries].hour_start);
				}
				else if (strncmp(str1, "DTEND:", 6) == 0) {
					int num;
					sscanf(str1,"DTEND:%s", &entry[num_entries].dt_end);
					sscanf(str1,"DTEND:%dT%d", &num, &entry[num_entries].hour_end);
				}
				else if (strncmp(str1, "LOCATION:", 9) == 0) {
					str1[strlen(str1) - 1] = '\0';
					char *p_location = str1 + 9;
					strncpy(entry[num_entries].location, p_location, MAX_LINE_LEN);
				}
				else if (strncmp(str1, "SUMMARY:", 8) == 0) {
					str1[strlen(str1) - 1] = '\0';
					char *p_summary = str1 + 8;
					strncpy(entry[num_entries].summary, p_summary, MAX_LINE_LEN);
				}
				p = fgets(str1, MAX_LINE_LEN,filepoint);
			}
			if (strncmp(str1, "END:VEVENT", 10) == 0) {
				if(strncmp(entry[num_entries].dt_repeat_until, "RRULE:", 6) != 0){
					strncpy(entry[num_entries].dt_repeat_until, "N\n", MAX_LINE_LEN);
				}
			}
			p = fgets(str1, MAX_LINE_LEN,filepoint);
			num_entries += 1;
		}	
	fclose(filepoint);
	int start = from_y * 10000 + from_m * 100 + from_d;
	int end = to_y * 10000 + to_m * 100 + to_d;;
	
	for(i = 0; i < num_entries; i++){
		
		if(strncmp(entry[i].dt_repeat_until, "RRULE:", 6) == 0){
			char time_C[MAX_LINE_LEN];
			char time_D[MAX_LINE_LEN];
						
			while(entry[i].day < start){
				dt_increment(time_C, entry[i].dt_start, 7);
				strncpy(entry[i].dt_start, time_C, MAX_LINE_LEN);
						
				dt_increment(time_D, entry[i].dt_end, 7);
				strncpy(entry[i].dt_end, time_D, MAX_LINE_LEN);
						
				sscanf(entry[i].dt_start,"%dT%d", &entry[i].day, &entry[i].hour_start);
			}
		}
	}
	
	
	int k;
	int first_in_all = 1;
 	while (start <= end){		
		
		int first_in_day = 1;
		qsort(entry, num_entries, sizeof(event_t), cmp);	
		for(k = 0; k < num_entries; k++){		
			if(entry[k].day == start){			
				char time_E[100];
				char output[100];
				strncpy(time_E, entry[k].dt_start, MAX_LINE_LEN);
				dt_format(output, time_E, MAX_LINE_LEN);
				
				int j;
				char temp[MAX_LINE_LEN];
				for(j = 0; j < strlen(output) + 1; j++){
					if(j == strlen(output))
						temp[j] = '\0';
					else
						temp[j] = '-';
				}
				
				if(strncmp(entry[k].dt_repeat_until, "N", 1) == 0){ //for those events that are not repeated
						
					if(first_in_all == 1 && first_in_day == 1)
						printf("%s\n%s\n", output, temp);
					else{
						if(first_in_day == 1)
							printf("\n%s\n%s\n", output, temp);
					}
						
					first_in_all = 0;
					first_in_day = 0;
					char time_a[MAX_LINE_LEN];
					char time_b[MAX_LINE_LEN];
					time_format(time_a, entry[k].hour_start);
					time_format(time_b, entry[k].hour_end);
					printf("%s to %s: %s [%s]\n", &time_a, &time_b, &entry[k].summary, &entry[k].location);
				}
				if(strncmp(entry[k].dt_repeat_until, "RRULE:", 6) == 0 && start < entry[k].until){ //events that are repeated
						
					if(first_in_all == 1 && first_in_day == 1)
						printf("%s\n%s\n", output, temp);
					else{
						if(first_in_day == 1)
							printf("\n%s\n%s\n", output, temp);
					}
						
					first_in_all = 0;
					first_in_day = 0;
					char time_a[MAX_LINE_LEN];
					char time_b[MAX_LINE_LEN];
					time_format(time_a, entry[k].hour_start);
					time_format(time_b, entry[k].hour_end);

					printf("%s to %s: %s [%s]\n", &time_a, &time_b, &entry[k].summary, &entry[k].location);
						
					char time_B[MAX_LINE_LEN];
					char time_A[MAX_LINE_LEN];
						
					dt_increment(time_A, entry[k].dt_start, 7);
					strncpy(entry[k].dt_start, time_A, MAX_LINE_LEN);
						
					dt_increment(time_B, entry[k].dt_end, 7);
					strncpy(entry[k].dt_end, time_B, MAX_LINE_LEN);
						
						
					sscanf(entry[k].dt_start,"%dT%d", &entry[k].day, &entry[k].hour_start);
				}
			}	
		}
	start += 1;
	}	
	
    exit(0);
}
