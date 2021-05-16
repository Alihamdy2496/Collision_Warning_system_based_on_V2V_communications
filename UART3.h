#ifndef UART1_H_
#define UART1_H_
#include "TM4C123GH6PM.h"
#include "standard_types.h"
/*#include "GPIO_Driver.h"*/

extern void UART3_INIT(void);
extern void UART3_sendByte(char c);
extern uint8 UART3_recieveByte(void);
extern void UART3_sendString(const uint8 *Str);
extern void UART3_receiveString(uint8 *Str);


#endif /* UART1_H_ */
