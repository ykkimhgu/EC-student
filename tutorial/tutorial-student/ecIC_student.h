#ifndef __EC_TIM_H
#define __EC_TIM_H
#include "stm32f411xe.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


/* Input Capture*/

// Edge Type
#define RISE 0
#define FALL 1
#define BOTH 2

//Input Capture

typedef struct{
	GPIO_TypeDef *port;
	int pin;   
	TIM_TypeDef *timer;
	int ch;  		//int Timer Channel
	int ICnum;  //int IC number
} IC_t;



void ICAP_init(IC_t *ICx, GPIO_TypeDef *port, int pin);
void ICAP_setup(IC_t *ICx, int ICn_type, int edge_type);
void ICAP_counter_us(IC_t *ICx, int usec);

void ICAP_pinmap(IC_t *timer_pin);

uint32_t is_pending_TIM(TIM_TypeDef *TIMx);
void clear_pending_TIM(TIM_TypeDef *TIMx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
