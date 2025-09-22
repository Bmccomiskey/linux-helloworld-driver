#this file is a shell script used for testing the clock

#***** TESTS *****

echo "-- TEST #1 (Reading)  --"
echo "Should print two times that are ten seconds apart"
echo "Running test..."

sleep 1

cat /dev/clock_device

sleep 10

cat /dev/clock_device 

echo "-- TEST #2 (Writing) __"
echo "Should sound an alarm in 5 seconds"
echo "Alarm set..."

echo "5" > /dev/clock_device

sleep 6

echo "Checking for alarm..."
if dmesg | grep -q "TIMES UP!!!"; then
    echo "TEST PASSED!!"
else
    echo "TEST FAILED"
fi

sleep 1 
echo "Tests concluded"





