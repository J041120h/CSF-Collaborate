CONTRIBUTIONS

TODO: write a brief summary of how each team member contributed to
the project.
Andy wrote the file memory mapping while Harry wrote the parallel sequencing. We work together on the README.

REPORT

TODO: add your report according to the instructions in the
"Experiments and analysis" section of the assignment description.

1.	Indicates the amount of time that your parsort program took so sort the test data for each threshold value, and States a reasonable explanation for why you saw the times you did:

real    0m0.736s
user    0m0.711s
sys     0m0.020s
Test run with threshold 1048576
Finished

real    0m0.578s
user    0m0.967s
sys     0m0.050s
Test run with threshold 524288
Finished

real    0m0.335s
user    0m0.809s
sys     0m0.079s
Test run with threshold 262144
Finished

real    0m0.274s
user    0m0.852s
sys     0m0.107s
Test run with threshold 131072
Finished

real    0m0.254s
user    0m0.968s
sys     0m0.148s
Test run with threshold 65536
Finished

real    0m0.224s
user    0m1.008s
sys     0m0.219s
Test run with threshold 32768
Finished

real    0m0.241s
user    0m1.072s
sys     0m0.308s
Test run with threshold 16384
Finished

real    0m0.249s
user    0m1.139s
sys     0m0.411s

As the time decreases with the decreasing threshold, our results align with the expectation. 
This is because when the threshold is smaller, which means the merge-sort won’t enter seq-sort until a smaller threshold, more subsequent processes will be created. 
Therefore, we will make more effective use of the CPU, decreasing operating time.

2. States a reasonable explanation for why you saw the times you did
In our case, multi-processing has increased the efficiency of the sorting, and increase the number of process in parallel will further
increase the efficiency. To start with, the mmap we used created a direct mapping betwwen memory and the file that can be shared between
processes, without requiring further copy or I/O, so increase process number won't have that much extra cost. In the program, tasks are 
finally divided in a way such that each sequential sort is runned in a separate process. As we are constantly using binary divison of array,
the data block that each process read from and write to is different, avoiding data dependency between them and allowing OS kernel to freely
schedule those processes in parallel on multiple CPU cores. Overall, the program divide the whole task into independent subtasks and separately
finish those task with multiple process, saving running time for the most tedious part of the whole task.

