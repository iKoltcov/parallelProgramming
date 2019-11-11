printf "Build starts\n"
rm -rf ./bin/cc/
mkdir -p ./bin/cc/

cc -O3 lab1.c -o ./bin/cc/lab1-seq -lm

for((i = 2; i <= 20; i += 2))
do
    cc -O3 lab1.c -xautopar -xloopinfo -o ./bin/cc/lab1-par-$i -lm
done
printf "Success\n"
