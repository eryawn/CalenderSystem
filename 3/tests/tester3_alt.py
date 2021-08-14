#!/usr/bin/env python3

#
# UVic SENG 265, Summer 2019, Assignment #3
#
# This test-driver program invokes methods on the class to be
# completed for the assignment.
# 
# The content of tester3.py must not be modified. It will be used
# when evaluating your solution to calprint3.py.
#

import sys
import argparse
import datetime
import calprint3


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--file', type=str, help='file to be processed')
    parser.add_argument('--start', type=str, help='start of date range')
    parser.add_argument('--end', type=str, help='end of data range')

    args = parser.parse_args()

    if not args.file:
        print("Need --file=<ics filename>")

    if not args.start:
        print("Need --start=dd/mm/yyyy")

    if not args.end:
        print("Need --end=dd/mm/yyyy")

    (start_dd, start_mm, start_yy) = \
        (int(field) for field in args.start.split("/"))

    (end_dd, end_mm, end_yy) = \
        (int(field) for field in args.end.split("/"))

    start_dt = datetime.datetime(start_yy, start_mm, start_dd)
    end_dt   = datetime.datetime(end_yy, end_mm, end_dd)

    c = calprint3.Calprint(args.file)
   
    curr_dt = start_dt
    blank_line = None
    while (curr_dt <= end_dt):
        days_events = c.get_events_for_day(curr_dt)
        if days_events and blank_line:
            print()
            print(days_events)
        elif days_events:
            print(days_events)
            blank_line = " "
        curr_dt = curr_dt + datetime.timedelta(days=1)


if __name__ == "__main__":
    main()
