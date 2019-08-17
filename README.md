# BurstBenchmarks
I was curious how well Burst/IL2CPP optimizes C# code against GCC/Clang with C, so I ported five famous benchmarks, a raytracer, and a minified flocking simulation, with different workloads and made them identical between the two languages. C code compiled with all possible optimizations using `-DNDEBUG -Ofast -march=native -flto` compiler options. Benchmarks were done on Windows 10 w/ AMD FX-4300 (4GHz) using standalone build. Mono JIT is included for fun.

Burst 1.1.2<br/>
GCC 8.1.0<br/>
Clang (LLVM 8.0.1)<br/>
IL2CPP and Mono (Unity 2019.2.0f1)

## Integer math

|          | Fibonacci         | Sieve of Eratosthenes |
|----------|-------------------|-----------------------|
| Burst    | 95,527,914 ticks  | 43,738,580 ticks      |
| GCC      | 102,998,330 ticks | 47,215,659 ticks      |
| Clang    | 92,297,296 ticks  | 47,017,112 ticks      |
| IL2CPP   | 152,131,662 ticks | 48,402,961 ticks      |
| Mono JIT | 199,301,525 ticks | 56,503,823 ticks      |

## Single-precision math

|          | Mandelbrot        | Pixar Raytracer     | Fireflies Flocking  | Polynomials       | Particle Kinematics |
|----------|-------------------|---------------------|---------------------|-------------------|---------------------|
| Burst    | 37,951,741 ticks  | 345,809,604 ticks   | 184,693,828 ticks   | 52,312,299 ticks  | 190,654,223 ticks   |
| GCC      | 28,761,058 ticks  | 144,083,652 ticks   | 128,894,075 ticks   | 39,252,467 ticks  | 111,352,855 ticks   |
| Clang    | 40,431,972 ticks  | 163,787,850 ticks   | 152,781,861 ticks   | 26,659,845 ticks  | 193,759,862 ticks   |
| IL2CPP   | 97,272,960 ticks  | 282,197,013 ticks   | 383,393,328 ticks   | 46,888,288 ticks  | 185,309,296 ticks   |
| Mono JIT | 313,936,473 ticks | 2,851,363,418 ticks | 1,034,860,724 ticks | 172,780,244 ticks | 377,228,204 ticks   |

## Double-precision math

|          | NBody             |
|----------|-------------------|
| Burst    | 136,450,555 ticks |
| GCC      | 202,893,162 ticks |
| Clang    | 112,909,174 ticks |
| IL2CPP   | 236,041,412 ticks |
| Mono JIT | 326,669,135 ticks |

Notes
--------
Suppressing the generation of static unwind tables for exception handling with GCC using `-fno-asynchronous-unwind-tables` leads to better performance in the recursive Fibonacci test: 84,983,484 ticks. All other tests remain unaffected.

Discussion
--------
Feel free to join the discussion in the [thread](https://forum.unity.com/threads/benchmarking-burst-against-gcc-machine-code-fibonacci-mandelbrot-nbody.715133/) on Unity forums.
