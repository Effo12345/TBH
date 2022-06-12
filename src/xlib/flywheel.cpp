#include "flywheel.hpp"

int targetVelocity = 0;
float driveApprox = 0.0f;
bool firstCross = false;
float driveAtZero = 0.0f;
float currentError;
float prevError;
float drive = 0.0f;

double gain = 0.00025f;

void FwVelocitySet(int velocity, float predicted_drive) {
    targetVelocity = velocity;

    currentError = targetVelocity - fw.getActualVelocity();
    prevError = currentError;

    driveApprox = predicted_drive;

    firstCross = true;
    driveAtZero = 0;
}

void FWControlVelocity() {
    currentError = targetVelocity - fw.getActualVelocity();

    drive = drive + (currentError * gain);
    
    drive = std::clamp(drive, 0.0f, 1.0f);

    if(sgn(currentError) != sgn(prevError)) {
        if(firstCross) {
            drive = driveApprox;
            firstCross = false;
        }
        else
            drive = 0.5 * (drive + driveAtZero);

        driveAtZero = drive;
    }
    prevError = currentError;

    fw.moveVoltage(drive * MAX_VOLTAGE);

    grapher.newData(targetVelocity, 0);
    grapher.newData(fw.getActualVelocity(), 1);
}

void FWSetGain(double newGain) {
    gain = newGain;
    return;
}

double FWGetGain() {
    return gain;
}