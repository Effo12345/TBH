#pragma once
#include "api.hpp"
#include "globals.hpp"

extern void FWVelocitySet(int velocity, float predicted_drive);
extern void FWControlVelocity();
extern void FWSetGain(double gain);
extern double FWGetGain();

