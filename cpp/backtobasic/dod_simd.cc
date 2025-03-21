When a CPU executes an instruction, it may need to:

- Fetch the instruction itself - This comes from memory (RAM or cache).
- Fetch data required by the instruction - Variables or operands stored in memory.
- Store results - Writing back computed results.

The CPU first checks whether the instruction and its required data are in the cache. If they are:

- It fetches them quickly, improving execution speed.
- This reduces the need to access slower RAM.

If the instruction or data is not in the cache (cache miss):

- The CPU fetches them from RAM (which is slower).
- The cache stores this newly fetched data for future use.

What is a Cache Line?
A cache line is the smallest unit of data that can be transferred between the CPU cache and main memory (RAM). When the CPU accesses memory, it does not fetch a single byte or word; instead, it loads an entire cache line, typically 64 bytes on modern processors.

Each cache line contains:
+Tag: Identifies which memory address is stored.
+Data: The actual data (e.g., 64 bytes).
+Metadata: Information like validity and usage bits.

This design exploits spatial locality, meaning that if one part of the cache line is accessed, nearby data is likely to be needed soon.

Solutions:

1. Spatial & Temporal Locality
Modern caches exploit locality of reference:
-Temporal locality: Recently accessed data is likely to be accessed again soon.
    Example: Loop counters or frequently used variables.

-Spatial locality: Nearby memory locations are likely to be accessed together.

    Example: Iterating over an array.

    #include <iostream>
    #include <vector>
    #include <chrono>
    #include <cstdlib>
    
    const int SIZE = 10'000'000;  // Large array size
    std::vector<int> arr(SIZE, 1); // Initialize array with 1s
    
    // Sequential access (cache-friendly)
    void sequentialAccess() {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < SIZE; i++) {
            arr[i] += 1;
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Sequential Access Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms\n";
    }
    
    // Random access (cache-inefficient)
    void randomAccess() {
        std::vector<int> indices(SIZE);
        for (int i = 0; i < SIZE; i++) indices[i] = rand() % SIZE;  // Generate random indices
    
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < SIZE; i++) {
            arr[indices[i]] += 1;
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Random Access Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms\n";
    }
    
    int main() {
        sequentialAccess();
        randomAccess();
        return 0;
    }

nnlong299 ~/practice/utils/cpp (main) $ ./a
Sequential Access Time: 22 ms
Random Access Time: 203 ms    

2. Cache Line Size (Aligning Data in Memory)
-Data is fetched in fixed-size blocks called cache lines.
-When a memory location is accessed, the entire cache line is loaded into the cache.
-This improves performance by exploiting spatial locality.

3. Struct of Arrays (SoA) DOD (Data-Oriented Design) vs Array of Structs (AoS) OOP (Object-Oriented Programming)

Bad:
#include <iostream>
#include <vector>
#include <chrono>

// Traditional Object-Oriented Approach (AoS)
struct Entity {
    float x, y, z;
    int health;
};

std::vector<Entity> entities(10'000'000); // Large dataset

void processAoS() {
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < entities.size(); i++) {
        entities[i].x += 1.0f; // Accessing x
        entities[i].y += 1.0f; // Accessing y
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "AoS Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
}

int main() {
    processAoS();
    return 0;
}

Good:
#include <iostream>
#include <vector>
#include <chrono>

// Data-Oriented Approach (SoA)
struct Entities {
    std::vector<float> x, y, z;
    std::vector<int> health;

    Entities(size_t size) {
        x.resize(size, 0.0f);
        y.resize(size, 0.0f);
        z.resize(size, 0.0f);
        health.resize(size, 100);
    }
};

Entities entities(10'000'000);

void processSoA() {
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < entities.x.size(); i++) {
        entities.x[i] += 1.0f; // Accessing x
        entities.y[i] += 1.0f; // Accessing y
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "SoA Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
}

int main() {
    processSoA();
    return 0;
}


4. SIMD (Single Instruction, Multiple Data)
-Modern CPUs support SIMD instructions.
-These instructions operate on multiple data elements simultaneously.
-Example: Intel SSE (Streaming SIMD Extensions), AVX (Advanced Vector Extensions).

sudo apt install libomp-dev
g++ -mavx2 -fopenmp  a.cc -o a 

#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h>  // AVX2 intrinsics

// Structure of Arrays (SoA)
struct Entities {
    std::vector<float> x, y, z;
    std::vector<int> health;

    Entities(size_t size) {
        x.resize(size, 0.0f);
        y.resize(size, 0.0f);
        z.resize(size, 0.0f);
        health.resize(size, 100);
    }
};

Entities entities(10'000'000);

// SIMD Optimized Processing
void processSoA_SIMD() {
    size_t size = entities.x.size();
    auto start = std::chrono::high_resolution_clock::now();

    // Process in chunks of 8 (256-bit AVX registers hold 8 floats)
    size_t i = 0;
    for (; i + 8 <= size; i += 8) {
        __m256 x_vec = _mm256_loadu_ps(&entities.x[i]); // Load 8 floats
        __m256 y_vec = _mm256_loadu_ps(&entities.y[i]);

        __m256 add_val = _mm256_set1_ps(1.0f); // Vector with all elements = 1.0

        x_vec = _mm256_add_ps(x_vec, add_val); // x += 1
        y_vec = _mm256_add_ps(y_vec, add_val); // y += 1

        _mm256_storeu_ps(&entities.x[i], x_vec); // Store back
        _mm256_storeu_ps(&entities.y[i], y_vec);
    }

    // Process remaining elements
    for (; i < size; i++) {
        entities.x[i] += 1.0f;
        entities.y[i] += 1.0f;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "SoA SIMD Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
}

int main() {
    processSoA_SIMD();
    return 0;
}

5. SIMD + OpenMP

#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h>  // For AVX2
#include <omp.h>        // For OpenMP

const int SIZE = 10'000'000; // 10M elements

std::vector<float> A(SIZE, 1.0f);
std::vector<float> B(SIZE, 2.0f);
std::vector<float> C(SIZE, 0.0f);

// SIMD + Multi-threaded AVX2 addition
void vector_add_simd_omp() {
    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for  // OpenMP multi-threading
    for (int i = 0; i < SIZE; i += 8) {
        __m256 a_vec = _mm256_loadu_ps(&A[i]);  // Load 8 floats from A
        __m256 b_vec = _mm256_loadu_ps(&B[i]);  // Load 8 floats from B
        __m256 c_vec = _mm256_add_ps(a_vec, b_vec); // C = A + B (vectorized)

        _mm256_storeu_ps(&C[i], c_vec);  // Store result back to C
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "SIMD + OpenMP Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
}

// Baseline scalar (non-SIMD, non-parallel) version
void vector_add_scalar() {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < SIZE; i++) {
        C[i] = A[i] + B[i];  // Normal addition
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Scalar (No SIMD, No OpenMP) Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
}

int main() {
    vector_add_scalar();     // Slow version
    vector_add_simd_omp();   // Fast version with SIMD + OpenMP
    return 0;
}


6. std::simd c++23

#include <iostream>
#include <vector>
#include <experimental/simd>  // Use std::simd (C++23 or experimental)

namespace stdx = std::experimental;  // Alias for convenience

const int SIZE = 16;

void vector_add_simd() {
    alignas(64) float A[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    alignas(64) float B[SIZE] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    alignas(64) float C[SIZE];

    using simd_t = stdx::native_simd<float>;  // SIMD type for native SIMD width
    constexpr int stride = simd_t::size();    // How many floats fit in one SIMD register

    for (int i = 0; i < SIZE; i += stride) {
        simd_t a = stdx::load<simd_t>(&A[i]);  // Load SIMD vector
        simd_t b = stdx::load<simd_t>(&B[i]);
        stdx::store(&C[i], a + b);             // Perform vectorized addition and store
    }

    for (float f : C) std::cout << f << " ";  // Print results
}

int main() {
    vector_add_simd();
    return 0;
}
