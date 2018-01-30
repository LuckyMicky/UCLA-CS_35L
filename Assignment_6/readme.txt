Daxuan Shu
204853061
readme.txt

1. Unmodified version
time ./srt 1-test.ppm >1-test.ppm.tmp
real	0m49.380s
user	0m49.372s
sys	0m0.000s

2. Modified version
time ./srt 1-test.ppm >1-test.ppm.tmp
real	0m49.543s
user	0m49.533s
sys	0m0.003s



time ./srt 2-test.ppm >2-test.ppm.tmp
real	0m24.854s
user	0m49.451s
sys	0m0.002s


time ./srt 4-test.ppm >4-test.ppm.tmp
real	0m12.679s
user	0m50.124s
sys	0m0.002s


time ./srt 8-test.ppm >8-test.ppm.tmp
real	0m6.703s
user	0m52.023s
sys	0m0.001s

The real time is close to user time divide by number of threads.

There was a slightly increase for user time due to the cost of creating more threads.

When allpying the 8 threads, we can see that it is much more faster than the 1-thread version.

This shows the power of parallelism.

3. Issues previously ran into
when using -lpthread, I saw there were errors like no reference for error pointer. After fixing the code in main.c, it works now.


