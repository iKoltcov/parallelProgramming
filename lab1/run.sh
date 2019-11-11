N1=14000
N2=192500
let "delta = (N2 - N1)/10"

seqPath="./gcc-seq-output.txt"
parPath="./gcc-par-output.txt"

rm -f $seqPath
rm -f $parPath

for ((i=$N1; i<=$N2;i+=$delta))
do
	./bin/gcc/lab1-seq $i >> $seqPath

	{
		for((j = 2; j <= 20; j += 2))
		do
			./bin/gcc/lab1-par-$j $i
		done
	} >> $parPath
	echo -e '\n' >> $parPath

	echo "$i/$N2"
done

seqPath="./cc-seq-output.txt"
parPath="./cc-par-output.txt"

rm -f $seqPath
rm -f $parPath

for ((i=$N1; i<=$N2;i+=$delta))
do
	./bin/cc/lab1-seq $i >> $seqPath

	{
		for((j = 2; j <= 20; j += 2))
		do
			./bin/cc/lab1-par-$j $i
		done
	} >> $parPath
	echo -e '\n' >> $parPath

	echo "$i/$N2"
done
echo "Done"
