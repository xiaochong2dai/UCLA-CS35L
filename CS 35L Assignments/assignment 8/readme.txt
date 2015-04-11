The homework was to divide the SRT code into several parts so that it can be run at the same time by 
multiple threads. First, I added the pthread.h in the main.c file, and lpthread library in the Makefile.

Then, I found where the main.c should be modified. As TA said, there are four nested for loops 
that went through each pixel and ray-trace them, and we can use multithread there.
 
The first difficulty for me was to use the pthread_create to create threads since the arguments 
took me a while to understand. My TA mentioned we could use struct in the 4th varaible of pthread_create,
but later, I found we could use a easier way, just using global variables.

The second difficuty was how should I divide the work. Divide them by columns or rows or something else?
Based on TA's suggestions, I divided the work by columns, and each thread is responsible for a certain
amount of columns.

The third difficuty I met was how to assign these columns to n threads. I was thinking maybe we can divide
the total column number by the  number of threads, then I realized that might not be a good way since we 
can not guarantee each thread get most close work. I ended up using an extra array to control the column 
numbers. In this case, for example, if we have 50 threads, then the 1st thread will be responsible for the
1st, 51st and 101st columns. The second thread will be responsible for the 2nd, 52nd, and 102th columns.
Since we only have 128 colums, then the 30th thread will only be responsible for the 30th, 80th columns.
In this case, we can make sure all the threads can only differ a maximum work of 1 column, meaning we can
averagely assign all the work to threads. If we have a thread number which is greater than the column, 
meaning if we have more than 128 threads in this case, then we will only use the first 128 threads, and 
all the rest threads will be idle. Also, by this way, each thread can't step into other threads, so we can 
avoid data race. 

The first time when I trying to run the command "make clean check", I got a warning. 
main.c: In function ‘multiThreads’:
main.c:235: warning: control reaches end of non-void function
I googled this warning, and I realized that I have to return something in the 
'multiThreads’ function. I thought we didn't have to return, since it's a 'void *'
type, but later I realized the difference between 'void *' and 'void'. So at the end 
of the ‘multiThreads’, I added "return NULL", and this warning disappeared.

Below is the outcome when I run 'make clean check':

rm -f *.o *.ppm *.tmp srt srt.tgz
gcc -std=gnu99 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o main.o main.c
gcc -std=gnu99 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o raymath.o raymath.c
gcc -std=gnu99 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o shaders.o shaders.c
gcc -std=gnu99 -g -O2 -Wall -Wextra -Wno-unused-parameter -o srt main.o raymath.o shaders.o -lm -lpthread
time ./srt 1-test.ppm >1-test.ppm.tmp && mv 1-test.ppm.tmp 1-test.ppm

real 0m46.405s
user 0m46.410s
sys  0m0.001s
time ./srt 2-test.ppm >2-test.ppm.tmp && mv 2-test.ppm.tmp 2-test.ppm

real 0m24.643s
user 0m49.120s
sys  0m0.003s
time ./srt 4-test.ppm >4-test.ppm.tmp && mv 4-test.ppm.tmp 4-test.ppm

real 0m13.725s
user 0m52.889s
sys  0m0.005s
time ./srt 8-test.ppm >8-test.ppm.tmp && mv 8-test.ppm.tmp 8-test.ppm

real 0m7.591s
user 0m54.175s
sys  0m0.005s
for file in 1-test.ppm 2-test.ppm 4-test.ppm 8-test.ppm; do \
      diff -u 1-test.ppm $file || exit; \
      done

From the above time, we can see that as threads are doubled, the real time decreases approximately half.
So we can see the real speed is almost proportionally to the number of threads. On the other hand, The 
user time seems to be alway stable. Dividing the work among threads can greatly improve the performance 
of programs. As to how much performance can be improved, it might be determined by the CPU ( dual core,
quad-core, etc).
