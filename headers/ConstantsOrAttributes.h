#pragma once

constexpr unsigned int width {600};
constexpr unsigned int height {600};
constexpr unsigned int framerate {60};

int score {0};
int previousScore {0};
int missed {0};
float toSpawn {2};
float difIncrease {0.01};


float fireDelay{0.25};
float fireDelayOriginal {fireDelay};
float hitAmount{0};

float maxTargetLifetime {5};
float radiusL {100};
float radiusM {50};
float radiusS {25};
float secSinceSpawn{0};

float maxLifetime {3};


int rotationSpeed {350};
int movementSpeed {10};


float magic {69};
