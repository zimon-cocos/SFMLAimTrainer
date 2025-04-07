#pragma once

constexpr unsigned int width {600};
constexpr unsigned int height {600};
constexpr unsigned int framerate {60};

int score {0};
int previousScore {0};
int missed {0};

float fireDelay{0.5};
float hitAmount{0};


float maxLifetime {3};
int rotationSpeed {350};
int movementSpeed {10};
