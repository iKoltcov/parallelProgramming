N1=900
N2=18000
let "delta = (N2 - N1)/10"

seqPath="./outputs/openmp-output.txt"

rm -f $seqPath

for ((i=$N1; i<=$N2;i+=$delta))
do
	./bin/lab4 $i >> $seqPath

	echo "$i/$N2"
done