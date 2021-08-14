import datetime
import sys
import argparse

def main():
    events_2d_list = []
    """get_events("prudence.ics", events_2d_list)
    #print(events_2d_list)
    print(events_2d_list[-1][1].strftime('%b %d, %Y (%a)'))
    print(events_2d_list[-1][1].strftime('%I:%M %p'))
    events_2d_list[-1][1] = events_2d_list[-1][1] + datetime.timedelta(days = 1)
    print(events_2d_list[-1][1].strftime('%b %d, %Y (%a)'))
    print(events_2d_list[-1][1].strftime('%I:%M %p'))
    str = events_2d_list[-1][1]
    print(str)
    print(time_format(str))
    a =b =c =1
    print(c)"""
    
    start = datetime.datetime(2019,5,1)
    end = datetime.datetime(2019,8,31)
    get_events("prudence.ics", events_2d_list)
    update_events(events_2d_list, start)
    print_events(events_2d_list, start, end)
    

    
    

def time_format(datetime):
    
    time = datetime.strftime('%I:%M %p')
    if time[0] == "0":
        time = " " + time[1:]
    return time.lower()

def day_header_format(datetime, first_in_all):

    day_header = datetime.strftime('%B %d, %Y (%a)')
    underline = "-" * len(day_header)
    
    if first_in_all:
        return day_header + "\n" + underline + "\n"
    else:
        return "\n" + day_header + "\n" + underline + "\n"
   
def get_events(filename, events_2d_list):

    handle = open(filename, "r")
    sentence = handle.readline().strip()
   
    while sentence[:13] != "END:VCALENDAR":
    
        events_2d_list.append([None] * 5)
        while sentence[:10] != "END:VEVENT":
            
            if sentence[:8] == "DTSTART:":
                events_2d_list[-1][0] = datetime.datetime.strptime(sentence, "DTSTART:%Y%m%dT%H%M%S")              
            elif sentence[:6] == "DTEND:":
                events_2d_list[-1][1] = datetime.datetime.strptime(sentence, "DTEND:%Y%m%dT%H%M%S")
            elif sentence[:6] == "RRULE:":
                events_2d_list[-1][2] = (datetime.datetime(int(sentence[-24:-20]),int(sentence[-20:-18]),int(sentence[-18:-16]),
                int(sentence[-15:-13]),int(sentence[-13:-11]),int(sentence[-11:-9])))
            elif sentence[:9] == "LOCATION:":
                events_2d_list[-1][3] = (sentence[9:])
            elif sentence[:8] == "SUMMARY:":
                events_2d_list[-1][4] = (sentence[8:])
                
            sentence = handle.readline().strip()
            
        sentence = handle.readline().strip()
    handle.close()
    
def update_events(events_2d_list, start):
    
    for i in range(0, len(events_2d_list)):
        if events_2d_list[i][2]:
            while events_2d_list[i][0] < start:
                events_2d_list[i][0] += datetime.timedelta(days = 7)
        
def print_day_header(event, start, first_in_all):
 
    if (event[2] and event[0] < event[2]) or not event[2]:
        day_header = day_header_format(event[0], first_in_all)
        print(day_header, end = "")
        return False, True
        
    return first_in_all, False
            
def print_event(event, start):
    
    if not event[2] or event[0] <= event[2]:
    
        time_start, time_end = time_format(event[0]), time_format(event[1])
        print('{} to {}: {} [{}]\n'.format(time_start, time_end, event[4], event[3]), end = "")
        if event[2] and event[0] <= event[2]:
            event[0] += datetime.timedelta(days = 7)
            event[1] += datetime.timedelta(days = 7)
        
def print_events(events_2d_list, start, end):

    first_in_all = True    
    while start <= end:   
        events_2d_list.sort(key = lambda x : [x[0], x[4]])
        
        for i in range(0, len(events_2d_list)):       
            if start <= events_2d_list[i][0] and events_2d_list[i][1] < start + datetime.timedelta(days = 1):              
                first_in_all, is_printed = print_day_header(events_2d_list[i], start, first_in_all)
                if is_printed:
                    break
        
        for j in range(i, len(events_2d_list)):            
            if start <= events_2d_list[j][0] and events_2d_list[j][1] < start + datetime.timedelta(days = 1):
                print_event(events_2d_list[j], start)
                
        start += datetime.timedelta(days = 1)    
       
main()