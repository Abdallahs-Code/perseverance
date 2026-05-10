#include "../lib/HAL_ULTRASONIC/ultrasonic.h"
#include "fsm.h"
#include "motor.h"
#include "bluetooth.h"

FSM_State_t currentState = STATE_FINISH;

int main(void)
{
    uint8 leftSensor;
    uint8 frontSensor;
    uint8 rightSensor;

    float    distances[ULTRASONIC_SENSOR_COUNT];
    uint8    thresholds[ULTRASONIC_SENSOR_COUNT];

    Motor_Init();
    ultrasonicBegin();
    // Bluetooth_Init(9600);
    //delay(10000); //debugging

    while (1)
    {
        /* Read all three sensors and check thresholds */
        ultrasonicReadAllCm(distances, MAX_ECHO_US);
        ultrasonicCheckThresholds(distances, thresholds);

        leftSensor  = thresholds[0];
        frontSensor = thresholds[1];
        rightSensor = thresholds[2];

        /* ================= DEFAULT ================= */

        while (currentState == STATE_DEFAULT)
        {
            // if (distances[0] < LEFT_MIN_DISTANCE)
            // {
            //     Car_LeftAlign();
            // }
            // else if (distances[2] < RIGHT_MIN_DISTANCE)
            // {
            //     Car_RightAlign();
            // }
            // else
            // {
                Car_MoveForward();
            // }
            ultrasonicReadAllCm(distances, MAX_ECHO_US);
            ultrasonicCheckThresholds(distances, thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );
            // char message[150];

            // sprintf(
            //     message,
            //     "L:%d F:%d R:%d State:%d | DL:%.2f DF:%.2f DR:%.2f\r\n",
            //     leftSensor,
            //     frontSensor,
            //     rightSensor,
            //     currentState,
            //     distances[0],
            //     distances[1],
            //     distances[2]
            // );

            // Bluetooth_Send(message);
        }


        /* ========= POSSIBLE RIGHT CORNER ========= */

        while (currentState == STATE_POSSIBLE_RIGHT_CORNER)
        {
            Car_MoveForward();

            ultrasonicReadAllCm(distances, MAX_ECHO_US);
            ultrasonicCheckThresholds(distances, thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );

            // char message[150];

            // sprintf(
            //     message,
            //     "L:%d F:%d R:%d State:%d | DL:%.2f DF:%.2f DR:%.2f\r\n",
            //     leftSensor,
            //     frontSensor,
            //     rightSensor,
            //     currentState,
            //     distances[0],
            //     distances[1],
            //     distances[2]
            // );

            // Bluetooth_Send(message);                           
        }


        /* =============== RIGHT CORNER ============== */

        while (currentState == STATE_RIGHT_CORNER)
        {
            Car_TurnRight();

            ultrasonicReadAllCm(distances, MAX_ECHO_US);
            ultrasonicCheckThresholds(distances, thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );
            // char message[150];

            // sprintf(
            //     message,
            //     "L:%d F:%d R:%d State:%d | DL:%.2f DF:%.2f DR:%.2f\r\n",
            //     leftSensor,
            //     frontSensor,
            //     rightSensor,
            //     currentState,
            //     distances[0],
            //     distances[1],
            //     distances[2]
            // );

            // Bluetooth_Send(message);                           
        }


        /* ========= POST RIGHT CORNER ========= */

        while (currentState == STATE_POST_RIGHT_CORNER)
        {
            Car_MoveForward();

            ultrasonicReadAllCm(distances, MAX_ECHO_US);
            ultrasonicCheckThresholds(distances, thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );
            // char message[150];

            // sprintf(
            //     message,
            //     "L:%d F:%d R:%d State:%d | DL:%.2f DF:%.2f DR:%.2f\r\n",
            //     leftSensor,
            //     frontSensor,
            //     rightSensor,
            //     currentState,
            //     distances[0],
            //     distances[1],
            //     distances[2]
            // );

            // Bluetooth_Send(message);                           
        }


        /* ========= POSSIBLE LEFT CORNER ========= */

        while (currentState == STATE_POSSIBLE_LEFT_CORNER)
        {
            Car_MoveForward();

            ultrasonicReadAllCm(distances, MAX_ECHO_US);
            ultrasonicCheckThresholds(distances, thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );
            // char message[150];

            // sprintf(
            //     message,
            //     "L:%d F:%d R:%d State:%d | DL:%.2f DF:%.2f DR:%.2f\r\n",
            //     leftSensor,
            //     frontSensor,
            //     rightSensor,
            //     currentState,
            //     distances[0],
            //     distances[1],
            //     distances[2]
            // );

            // Bluetooth_Send(message);                           
        }


        /* =============== LEFT CORNER ============== */

        while (currentState == STATE_LEFT_CORNER)
        {
            Car_TurnLeft();

            ultrasonicReadAllCm(distances, MAX_ECHO_US);
            ultrasonicCheckThresholds(distances, thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );
            // char message[150];

            // sprintf(
            //     message,
            //     "L:%d F:%d R:%d State:%d | DL:%.2f DF:%.2f DR:%.2f\r\n",
            //     leftSensor,
            //     frontSensor,
            //     rightSensor,
            //     currentState,
            //     distances[0],
            //     distances[1],
            //     distances[2]
            // );

            // Bluetooth_Send(message);                           
        }


        /* ========= POST LEFT CORNER ========= */

        while (currentState == STATE_POST_LEFT_CORNER)
        {
            Car_MoveForward();

            ultrasonicReadAllCm(distances, MAX_ECHO_US);
            ultrasonicCheckThresholds(distances, thresholds);
            leftSensor  = thresholds[0];
            frontSensor = thresholds[1];
            rightSensor = thresholds[2];

            currentState = FSM_UpdateState(
                                leftSensor,
                                frontSensor,
                                rightSensor,
                                currentState
                           );

            // char message[150];

            // sprintf(
            //     message,
            //     "L:%d F:%d R:%d State:%d | DL:%.2f DF:%.2f DR:%.2f\r\n",
            //     leftSensor,
            //     frontSensor,
            //     rightSensor,
            //     currentState,
            //     distances[0],
            //     distances[1],
            //     distances[2]
            // );

            // Bluetooth_Send(message);                           
        }


        /* ================= FINISH ================= */

        while (currentState == STATE_FINISH)
        {
            Car_Stop();
            
            ultrasonicReadAllCm(distances, MAX_ECHO_US);
            ultrasonicCheckThresholds(distances, thresholds);
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
    }
}