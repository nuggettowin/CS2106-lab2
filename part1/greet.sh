# day of week, day month year time
name=$(whoami)
day=$(date +%A)
#07 February 2026
dayOfMonth=$(date +%d)
month=$(date +%B)
year=$(date +%Y)
#22:40:54
time=$(date +%T)
echo "Hello $name, today is $day, $dayOfMonth $month $year, and the time is $time."