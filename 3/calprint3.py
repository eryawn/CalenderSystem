#
# UVic SENG 265, Summer 2019, Assignment #3
#
# Student work is to appear in this module.
#
import datetime
import re

class Calprint:

    def __init__(self, filename):
        self.hello = "Alive"
        self.filename = filename
        self.events_list = []
        self.get_events()
        
        

    def get_events_for_day(self, dt):
        
        self.update_events(dt)
        self.events_list.sort(key = lambda x : [x.start, x.summary])
        content = ""

        for i in range(0, len(self.events_list)):   
        
            if dt <= self.events_list[i].start and \
                self.events_list[i].end <= dt + datetime.timedelta(days = 1): 
            
                day_header, is_printed = self.get_day_header(self.events_list[i])
                content += day_header
                
                #break loop to output events if the day header has been printed
                if is_printed:
                    break
        
        for j in range(i, len(self.events_list)): 
            
            #prepare to output events if some events happen in this day       
            if dt <= self.events_list[j].start and \
                self.events_list[j].end <= dt + datetime.timedelta(days = 1):
                content += self.print_events(self.events_list[j])
            
        return content
    

    def get_events(self):

        handle = open(self.filename, "r")
        line = handle.readline()
        
        while not re.match(r"END:VCALENDAR", line):
            
            start, end, until, location, summary = None, None, None, None, None
            while not re.match(r"END:VEVENT", line):
                
                m = re.match(r"DTSTART:(\d{8}T\d{6})", line)
                if m:
                    start = datetime.datetime.strptime(m.group(1), "%Y%m%dT%H%M%S")
                    
                m = re.match(r"DTEND:(\d{8}T\d{6})", line)
                if m:
                    end = datetime.datetime.strptime(m.group(1), "%Y%m%dT%H%M%S")
                    
                m = re.match(r"RRULE:.*(\d{8}T\d{6}).*", line)
                if m:
                    until = datetime.datetime.strptime(m.group(1), "%Y%m%dT%H%M%S")
                    
                m = re.match(r"LOCATION:(.*)", line)
                if m:
                    location = m.group(1)
                    
                m = re.match(r"SUMMARY:(.*)", line)
                if m:
                    summary = m.group(1)
                    
                line = handle.readline()
            
            event = Event(start, end, until, location, summary)
            self.events_list.append(event)
            
            
            line = handle.readline()    
        handle.close()

    def update_events(self, dt):
    
        for event in self.events_list:
            if event.until:
                while event.start < dt:
                    event.start += datetime.timedelta(days = 7)
                    event.end += datetime.timedelta(days = 7)
        
    def get_day_header(self, event):


        if not event.until or event.start <= event.until:
            return self.day_header_format(event.start), True
        return "", False
    
  
    
    def print_events(self, event):
    
        time_start, time_end = self.time_format(event.start), self.time_format(event.end)
    

        if not event.until:            
            return ('\n{} to {}: {} [{}]'.format(time_start, time_end, event.summary, 
                event.location))
        

        elif event.end <= event.until:
            return ('\n{} to {}: {} [{}]'.format(time_start, time_end, event.summary, 
                event.location))
        

            event.start += datetime.timedelta(days = 7)
            event.end += datetime.timedelta(days = 7)
        return ""


    def day_header_format(self, datetime):

        day_header = datetime.strftime('%B %d, %Y (%a)')
        day_header += "\n" + "-" * len(day_header)
   

        return day_header
        

   
    def time_format(self, datetime):
    
        time = datetime.strftime('%I:%M %p').lower()   
        return time if time[0] != "0" else " " + time[1:]
        
        
class Event:
    
    def __init__(self, start, end, until, location, summary):
        self.start = start
        self.end = end
        self.until = until
        self.location = location
        self.summary = summary
        
