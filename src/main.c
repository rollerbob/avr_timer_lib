#include "main.h"
#include "timer.h"

Frame_timer_t frames_up_start = {FRAME_TIMER, UP_COUNT, START_AT_TARGET, 0, 100, 0};
Frame_timer_t frames_down_start = {FRAME_TIMER, DOWN_COUNT, START_AT_TARGET, 100, 100, 0};
Frame_timer_t frames_up_stop = {FRAME_TIMER, UP_COUNT, STOP_AT_TARGET, 0, 100, 0};
Frame_timer_t frames_down_stop = {FRAME_TIMER, DOWN_COUNT, STOP_AT_TARGET, 100, 100, 0};

Second_timer_t seconds_up_start = {SECOND_TIMER, UP_COUNT, START_AT_TARGET, 0, 0, 5, 0};
Second_timer_t seconds_down_start = {SECOND_TIMER, DOWN_COUNT, START_AT_TARGET, FRAME_RATE, 5, 5, 0};
Second_timer_t seconds_up_stop = {SECOND_TIMER, UP_COUNT, STOP_AT_TARGET, 0, 0, 5, 0};
Second_timer_t seconds_down_stop = {SECOND_TIMER, DOWN_COUNT, STOP_AT_TARGET, FRAME_RATE, 5, 5, 0};

Minute_timer_t minute_up_start = {MINUTE_TIMER, UP_COUNT, START_AT_TARGET, 0, 0, 0, 2, 0};
Minute_timer_t minute_down_start = {MINUTE_TIMER, DOWN_COUNT, START_AT_TARGET, FRAME_RATE, 60, 2, 2, 0};
Minute_timer_t minute_up_stop = {MINUTE_TIMER, UP_COUNT, STOP_AT_TARGET, 0, 0, 0, 2, 0};
Minute_timer_t minute_down_stop = {MINUTE_TIMER, DOWN_COUNT, STOP_AT_TARGET, FRAME_RATE, 60, 2, 2, 0};

int main(void)
{
    Init_timer_module();

    for (;;)
    {
        Update_timer_module();

        Update_timer((void *)&frames_up_start);
        Update_timer((void *)&frames_down_start);
        Update_timer((void *)&frames_up_stop);
        Update_timer((void *)&frames_down_stop);

        Update_timer((void *)&seconds_up_start);
        Update_timer((void *)&seconds_down_start);
        Update_timer((void *)&seconds_up_stop);
        Update_timer((void *)&seconds_down_stop);

        Update_timer((void *)&minute_up_start);
        Update_timer((void *)&minute_down_start);
        Update_timer((void *)&minute_up_stop);
        Update_timer((void *)&minute_down_stop);
    }
    
    return 0;
}