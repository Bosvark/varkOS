#include "gpio.h"
#include "mmio.h"

enum{
	GPBASE		= 0x20200000,
	GPFSEL0		= 0x20200000,
	GPFSEL1		= 0x20200004,
	GPFSEL2		= 0x20200008,
	GPFSEL3		= 0x2020000C,
	GPFSEL4		= 0x20200010,
	GPFSEL5		= 0x20200014,
	GPSET0		= 0x2020001C,
	GPSET1		= 0x20200020,
	GPCLR0		= 0x20200028,
	GPCLR1		= 0x2020002C,
	GPLEV0		= 0x20200034,
	GPLEV1		= 0x20200038,
	GPEDS0		= 0x20200040,
	GPEDS1		= 0x20200044,
	GPREN0		= 0x2020004C,
	GPREN1		= 0x20200050,
	GPFEN0		= 0x20200058,
	GPFEN1		= 0x2020005C,
	GPHEN0		= 0x20200064,
	GPHEN1		= 0x20200068,
	GPLEN0		= 0x20200070,
	GPLEN1		= 0x20200074,
	GPAREN0		= 0x2020007C,
	GPAREN1		= 0x20200080,
	GPAFEN0		= 0x20200088,
	GPAFEN1		= 0x2020008C,
	GPPUD		= 0x20200094,
	GPPUDCLK0	= 0x20200098,
	GPPUDCLK1	= 0x2020009C
};

int get_fsel_bank_no(int pinno);

void gpioSetOutput(int no)
{
	int pinno=no%10;
	int bankno=get_fsel_bank_no(pinno);
	int ireg=mmio_read(bankno);

	ireg |= 1 << (pinno*3);
	mmio_write(bankno, ireg);
}

void gpioSetInput(int no)
{
	int pinno=no%10;
	int bankno=get_fsel_bank_no(pinno);
	int ireg=mmio_read(bankno);

	ireg &= ~(1 << (pinno*3));
	mmio_write(bankno, ireg);
}

void gpioToggle(int pinno)
{
	gpioRead(pinno) ? gpioWrite(pinno, 0) : gpioWrite(pinno, 1);
}

int gpioRead(int pinno)
{
	int ireg;

	if(pinno >= 32){
		pinno = pinno - 32;
		ireg = mmio_read(GPLEV1);
	}else{
		ireg = mmio_read(GPLEV0);
	}

	return (ireg & (1 << pinno));
}

void gpioWrite(int pinno, int level)
{
	if(pinno >= 32){
		pinno = pinno - 32;
		mmio_write(level ? GPSET1 : GPCLR1, (1 << pinno));
	}else{
		mmio_write(level ? GPSET0 : GPCLR0, (1 << pinno));
	}
}

void gpioWriteSafe(int pinno, int level)
{
	int regval;

	if(pinno >= 32){
		pinno = pinno - 32;
		regval = mmio_read(GPLEV1);
	}else{
		regval = mmio_read(GPLEV0);
	}

	regval |= (1 << pinno);

	if(pinno >= 32){
		mmio_write(level ? GPSET1 : GPCLR1, regval);
	}else{
		mmio_write(level ? GPSET0 : GPCLR0, regval);
	}
}

//
// Local functions
//

int get_fsel_bank_no(int pinno)
{
	if(pinno >= 50)			return GPFSEL5;
	else if(pinno >= 40)	return GPFSEL4;
	else if(pinno >= 30)	return GPFSEL3;
	else if(pinno >= 20)	return GPFSEL2;
	else if(pinno >= 10)	return GPFSEL1;
	else					return GPFSEL0;

}
