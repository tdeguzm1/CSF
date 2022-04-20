 * Code to cache simulation
 * CSF Assignment 3 MS1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu


This assignment was done individually, with all requirements for MS3 complete.
No bugs were found, and gradescope autograders were passed. Valgrind also passed.



Analysis of best cache:

I did an analysis of various configurations of chaces to determine what storage distribution would be most effective.
To do this, I conducted an experiment, keeping certain factors constant and changing others. I kept the total cache size
constant (131,072 bytes), and for simplicity, I kept the slot size at a constant 16 bytes for the beginning experiments.
I also only used the swim.trace file for consistency. I will make the assumption that this is a "typical" file.
You can see the results of the experiments below.  Note that the hit rate and the number of misses (and therefore the miss peanalities)
are inversely correlated, so I will refer primarily to misses, implying that more misses indicates a lower hit rate.  


Looking first at only a (1-8192-16) memory configuration for a fully associative cache, the most performant variation is the 
write-allocate, write-back, lru configuration in terms of lowest number of total cycles and load/store misses. A quick test 
with a direct-mapped cache shows this is true for them too.  Therefore, the following tests will focus on this set of parameters. 
 

By running a series of experiments, I determined that the performance decreases as you travel from a fully associative cache to
a direct mapped cache.  However, this decrease is not linear.  In fact a cache with parameters of:

./csim 256 32 16 write-allocate write-back lru < swim.trace

Has the same number of cycle times and misses as a fully associative cache.  Since fully associative caches are difficult to implement
in hardware, they are not desirable.  Therefore, this 256-32-16 cache is a good compromise, providing optimal performance for
a 'typical' program, while having less hardware complexity than a fully associative cache. 


So far, the only variable I haven't looked at is varying the number of bytes per slot.  Therefore, I will keep the number of sets constant
and vary the number of slots and bytes per slot. By decreasing the number of slots and increasing the number of bytes per slot, I was 
able to decrease the number of misses, but I increased the total cycles (because the larger number of bytes needed to be stored
and/or loaded to/from memory).  Similary, by increasing the number of slots and decreasing the number of bytes per slot, I signifcantly
decreased the total cycle time, but the number of misses was increased. I also tested a couple trials with 
write-through to see if that would make improvements with large numbers of bytes per slot, but the benefits were minimal.

It would be neglectful if I only tested changing the number of slots, and not the number of sets, so I will do that next.
The tests yielded similar results, where increasing sets and decreasing bytes per slot led to a decrease in total cycles, but an 
increase in the number of misses.

Also, the choice of a total cache size was somewhat arbitrary.  131,072 bytes, which is around 128 kB, doesn't seem unreasoably large,
and it worked well and ran quickly (<1 second).  By increasing the total cache size, the counts and misses decrease substantially.
Similarly, decreasing cache total size causes it to take longer and miss more. However, a larger cache has more overhead and is harder 
to implement on the hardware level. Balancing time with size (and therefore hardware complexity), this 128 kB cache seemed reasonable. 

In conclusion, the "256-32-16  write-allocate write-back lru" configuration cache seemed to be a good compromise between 
direct-mapped-ness and full associativity. It also has a good balance between optimizing the number of misses and the total
cycle time.  However, if the hardware is not an issue, a better cache is biased toward greater associativity and larger overall size.  
Also, if the runtime (total cycles) is more important, having a lower number of bytes per slot seemes to be the better option. Simiarly, 
if minimizing the number of misses is most important, having a greater number of bytes per slot is optimal.  



Experimental results:

***Fully Associative 16-byte slot size caches

./csim 1 8192 16 write-allocate write-back fifo < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219736
Load misses: 932
Store hits: 72025
Store misses: 10500
Total cycles: 6141461

./csim 1 8192 16 write-allocate write-through fifo < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219736
Load misses: 932
Store hits: 72025
Store misses: 10500
Total cycles: 12077561

./csim 1 8192 16 no-write-allocate write-through fifo < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 218148
Load misses: 2520
Store hits: 58050
Store misses: 24475
Total cycles: 9536698

./csim 1 8192 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219742
Load misses: 926
Store hits: 72028
Store misses: 10497
Total cycles: 6153467

./csim 1 8192 16 write-allocate write-through lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219742
Load misses: 926
Store hits: 72028
Store misses: 10497
Total cycles: 12074267

/csim 1 8192 16 no-write-allocate write-through lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 218148
Load misses: 2520
Store hits: 58050
Store misses: 24475
Total cycles: 9536698



***Direct Mapped 16-byte slots (note lru/fifo doesn't matter with direct mapped)

./csim 8192 1 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219625
Load misses: 1043
Store hits: 72012
Store misses: 10513
Total cycles: 6258950

./csim 8192 1 16 write-allocate write-through lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219625
Load misses: 1043
Store hits: 72012
Store misses: 10513
Total cycles: 12125750

./csim 8192 1 16 no-write-allocate write-through lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 218124
Load misses: 2544
Store hits: 58049
Store misses: 24476
Total cycles: 9546273



***Tests of 16-byte write-allocate, write-back, lru caches

./csim 4096 2 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219700
Load misses: 968
Store hits: 72020
Store misses: 10505
Total cycles: 6199825

./csim 2048 4 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219730
Load misses: 938
Store hits: 72027
Store misses: 10498
Total cycles: 6157855

./csim 1024 8 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219733
Load misses: 935
Store hits: 72027
Store misses: 10498
Total cycles: 6157458

./csim 512 16 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219741
Load misses: 927
Store hits: 72028
Store misses: 10497
Total cycles: 6153866

./csim 256 32 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219742
Load misses: 926
Store hits: 72028
Store misses: 10497
Total cycles: 6153467

./csim 128 64 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219742
Load misses: 926
Store hits: 72028
Store misses: 10497
Total cycles: 6153467

./csim 64 128 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219742
Load misses: 926
Store hits: 72028
Store misses: 10497
Total cycles: 6153467

./csim 32 256 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219742
Load misses: 926
Store hits: 72028
Store misses: 10497
Total cycles: 6153467

./csim 16 512 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219742
Load misses: 926
Store hits: 72028
Store misses: 10497
Total cycles: 6153467

./csim 8 1024 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219742
Load misses: 926
Store hits: 72028
Store misses: 10497
Total cycles: 6153467

./csim 4 2048 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219742
Load misses: 926
Store hits: 72028
Store misses: 10497
Total cycles: 6153467

./csim 2 4096 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219742
Load misses: 926
Store hits: 72028
Store misses: 10497
Total cycles: 6153467



***Tests of 256-set write-allocate, write-back, lru caches

./csim 256 128 4  write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219148
Load misses: 1520
Store hits: 61677
Store misses: 20848
Total cycles: 2922373

./csim 256 64 8  write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219354
Load misses: 1314
Store hits: 64023
Store misses: 18502
Total cycles: 5055279

./csim 256 32 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219742
Load misses: 926
Store hits: 72028
Store misses: 10497
Total cycles: 6153467

./csim 256 16 32 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 220069
Load misses: 599
Store hits: 76718
Store misses: 5807
Total cycles: 7256194

./csim 256 8 64 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 220257
Load misses: 411
Store hits: 79460
Store misses: 3065
Total cycles: 8107582

./csim 256 64 8  write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219354
Load misses: 1314
Store hits: 64023
Store misses: 18502
Total cycles: 5055279



***Tests of 32-slot write-allocate, write-back, lru caches

./csim 1024 32 4 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219148
Load misses: 1520
Store hits: 61677
Store misses: 20848
Total cycles: 2922373

./csim 512 32 8 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219354
Load misses: 1314
Store hits: 64023
Store misses: 18502
Total cycles: 5055279

./csim 256 32 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219742
Load misses: 926
Store hits: 72028
Store misses: 10497
Total cycles: 6153467

./csim 128 32 32 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 220066
Load misses: 602
Store hits: 76718
Store misses: 5807
Total cycles: 7261791

./csim 64 32 64 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 220256
Load misses: 412
Store hits: 79461
Store misses: 3064
Total cycles: 8113981