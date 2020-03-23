# BurstBenchmarks
I was curious how well Burst/IL2CPP optimizes C# code against GCC/Clang with C, so I've ported five famous benchmarks, plus a raytracer, a minified flocking simulation, particle kinematics, a stream cipher, a hashing algorithm, and radix sort, with different workloads and made them identical between the two languages. C code compiled with all possible optimizations using `-DNDEBUG -Ofast -march=native -flto` compiler options. Benchmarks were done on Windows 10 w/ Ryzen 5 1400 using standalone build. Mono JIT and RyuJIT are included for fun.

This project is [donated](https://github.com/nxrighthere/BurstBenchmarks/pull/1) to Unity's Burst compiler team as a performance test-suite to identify inconsistencies in the generated machine code in comparison to other compilers.

Burst 1.2.3<br/>
GCC 9.2.0<br/>
Clang (LLVM 9.0.1)<br/>
IL2CPP and Mono JIT (Unity 2019.3.2f1)<br/>
RyuJIT (.NET Core 3.1.101)

## Integer math

<p align="center"> 
  <img src="https://i.imgur.com/oq0X5G5.png" alt="Fibonacci">
  <img src="https://i.imgur.com/YwhPdEy.png" alt="Sieve of Eratosthenes">
  <img src="https://i.imgur.com/8wMZwJO.png" alt="Arcfour">
  <img src="https://i.imgur.com/9lwLAca.png" alt="Seahash">
  <img src="https://i.imgur.com/1nWJKk3.png" alt="Radix">
</p>

## Single-precision math

<p align="center"> 
  <img src="https://i.imgur.com/WdOoWN4.png" alt="Mandelbrot">
  <img src="https://i.imgur.com/hoyWBPh.png" alt="Pixar Raytracer">
  <img src="https://i.imgur.com/NhCzjDA.png" alt="Fireflies Flocking">
  <img src="https://i.imgur.com/DoJioB8.png" alt="Polynomials">
  <img src="https://i.imgur.com/IdVjMo3.png" alt="Particle Kinematics">
</p>

## Double-precision math

<p align="center"> 
  <img src="https://i.imgur.com/JnhjPm9.png" alt="NBody">
</p>

Discussion
--------
You can find the discussion in the [thread](https://forum.unity.com/threads/benchmarking-burst-against-gcc-machine-code-fibonacci-mandelbrot-nbody.715133/) on Unity forums.

Supporters
--------
These wonderful people make open-source better:
<p align="left"> 
  <img src="https://github.com/Rageware/Supporters/blob/master/burstbenchmarks-supporters.png" alt="supporters">
</p>

This project is sponsored by [JetBrains](https://www.jetbrains.com/?from=BurstBenchmarks).<br/>
<a href="https://www.jetbrains.com/?from=BurstBenchmarks"><img src="https://i.imgur.com/zLguWAH.png" alt="Mandelbrot"></a>
