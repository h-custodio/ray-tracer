# CPU Ray Tracer

A from-scratch C++23 CPU ray tracer focused on physically-inspired rendering, object/material abstraction, multithreaded rendering, SIMD-aware vector math, and performance analysis.

The project started as a straightforward single-threaded ray tracer and was iteratively optimized using profiling and low-level performance analysis. The final implementation combines recursive ray tracing with **49 worker threads** and SIMD-backed vector operations, reducing render time from approximately **1069 s to 173 s** on the development machine — a **~6.17× speedup**.

> **Key engineering focus:** understanding where CPU time is actually spent, profiling the hot path, and applying parallelism and low-level optimization based on measured bottlenecks rather than assumptions.

---

## Overview

This renderer generates a procedurally constructed scene containing hundreds of spheres with diffuse, metallic, and dielectric materials. Rays are sampled from a configurable camera and recursively traced through the scene to simulate reflection, refraction, depth of field, and indirect light transport.

The renderer outputs the final framebuffer as a binary **PPM (P6)** image.

The project was developed with a particular emphasis on the performance characteristics of CPU-based rendering:

- Profiling with Linux `perf`
- Multithreaded image rendering
- SIMD-backed vector operations
- Cache and branch-miss analysis
- Recursive ray tracing
- Object-oriented scene/material abstractions
- Runtime instrumentation and benchmark comparison

---

## Features

### Rendering

- Recursive path/ray tracing
- Configurable maximum ray depth
- Configurable samples per pixel
- Anti-aliasing through randomized pixel sampling
- Sky gradient background
- Gamma correction
- Binary PPM (`P6`) output

### Materials

Three material models are implemented:

- **Lambertian** — diffuse surfaces using randomized scattering
- **Metal** — reflective surfaces with configurable fuzziness
- **Dielectric** — glass-like materials supporting reflection and refraction

The dielectric implementation uses **Schlick's approximation** to probabilistically determine reflection versus refraction.

### Camera

The camera supports:

- Configurable aspect ratio
- Vertical field of view
- Camera position and look-at target
- Camera-relative up vector
- Configurable focus distance
- Defocus blur / depth of field

### Scene

The default scene is procedurally generated and contains:

- A large ground sphere
- A randomized field of small spheres
- Diffuse, metallic, and glass materials
- Three larger feature spheres with distinct materials

---

## Performance Engineering

Performance was a major part of this project rather than an afterthought.

The renderer was first profiled as a single-threaded CPU implementation. The initial benchmark used:

| Configuration | Average Render Time | Speedup |
| --- | ---: | ---: |
| Baseline | **1069.16 s** | — |
| Multithreading — 28 threads | **182.71 s** | **5.85×** |
| Multithreading — 49 threads | **177.10 s** | **6.04×** |
| Multithreading + SIMD-backed `Vec3` | **173.15 s** | **6.17×** |

Benchmarks were averaged across four runs on the development machine.

### Profiling the bottleneck

The original `perf` profile showed that the majority of runtime was concentrated in the ray/object intersection path:

```text
Sphere::hit()              ~68%
HittableList::hit()        ~29%
Renderer::render()          ~1%
```

This was significant because the renderer can invoke intersection testing billions of times over the course of a high-sample render.

Rather than prematurely optimizing unrelated code, the optimization process focused on this hot path.

### Multithreading

The rendering workload is naturally parallelizable because pixels can be rendered independently.

The image was partitioned into row ranges and distributed across worker threads. Each worker produces its own framebuffer segment, which is merged after all workers complete.

Testing different worker counts showed that simply matching the processor count was not optimal. A configuration around **1.75× the logical processor count** performed best in the tested environment, likely because finer-grained partitioning helped distribute uneven workloads across the scene.

### SIMD-aware vector math

The `Vec3` implementation was redesigned around a padded 4-component representation backed by Agner Fog's **Vector Class Library (`Vec4f`)**.

This was intended to improve the compiler's ability to use SIMD-friendly operations for vector arithmetic occurring heavily inside intersection calculations.

The SIMD-backed implementation provided a smaller but measurable improvement after multithreading:

**177.10 s → 173.15 s**

The optimization also demonstrated an important tradeoff: padding the vector representation increased L1 data-cache pressure. This made the change a useful case study in how an optimization at the instruction level can introduce memory-system costs.

---

## Performance Analysis

The included [`Ray Tracer Optimization Report.pdf`](./Ray%20Tracer%20Optimization%20Report.pdf) documents the optimization process in detail, including profiling data, benchmark methodology, cache/branch analysis, and the reasoning behind the implemented changes.

### Environment used for benchmarking

- **CPU:** Intel Core i7-14700F
- **P-Cores:** 8 cores / 16 threads
- **E-Cores:** 12 cores / 12 threads
- **Total logical processors:** 28
- **Instruction set:** AVX2
- **RAM:** 32 GB
- **OS:** Windows 11
- **Architecture:** x86-64

### Rendering configuration

The benchmarked scene used:

- Resolution: **1200 × 675**
- Aspect ratio: **16:9**
- Samples per pixel: **500**
- Maximum ray depth: **50**
- Vertical FOV: **20°**
- Defocus angle: **0.6°**
- Focus distance: **10**

The renderer's output is intentionally computationally expensive, making the performance work measurable.

---

## Architecture

The renderer is split into small components with clear responsibilities:

```text
src/
├── main.cpp        # Program entry point, scene setup, threading, timing
├── render.h        # Rendering loop and recursive ray-color calculation
├── camera.h        # Camera geometry, viewport, sampling, defocus
├── ray.h           # Ray representation and ray evaluation
├── vec3.h          # Vector/point math and SIMD-backed operations
├── color.h         # Color representation, gamma correction, PPM output
├── hittable.h      # Scene-object interface and hit records
├── sphere.h        # Sphere-ray intersection implementation
├── material.h      # Lambertian, Metal, and Dielectric materials
├── interval.h      # Ray-intersection intervals and clamping
├── utils.h         # Constants, random generation, math utilities
└── external/
    └── version2-master/
        └── ...     # Vector Class Library
```

### Core rendering flow

```text
Camera
  │
  ├── Generate sampled ray
  │
  ▼
Renderer::ray_color()
  │
  ▼
HittableList::hit()
  │
  ├── Sphere::hit()
  │
  └── Material::scatter()
          │
          ├── Lambertian
          ├── Metal
          └── Dielectric
          │
          ▼
     Recursive ray
```

This separation makes the core rendering pipeline easy to extend with additional geometry and material types.

---

## Technical Details

### Ray-sphere intersection

Sphere intersections are calculated using the quadratic equation, with the implementation using a simplified form of the quadratic roots to identify the closest valid intersection.

### Recursive ray tracing

When a ray intersects an object, the material determines the behavior of the next ray:

- Diffuse surfaces scatter in a randomized hemisphere
- Metals reflect the incoming ray with configurable roughness
- Dielectrics choose between reflection and refraction

Recursion terminates when:

1. The ray misses the scene, or
2. The configured maximum ray depth is reached.

### Anti-aliasing

Each pixel is sampled multiple times with randomized offsets inside the pixel. The accumulated samples are averaged before being written to the framebuffer.

### Depth of field

When defocus is enabled, ray origins are randomly sampled from a camera defocus disk rather than originating from a single point. This produces a depth-of-field effect while preserving a common focal plane.

---

## Building

This project requires a compiler with **C++23** support.

### GCC / Clang

From the `src` directory:

```bash
g++ -std=c++23 -Wall -Wextra -pedantic -O3 -march=native -g main.cpp -o raytracer
```

Then run:

```bash
./raytracer
```

On Windows with a compatible GCC toolchain:

```bash
g++ -std=c++23 -Wall -Wextra -pedantic -O3 -march=native -g main.cpp -o raytracer.exe
```

Then:

```bash
./raytracer.exe
```

### Compiler flags

| Flag | Purpose |
| --- | --- |
| `-std=c++23` | Enables C++23 language features |
| `-Wall -Wextra -pedantic` | Enables additional compiler diagnostics |
| `-O3` | Enables aggressive compiler optimization |
| `-march=native` | Enables CPU-specific instruction sets |
| `-g` | Includes debugging/profiling information |

`-march=native` is particularly relevant to this project because the vector implementation and performance experiments are intended to take advantage of the capabilities of the machine running the renderer.

---

## Output

Running the program produces:

```text
display.ppm
```

The image is written using the binary PPM `P6` format.

Most image viewers do not handle PPM particularly well, so the output can be converted to a more common format such as PNG using ImageMagick:

```bash
magick display.ppm display.png
```

---

## Benchmarking

The program includes runtime instrumentation for:

- Initialization
- Scene setup
- Rendering
- Output generation
- Total execution time

It also reports the approximate fraction of total runtime spent in each phase.

Example output:

```text
Initialize Time:    ...
Scene Setup Time:   ...
Render Time:        ...
Write Output Time:  ...
Total Time:         ...

========== Runtime Fractions (Approximate) ==========
Initialization Fraction: ...
Scene Setup Fraction:    ...
Render Fraction:         ...
Write Fraction:          ...
```

For lower-level profiling on Linux, compile with debugging symbols and run:

```bash
perf stat ./raytracer
```

A detailed optimization report is included in the repository for the benchmark results and profiling methodology.

---

## Design Decisions

### Why CPU rendering?

The project was intentionally implemented as a CPU renderer to explore the relationship between algorithmic workload, compiler optimization, multithreading, memory access, and SIMD execution.

### Why static row partitioning?

Rendering individual pixels is independent, making the image naturally divisible into row ranges. Static partitioning provides a simple synchronization model while avoiding shared writes to the same framebuffer locations.

Each worker owns its framebuffer segment and the main thread combines the segments after joining all workers.

### Why `shared_ptr`?

Scene objects and materials are represented through polymorphic interfaces. `std::shared_ptr` provides straightforward ownership semantics for objects stored behind the `Hittable` and `Material` interfaces.

The performance report also investigates the potential cache implications of this representation.

---

## Project Background

This project was developed as an independent implementation based on the concepts presented in **Ray Tracing in One Weekend**.

The implementation goes beyond a basic tutorial renderer by using the project as a vehicle for studying:

- CPU performance profiling
- Parallel workload decomposition
- SIMD-aware data representation
- Cache behavior
- Compiler optimization
- Recursive rendering algorithms
- Object-oriented C++ design

The included optimization report documents the progression from the initial baseline to the optimized implementation.

---

## Future Improvements

There are several directions for improving both rendering quality and performance:

- Bounding Volume Hierarchies (BVH) to reduce unnecessary intersection tests
- More efficient scene/object storage
- Improved work scheduling and dynamic load balancing
- Further SIMD/vectorization experiments
- Alternative memory layouts such as Structure of Arrays (SoA)
- More advanced sampling strategies
- Additional geometric primitives
- More physically accurate light transport
- Additional output formats
- Automated performance regression benchmarks

The most significant algorithmic opportunity is a **BVH**, which could reduce the number of sphere intersection tests substantially and address the current `HittableList` linear traversal bottleneck.

---

## References

- **Ray Tracing in One Weekend** — Peter Shirley
- **Vector Class Library** — Agner Fog
- Linux `perf` performance analysis tools

---

## License

See [`LICENSE`](./LICENSE) for licensing information.
