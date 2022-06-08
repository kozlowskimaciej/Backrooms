#ifndef CONFIG_3534_H
#define CONFIG_3534_H

#include "myMath.h"

namespace Conf
{
    inline constexpr float FOVangle{PI / 2.5f};
    inline constexpr float renderDistance{40.0f};
    inline constexpr float renderChunkDistance{4.0f};
    inline constexpr float chunkWidth{10.0f};
    inline constexpr float bigChunkWidth{300.0f};
    inline constexpr float wallWidth{2.0f};

    inline constexpr unsigned int windowWidth{1600};
    inline constexpr unsigned int windowHeight{900};

    inline constexpr double huntDuration{12.0};

    inline constexpr float linearPlayerVelocity{700.0f};
    inline constexpr float linearEnemyVelocity{600.0f};

    // Simulation parameters
    inline constexpr float timeStep = 1.0f / 60.0f; // Step of time between events
    inline constexpr int32 velocityIterations = 4;  // Velocity calculations during one step
    inline constexpr int32 positionIterations = 3;  // Position calculations during one step

}

#endif
