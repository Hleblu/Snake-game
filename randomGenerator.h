#pragma once
#include <cstdint>

class RandomGenerator
{
private:
    static uint64_t rotl(const uint64_t x, int k) {
        return (x << k) | (x >> (64 - k));
    }

    static uint64_t s[4];

    static uint64_t splitmix64(uint64_t& z) {
        z += 0x9e3779b97f4a7c15;
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }

public:
    static void seed(uint64_t seed_val) {
        uint64_t z = seed_val;
        for (int i = 0; i < 4; i++) {
            s[i] = splitmix64(z);
        }
    }

    static uint64_t next() {
        const uint64_t result = s[0] + s[3];
        const uint64_t t = s[1] << 17;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;
        s[3] = rotl(s[3], 45);

        return result;
    }

    static int getInt(const int min, const int max) {
        return static_cast<int>(next() % (max - min + 1)) + min;
    }
};

inline uint64_t RandomGenerator::s[4] = { 
    0x123456789abcdef0,
    0xfedcba9876543210,
    0x0f0f0f0f0f0f0f0f,
    0x1e1e1e1e1e1e1e1e
};