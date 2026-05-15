#include "fsm.h"

FSM_State_t FSM_UpdateState(uint8 l, uint8 f, uint8 r, FSM_State_t currentState, float* distances)
{
    switch (currentState)
    {

        /* ================= DEFAULT ================= */
        case STATE_DEFAULT:

            if (l == 1 && f == 0 && r == 0) return STATE_POSSIBLE_RIGHT_CORNER;
            if (l == 0 && f == 0 && r == 1) return STATE_POSSIBLE_LEFT_CORNER;
            if (l == 0 && f == 0 && r == 0) return STATE_FINISH;
            return STATE_DEFAULT;


        /* ========== POSSIBLE RIGHT CORNER ========== */
        case STATE_POSSIBLE_RIGHT_CORNER:

            if (l == 1 && f == 1) return STATE_RIGHT_CORNER;    // dead-end confirmed
            if (l == 1 && r == 1) return STATE_DEFAULT;          // T-junction passed
            if (l == 0 && f == 0 && r == 0) return STATE_FINISH;
            return STATE_POSSIBLE_RIGHT_CORNER;


        /* ========== RIGHT CORNER ========== */
        case STATE_RIGHT_CORNER:

            // Turn right while front is blocked; move forward after.
            // Exit once fully in the new corridor (both walls, clear front).
            if (l == 1 && f == 0 && r == 1) return STATE_DEFAULT;
            return STATE_RIGHT_CORNER;


        /* ========== POSSIBLE LEFT CORNER ========== */
        case STATE_POSSIBLE_LEFT_CORNER:

            if (f == 1 && r == 1) return STATE_LEFT_CORNER;     // dead-end confirmed
            if (l == 1 && r == 1) return STATE_DEFAULT;          // T-junction passed
            if (l == 0 && f == 0 && r == 0) return STATE_FINISH;
            return STATE_POSSIBLE_LEFT_CORNER;


        /* ========== LEFT CORNER ========== */
        case STATE_LEFT_CORNER:

            // Turn left while front is blocked; move forward after.
            // Exit once fully in the new corridor (both walls, clear front).
            if (l == 1 && f == 0 && r == 1) return STATE_DEFAULT;
            return STATE_LEFT_CORNER;


        /* ========== FINISH ========== */
        case STATE_FINISH:

            if (l == 1 && f == 0 && r == 1) return STATE_DEFAULT;
            return STATE_FINISH;


        default:
            return STATE_DEFAULT;
    }
}
