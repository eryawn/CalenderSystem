# A description of the ten tests for A#1 (UVic SENG 265, Summer 2019).

The tests are sequenced in a way that suggests an implementation
strategy. That is, earlier tests require (roughly speaking) less
functionality in the code than later tests. Earlier tests concentrate
on calendars containing single events rather than repeated events.
Therefore do your implementation work in a way to ensures the
lower-numbered tests pass before you write code to satisfy the later
tests.


## Important note

**Regarding dates provided as arguments:** Do not make any
assumptions about the number of digits need for day and month. That
is, the following must be considered equivalent (i.e., June 5, 2019):
* 1/5/2019
* 01/5/2019
* 1/05/2019
* 01/05/2019


## Description of tests

All ICal files (i.e., suffix .ics) are in /home/zastre/seng265/a1/tests.
The assumption in the arguments given below is that these ICal files
are available in the same directory in which ./calprint is executed.

In some of the test cases where there is no output, the corresonding
test file itself is a blank file (i.e., 0 bytes).

* Test 01:
    * Range of dates comprises a single day.
    * That day has the only event in the calendar.
    * Args: --from=18/6/2019 --to=18/6/2019 --file=one.ics
    * Expected output: /home/zastre/seng265/a1/tests/test01.txt

* Test 02:
    * Range of dates comprises a single day.
    * There are _no_ events on that single day. 
    * Args: --from=1/6/2019 --to=1/6/2019 --file=one.ics
    * Expected output: /home/zastre/seng265/a1/tests/test02.txt

* Test 03:
    * Range of dates comprises more than one day.
    * The range includes the only event in the calendar.
    * Args: --from=31/5/2019 --to=30/6/2019 --file=one.ics
    * Expected output: /home/zastre/seng265/a1/tests/test03.txt

* Test 04:
    * Range of dates comprises more than one day.
    * The range _excludes_ the single event in the calendar.
    * Args: --from=1/10/2019 --to=31/10/2019 --file=one.ics
    * Expected output: /home/zastre/seng265/a1/tests/test04.txt

* Test 05:
    * Range of dates comprises a single day.
    * That day is the same as one of events in the calendar (i.e, 
      not all events in the calendar appear in the output).
    * Args: --from=20/6/2019 --to=20/06/2019 --file=two.ics
    * Expected output: /home/zastre/seng265/a1/tests/test05.txt

* Test 06:
    * Range of dates comprises more than one day.
    * The range includes all events in the calendar.
    * Args: --from=12/6/2019 --to=24/06/2019 --file=two.ics
    * Expected output: /home/zastre/seng265/a1/tests/test06.txt

* Test 07:
    * Range of dates comprises more than one day.
    * The range _excludes_ all events in the calendar.
    * Args: --from=1/5/2019 --to=30/5/2019 --file=two.ics
    * Expected output: /home/zastre/seng265/a1/tests/test07.txt

* Test 08:
    * Range of dates comprises a single day.
    * The range includes the starting date of a repeating
      event in the calendar.
    * Args: --from=4/6/2019 --to=04/6/2019 --file=many.ics
    * Expected output: /home/zastre/seng265/a1/tests/test08.txt

* Test 09:
    * Range of dates comprises more than one day.
    * The range of dates includes every possible occurrence of 
      the event.
    * Args: --from=1/6/2019 --to=31/07/2019 --file=many.ics
    * Expected output: /home/zastre/seng265/a1/tests/test09.txt

* Test 10:
    * Range of dates comprises more than one day.
    * The range of dates excludes any possible occurence of the event.
    * Args: --from=1/5/2019 --to=31/5/2019 --file=many.ics
    * Expected output: /home/zastre/seng265/a1/tests/test10.txt

* Test 11:
    * Range of dates comprises a single day.
    * On that single day are two events (one a single event, the
      other part of a repeating event).
    * Args: --from=18/6/2019 --to=18/06/2019 --file=three.ics
    * Expected output: /home/zastre/seng265/a1/tests/test11.txt

* Test 12:
    * Range of dates comprises more than one day.
    * The range of dates includes all dates for which the single 
      events and repeated events occur.
    * Args: --from=1/5/2019 --to=31/7/2019 --file=three.ics
    * Expected output: /home/zastre/seng265/a1/tests/test12.txt

* Test 13:
    * Range of dates comprises a single day.
    * On that single day are four events (all of them based on
      a repeating event).
    * Args: --from=23/5/2019 --to=23/5/2019 --file=prudence.ics
    * Expected output: /home/zastre/seng265/a1/tests/test13.txt

* Test 14:
    * Range of dates comprises a single day.
    * There are _no_ events on that single day (i.e., none of the
      repeating events or single events occur).
    * Args: --from=24/5/2019 --to=24/5/2019 --file=prudence.ics
    * Expected output: /home/zastre/seng265/a1/tests/test14.txt

* Test 15:
    * Range of dates comprises more than one day.
    * All events in the calendar (single and repeating) appear
      in the output.
    * Args: --from=1/5/2019 --to=31/8/2019 --file=prudence.ics
    * Expected output: /home/zastre/seng265/a1/tests/test15.txt

