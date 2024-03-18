Bennett Ehret
1746849

I worked with Reese Nelson on pi_par.c and pi_seq.c, mainly on debugging and idea generation. Most of the idea sharing came with the parallelizing, where we got a little stumped as to why we would get different results when trying the same number of terms with different threads. We decided this might have just been floating point error, however there may be a flaw in that part of the algorithm.

I additionally noticed that my monte_par.c was had a significantly slower runtime than the sequential version. I do believe that my algorithm is correct, i'm not sure why i would be getting such a slower runtime.