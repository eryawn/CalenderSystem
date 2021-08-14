import re
import datetime
import calprint3
def main():
    
    line = "20180202T092345"
    m = re.search(r"(\d{8})t?(\d{6})", line, re.I)
    
    end = datetime.datetime.strptime(m.groups()[0] + m.groups()[1], 
                        "%Y%m%d%H%M%S")   
    print(end)
    #b = [int(item) for item in m.groups()]
    #date = datetime.datetime(b[0], b[1], b[2])
    print(m.groups()[0])
    
    #date = datetime.datetime(18,9,9,9,9, None)
    #print(date)
    
    m = re.search(r"(\d{6,8})\D+(\d{4,6})", "180202[110934", re.I)
    print(m.groups())
    date = datetime.datetime(18,12,9,9,9, 2)
    print(date)
    print(help(calprint3))
main()