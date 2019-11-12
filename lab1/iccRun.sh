N1=900
N2=28000
let "delta = (N2 - N1)/10"

seqPath="./outputs/icc-seq-output.txt"
parPath="./outputs/icc-par-output.txt"

rm -f $seqPath
rm -f $parPath

for ((i=$N1; i<=$N2;i+=$delta))
do
	./bin/icc/lab1-seq $i >> $seqPath

	{
		for((j = 2; j <= 4; j += 1))
		do
			./bin/icc/lab1-par-$j $i
		done
	} >> $parPath
	echo -ne '\n' >> $parPath

	echo "$i/$N2"
done