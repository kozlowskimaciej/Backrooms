#ifndef RANDOM_GENERATOR
#define RANDOM_GENERATOR

#include <cstdint>
#include <random>
#include <cassert>

class RandomGenerator
{
public:
    RandomGenerator(int baseSeed);

    void seed(int x, int y);
    bool drawLots(unsigned int chance, unsigned int max);
    bool drawLots(double chance, double max);
    void setBaseSeed(int seed) { baseSeed_ = seed; }

private:
    int chunkSeed(int x, int y);

    std::mt19937 mt;
    int baseSeed_;
};

#endif
