#
# UVic SENG 265, Summer 2019, Assignment #3
#
# Student work is to appear in this module.
# Xuhui Wang/Eric Wang V00913734
import datetime
import re

class Calprint:

    def __init__(self, filename):
        self.filename = filename
        self.events_list = []
        self.get_events()
        
    def get_events_for_day(self, dt):
        """
        Parameters: dt(datetime)
        Returns: events(string)
        
        Given a date, return string of all events within the ics file 
        occurring during the date. For example, if the argument is
        12/6/2019, the output is
        June 12, 2019 (Wed)
        -------------------
        10:00 am to 12:15 pm: ENGL 146 [CLE A314]
        2:30 pm to  3:30 pm: MATH 101 [ECS 123]
        """
        
        # Sort events in chronological order and update events.
        self.update_events(dt)
        self.events_list.sort(key = lambda x : [x.start, x.summary])
        content = ""        
        has_date_header = False
        
        for event in self.events_list:

            #  Generate the date header and events that happens.
            if dt <= event.start and \
                event.end <= dt + datetime.timedelta(days = 1):
          
                if has_date_header:
                    content += self.event_format(event)        
                else:
                    date_header, has_date_header = \
                        self.date_header_format(event)
                    content += date_header + self.event_format(event)
                    
        return content if content else None

    def get_events(self):
        """
        Given a file name, read the text input from the file 
        line by line, and create 1d list to contain events which are
        objects consisting of information from the text. For example,  
        events_list[0] is the first event in this list.
        events_list[0].start is datetime object representing the start 
        date of the event.
        events_list[0].end is datetime object representing the end 
        date of the event.
        events_list[0].until is datetime object representing the until 
        date of the event if it is a repeated event. Otherwise None.
        events_list[0].location is string representing the location of 
        the event, like "Dr. Chip's office".
        events_list[0].summary is string representing the summary of  
        the event, like "Mom's birthday party".
        """
        handle = open(self.filename, "r")
        line = handle.readline()
        
        # Indicates the last line of the text.
        while not re.search(r"end.*calendar", line, re.I):           
            until = None
            
            # Indicates the last line of an event.
            while not re.search(r"end.*event", line, re.I):              
                m = re.search(r"start\D*(\d{8})\D+(\d{6})", line, re.I)
                if m:
                    start = datetime.datetime.strptime(
                        m.groups()[0] + m.groups()[1], "%Y%m%d%H%M%S")                   
                m = re.search(r"end\D*(\d{8})\D+(\d{6})", line, re.I)
                if m:
                    end = datetime.datetime.strptime(
                        m.groups()[0] + m.groups()[1], "%Y%m%d%H%M%S")                   
                m = re.search(r"until\D*(\d{8})\D+(\d{6})", line, re.I)
                if m:
                    until = datetime.datetime.strptime(
                        m.groups()[0] + m.groups()[1], "%Y%m%d%H%M%S")                  
                m = re.search(r"location\W*(.*)", line, re.I)
                if m:
                    location = m.groups()[0]                   
                m = re.search(r"summary\W*(.*)", line, re.I)
                if m:
                    summary = m.groups()[0]                    
                line = handle.readline()
            
            event = Event(start, end, until, location, summary)
            
            # Append the event to the events list.
            self.events_list.append(event)                     
            line = handle.readline()    
        handle.close()

    def update_events(self, dt):
        """
        Parameters: dt(datetime)
 
        Update start and end dates of some repeated events, to ensure 
        that the events come after the start date in argument.
        For example, the start date of an repeated event is   
        June 12, 2019 (Wed),
        The start date in argument is
        June 17, 2019 (Mon).
        This function updates the start date of the event to
        June 19, 2019 (Wed).
        """
        for event in self.events_list:
            if event.until:
                while event.start < dt:
                    event.start += datetime.timedelta(days = 7)
                    event.end += datetime.timedelta(days = 7)
        
    def date_header_format(self, event):
        """
        Parameters: event(event)
        Returns: date_header(string), has_date_header(boolean)
        
        Given an event object, return the date header which is
        a more readable version of the date, including underline
        and blank line. For example,
        if the datetime object corresponds to:
        2019-05-06
        the returned string is:
        May 06, 2019 (Mon)
        ------------------
        """
        # Generate the day header if it is an unrepeated event or   
        # it is a repeated event where end date comes before its 
        # until date.
        if not event.until or event.end <= event.until:
            date_header = event.start.strftime('%B %d, %Y (%a)')
            date_header += "\n" + "-" * len(date_header)                  
            return date_header, True
        return "", False
       
    def event_format(self, event):
        """
        Parameters: event(event)
        Returns: event_info(string)
 
        Given an event object, return the information about the 
        event.  
        """
        # Generate the event if it is an unrepeated event or   
        # it is a repeated event where end date comes before its 
        # until date.
        if not event.until or event.end <= event.until:    
            time_start = self.time_format(event.start)
            time_end = self.time_format(event.end)
            return ('\n{} to {}: {} [{}]'.format(time_start, 
                time_end, event.summary, event.location))      
        return ""
          
    def time_format(self, datetime):
        """
        Parameters: datetime(datetime)
        Returns: time(string)
 
        Given a datetime, creates a more readable version of the
        time. For example, if the datetime object corresponds to:
        2019-05-06 11-15-00
        the returned string is:
        11:15 am.
        """
        time = datetime.strftime('%I:%M %p').lower()   
        return time if re.match(r"[^0].*", time) else re.sub(r"0", " ",
            time, 1)
        
        
class Event:
    
    def __init__(self, start, end, until, location, summary):
        self.start = start
        self.end = end
        self.until = until
        self.location = location
        self.summary = summary
        
