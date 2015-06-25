make clean
make
cat $1  | ./server > test.c
python remove_dead_goto.py 
make clean
gcc test.c
./a.out