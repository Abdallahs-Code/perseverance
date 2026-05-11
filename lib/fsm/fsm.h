#ifndef FSM_H
#define FSM_H

#include "std_types.h"


typedef enum
{
    STATE_DEFAULT = 0,

    STATE_POSSIBLE_RIGHT_CORNER,
    STATE_RIGHT_CORNER,

    STATE_POSSIBLE_LEFT_CORNER,
    STATE_LEFT_CORNER,

    STATE_FINISH

} FSM_State_t;

FSM_State_t FSM_UpdateState(uint8 l, uint8 f, uint8 r, FSM_State_t currentState, float* distances);


#endif