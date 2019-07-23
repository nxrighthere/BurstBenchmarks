# BurstBenchmarks
I was curious how well Burst optimizes C# code against GCC with C, so I ported three famous benchmarks with different workloads and made them identical between the two. C code compiled with all possible optimizations using `-DNDEBUG -Ofast -march=native -flto` compiler options. Benchmarks were done on Windows 10 w/ AMD FX-4300 (4GHz) using standalone build. Mono JIT is included for fun.

Burst 1.1.1

GCC 8.1.0

|          | Fibonacci       | Mandelbrot      | NBody           |
|----------|-----------------|-----------------|-----------------|
| Burst    | 95657085 ticks  | 65528410 ticks  | 122715369 ticks |
| GCC      | 103578985 ticks | 28788322 ticks  | 203429182 ticks |
| Mono JIT | 195152736 ticks | 116309579 ticks | 330834294 ticks |
