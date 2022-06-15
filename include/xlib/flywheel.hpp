#pragma once
#include "api.hpp"
#include "globals.hpp"

namespace xlib {
    class Flywheel {
        float targetVelocity = 0.0f;
        float driveApprox = 0.0f;
        bool firstCross = false;
        float driveAtZero = 0.0f;
        float currentError;
        float prevError;
        float drive = 0.0f;

        double gain = 0.00025f;

    public:

        void setVelocity(int velocity, float predicted_drive);

        void setVelocity(int velocity);

        void controlVelocity();

        void setGain(double newGain);

        double getGain();
    };

    extern Flywheel fw;
}


