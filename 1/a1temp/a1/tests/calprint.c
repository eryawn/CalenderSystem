/*
 * calprint.c
 *
 * Starter file provided to students for Assignment #1, SENG 265,
 * Summer 2019.
 * Name: Xuhui Wang  ID: V00913734
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
	char until[MAX_LINE_LEN];
	char summary[MAX_LINE_LEN];
	char location[MAX_LINE_LEN];
} event_t;

event_t entry[MAX_EVENTS];
int 	num_entries = 0;

/*
 *	Function cmp:
 *
 * A comparator for the built-in function qsort. Firstly the
 * function compares start date (a date that comes before another one
 * is the smaller one). When dates are equal, it compares summary.
 */

int cmp(const void *a, const void *b)
{
	if(strncmp((*(event_t *)a).dt_start, (*(event_t *)b).dt_start, MAX_LINE_LEN) == 0)
		return strncmp((*(event_t *)a).summary, (*(event_t *)b).summary, MAX_LINE_LEN);
	
	return strncmp((*(event_t *)a).dt_start, (*(event_t *)b).dt_start, MAX_LINE_LEN); 
}

/*
 *	Function dt_increment: provided by the instructor
 *
 * Given a date-time, it adds the number of days in a way that
 * results in the correct year, month, and day. For example,
 * if the string in "before" corresponds to:
 *	20190520T111500
 *
 * then the datetime string stored in "after", assuming that
 * "num_days" is 100, will be:
 * 	20190828T111500, which is 100 days after May 20, 2019.
 */
  
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

/*
 *	Function dt_format: provided by the instructor
 *
 * Given a date-time, creates a more readable version of the
 * calendar date by using some C-library routines. For example,
 * if the string in "time" corresponds to:
 * 	20190520T111500
 *
 * then the string stored at "formatted_time" is:
 * 	May 20, 2019 (Mon).
 */
 
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

/*
 *	Function time_format:
 *
 * Given a time stamp, creates a more readable version of the
 * time. For example, if the string in "time" corresponds to:
 * 	20190520T111500
 *
 * then the string stored at "formatted_time" is:
 *	11:15 am.
 */
 
void time_format(char *formatted_time, char *time){

	int hour_i, min_i, hour_i_12f;
	char hour_str[3], min_str[3];
	
	//slice the 4 bits that indicate hour and minute in time stamp
	hour_str[0] = time[9];
	hour_str[1] = time[10];
	min_str[0] = time[11];
	min_str[1] = time	[12];	
	hour_i = atoi(hour_str);
	min_i = atoi(min_str);
	
	if(hour_i >= 13)
		hour_i_12f = hour_i % 12;	//transfer hour to 12-hour format
	else
		hour_i_12f = hour_i;

	if(hour_i >= 12)	//add am or pm to the formatted time
		sprintf(formatted_time, "%2d:%02d pm", hour_i_12f, min_i);
	else
		sprintf(formatted_time, "%2d:%02d am", hour_i, min_i);
}

/*
 *	Function day_header_format:
 *
 * Given a start date, creates a more readable version of the
 * date, including underline and blank line. For example,
 * if the string in "dt_start" corresponds to:
 *	20190506T100000
 * 
 * then the string stored at "formatted_day" is:
 *	May 06, 2019 (Mon)
 *	------------------
 */

void day_header_format(char *day_header, char *dt_start, int first_in_all, int first_in_day){
	
	 if(first_in_all == 0 && first_in_day == 0){//no blank line and day header between events within the same day
		sprintf(day_header, "");				//if the event is neither first event in all nor first event on this day
		return;
	}
	
	char underline[MAX_LINE_LEN], day[MAX_LINE_LEN];
	int j;
	dt_format(day, dt_start, MAX_LINE_LEN);
			
	for(j = 0; j < strlen(day) + 1; j++){//create the underline that matches the length of the day
		if(j == strlen(day))	
			underline[j] = '\0';
		else
			underline[j] = '-';
	}

	if(first_in_all == 1 && first_in_day == 1)	//no blank line at the start of the output
		sprintf(day_header ,"%s\n%s\n", day, underline); //combine day and underline into one day header

	else{
		if(first_in_day == 1)	//different days in output are separated by a single blank line
			sprintf(day_header, "\n%s\n%s\n", day, underline);
	}
}

/*
 *	Function get_events:
 *
 * Given a file pointer, read the text input from a file  
 * line by line, and create the struct "event_t" with those
 * information. Every string in event's data attribute is
 * clean with prefix and postfix stripped off. For example, 
 * 	dt_start: 20190506T100000
 *	dt_end:   20190506T103000
 *	until: 	  20190806T235959
 *	summary:  MATH 101 tutorial
 *	location: CLE A308
 */

void get_events(char *file){
		
	FILE *file_pointer;
	file_pointer = fopen(file,"r");
	char *p = NULL;
	char str[MAX_LINE_LEN], temp_a, temp_b;
	
	p = fgets(str, MAX_LINE_LEN,file_pointer);	//read one line into str
		while(strncmp(str, "END:VCALENDAR", 13) != 0){	//"END:VCALENDAR" indicates the last line of the text
		
			while(strncmp(str, "END:VEVENT", 10) != 0){	//"END:VEVENT" indicates the last line of an event
			
				if (strncmp(str, "DTSTART:", 8) == 0)//write the start date into event and strip the prefix string off
					sscanf(str,"DTSTART:%s", &entry[num_entries].dt_start);
				
				else if (strncmp(str, "DTEND:", 6) == 0)//write the end date into event and strip the prefix string off
					sscanf(str,"DTEND:%s", &entry[num_entries].dt_end);

				else if (strncmp(str, "LOCATION:", 9) == 0){//write the location into event and strip the prefix and postfix string off
					str[strlen(str) - 1] = '\0';
					char *p_location = str + 9;
					strncpy(entry[num_entries].location, p_location, MAX_LINE_LEN);
				}
				else if (strncmp(str, "SUMMARY:", 8) == 0) {//write the summary into event and strip the prefix and postfix string off
					str[strlen(str) - 1] = '\0';
					char *p_summary = str + 8;
					strncpy(entry[num_entries].summary, p_summary, MAX_LINE_LEN);
				}
				else if (strncmp(str, "RRULE:", 6) == 0)//write the until date into event and strip the prefix and postfix string off
					sscanf(str,"RRULE:FREQ=WEEKLY;WKST=MO;UNTIL=%s;BYDAY=[A-Z][A-Z]", 
								&entry[num_entries].until, &temp_a, &temp_b);
											
				p = fgets(str, MAX_LINE_LEN,file_pointer);
			}

			p = fgets(str, MAX_LINE_LEN,file_pointer);
			num_entries += 1;
		}	
	fclose(file_pointer);
}

/*
 *	Function update_events:
 *
 * Update the start dates of some repeated events, to ensure 
 * that the events come after the start date in program arguments.
 * For example, the start date of an event is 
 *	June 12, 2019 (Wed),
 *	The start date in program arguments is
 *	June 17, 2019 (Mon).
 *	This function updates the start date of the event to
 *	June 19, 2019 (Wed).
 */
 
void update_events(char *start){

	int i;
	char temp[MAX_LINE_LEN];
	for(i = 0; i < num_entries; i++){
		
		if(strncmp(entry[i].until, "20", 2) == 0){	//if that is a repeated event
			while(strncmp(entry[i].dt_start, start, MAX_LINE_LEN) < 0){
				dt_increment(temp, entry[i].dt_start, 7);
				strncpy(entry[i].dt_start, temp, MAX_LINE_LEN);//update the start date
			}
		}
	} 
}

/*
 *	Function print_events:
 *
 * Given a start date and an end date in program arguments,  
 * output all events within the ics file occurring during the 
 * specified range of dates. For example, if the argument is 
 * --from=12/6/2019 --to=12/6/2019 --file=prudence.ics,
 * the output is
 * 	June 12, 2019 (Wed)
 * 	-------------------
 * 	10:00 am to 12:15 pm: ENGL 146 [CLE A314]
 * 	2:30 pm to  3:30 pm: MATH 101 [ECS 123]
 */

void print_events(char *start, char *end){
	
	int i, first_in_all;
	char day_header[MAX_LINE_LEN], temp[MAX_LINE_LEN], time_start[MAX_LINE_LEN], time_end[MAX_LINE_LEN];
	first_in_all = 1;	//next event is the one that comes first in all events

	while (strncmp(start, end, 8) <= 0){//keep outputing events if the start date is smaller than the end date	
		int first_in_day = 1;	//next event is the one that comes first on this day
		qsort(entry, num_entries, sizeof(event_t), cmp);//sort events in chronological order at the beginning of this day
		
		for(i = 0; i < num_entries; i++){	
					
			if(strncmp(entry[i].dt_start, start, 8) == 0){//prepare to output if the event happens in this day			
				time_format(time_start, entry[i].dt_start);	//format the start time and the end time
				time_format(time_end, entry[i].dt_end);
				//format the day header in order to print if there should be one
				day_header_format(day_header, entry[i].dt_start, first_in_all, first_in_day);
				first_in_all = 0;
				
     			if(strncmp(entry[i].until, "20", 2) != 0){//output the unrepeated event only once
					printf(day_header);	//output the day header	
					printf("%s to %s: %s [%s]\n", &time_start, &time_end, &entry[i].summary, &entry[i].location);//output the event
					first_in_day = 0;
				}
				
				else if(strncmp(start, entry[i].until, 8) <= 0){//output the repeated event that does not exceed the until day
 					printf(day_header);	//output the day header
					printf("%s to %s: %s [%s]\n", &time_start, &time_end, &entry[i].summary, &entry[i].location);//output the event
					first_in_day = 0;
					dt_increment(temp, entry[i].dt_start, 7);	//add 7 days to the start day of the repeated event 
					strncpy(entry[i].dt_start, temp, MAX_LINE_LEN);
				}
			}	
		}
		dt_increment(temp, start, 1);//the next day begins
		strncpy(start, temp, MAX_LINE_LEN);
	}
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
	
	char start[MAX_LINE_LEN], end[MAX_LINE_LEN];
	sprintf(start, "%d%02d%02dT000000", from_y, from_m, from_d);//transfer start and end date into string
	sprintf(end, "%d%02d%02dT235959", to_y, to_m, to_d);	
	
	get_events(filename);
	update_events(start);
	print_events(start, end);

	exit(0);
}