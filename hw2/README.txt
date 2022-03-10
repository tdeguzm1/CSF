tdeguzm1
Theo DeGuzman

This project was completed individally.

All C and asm function work as expected with a comprehensive test suite.

I am using 25-26 late hours on this assignment, due to 4 midterms this past week
and an unforseen work emergency.  I have not used any late hours on any previous assignments.


TLDR
A point of note, I actually submitted a fully working (and correctly formatted) solution 
before the deadline.  The only problem was that I did not have asm main return 0 by setting %rax to zero.
This took about 6 hours to figure out, and its my fault, as I interpreted "your code exited with a non-zero value"
to mean that the ruby script driving the autograder exited with a non-zero (triggered by a faulty comparison).
Perhaps you could rename that error message "Your submitted main funciton did not return zero upon exit."
All to say, totally my fault, but if it weren't for that, I would have finished by the deadline.

To be honest, I actually learned a lot through that debuging process, so maybe it was a good thing in the long run.

Also casm_hexdump doesn't work because apparently the C complier sets rbp to the high end of the allocated space on the
stack and then references using negative indexes that get less negative, but in my asm functions, I set rbp to the low end
of the allocated space, and index using positive values that get larger.  The latter was the method taught in the lectures/powerpoints.
All in all though, asm_hexdum and c_hexdump work correctly with eachother, and casm_hexdump works if you allocate modify c_hexmain
to have very large arrays instead of properly sized ones. Because having a working casm_hexdump is not a requirement, I did not
fix this in my final submission, but I do want to acknowldge it and its possible solutions.

I also did not use any global variables or dynamic memory allocation (only stack allocation), 
which I am pretty proud of.