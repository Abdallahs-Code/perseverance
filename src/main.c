#include "../lib/HAL_ULTRASONIC/ultrasonic.h"
#include "fsm.h"
#include "motor.h"
#include <avr/interrupt.h>

FSM_State_t currentState = STATE_DEFAULT;

int main(void)
{
    uint8 leftSensor;
    uint8 frontSensor;
    uint8 rightSensor;

    uint8    thresholds[3];

    Motor_Init();
    Ultrasonic_Init();
    sei();

    while (1)
    {
        /* Read all three sensors and check thresholds */
        Ultrasonic_CheckThresholds(thresholds);

        leftSensor  = thresholds[0];
        frontSensor = thresholds[1];
        rightSensor = thresholds[2];

        /* ================= DEFAULT ================= */

        while (currentState == STATE_DEFAULT)
        {
            Car_MoveForward();

            Ultrasonic_CheckThresholds(thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );
        }


        /* ========= POSSIBLE RIGHT CORNER ========= */

        while (currentState == STATE_POSSIBLE_RIGHT_CORNER)
        {
            Car_MoveForward();

            Ultrasonic_CheckThresholds(thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );
        }


        /* =============== RIGHT CORNER ============== */

        while (currentState == STATE_RIGHT_CORNER)
        {
            Car_TurnRight();

            Ultrasonic_CheckThresholds(thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );
        }


        /* ========= POST RIGHT CORNER ========= */

        while (currentState == STATE_POST_RIGHT_CORNER)
        {
            Car_MoveForward();

            Ultrasonic_CheckThresholds(thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );
        }


        /* ========= POSSIBLE LEFT CORNER ========= */

        while (currentState == STATE_POSSIBLE_LEFT_CORNER)
        {
            Car_MoveForward();

            Ultrasonic_CheckThresholds(thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );
        }


        /* =============== LEFT CORNER ============== */

        while (currentState == STATE_LEFT_CORNER)
        {
            Car_TurnLeft();

            Ultrasonic_CheckThresholds(thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );
        }


        /* ========= POST LEFT CORNER ========= */

        while (currentState == STATE_POST_LEFT_CORNER)
        {
            Car_MoveForward();

            Ultrasonic_CheckThresholds(thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );
        }


        /* ================= FINISH ================= */

        while (currentState == STATE_FINISH)
        {
            Car_Stop();
        }
    }
}