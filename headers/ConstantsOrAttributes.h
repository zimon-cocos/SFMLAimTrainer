#pragma once
#include "ConstantsOrAttributes.h"

namespace Constants
{
    inline constexpr unsigned int width {900};
    inline constexpr unsigned int height {900};

    inline constexpr unsigned int framerate {60};
    inline constexpr float timeSpeed {1};   

    inline constexpr float spawnRadius {1300.0f};   
    inline constexpr float difIncrease {0.001f};  
    
    inline constexpr int radiusL {100};
    inline constexpr int radiusM {50};
    inline constexpr int radiusS {25};    
    inline constexpr float maxLifetime {4.0f};    

    inline constexpr int bossDamage {1000};
    inline float bossSpeed {0.2f};    

    inline constexpr float maxTargetLifetime {15.0f};  
    inline constexpr float targSpeed {0.15f};   
    inline constexpr int astDamage {10};    

    inline constexpr int projectileSpeed {30};
    inline constexpr double maxProjectileLifetime {1.0};  


}

namespace Attributes
{
    int score {0};
    int previousScore {0};
    int missed {0};

    float spawnAsteroidInterval {2};

    float secSinceFiring {0.25};
    float fireDelay {0.25};

    float secSinceSpawn{0};

    float bossTimer {6 * 60};
    bool bossSpawned {false};

}
