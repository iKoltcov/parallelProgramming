printf "Build starts\n"
rm -rf ./bin/
mkdir -p ./bin/

gcc -O3 -o ./bin/lab2 lab2.c -m64 -L/usr/include/FW_1.3.1_Lin64/lib -lfwBase -lfwImage -lfwSignal -lm
printf "Success\n"
