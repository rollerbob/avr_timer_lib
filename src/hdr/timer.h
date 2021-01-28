/*!
 * @file timer.h
 * @author Белов Андрей (belov@uralintelcom.ru)
 * @brief Универсальная библиотека для работы с таймерами
 * @version 0.1
 * @date 2021-01-28
 * 
 * @copyright Copyright Уралинтелком (С) 2021
 * 
 * Общий формат декларации таймера
 * Timer_t timer = {
 *  Timer_type_e type - тип таймера
 *  Timer_counting_e counting - направление счёта
 *  Timer_behavior_e behavior - действие при достижении цели
 *  uint16_t frame - счётчик фреймов
 *  [uint8_t second - счётчик секунд]
 *  [uint8_t minute - счётчик минут]
 *  uint8(16)_t max_cntr - целевое значение счётчика
 *  uint8_t target - флаг достижения целевого значения
 * }
 * Реализованы три типа таймеров:
 * - Фрэймовый FRAME_TIMER (таймер изменяющий своё значение каждый системный тик)
 * - Секундный SECOND_TIMER
 * - Минутный  MINUTE_TIMER
 * 
 * Направление счёта:
 * - Вверх UP_COUNT - таймер инкрементирует своё значение
 * - Вниз DOWN_COUNT - таймер декрементирует своё значение
 * 
 * Действие при достижении цели:
 * - STOP_AT_TARGET - при достижении целевого значения таймера счёт останваливается
 * - START_AT_TARGET - при достижении целевого значения таймера счёт начинается заново
 * 
 * Cчётчик фреймов:
 * - Изменяется каждый системный тик. Для большей точности используется 16-ти битное число
 * - Присутствует во всех трёх типах таймеров
 * 
 * Счётчик секунд
 * - Изменяется раз в секунду. Используется 8-битная переменная так как на 
 * целевых значениях больше 60 целесообразнее использовать минутный таймер.
 * - Присутствует только в секундном и минутном таймерах
 * 
 * Счётчик минут
 * - Изменяется раз в минуту. Используется 8-ми битная переменная так как "256 
 * минут хватит всем".
 * - Присутствует только в минутном таймере.
 * 
 * Целевое значение счётчика.
 * - Значение таймера при котором на одно обновление таймера будет выставлена 
 * переменная target в 1. Для каждого типа таймера целевое значение
 * соответствует его типу, то есть для фреймового таймера устанавливаются
 * фреймы, для секундного секунды, для минутного минуты.
 * - Для фреймового таймера переменная 16-ти битная, для остальных - 8-ми битная
 * 
 * Флаг достижения целевого значения
 * - Устанавливается в 1 на один суперцикл если значение старшего счётчика 
 * таймера достигло целевого значения. Сбрасывается в 0 в следующем суперцикле.
 * 
 */

#ifndef TIMER_H
#define TIMER_H

#include "defines.h"
#include "custom_types.h"
#include <avr/interrupt.h>

// ====== Настройки модуля =====================================================
// Расчёт регистра OCR для натсройки хардварного таймера
// Прескаллер из настроек регистра TCCR2
#define TIMER2_PRESCALLER               (64UL)

// Расчёт значения регистра OCR2
#define OCR_REG                         (FCPU/(TIMER2_PRESCALLER*FRAME_RATE) - 1)

// ====== Пользовательские типы данных =========================================
typedef enum
{
    FRAME_TIMER,
    SECOND_TIMER,
    MINUTE_TIMER
} Timer_type_e;

typedef enum
{
    DOWN_COUNT,
    UP_COUNT
} Timer_counting_e;

typedef enum
{
    STOP_AT_TARGET,
    START_AT_TARGET
} Timer_behavior_e;

typedef struct
{
    Timer_type_e type;
    Timer_counting_e counting;
    Timer_behavior_e behavior;
    uint16_t frame;
    uint8_t second;
    uint8_t minute;
    uint8_t max_cntr;
    uint8_t target;
} Minute_timer_t;

typedef struct
{
    Timer_type_e type;
    Timer_counting_e counting;
    Timer_behavior_e behavior;
    uint16_t frame;
    uint8_t second;
    uint8_t max_cntr;
    uint8_t target;
} Second_timer_t;

typedef struct
{
    Timer_type_e type;
    Timer_counting_e counting;
    Timer_behavior_e behavior;
    uint16_t frame;
    uint16_t max_cntr;
    uint8_t target;
} Frame_timer_t;

// ====== Заголовки публичных функций ==========================================
void Init_timer_module(void);

void Update_timer_module(void);

void Update_timer(void *timer);

void Reset_timer(void *timer);

#endif