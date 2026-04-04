#pragma once
#include <cstdint>

class RandomGenerator
{
private:
    static std::uint64_t rotl(const std::uint64_t x, int k) {
        return (x << k) | (x >> (64 - k));
    }

    static std::uint64_t s[4];

    static std::uint64_t splitmix64(std::uint64_t& z) {
        z += 0x9e3779b97f4a7c15;
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }

public:
    static void seed(std::uint64_t seed_val) {
        std::uint64_t z = seed_val;
        for (int i = 0; i < 4; i++) {
            s[i] = splitmix64(z);
        }
    }

    static std::uint64_t next() {
        const std::uint64_t result = s[0] + s[3];
        const std::uint64_t t = s[1] << 17;

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

    static float getFloat() {
        return (next() >> (64 - 24)) * 5.9604644775390625e-8f;
    }

    static float getFloat(const float min, const float max) {
        return getFloat() * (max - min) + min;
    }
};

inline std::uint64_t RandomGenerator::s[4] = {
    0x123456789abcdef0,
    0xfedcba9876543210,
    0x0f0f0f0f0f0f0f0f,
    0x1e1e1e1e1e1e1e1e
};