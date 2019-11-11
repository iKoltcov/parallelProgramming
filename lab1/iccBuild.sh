printf "Build starts\n"
rm -rf ./bin/icc/
mkdir -p ./bin/icc/

icc -O3 lab1.c -o ./bin/icc/lab1-seq -lm

for((i = 2; i <= 20; i += 2))
do
    icc -O3 -parallel -par-threshold=$i -o ./bin/icc/lab1-par-$i lab1.c -lm    
done
printf "Success\n"
