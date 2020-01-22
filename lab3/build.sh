printf "Build starts\n"
rm -rf ./bin/
mkdir -p ./bin/
rm -rf ./outputs/
mkdir -p ./outputs/

gcc -O3 -Wall -Werror -o ./bin/lab3 lab3.c -lm -fopenmp

printf "Success\n"