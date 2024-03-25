Here is the experiment we conduct:
Fifo

Set associative case 1

./csim 128 2 4 write-allocate write-back fifo < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 288118
Load misses: 30079
Store hits: 162041
Store misses: 35445
Total cycles: 11420245

Hit Rate: 0.905471
Store Rate: 0.820519

./csim 128 2 4 no-write-allocate write-through fifo < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 286199
Load misses: 31998
Store hits: 149830
Store misses: 47656
Total cycles: 23384429

Hit Rate: 0.89944
Store Rate: 0.758687

./csim 128 2 4 write-allocate write-through fifo < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 288118
Load misses: 30079
Store hits: 162041
Store misses: 35445
Total cycles: 26751159

Hit Rate: 0.905471
Store Rate: 0.820519

Set associative case 2

./csim 64 4 4 write-allocate write-back fifo < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 295156
Load misses: 23041
Store hits: 162775
Store misses: 34711
Total cycles: 10508517

Hit Rate: 0.927589
Store Rate: 0.824236
./csim 64 4 4 no-write-allocate write-through fifo < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 293469
Load misses: 24728
Store hits: 150742
Store misses: 46744
Total cycles: 22665611

Hit Rate: 0.922287
Store Rate: 0.763305

./csim 64 4 4 write-allocate write-through fifo < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 295156
Load misses: 23041
Store hits: 162775
Store misses: 34711
Total cycles: 25981731

Hit Rate: 0.927589
Store Rate: 0.824236

Direct Mapping

./csim 256 1 4 write-allocate write-back fifo < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 263687
Load misses: 54510
Store hits: 153611
Store misses: 43875
Total cycles: 16147184

Hit Rate: 0.828691
Store Rate: 0.777832

./csim 256 1 4 no-write-allocate write-through fifo < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 263444
Load misses: 54753
Store hits: 142398
Store misses: 55088
Total cycles: 25629742

Hit Rate: 0.827927
Store Rate: 0.721054

./csim 256 1 4 write-allocate write-through fifo < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 263687
Load misses: 54510
Store hits: 153611
Store misses: 43875
Total cycles: 30004398

Hit Rate: 0.828691
Store Rate: 0.777832

Fully associative

./csim 1 256 4 write-allocate write-back fifo < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 301858
Load misses: 16339
Store hits: 165349
Store misses: 32137
Total cycles: 9147193

Hit Rate: 0.948651
Store Rate: 0.837269

./csim 1 256 4 no-write-allocate write-through fifo < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 300841
Load misses: 17356
Store hits: 153851
Store misses: 43635
Total cycles: 21938892

Hit Rate: 0.945455
Store Rate: 0.779048

./csim 1 256 4 write-allocate write-through fifo < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 301858
Load misses: 16339
Store hits: 165349
Store misses: 32137
Total cycles: 25063407

Hit Rate: 0.948651
Store Rate: 0.837269


lru

Set associative case 1

./csim 128 2 4 write-allocate write-back lru< input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 291761
Load misses: 26436
Store hits: 163375
Store misses: 34111
Total cycles: 10634022

Hit Rate: 0.916919
Store Rate: 0.827274

./csim 128 2 4 no-write-allocate write-through lru < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 289237
Load misses: 28960
Store hits: 151955
Store misses: 45531
Total cycles: 23085792

Hit Rate: 0.908987
Store Rate: 0.769447

./csim 128 2 4 write-allocate write-through lru < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 291761
Load misses: 26436
Store hits: 163375
Store misses: 34111
Total cycles: 26258436

Hit Rate: 0.916919
Store Rate: 0.827274

Set associative case 2

./csim 64 4 4 write-allocate write-back lru < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 300101
Load misses: 18096
Store hits: 164798
Store misses: 32688
Total cycles: 9371785

Hit Rate: 0.94313
Store Rate: 0.834479

./csim 64 4 4 no-write-allocate write-through lru < input/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 297735
Load misses: 20462
Store hits: 153917
Store misses: 43569
Total cycles: 22246452

Hit Rate: 0.935694
Store Rate: 0.779382

./csim 64 4 4 write-allocate write-through lru < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 300101
Load misses: 18096
Store hits: 164798
Store misses: 32688
Total cycles: 25291899

Hit Rate: 0.94313
Store Rate: 0.834479

Direct Mapping

./csim 256 1 4 write-allocate write-back lru < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 263687
Load misses: 54510
Store hits: 153611
Store misses: 43875
Total cycles: 16147184

Hit Rate: 0.828691
Store Rate: 0.777832

./csim 256 1 4 no-write-allocate write-through lru < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 263444
Load misses: 54753
Store hits: 142398
Store misses: 55088
Total cycles: 25629742

Hit Rate: 0.827927
Store Rate: 0.721054

./csim 256 1 4 write-allocate write-through lru < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 263687
Load misses: 54510
Store hits: 153611
Store misses: 43875
Total cycles: 30004398

Hit Rate: 0.828691
Store Rate: 0.777832

Fully associative

./csim 1 256 4 write-allocate write-back lru < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 307414
Load misses: 10783
Store hits: 167494
Store misses: 29992
Total cycles: 7942194

Hit Rate: 0.966112
Store Rate: 0.848131

./csim 1 256 4 no-write-allocate write-through lru < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 304782
Load misses: 13415
Store hits: 157960
Store misses: 39526
Total cycles: 21552842

Hit Rate: 0.957841
Store Rate: 0.799854

./csim 1 256 4 write-allocate write-through lru < input/gcc.trace

Total loads: 318197
Total stores: 197486
Load hits: 307414
Load misses: 10783
Store hits: 167494
Store misses: 29992
Total cycles: 24301008

Hit Rate: 0.966112
Store Rate: 0.848131


This is the experiment we conducted for our cache simulator. Basically, we try a written approach combining with LRU and FIFO respectively, and test these combinations
on a fully associative, two-set associative cache with different set numbers, and direct mapping cache structure. The size of the cache we used is 1 MB. Overall, we found that the direct mapping way of organizing the cache
has the lowest hit rate for both loading and storing, implicating more loading and writing to memory and more cycles. The overall cycle count and hit rate for loading and
storing are similar for fully associative cache and set associative cache, with fully associative cache having a slightly higher hit and lower cycle. However, the running time
for set associative (both) is faster than fully associative, as checking the validity of the block (sequential access in the case, as we did not use a map) is significantly faster
in a set with less block. For two ways of replacing block, LRU has an overall lower cycle and higher hit rate than FIFO. For three ways of storage handling, the combination of 
write allocate and write back is superior, having a higher rate than no-write-allocate & write through, and significantly less cycle than write allocate & write through (about
50 %). Overall, we think the combination of write-allocate & write-back back & LRU with a set associative cache is better. For specific set numbers in the set associative cache, we think
there is great flexibility here (also an advantage of set associative). If we have more sets, we will have faster searching time while more miss and cycle. If we have fewer
sets, we will have less miss and cycle, while experiencing longer search time. The choice should be made based on the application scenario and cache size.

