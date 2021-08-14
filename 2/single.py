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
        if events_2d_list[i][2] != None:
            while events_2d_list[i][0] < start:
                events_2d_list[i][0] += datetime.timedelta(days = 7)
    
def day_header_format(datetime, first_in_all, first_in_day):
    
    if first_in_all == False and first_in_day == False:
        return ""
        
    day_header = datetime.strftime('%B %d, %Y (%a)')
    underline = "-" * len(day_header)
    
    if first_in_all and first_in_day:
        return day_header + "\n" + underline + "\n"
    else:
        return "\n" + day_header + "\n" + underline + "\n"
        
def print_events(events_2d_list, start, end):
    
    first_in_all = True
    while start <= end:
    
        first_in_day = True
        sorted(events_2d_list, key = lambda x : x[4])
        sorted(events_2d_list, key = lambda x : x[0])
        
        for i in range(0, len(events_2d_list)):
            time_start = time_format(events_2d_list[i][0])
            time_end = time_format(events_2d_list[i][1])
            if start <= events_2d_list[i][0] and events_2d_list[i][0] <= start + datetime.timedelta(days = 1):
                
                if events_2d_list[i][2] == None:
                    day_header = day_header_format(events_2d_list[i][0], first_in_all, first_in_day)
                    first_in_all = first_in_day = False
                    print(day_header + time_start + " to " + time_end + ": " + events_2d_list[i][4] + " [" + events_2d_list[i][3] + "]\n", end = "")
                 
                elif events_2d_list[i][0] <= events_2d_list[i][2]:
                    day_header = day_header_format(events_2d_list[i][0], first_in_all, first_in_day)
                    first_in_all = first_in_day = False
                    print(day_header + time_start + " to " + time_end + ": " + events_2d_list[i][4] + " [" + events_2d_list[i][3] + "]\n", end = "")                   
                       
                    events_2d_list[i][0] += datetime.timedelta(days = 7)
        
        start += datetime.timedelta(days = 1)
        
        
        
        
        
        
main()