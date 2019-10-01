printf "Build starts\n"
rm -rf ./bin/
mkdir -p ./bin/

gcc -O3 -Wall -Werror -o ./bin/lab1-seq lab1.c -lm

for((a=2; a <= 64; a *= 2))
do
    gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=$a lab1.c -o ./bin/lab1-par-$a -lm
done
printf "Success\n"
