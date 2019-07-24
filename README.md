# BurstBenchmarks
I was curious how well Burst optimizes C# code against GCC with C, so I ported three famous benchmarks with different workloads and made them identical between the two. C code compiled with all possible optimizations using `-DNDEBUG -Ofast -march=native -flto` compiler options. Benchmarks were done on Windows 10 w/ AMD FX-4300 (4GHz) using standalone build. Mono JIT is included for fun.

Burst 1.1.1

GCC 8.1.0

|          | Fibonacci         | Mandelbrot        | NBody             |
|----------|-------------------|-------------------|-------------------|
| Burst    | 95,657,085 ticks  | 65,528,410 ticks  | 122,715,369 ticks |
| GCC      | 103,578,985 ticks | 28,788,322 ticks  | 203,429,182 ticks |
| Mono JIT | 195,152,736 ticks | 116,309,579 ticks | 330,834,294 ticks |

### Notes
Suppressing the generation of static unwind tables for exception handling with GCC using `-fno-asynchronous-unwind-tables` leads to better performance in the recursive Fibonacci test: 84,983,484 ticks. All other tests remain unaffected.

Eliminating nested stack allocations in the Mandelbrot test leads to better performance of GCC over Burst by 7%.

### Discussion
Feel free to join the discussion in the [thread](https://forum.unity.com/threads/benchmarking-burst-against-gcc-machine-code-fibonacci-mandelbrot-nbody.715133/) on Unity forums.
