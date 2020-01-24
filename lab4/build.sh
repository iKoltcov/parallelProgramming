printf "Build starts\n"
rm -rf ./bin/
mkdir -p ./bin/
rm -rf ./outputs/
mkdir -p ./outputs/

gcc -O3 -Wall -o ./bin/lab4 lab4.c -lm -fopenmp

printf "Success\n"