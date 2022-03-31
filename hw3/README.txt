This assignment was done individually, with all requirements for MS2 complete.

No bugs were found, and gradescope autograders were passed. Valgrind also passed.
Some opportunites for optimization are present and will be implemeted for MS3.
Also Fifo should work, even though LRU was the requirement for this milestone. 



Experimental results:

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