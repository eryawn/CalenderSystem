/*
 * calprint4.c
 *
 * Starter file provided to students for Assignment #4, SENG 265,
 * Summer 2019.
 * Xuhui Wang  ID: V00913734
 */

#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "emalloc.h"
#include "ics.h"
#include "listy.h"

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

node_t * get_events(char *filename) {
	node_t *head = NULL;
	char   *line = NULL;
	size_t len = 0;
	FILE   *fp = fopen(filename,"r");
	getline(&line, &len, fp);
	
	/*Keep extracting the file until we meet the last line of the file.*/
	while (strncmp(line, "END:VCALENDAR", 13) != 0){
		event_t *temp_event = (event_t *)emalloc(sizeof(event_t));
		*temp_event->rrule = 0;
		while (strncmp(line, "END:VEVENT", 10) != 0){ /*last line of event*/
			if (strncmp(line, "DTSTART:", 8) == 0){
				strncpy(temp_event->dtstart, line + 8, 15);
				*(temp_event->dtstart + 15) = 0;
			}	
			else if (strncmp(line, "DTEND:", 6) == 0){
				strncpy(temp_event->dtend, line + 6, 15);
				*(temp_event->dtend + 15) = 0;
			}		
			else if (strncmp(line, "LOCATION:", 9) == 0){
				strncpy(temp_event->location, line + 9, strlen(line) - 10);
				*(temp_event->location + strlen(line) - 10) = 0;
			}
			else if (strncmp(line, "SUMMARY:", 8) == 0){
				strncpy(temp_event->summary, line + 8, strlen(line) - 9);
				*(temp_event->summary + strlen(line) - 9) = 0;
			}
			else if (strncmp(line, "RRULE:", 6) == 0){
				strncpy(temp_event->rrule, line + 32, 15);
				*(temp_event->rrule + 15) = 0;
			}	
			getline(&line, &len, fp);
		}
		node_t *temp_node = new_node(temp_event);
		head = add_end(head, temp_node); /*insert node into list*/
		getline(&line, &len, fp);	  
	}
	free(line);
	fclose(fp);	
	return head;
}



ssize_t getline(char** line,size_t *n,FILE *fp)
{
    char *buf = *line;
    ssize_t c,i=0;//i来记录字符串长度，c来存储字符
    if(buf==NULL||*n==0)
    {
        *line = malloc(10);
        buf = *line;
        *n = 10;
    }
    //buf为或n为0时动态为期分配空间
    while((c=fgetc(fp))!='\n')
    {
        if(c==EOF)
            return -1;
        if(i<*n-2)//留2个空间给\n和\0
        {
            *(buf+i++)=c;
        }
        else
        {
            *n = *n+10;
            buf = realloc(buf,*n);//空间不足时，重新进行分配
            *(buf+i++)=c;
        }
    }
    *(buf+i++)='\n';
    *(buf+i)='\0';
    return i;
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

void dt_increment(char *after, const char *before, int const num_days){
	struct tm temp_time;
	time_t    full_time;

	memset(&temp_time, 0, sizeof(struct tm));
	sscanf(before, "%4d%2d%2d", &temp_time.tm_year,
		&temp_time.tm_mon, &temp_time.tm_mday);
	temp_time.tm_year -= 1900;
	temp_time.tm_mon -= 1;
	temp_time.tm_mday += num_days;

	full_time = mktime(&temp_time);
	after[0] = '\0';

	strftime(after, 9, "%Y%m%d", localtime(&full_time));
	strncpy(after + 8, before + 8, 80 - 8); 
	after[80 - 1] = '\0';
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

void dt_format(char *formatted_time, const char *dt_time, const int len){
	struct tm temp_time;
	time_t    full_time;
 
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
	char *hour_str = (char *)emalloc(sizeof(char) * 3);
	char *min_str = (char *)emalloc(sizeof(char) * 3);
	
	/*Slice the 4 bits that indicate hour and minute in time stamp.*/
	strncpy(hour_str, time + 9, 2);
	*(hour_str + 2) = 0;
	strncpy(min_str, time + 11, 2);
	*(min_str + 2) = 0;

	hour_i = atoi(hour_str);
	min_i = atoi(min_str);	
	if (hour_i >= 13)
		hour_i_12f = hour_i % 12; /*transfer hour to 12-hour format*/
	else
		hour_i_12f = hour_i;

	if (hour_i >= 12)	/*add am or pm to the formatted time*/
		sprintf(formatted_time, "%2d:%02d pm", hour_i_12f, min_i);
	else
		sprintf(formatted_time, "%2d:%02d am", hour_i, min_i);
	free(hour_str);
	free(min_str);
}

/*
 *	Function day_header_format:
 *
 * Given a start date, creates a more readable version of the
 * date, including underline and blank line. For example,
 * if the string in "dt_start" corresponds to:
 *	20190506T100000
 * 
 * then the string stored at "day_header" is:
 *	May 06, 2019 (Mon)
 *	------------------
 */

void day_header_format(char *day_header, char *dt_start, int first_in_all){
	char *underline = (char *)emalloc(sizeof(char) * 80);
	char *day = (char *)emalloc(sizeof(char) * 80);
	int j = 0;
	dt_format(day, dt_start, 80);
	
	/*Create the underline that matches the length of the day.*/
	for (j = 0; j < strlen(day) + 1; j++){
		if (j == strlen(day))	
			underline[j] = '\0';
		else
			underline[j] = '-';
	}

	if (first_in_all == 1)	/*no blank line*/
		sprintf(day_header ,"%s\n%s\n", day, underline);
	else 
		sprintf(day_header, "\n%s\n%s\n", day, underline);
	free(underline);
	free(day);
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

void update_events(char *start, node_t *head){
	char *temp = (char *)emalloc(sizeof(char) * 80);
	node_t *node = head;
	
	while(node) {		
		if (*node->val->rrule){	/*a repeated event*/
			while (strncmp(node->val->dtstart, start, 15) < 0){
				dt_increment(temp, node->val->dtstart, 7);
				strncpy(node->val->dtstart, temp, 15);/*update*/
			}			
		}
		node = node->next;
	}
	free(temp);
}

/*
 *	Function cmp:
 *
 * A comparator for the function sort. Firstly the
 * function compares start date (a date that comes before another one
 * is the smaller one). When dates are equal, it compares summary.
 */
 
int cmp(node_t *a, node_t *b){
	if (strncmp(a->val->dtstart, b->val->dtstart, 15) == 0)
		return strncmp(a->val->summary, b->val->summary, 80);
	return strncmp(a->val->dtstart, b->val->dtstart, 15);
} 

/*
 *	Function sort:
 *
 * In order to create as less events as possible and save space, I 
 * sort the list everyday. Bubble sort is an easy way at this point.
 * Instead of swapping nodes, I swap values of adjacent nodes.
 */

/* void sort(node_t *head){
	node_t *node = head;
	while (node){
		node_t *current = head;
		while (current->next){
			if (cmp(current, current->next) > 0){
				event_t *temp = current->next->val;
				current->next->val = current->val; /*swap events*/
				current->val = temp;
			}
			current = current->next;    
		}
		node = node->next;
	}
} */

/*
 *	Function print_events:
 *
 * Given a start date as parameter,  
 * output all events within the ics file occurring during the 
 * specified day. For example, if the parameter is 
 * 	20190612T000000, the output is
 * 	June 12, 2019 (Wed)
 * 	-------------------
 * 	10:00 am to 12:15 pm: ENGL 146 [CLE A314]
 * 	2:30 pm to  3:30 pm: MATH 101 [ECS 123]
 */

int print_events(node_t *node, char *start, int first_in_all){
	
	char *day_header = (char *)emalloc(sizeof(char) * 80);	
	char *time_start = (char *)emalloc(sizeof(char) * 9);
	char *time_end = (char *)emalloc(sizeof(char) * 9);	
	int first_in_day = 1; /*the first one in all events*/
	
	while (node){
		/*The event happens in this day.*/
		if (strncmp(node->val->dtstart, start, 8) == 0){
			
			/*The event is an unrepeated event or a repeated event 
			  that does not exceed the rrule day.
			*/
			if (*node->val->rrule == 0 || 
				strncmp(start, node->val->rrule, 8) <= 0){
				
				/*Format the start time and the end time.*/
				time_format(time_start, node->val->dtstart);	
				time_format(time_end, node->val->dtend);
				if (first_in_day == 1){ /*format the day header*/
					day_header_format(day_header, node->val->dtstart,					
						first_in_all);
					printf(day_header);
				}
				printf("%s to %s: %s [%s]\n", time_start, time_end, 
					node->val->summary, node->val->location);
				first_in_all = 0;
				first_in_day = 0;
			}
		}
		node = node->next;
	}	
	
	free(day_header);
	free(time_start);
	free(time_end);
	return first_in_all;
}

/*
 *	Function print_calendar:
 *
 * Given a start date and an end date as the parameters,  
 * output all events within the ics file occurring during the 
 * specified range of dates. For example, if the argument is 
 * --from=12/6/2019 --to=12/6/2019 --file=prudence.ics,
 * the output is
 * 	June 12, 2019 (Wed)
 * 	-------------------
 * 	10:00 am to 12:15 pm: ENGL 146 [CLE A314]
 * 	2:30 pm to  3:30 pm: MATH 101 [ECS 123]
 */

void print_calendar(node_t *head, char *start, char *end){	
	int  first_in_all = 1;
	char *temp = (char *)emalloc(sizeof(char) * 80);
	
	while (strncmp(start, end, 17) <= 0){
		update_events(start, head);
		//sort(head);	
		first_in_all = print_events(head, start, first_in_all);		
		dt_increment(temp, start, 1);/*the next day begins*/
		strncpy(start, temp, 15);
	}
	free(temp);
}

int main(int argc, char *argv[])
{
	int from_y = 0, from_m = 0, from_d = 0;
	int to_y = 0, to_m = 0, to_d = 0;
	char *filename = NULL;
	int i; 

	for (i = 0; i < argc; i++) {
		if (strncmp(argv[i], "--start=", 7) == 0) {
			sscanf(argv[i], "--start=%d/%d/%d", &from_d, &from_m, &from_y);
		} else if (strncmp(argv[i], "--end=", 5) == 0) {
			sscanf(argv[i], "--end=%d/%d/%d", &to_d, &to_m, &to_y);
		} else if (strncmp(argv[i], "--file=", 7) == 0) {
			filename = argv[i]+7;
		}
	}
	if (from_y == 0 || to_y == 0 || filename == NULL) {
		fprintf(stderr, 
			"usage: %s --start=dd/mm/yyyy --end=dd/mm/yyyy --file=icsfile\n",
			argv[0]);
		exit(1);
	}
    
	char *start = malloc(sizeof(char) * 17);
	char *end = malloc(sizeof(char) * 17);
	sprintf(start, "%d%02d%02dT000000", from_y, from_m, from_d);
	sprintf(end, "%d%02d%02dT235959", to_y, to_m, to_d);	  
	node_t *head = get_events(filename);		
	print_calendar(head, start, end);	
	
	/*Free the memory allocated dynamically.*/
	node_t *temp = NULL;
	while (head){
		temp = head->next;
		free(head->val);
		free(head);
		head = temp;
	}
	free(start);
	free(end);
	exit(0);
}
