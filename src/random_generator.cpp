#include "random_generator.h"

RandomGenerator::RandomGenerator(int baseSeed)
    : baseSeed_{baseSeed} {}

void RandomGenerator::seed(int x, int y)
{
    mt.seed(chunkSeed(x, y) + baseSeed_);
}

bool RandomGenerator::drawLots(unsigned int chance, unsigned int max)
{
    assert(chance <= max);

    return (mt() % max) < chance;
}

bool RandomGenerator::drawLots(double chance, double max)
{
    assert(chance <= max);

    int bigNumber = mt();
    return (bigNumber / max - static_cast<int>(bigNumber / max)) * max < chance;
}

int RandomGenerator::chunkSeed(int x, int y)
{
    return (x & 0xFFFF) << 16 | (y & 0xFFFF);
}