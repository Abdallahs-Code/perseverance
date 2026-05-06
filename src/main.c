#include "fsm.h"
#include "ir.h"
#include "motor.h"

FSM_State_t currentState = STATE_DEFAULT;

int main(void)
{
    uint8 leftSensor;
    uint8 frontSensor;
    uint8 rightSensor;

    IR_Init();
    Motor_Init();

    while (1)
    {

        /* ================= DEFAULT ================= */

        while (currentState == STATE_DEFAULT)
        {
            Car_MoveForward();

            IR_ReadAll(
                &leftSensor,
                &frontSensor,
                &rightSensor
            );

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

            IR_ReadAll(
                &leftSensor,
                &frontSensor,
                &rightSensor
            );

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

            IR_ReadAll(
                &leftSensor,
                &frontSensor,
                &rightSensor
            );

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

            IR_ReadAll(
                &leftSensor,
                &frontSensor,
                &rightSensor
            );

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

            IR_ReadAll(
                &leftSensor,
                &frontSensor,
                &rightSensor
            );

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

            IR_ReadAll(
                &leftSensor,
                &frontSensor,
                &rightSensor
            );

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

            IR_ReadAll(
                &leftSensor,
                &frontSensor,
                &rightSensor
            );

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

    return 0;
}