#!/usr/bin/bash

# This script makes several assumptions:
# 
# (1) All of the .ics files are in the same directory ast the
#     'calprint' executable.
# 
# (2) The script itself is executed in the same directory as
      the 'calprint' executable.
# 
# (3) The script is being run on a machine from which the
#     directory /home/zastre/seng265/a1/tests is visible
#
# If no output other that 'Checking for <sometest>' appears on
# the console, then that test has passed (i.e., there are no
# differences between what the executable produces as output and
# what is expected for that test)


# Uncomment the following 'set' statement if you want to see 
# precisely what 'bash' is doing to execute this script.
# set -x  

EXECUTABLE=./calprint
EXPECTEDS_DIR=/home/zastre/seng265/a1/tests
DIFFBIN=/usr/bin/diff

declare -a ARGLIST=(
    "--from=18/6/2019 --to=18/6/2019 --file=one.ics":"test01.txt"
    "--from=1/6/2019 --to=1/6/2019 --file=one.ics":"test02.txt"
    "--from=31/5/2019 --to=30/6/2019 --file=one.ics":"test03.txt"
    "--from=1/10/2019 --to=31/10/2019 --file=one.ics":"test04.txt"
    "--from=20/6/2019 --to=20/06/2019 --file=two.ics":"test05.txt"
    "--from=12/6/2019 --to=24/06/2019 --file=two.ics":"test06.txt"
    "--from=1/5/2019 --to=30/5/2019 --file=two.ics":"test07.txt"
    "--from=4/6/2019 --to=04/6/2019 --file=many.ics":"test08.txt"
    "--from=1/6/2019 --to=31/07/2019 --file=many.ics":"test09.txt"
    "--from=1/5/2019 --to=31/5/2019 --file=many.ics":"test10.txt"
    "--from=18/6/2019 --to=18/06/2019 --file=three.ics":"test11.txt"
    "--from=1/5/2019 --to=31/7/2019 --file=three.ics":"test12.txt"
    "--from=23/5/2019 --to=23/5/2019 --file=prudence.ics":"test13.txt"
    "--from=24/5/2019 --to=24/5/2019 --file=prudence.ics":"test14.txt"
    "--from=1/5/2019 --to=31/8/2019 --file=prudence.ics":"test15.txt"
    )

for A in "${ARGLIST[@]}"
do
    IFS=\:; read -a FIELDS <<< "$A"
    TEST_FILE=${FIELDS[1]}
    IFS=\ ; read -a TEST_ARGS <<< ${FIELDS[0]}

    echo "Checking for: $TEST_FILE"

    $EXECUTABLE ${TEST_ARGS[0]} ${TEST_ARGS[1]} ${TEST_ARGS[2]}|  \
        $DIFFBIN - $EXPECTEDS_DIR/$TEST_FILE
done
