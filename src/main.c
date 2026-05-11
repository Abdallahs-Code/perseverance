#include "../lib/HAL_ULTRASONIC/ultrasonic.h"
#include "fsm.h"
#include "motor.h"
#include "bluetooth.h"
#include <Arduino.h>

#include <stdio.h>
#include <string.h>

FSM_State_t currentState = STATE_FINISH;
#define MAX_TURNS 50

char turnSequence[MAX_TURNS];
uint8 turnCount = 0;


uint8 rightTurnSaved = 0;
uint8 leftTurnSaved  = 0;
uint8 finishSent     = 0;
uint8 first_time = 1;

int main(void)
{
    uint8 leftSensor;
    uint8 frontSensor;
    uint8 rightSensor;

    float    distances[ULTRASONIC_SENSOR_COUNT];
    uint8    thresholds[ULTRASONIC_SENSOR_COUNT];

    char message[200];
    uint8 i;

    Motor_Init();
    Ultrasonic_Init();
    sei();
    Bluetooth_Init(9600);
    //delay(10000); //debugging

    while (1)
    {
        /* Read all three sensors and check thresholds */
        Ultrasonic_CheckSensors(distances, thresholds);

        leftSensor  = thresholds[0];
        frontSensor = thresholds[1];
        rightSensor = thresholds[2];

        /* ================= DEFAULT ================= */

        while (currentState == STATE_DEFAULT)
        {
            if (distances[0] < LEFT_MIN_DISTANCE)
            {
                Car_LeftAlign();
            }
            else if (distances[2] < RIGHT_MIN_DISTANCE)
            {
                Car_RightAlign();
            }
            else
            {
                Car_MoveForward();
            }
            Ultrasonic_CheckSensors(distances, thresholds);
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
            if (distances[0] < LEFT_MIN_DISTANCE)
            {
                Car_LeftAlign();
            }
            else if (distances[2] < RIGHT_MIN_DISTANCE)
            {
                Car_RightAlign();
            }
            else
            {
                Car_MoveForward();
            }

            Ultrasonic_CheckSensors(distances, thresholds);
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

        if(currentState != STATE_RIGHT_CORNER){
            rightTurnSaved = 0;
        }        

        while (currentState == STATE_RIGHT_CORNER)
        {

            if (rightTurnSaved == 0)
            {
                turnSequence[turnCount] = 'R';
                turnCount++;

                rightTurnSaved = 1;
            }  

            Car_TurnRight();          

            Ultrasonic_CheckSensors(distances, thresholds);
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
            if (distances[0] < LEFT_MIN_DISTANCE)
            {
                Car_LeftAlign();
            }
            else if (distances[1] < POST_CORNER_MIN_DISTANCE)
            {
                Car_RightAlign();
            }
            else
            {
                Car_MoveForward();
            }

            Ultrasonic_CheckSensors(distances, thresholds);
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
            if (distances[0] < LEFT_MIN_DISTANCE)
            {
                Car_LeftAlign();
            }
            else if (distances[2] < RIGHT_MIN_DISTANCE)
            {
                Car_RightAlign();
            }
            else
            {
                Car_MoveForward();
            }

            Ultrasonic_CheckSensors(distances, thresholds);
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
        if(currentState != STATE_LEFT_CORNER){
            leftTurnSaved = 0;
        }

        while (currentState == STATE_LEFT_CORNER)
        {

            if (leftTurnSaved == 0)
            {
                turnSequence[turnCount] = 'L';
                turnCount++;

                leftTurnSaved  = 1;
            }         

            Car_TurnLeft();

            Ultrasonic_CheckSensors(distances, thresholds);
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
            if (distances[2] < RIGHT_MIN_DISTANCE)
            {
                Car_RightAlign();
            }
            else if (distances[1] < POST_CORNER_MIN_DISTANCE)
            {
                Car_LeftAlign();
            }
            else
            {
                Car_MoveForward();
            }

            Ultrasonic_CheckSensors(distances, thresholds);
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

        if (currentState != STATE_FINISH)
        {
            finishSent = 0;
            first_time = 0;
        }        

        while (currentState == STATE_FINISH)
        {
            Car_Stop();

            if (finishSent == 0 && first_time == 0)
            {
                /* Start message */
                sprintf(message, "Turns: %d\r\nSequence: ", turnCount);

                /* Append all turns */
                for (i = 0; i < turnCount; i++)
                {
                    char temp[5];

                    sprintf(temp, "%c", turnSequence[i]);

                    strcat(message, temp);

                    if (i < turnCount - 1)
                    {
                        strcat(message, ", ");
                    }
                }

                strcat(message, "\r\n");

                /* Send once */
                Bluetooth_Send(message);


                turnCount = 0;
                finishSent = 1;
            }            
            
            Ultrasonic_CheckSensors(distances, thresholds);
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