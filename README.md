# BurstBenchmarks
I was curious how well Burst optimizes C# code against GCC with C, so I ported four famous benchmarks and a raytracer with different workloads and made them identical between the two. C code compiled with all possible optimizations using `-DNDEBUG -Ofast -march=native -flto` compiler options. Benchmarks were done on Windows 10 w/ AMD FX-4300 (4GHz) using standalone build. Mono JIT is included for fun.

Burst 1.1.1<br/>
GCC 8.1.0

|          | Fibonacci         | Mandelbrot        | NBody             | Sieve of Eratosthenes | Pixar Raytracer     |
|----------|-------------------|-------------------|-------------------|-----------------------|----------------------
| Burst    | 95,390,206 ticks  | 38,096,272 ticks  | 122,758,625 ticks | 43,365,134 ticks      | 238,603,517 ticks   |
| GCC      | 103,722,611 ticks | 28,831,784 ticks  | 203,402,512 ticks | 48,180,793 ticks      | 70,737,868 ticks    |
| Clang    | 92,325,076 ticks  | 40,857,282 ticks  | 114,271,368 ticks | 47,197,651 ticks      | 88,329,427 ticks    |
| IL2CPP   | 157,996,002 ticks | 112,912,355 ticks | 240,394,952 ticks | 49,242,879 ticks      | 316,060,369 ticks   |
| Mono JIT | 195,402,532 ticks | 122,758,625 ticks | 330,030,966 ticks | 57,577,577 ticks      | 1,386,450,841 ticks |

### Notes
Suppressing the generation of static unwind tables for exception handling with GCC using `-fno-asynchronous-unwind-tables` leads to better performance in the recursive Fibonacci test: 84,983,484 ticks. All other tests remain unaffected.

### Discussion
Feel free to join the discussion in the [thread](https://forum.unity.com/threads/benchmarking-burst-against-gcc-machine-code-fibonacci-mandelbrot-nbody.715133/) on Unity forums.
