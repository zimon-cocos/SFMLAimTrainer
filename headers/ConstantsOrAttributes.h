#pragma once

constexpr unsigned int width {900};
constexpr unsigned int height {900};
constexpr unsigned int framerate {60};
constexpr float timeSpeed {1};



int score {0};
int previousScore {0};
int missed {0};
constexpr float spawnRadius {1300.0f};
float toSpawn {2};
float difIncrease {0.01};


float fireDelay{0.25};
float fireDelayOriginal {0.25};
float hitAmount{0};

float maxTargetLifetime {15};
float radiusL {100};
float radiusM {50};
float radiusS {25};
float secSinceSpawn{0};
float targSpeed {0.15};
float bossTimer {3};
int astDamage {0};
bool bossSpawned {false};


float maxLifetime {4};


int rotationSpeed {300};
float movementSpeed {0};
float acceleration {12};
float deacceleration {12};
float maxSpeed {10};



