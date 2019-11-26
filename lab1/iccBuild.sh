printf "Build starts\n"
rm -rf ./bin/icc/
mkdir -p ./bin/icc/

icc -fast -ipo lab1.c -o ./bin/icc/lab1-seq -lm

for((i = 2; i <= 4; i += 1))
do
    icc -fast -ipo -parallel -par-threshold=$i -o ./bin/icc/lab1-par-$i lab1.c -lm  
done
printf "Success\n"