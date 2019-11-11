printf "Build starts\n"
rm -rf ./bin/gcc/
mkdir -p ./bin/gcc/

gcc -O3 -Wall -Werror -o ./bin/gcc/lab1-seq lab1.c -lm

for((i = 2; i <= 20; i += 2))
do
    gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=$i lab1.c -o ./bin/gcc/lab1-par-$i -lm
done
printf "Success\n"
