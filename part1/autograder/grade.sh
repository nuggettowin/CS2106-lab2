#!/bin/bash

# $# num arg passed, $1, $2 first and sec args, $@ list of args passed, $? return value from func
res="results.out"
: > $res
echo -e "Test date and time: $(date +%A), $(date +%d) $(date +%B) $(date +%Y), $(date +%T)\n" >> $res

allowed_arg_count=1
# Check if we have enough arguments
if [[ $# -gt $allowed_arg_count || $# -lt $allowed_arg_count ]]; then
    echo "Usage: ./grade.sh <MAXSCORE>"
    exit -1
fi
# Delete temporary files
rm -f ./ref/*.in.out

# Compile the reference program
gcc ./ref/*.c -o ./ref/fun

n=0
# Generate reference output files
for i in ./ref/*.in; do
    ./ref/fun < $i > $i.out
    ((n++))
done

# Remember to check maximum score given as argument, compared to the real number of test cases
if [[ $1 -lt $n ]]; then
    n=$1
fi
# Now mark submissions

#
# Note: See Lab02Qn.pdf for format of output file. Marks will be deducted for missing elements.
#

# Iterate over every submission directory
    # Compile C code
    # Print compile error message to output file (if any)
    # Generate output from C code using *.in files in /ref
    # Compare with reference output files  and award 1 mark if they are identical
    # print score for student
# print total submissions marked.
fileCount=0 
goodGcc=0 # expected gcc return int if compilation no error
for i in ./subs/*/; do
    score=0
    let fileCount=fileCount+1
    gcc ${i}*.c -o ${i}fun

    if [[ $? -ne $goodGcc ]]; then
        echo "Directory $(basename ${i}) has a compile error" >> $res
    else
        for j in ./ref/*.in; do
            ${i}fun < $j > ${i}.in.out
            if [[ -z $(diff ${j}.out ${i}.in.out) ]]; then # if identical to expected output
                score=$(( $score == $n ? score : score + 1 ))
            fi
        done
    fi

    echo "Directory $(basename ${i}) score $score / $n" >> $res
done
echo -e -n "\nProcessed $fileCount files." >> $res