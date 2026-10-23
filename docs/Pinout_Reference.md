---
description: Nucleo-64 STM32F411RE pin reference for lab/tutorial exercises
---

# STM32F411RE (Nucleo-64) Pin Reference

Board: **ST Nucleo-64, STM32F411RE**
Pin names below use the `PinName_t` convention from [ecPinNames.h](../include/lib-student/ecPinNames.h) (e.g. `PA_5`), plus the Arduino-header label where one exists (e.g. `D13`, `A0`).

Sourced from the [course hardware page](https://ykkim.gitbook.io/ec/ec-course/hardware/nucleo-f411re) and the ST UM1724 connector/timer/ADC/serial pinout maps it links to — also available as an [interactive pin matrix](https://claude.ai/artifact/DUkHcG8NMVCQV3NtcN9M2z).

---

## Fixed-function pins

| Function | Pin | PinName_t | Notes |
|---|---|---|---|
| User LED (LD2) | PA5 | `PA_5` | also `D13`, `TIM2_CH1`, ADC IN5 — see [Conflicts](#conflicts) |
| User Button (B1) | PC13 | `PC_13` | active-low, no external pull-up needed |

## Communication (USART / UART)

| Peripheral | Signal | Pin | PinName_t | AF |
|---|---|---|---|---|
| USART2 | TX | PA2 | `PA_2` | AF7 |
| USART2 | RX | PA3 | `PA_3` | AF7 |
| USART1 | TX (default) | PA9 | `PA_9` | AF7 |
| USART1 | RX (default) | PA10 | `PA_10` | AF7 |
| USART1 | TX (alt) | PB6 | `PB_6` | AF7 |
| USART1 | RX (alt) | PB3 | `PB_3` | AF7 |
| USART6 | TX | PA11 | `PA_11` | AF8 |
| USART6 | TX (alt) | PC6 | `PC_6` | AF8 |
| USART6 | RX | PA12 | `PA_12` | AF8 |
| USART6 | RX (alt) | PC7 | `PC_7` | AF8 |

> USART2 (PA2/PA3) is also the ST-LINK Virtual COM Port — the easiest one to view on a PC terminal with no extra wiring. USART6 has no Arduino-header label; it's only reachable on the Morpho connector (or PC6/PC7, which doubles as the Arduino D9 / TIM3_CH2 pins).

## Timers (PWM / Input Capture)

Most channels route to more than one physical pin — pick whichever option isn't already busy with something else.

| Timer | Channel | Pin | PinName_t | AF |
|---|---|---|---|---|
| TIM1 | CH1 | PA8 | `PA_8` | AF1 |
| TIM1 | CH2 | PA9 | `PA_9` | AF1 |
| TIM1 | CH3 | PA10 | `PA_10` | AF1 |
| TIM2 | CH1 | PA0 | `PA_0` | AF1 |
| TIM2 | CH1 (alt) | PA5 | `PA_5` | AF1 |
| TIM2 | CH1 (alt) | PA15 | `PA_15` | AF1 |
| TIM2 | CH2 | PA1 | `PA_1` | AF1 |
| TIM2 | CH2 (alt) | PB3 | `PB_3` | AF1 |
| TIM2 | CH3 | PB10 | `PB_10` | AF1 |
| TIM3 | CH1 | PC6 | `PC_6` | AF2 |
| TIM3 | CH1 (alt) | PA6 | `PA_6` | AF2 |
| TIM3 | CH1 (alt) | PB4 | `PB_4` | AF2 |
| TIM3 | CH2 | PC7 | `PC_7` | AF2 |
| TIM3 | CH2 (alt) | PB5 | `PB_5` | AF2 |
| TIM3 | CH3 | PC8 | `PC_8` | AF2 |
| TIM3 | CH4 | PC9 | `PC_9` | AF2 |
| TIM4 | CH1 | PB6 | `PB_6` | AF2 |
| TIM4 | CH2 | PB7 | `PB_7` | AF2 |
| TIM4 | CH3 | PB8 | `PB_8` | AF2 |
| TIM4 | CH4 | PB9 | `PB_9` | AF2 |

## ADC (analog input, ADC1)

| Arduino label | Pin | PinName_t | ADC1 channel |
|---|---|---|---|
| A0 | PA0 | `PA_0` | IN0 |
| A1 | PA1 | `PA_1` | IN1 |
| A2 | PA4 | `PA_4` | IN4 |
| A3 | PB0 | `PB_0` | IN8 |
| A4 | PC1 | `PC_1` | IN11 |
| A5 | PC0 | `PC_0` | IN10 |
| — (`D13`) | PA5 | `PA_5` | IN5 |
| — (`D12`) | PA6 | `PA_6` | IN6 |
| — (`D11`) | PA7 | `PA_7` | IN7 |
| — | PB1 | `PB_1` | IN9 |
| — | PC2 | `PC_2` | IN12 |
| — | PC3 | `PC_3` | IN13 |
| — | PC4 | `PC_4` | IN14 |
| — | PC5 | `PC_5` | IN15 |

> Channels IN2/IN3 don't exist on this package. Pins without an `Ax` label (PA5–PA7, PB1, PC2–PC5) aren't on the Arduino header's analog row, but are still valid — e.g. `ADC_init(PA_6)` works the same as `ADC_init(A2)`.

---

## Conflicts

A pin can only do one job at a time — if a lab uses one of these for its "usual" role, the other peripheral sharing that pin is unavailable:

| Pin | Shared by |
|---|---|
| PA5 | LED (LD2), `D13`, `TIM2_CH1`, ADC IN5 |
| PA0 / PA1 | `A0`/`A1` ADC, and `TIM2_CH1`/`CH2` |
| PA6 | `D12` ADC IN6, and `TIM3_CH1` (alt) |
| PA9 / PA10 | `TIM1_CH2`/`CH3`, and `USART1_TX`/`RX` (default) |
| PB3 | `USART1_RX` (alt), and `TIM2_CH2` (alt) |
| PB6 / PB7 | `TIM4_CH1`/`CH2`, `USART1_TX` (alt on PB6), I2C1 SCL/SDA (see [ecI2C2.c](../include/lib-student/ecI2C2.c)) |
| PB8 / PB9 | `TIM4_CH3`/`CH4`, and the alternate I2C1 SCL/SDA pins |
| PC6 / PC7 | `TIM3_CH1`/`CH2`, and `USART6_TX`/`RX` (alt) — PC7 is also `D9` |

If a lab needs the LED and `TIM2_CH1` PWM at the same time, remap the LED read/write to another GPIO pin, or move the PWM to a different TIM2 channel/timer.
