#include "fsm.h"

FSM_State_t FSM_UpdateState(uint8 l, uint8 f, uint8 r, FSM_State_t currentState)
{
    switch (currentState)
    {

        /* ================= DEFAULT ================= */
        case STATE_DEFAULT:

            if (l == 1 && f == 0 && r == 0)
                return STATE_POSSIBLE_RIGHT_CORNER;

            if (l == 0 && f == 0 && r == 1)
                return STATE_POSSIBLE_LEFT_CORNER;

            return STATE_DEFAULT;


        /* ========== POSSIBLE RIGHT CORNER ========== */
        case STATE_POSSIBLE_RIGHT_CORNER:

            if (l == 1 && f == 0 && r == 1)
                return STATE_DEFAULT;

            if (l == 1 && f == 1 && r == 0)
                return STATE_RIGHT_CORNER;

            if (l == 0 && f == 0 && r == 0)
                return STATE_FINISH;

            return STATE_POSSIBLE_RIGHT_CORNER;


        /* ========== POSSIBLE LEFT CORNER ========== */
        case STATE_POSSIBLE_LEFT_CORNER:

            if (l == 1 && f == 0 && r == 1)
                return STATE_DEFAULT;

            if (l == 0 && f == 1 && r == 1)
                return STATE_LEFT_CORNER;

            if (l == 0 && f == 0 && r == 0)
                return STATE_FINISH;

            return STATE_POSSIBLE_LEFT_CORNER;


        /* ========== RIGHT CORNER ========== */
        case STATE_RIGHT_CORNER:

            if (l == 1 && f == 0 && r == 0)
                return STATE_POST_RIGHT_CORNER;

            return STATE_RIGHT_CORNER;


        /* ========== LEFT CORNER ========== */
        case STATE_LEFT_CORNER:

            if (l == 0 && f == 0 && r == 1)
                return STATE_POST_LEFT_CORNER;

            return STATE_LEFT_CORNER;


        /* ========== POST RIGHT CORNER ========== */
        case STATE_POST_RIGHT_CORNER:

            if (l == 1 && f == 0 && r == 1)
                return STATE_DEFAULT;

            return STATE_POST_RIGHT_CORNER;


        /* ========== POST LEFT CORNER ========== */
        case STATE_POST_LEFT_CORNER:

            if (l == 1 && f == 0 && r == 1)
                return STATE_DEFAULT;

            return STATE_POST_LEFT_CORNER;


        /* ========== ALIGN STATES (to be modified to call the right aligned function) ========== */
        case STATE_RIGHT_ALIGN:
            return STATE_DEFAULT;
        case STATE_LEFT_ALIGN:
            return STATE_DEFAULT;


        /* ========== FINISH ========== */
        case STATE_FINISH:
            return STATE_FINISH;


        default:
            return STATE_DEFAULT;
    }
}