N1=900
N2=58000
let "delta = (N2 - N1)/10"

seqPath="./outputs/openmp-output.txt"

rm -f $seqPath

for ((i=$N1; i<=$N2;i+=$delta))
do
	./bin/lab3 $i >> $seqPath

	echo "$i/$N2"
done