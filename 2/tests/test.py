def main():
    1
    
    
def print_event(event):
    
    #time_start, time_end = time_format(event[0]), time_format(event[1])
    
    #print the event if it is an unrepeated event
    if not event[2]:            
        print('{} to {}: {} [{}]\n'.format(time_start, time_end, event[4], event[3]), end = "")
        print()
    #print the event if it is a repeated event where start date comes before its until date
    elif event[0] <= event[2]:
        print('{} to {}: {} [{}]\n'.format(time_start, time_end, event[4], event[3]), end = "")
        
        #add 7 days to the start date and end date if it is a repeated event
        event[0] += datetime.timedelta(days = 7)
        event[1] += datetime.timedelta(days = 7)
    222

