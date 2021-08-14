#!/usr/bin/env python3

import sys
import argparse
import datetime
# The code below configures the argparse module for use with
# assignment #2.
# Xuhui Wang/Eric Wang V00913734

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--file', type=str, help='file to be processed')
    parser.add_argument('--start', type=str, help='start of date range')
    parser.add_argument('--end', type=str, help='end of data range')

    args = parser.parse_args()

    #print ('file: {}; start: {}; end: {}'.format( args.file, args.start,
        #args.end))

    if not args.file:
        print("Need --file=<ics filename>")

    if not args.start:
        print("Need --start=dd/mm/yyyy")

    if not args.end:
        print("Need --end=dd/mm/yyyy")

    #get information about start and end date from strings, then create 
    #corresponding datetime objects.
    start = datetime.datetime.strptime(args.start, "%d/%m/%Y")
    end = datetime.datetime.strptime(args.end, "%d/%m/%Y")
    
    #make an empty 2d lists to store events
    events_2d_list = []
    
    get_events(args.file, events_2d_list)
    update_events(events_2d_list, start)
    print_calendar(events_2d_list, start, end)

#	Function get_events:
# Parameters: filename(string), events_2d_list(list)
#
# Given a file name, read the text input from the file 
# line by line, and create 1d lists for events using those
# information to fill up the 2d list. For example,  
#   events_2d_list[0] is the first event in this 2d list.
#   in events_2d_list[0][0], there is datetime object representing the start 
#   date of the event.
#   in events_2d_list[0][1], there is datetime object representing the end 
#   date of the event.
#   in events_2d_list[0][2], there is datetime object representing the until 
#   date of the event if it is a repeated event. Otherwise None.
#   in events_2d_list[0][3], there is string representing the location of the
#   event, like "Dr. Chip's office".
#   in events_2d_list[0][4], there is string representing the summary of the 
#   event, like "Mom's birthday party".

def get_events(filename, events_2d_list):

    handle = open(filename, "r")
    #read one line into the string line
    line = handle.readline().strip()
   
    #indicates the last line of the text
    while line[:13] != "END:VCALENDAR":
    
        #append an empty list for a new event to the 2d list
        events_2d_list.append([None] * 5)
        
        #indicates the last line of an event
        while line[:10] != "END:VEVENT":
            
            if line[:8] == "DTSTART:":
                events_2d_list[-1][0] = datetime.datetime.strptime(
                    line, "DTSTART:%Y%m%dT%H%M%S")
                        
            elif line[:6] == "DTEND:":               
                events_2d_list[-1][1] = datetime.datetime.strptime(
                    line, "DTEND:%Y%m%dT%H%M%S")
                        
            elif line[:6] == "RRULE:":               
                events_2d_list[-1][2] = (datetime.datetime(int(line[-24:-20]),
                    int(line[-20:-18]),int(line[-18:-16]), int(line[-15:-13]),
                        int(line[-13:-11]),int(line[-11:-9])))
            
            elif line[:9] == "LOCATION:":                
                events_2d_list[-1][3] = (line[9:])
                
            elif line[:8] == "SUMMARY:":               
                events_2d_list[-1][4] = (line[8:])
                
            line = handle.readline().strip()
            
        line = handle.readline().strip()
    handle.close()

#	Function update_events:
# Parameters: events_2d_list(list), start(datetime)
#
# Update start and end dates of some repeated events, to ensure 
# that the events come after the start date in program arguments.
# For example, the start date of an repeated event is   
#	June 12, 2019 (Wed),
#   The start date in program arguments is
#   June 17, 2019 (Mon).
#   This function updates the start date of the event to
#   June 19, 2019 (Wed).

def update_events(events_2d_list, start):
    
    for i in range(0, len(events_2d_list)):
        if events_2d_list[i][2]:
            while events_2d_list[i][0] < start:
                events_2d_list[i][0] += datetime.timedelta(days = 7)
                events_2d_list[i][1] += datetime.timedelta(days = 7)
 
#	Function print_calendar:
# Parameters: events_2d_list(list), start(datetime), end(datetime)
#
# Given a start date and an end date in program arguments,  
# output all events within the ics file occurring during the 
# specified range of dates. For example, if the argument is 
# --start=12/6/2019 --end=12/6/2019 --file=prudence.ics,
#   the output is
# 	June 12, 2019 (Wed)
# 	-------------------
# 	10:00 am to 12:15 pm: ENGL 146 [CLE A314]
# 	2:30 pm to  3:30 pm: MATH 101 [ECS 123]
 
def print_calendar(events_2d_list, start, end):
    
    #next event is the one that comes first in all events
    first_in_all = True
    
    #keep outputing events if the start date is smaller than the end date
    while start <= end: 
    
        #sort events in chronological order at the beginning of this day
        events_2d_list.sort(key = lambda x : [x[0], x[4]])
        
        for i in range(0, len(events_2d_list)):   
        
            #prepare to output the day header if there is an event that happens
            #in this day
            if start <= events_2d_list[i][0] and \
                events_2d_list[i][1] <= start + datetime.timedelta(days = 1): 
            
                first_in_all, is_printed = print_day_header(events_2d_list[i], 
                    first_in_all)
                
                #break loop to output events if the day header has been printed
                if is_printed:
                    break
        
        for j in range(i, len(events_2d_list)): 
        
            #prepare to output events if some events happen in this day       
            if start <= events_2d_list[j][0] and \
                events_2d_list[j][1] <= start + datetime.timedelta(days = 1):
                print_events(events_2d_list[j])
        
        #the next day begins
        start += datetime.timedelta(days = 1)
        
#	Function print_day_header:
# Parameters: event(list), first_in_all(boolean)
# Returns: first_in_all(boolean), is_printed(boolean)
#
# Given a list representing an event, output the day header.  
        
def print_day_header(event, first_in_all):

    #print the day header if it is an unrepeated event or it is a repeated  
    #event where end date comes before its until date
    if not event[2] or event[1] <= event[2]:
        day_header = day_header_format(event[0], first_in_all)
        print(day_header, end = "")
        
        #first_in_all is set to False and is_printed is set to True if the 
        #day header has been printed
        return False, True
        
    return first_in_all, False
    
#	Function print_events:
# Parameters: event(list)
#
# Given a list representing an event, output the information
# about the event.  
    
def print_events(event):
    
    time_start, time_end = time_format(event[0]), time_format(event[1])
    
    #print the event if it is an unrepeated event
    if not event[2]:            
        print('{} to {}: {} [{}]\n'.format(time_start, time_end, event[4], 
            event[3]), end = "")
        
    #print the event if it is a repeated event where end date comes before 
    #its until date
    elif event[1] <= event[2]:
        print('{} to {}: {} [{}]\n'.format(time_start, time_end, event[4], 
            event[3]), end = "")
        
        #add 7 days to the start date and end date if it is a repeated event
        event[0] += datetime.timedelta(days = 7)
        event[1] += datetime.timedelta(days = 7)

#	Function day_header_format:
# Parameters: datetime(datetime), first_in_all(boolean)
# Returns: day_header(string)
#
# Given a datetime, creates a more readable version of the
# date, including underline and blank line. For example,
#   if the datetime object corresponds to:
#   2019-05-06
#   the returned string is:
#   May 06, 2019 (Mon)
#   ------------------

def day_header_format(datetime, first_in_all):

    day_header = datetime.strftime('%B %d, %Y (%a)')
    day_header += "\n" + "-" * len(day_header) + "\n"
   
    #no blank line at the beginning of the first day header, different days 
    #in output are separated by a single blank line
    return day_header if first_in_all else "\n" + day_header
        
#	Function time_format:
# Parameters: datetime(datetime)
# Returns: time(string)
#
# Given a datetime, creates a more readable version of the
# time. For example, if the datetime object corresponds to:
#   2019-05-06 11-15-00
#   the returned string is:
#   11:15 am.
   
def time_format(datetime):
    
    time = datetime.strftime('%I:%M %p').lower()   
    return time if time[0] != "0" else " " + time[1:]


if __name__ == "__main__":
    main()
