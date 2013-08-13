#ifndef _GPIO_H_
#define _GPIO_H_

#define PIN_LED_OK	16

void gpioSetInput(int no);
void gpioSetOutput(int no);
void gpioToggle(int pinno);
int gpioRead(int pinno);
void gpioWrite(int pinno, int level);	// level = 1 : High - level = 0 : Low

#endif // _GPIO_H_
