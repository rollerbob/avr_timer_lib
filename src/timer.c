#include "timer.h"
// ====== Локальные переменные =================================================
volatile uint8_t _new_frame = 0;
uint8_t new_frame = 0;

// ====== Локальные функции ====================================================
// Функции обработки фрэймовых таймеров
void _Handle_frame_timer_down_stop(Frame_timer_t *timer)
{
    if (timer->frame > 0)
    {
        timer->frame--;
        if (!timer->frame)
            timer->target = 1;
    }
}
void _Handle_frame_timer_down_start(Frame_timer_t *timer)
{
    timer->frame--;
    if (!timer->frame)
    {
        timer->frame = timer->max_cntr;
        timer->target = 1;
    }
}
void _Handle_frame_timer_up_stop(Frame_timer_t *timer)
{
    if (timer->frame < timer->max_cntr)
    {
        timer->frame++;
        if (timer->frame == timer->max_cntr)
            timer->target = 1;
    }
}
void _Handle_frame_timer_up_start(Frame_timer_t *timer)
{
    timer->frame++;
    if (timer->frame == timer->max_cntr)
    {
        timer->frame = 0;
        timer->target = 1;
    }
}

// Массив указателей на эти функции
void (*frame_func[])(Frame_timer_t *timer) = {
    &_Handle_frame_timer_down_stop, 
    &_Handle_frame_timer_up_stop, 
    &_Handle_frame_timer_down_start, 
    &_Handle_frame_timer_up_start
};

// Функции обработки секундных таймеров
void _Handle_second_timer_down_stop(Second_timer_t *timer)
{
    timer->frame--;
    if (!timer->frame)
    {
        timer->frame = FRAME_RATE;
        if (timer->second > 0)
        {
            timer->second--;
            if (!timer->second)
                timer->target = 1;
        }
    }
}
void _Handle_second_timer_down_start(Second_timer_t *timer)
{
    timer->frame--;
    if (!timer->frame)
    {
        timer->frame = FRAME_RATE;
        timer->second--;
        if (!timer->second)
        {
            timer->second = timer->max_cntr;
            timer->target = 1;
        }
    }
}
void _Handle_second_timer_up_stop(Second_timer_t *timer)
{
    timer->frame++;
    if (timer->frame == FRAME_RATE)
    {
        timer->frame = 0;
        if (timer->second < timer->max_cntr)
        {
            timer->second++;
            if (timer->second == timer->max_cntr)
                timer->target = 1;
        }
    }
}
void _Handle_second_timer_up_start(Second_timer_t *timer)
{
    timer->frame++;
    if (timer->frame == FRAME_RATE)
    {
        timer->frame = 0;
        timer->second++;
        if (timer->second == timer->max_cntr)
        {
            timer->second = 0;
            timer->target = 1;
        }
    }
}

// Массив указателей на эти функции
void (*second_func[])(Second_timer_t *timer) = {
    &_Handle_second_timer_down_stop, 
    &_Handle_second_timer_up_stop, 
    &_Handle_second_timer_down_start, 
    &_Handle_second_timer_up_start
};

// Функции обработки минутных таймеров
void _Handle_minute_timer_down_stop(Minute_timer_t *timer)
{
    timer->frame--;
    if (!timer->frame)
    {
        timer->frame = FRAME_RATE;
        timer->second--;
        if (!timer->second)
        {
            timer->second = 60;
            if (timer->minute > 0)
            {
                timer->minute--;
                if (!timer->minute)
                    timer->target = 1;
            }
        }
    }
}
void _Handle_minute_timer_down_start(Minute_timer_t *timer)
{
    timer->frame--;
    if (!timer->frame)
    {
        timer->frame = FRAME_RATE;
        timer->second--;
        if (!timer->second)
        {
            timer->second = 60;
            timer->minute--;
            if (!timer->minute)
            {
                timer->minute = timer->max_cntr;
                timer->target = 1;
            }
        }
    }
}
void _Handle_minute_timer_up_stop(Minute_timer_t *timer)
{
    timer->frame++;
    if (timer->frame == FRAME_RATE)
    {
        timer->frame = 0;
        timer->second++;
        if (timer->second == 60)
        {
            timer->second = 0;
            if (timer->minute < timer->max_cntr)
            {
                timer->minute++;
                if (timer->minute == timer->max_cntr)
                    timer->target = 1;
            }
        }
    }
}
void _Handle_minute_timer_up_start(Minute_timer_t *timer)
{
    timer->frame++;
    if (timer->frame == FRAME_RATE)
    {
        timer->frame = 0;
        timer->second++;
        if (timer->second == 60)
        {
            timer->second = 0;
            timer->minute++;
            if (timer->minute == timer->max_cntr)
            {
                timer->minute = 0;
                timer->target = 1;
            }
        }
    }
}

// Массив указателей на эти функции
void (*minute_func[])(Minute_timer_t *timer) = {
    &_Handle_minute_timer_down_stop, 
    &_Handle_minute_timer_up_stop, 
    &_Handle_minute_timer_down_start, 
    &_Handle_minute_timer_up_start
};


void Init_timer_module(void)
{
    // Настройка регистров хардварного таймера 2
    TCCR2 = 1 << WGM21 | 1 << CS22;
    TIMSK = 1 << OCIE2;
    OCR2 = OCR_REG;
}

void Update_timer_module(void)
{
    cli();
    if (new_frame) new_frame = 0;
    else if (_new_frame) 
    {
        new_frame = 1;
        _new_frame = 0;
    }
    sei();
}

void Update_timer(void *timer)
{
    // Первым байтом всех таймеров идёт его тип.
    Timer_type_e timer_type = *(Timer_type_e *)timer;
    uint8_t indx = 0;

    Frame_timer_t *frame_timer;
    Second_timer_t *second_timer;
    Minute_timer_t *minute_timer;

    // Определяю что за таймер был мне прислан, обнуляю его таргет и вычисляю
    // требуемый обработчик
    switch (timer_type)
    {
    case FRAME_TIMER:
        frame_timer = (Frame_timer_t *)timer;
        if (frame_timer->target)
            frame_timer->target = 0;

        indx = (uint8_t)frame_timer->counting + (uint8_t)frame_timer->behavior * 2;

        if (new_frame)
            frame_func[indx](frame_timer);
        break;

    case SECOND_TIMER:
        second_timer = (Second_timer_t *)timer;
        if (second_timer->target)
            second_timer->target = 0;

        indx = (uint8_t)second_timer->counting + (uint8_t)second_timer->behavior * 2;

        if (new_frame)
            second_func[indx](second_timer);
        break;

    case MINUTE_TIMER:
        minute_timer = (Minute_timer_t *)timer;
        if (minute_timer->target)
            minute_timer->target = 0;

        indx = (uint8_t)minute_timer->counting + (uint8_t)minute_timer->behavior * 2;

        if (new_frame)
            minute_func[indx](minute_timer);
        break;

    default:
        break;
    }
}

void Reset_timer(void *timer)
{
    ;
}

ISR(TIMER2_COMP_vect)
{
    _new_frame = 1;
}