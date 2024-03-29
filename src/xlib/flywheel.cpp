#include "xlib/flywheel.hpp"

//Template sign function
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/*
 * The flywheel class implements the Take Back Half (TBH) velocity control
 * algorithm. It takes in a target velocity and operates asynchronously by
 * inheriting from the TaskWrapper to keep the flywheel's velocity as steady
 * as possible. It runs off of a direct drive motor.
 */

namespace xlib {
    //Set class variables based on input
    void Flywheel::moveVelocity(int velocity, float predicted_drive) {
        if(!active)
            init();

        targetVelocity = velocity;

        currentError = targetVelocity - (getActualVelocity() * 18);
        prevError = currentError;

        //If the predicted drive is unset, set driveApprox to a decent estimate
        //of the target velocity.
        if(predicted_drive == -1)
            driveApprox = targetVelocity / 3600;
        //Otherwise, set the drivaApprox value correctly
        else
            driveApprox = predicted_drive;

        firstCross = true;
        driveAtZero = 0;
    }

    //Based on the TaskWrapper, loop is called by startTask()
    //Running asynchronously, this function computes the desired flywheel velocity
    //and sets the motor base class to the correct voltage
    void Flywheel::loop() {       
        while(true) {
            //If the flywheel should spin backwards, set it to negative voltage
            //and continue
            if(doBackSpin) {
                moveVoltage(-12000);
                continue;
            }

            //Green motor by default, multiply by 18 to scale to 3600 RPM
            currentError = targetVelocity - (getActualVelocity() * 18);

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

            moveVoltage(drive * 12000);

            grapher.newData(targetVelocity, 0);
            grapher.newData((getActualVelocity() * 18), 1);

            pros::delay(20);
        }
    }

    //Initialize dependencies
    void Flywheel::init() {
        grapher.initGraph({0, 3600}, 250);
        selector.setActive(false);
        startTask();
        active = true;
    }

    void Flywheel::stop() {
        moveVoltage(0);
        //fclose(tbhTelem);
    }

    void Flywheel::toggleReverse() {
        if(!active)
            init();

        doBackSpin = !doBackSpin;
    }

    Flywheel::Flywheel(std::int8_t iport, float igain, Selector& sel)
        : okapi::Motor{iport}, //Calls the constructor for okapi::Motor 
          gain{igain}, selector{sel} {}
}