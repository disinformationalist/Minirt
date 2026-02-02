#ifndef XORO128_H
#define XORO128_H

#include <stdint.h>

typedef struct {
    uint64_t state[2];
    uint64_t splitmix_seed;
} Xoro128;

// Rotate left
static inline uint64_t rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

// SplitMix64 seeding
static inline uint64_t splitmix64(Xoro128* xoro) {
    uint64_t z = (xoro->splitmix_seed += 0x9E3779B97F4A7C15);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EB;
    return z ^ (z >> 31);
}

// Seed the RNG
static inline void sxoro128(Xoro128* xoro, uint64_t seed) {
    xoro->splitmix_seed = seed;
    xoro->state[0] = splitmix64(xoro);
    xoro->state[1] = splitmix64(xoro);
}

// Generate a random uint64_t
static inline uint64_t xoro128(Xoro128* xoro) {
    uint64_t s0 = xoro->state[0];
    uint64_t s1 = xoro->state[1];
    uint64_t result = s0 + s1;

    s1 ^= s0;
    xoro->state[0] = rotl(s0, 55) ^ s1 ^ (s1 << 14);
    xoro->state[1] = rotl(s1, 36);

    return result;
}

// Generate a double in [0,1)
static inline double xoro128d(Xoro128* xoro) {
    return (xoro128(xoro) >> 11) * (1.0 / (1ULL << 53));
}

#endif // XORO128_H

