sdio pins
=========

oob_irq
-------

port = 40020400
pin = 00000000
PB0

sdio_clk
--------

port = 40020800
pin = 0000000c
PC12 (AF12)

sdio_cmd
--------

port = 40020c00
pin = 00000002
PD2 (AF12)

sdio_d0
-------

port = 40020800
pin = 00000008
PC8 (AF12)

sdio_d1
-------

port = 40020800
pin = 00000009
PC9 (AF12)

sdio_d2
-------

port = 40020800
pin = 0000000a
PC10 (AF12)

sdio_d3
-------

port = 40020800
pin = 0000000b
PC11 (AF12)

control pins
============

power (not used)
----------------

port = 40020800
pin = 00000007
PC7

reset
-----

port = 40020800
pin = 00000001
PC1

low  - chip in reset
high - chip not in reset

32k_clk
-------

port = 40020400
pin = 00000001
PB1, PC7 instead?

low  - low power mode
high - normal power mode

bootstrap_0 (not used)
----------------------

port = 40020400
pin = 00000000
PB0

bootstrap_1 (not used)
----------------------

port = 40020400
pin = 00000002
PB2

Photon registers
================

STM32_RCC->CR = 0x3037083;
STM32_RCC->PLLCFGR = 0x5403C1A;
STM32_RCC->CFGR = 0x20940A;
STM32_RCC->CIR = 0x0;
STM32_RCC->AHB1RSTR = 0x0;
STM32_RCC->AHB2RSTR = 0x0;
STM32_RCC->AHB3RSTR = 0x0;
STM32_RCC->APB1RSTR = 0x0;
STM32_RCC->APB2RSTR = 0x0;
STM32_RCC->AHB1ENR = 0x2044100F;
STM32_RCC->AHB2ENR = 0x40;
STM32_RCC->AHB3ENR = 0x0;
STM32_RCC->APB1ENR = 0x10000001;
STM32_RCC->APB2ENR = 0x4810;
STM32_RCC->AHB1LPENR = 0x7E6791FF;
STM32_RCC->AHB2LPENR = 0xF1;
STM32_RCC->AHB3LPENR = 0x1;
STM32_RCC->APB1LPENR = 0x36FEC9FF;
STM32_RCC->APB2LPENR = 0x75F33;
STM32_RCC->BDCR = 0x8103;
STM32_RCC->CS = 0x14000000;
STM32_RCC->SSCGR = 0x0;
STM32_RCC->PLLI2SCFGR = 0x20003000;

STM32_SDIO->POWER = 0x3;
STM32_SDIO->CLKCR = 0x900;
STM32_SDIO->DLEN = 0x80;
STM32_SDIO->DCTRL = 0x869;
STM32_SDIO->STA = 0x40;
STM32_SDIO->MASK = 0x400000;

STM32_GPIOA->MODER = 0x42800A8;
STM32_GPIOA->OTYPER = 0x0;
STM32_GPIOA->OSPEEDR = 0xC3C00FC;
STM32_GPIOA->PUPDR = 0x0;
STM32_GPIOA->ODR = 0x0;
STM32_GPIOA->AFR[0] = 0x1110;
STM32_GPIOA->AFR[1] = 0x770;

STM32_GPIOB->MODER = 0xA0000008;
STM32_GPIOB->OTYPER = 0x0;
STM32_GPIOB->OSPEEDR = 0xF000A0C8;
STM32_GPIOB->PUPDR = 0x0;
STM32_GPIOB->ODR = 0x0;
STM32_GPIOB->AFR[0] = 0x0;
STM32_GPIOB->AFR[1] = 0xCC000000;

STM32_GPIOC->MODER = 0x2AA0004;
STM32_GPIOC->OTYPER = 0x0;
STM32_GPIOC->OSPEEDR = 0x2AA0008;
STM32_GPIOC->PUPDR = 0x1554000;
STM32_GPIOC->ODR = 0x2;
STM32_GPIOC->AFR[0] = 0x0;
STM32_GPIOC->AFR[1] = 0xCCCCC;

STM32_GPIOD->MODER = 0x20;
STM32_GPIOD->OTYPER = 0x0;
STM32_GPIOD->OSPEEDR = 0x20;
STM32_GPIOD->PUPDR = 0x10;
STM32_GPIOD->ODR = 0x0;
STM32_GPIOD->AFR[0] = 0xC00;
STM32_GPIOD->AFR[1] = 0x0;

Debug Particle
==============

Serial1.print("STM32_RCC->CR = 0x");
Serial1.print(RCC->CR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->PLLCFGR = 0x");
Serial1.print(RCC->PLLCFGR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->CFGR = 0x");
Serial1.print(RCC->CFGR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->CIR = 0x");
Serial1.print(RCC->CIR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->AHB1RSTR = 0x");
Serial1.print(RCC->AHB1RSTR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->AHB2RSTR = 0x");
Serial1.print(RCC->AHB2RSTR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->AHB3RSTR = 0x");
Serial1.print(RCC->AHB3RSTR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->APB1RSTR = 0x");
Serial1.print(RCC->APB1RSTR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->APB2RSTR = 0x");
Serial1.print(RCC->APB2RSTR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->AHB1ENR = 0x");
Serial1.print(RCC->AHB1ENR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->AHB2ENR = 0x");
Serial1.print(RCC->AHB2ENR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->AHB3ENR = 0x");
Serial1.print(RCC->AHB3ENR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->APB1ENR = 0x");
Serial1.print(RCC->APB1ENR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->APB2ENR = 0x");
Serial1.print(RCC->APB2ENR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->AHB1LPENR = 0x");
Serial1.print(RCC->AHB1LPENR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->AHB2LPENR = 0x");
Serial1.print(RCC->AHB2LPENR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->AHB3LPENR = 0x");
Serial1.print(RCC->AHB3LPENR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->APB1LPENR = 0x");
Serial1.print(RCC->APB1LPENR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->APB2LPENR = 0x");
Serial1.print(RCC->APB2LPENR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->BDCR = 0x");
Serial1.print(RCC->BDCR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->CSR = 0x");
Serial1.print(RCC->CSR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->SSCGR = 0x");
Serial1.print(RCC->SSCGR, HEX);
Serial1.println(";");
Serial1.print("STM32_RCC->PLLI2SCFGR = 0x");
Serial1.print(RCC->PLLI2SCFGR, HEX);
Serial1.println(";");

Serial1.print("STM32_SDIO->POWER = 0x");
Serial1.print(SDIO->POWER, HEX);
Serial1.println(";");
Serial1.print("STM32_SDIO->CLKCR = 0x");
Serial1.print(SDIO->CLKCR, HEX);
Serial1.println(";");
Serial1.print("STM32_SDIO->DLEN = 0x");
Serial1.print(SDIO->DLEN, HEX);
Serial1.println(";");
Serial1.print("STM32_SDIO->DCTRL = 0x");
Serial1.print(SDIO->DCTRL, HEX);
Serial1.println(";");
Serial1.print("STM32_SDIO->STA = 0x");
Serial1.print(SDIO->STA, HEX);
Serial1.println(";");
Serial1.print("STM32_SDIO->MASK = 0x");
Serial1.print(SDIO->MASK, HEX);
Serial1.println(";");

Serial1.print("STM32_GPIOA->MODER = 0x");
Serial1.print(GPIOA->MODER, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOA->OTYPER = 0x");
Serial1.print(GPIOA->OTYPER, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOA->OSPEEDR = 0x");
Serial1.print(GPIOA->OSPEEDR, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOA->PUPDR = 0x");
Serial1.print(GPIOA->PUPDR, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOA->ODR = 0x");
Serial1.print(GPIOA->ODR, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOA->AFR[0] = 0x");
Serial1.print(GPIOA->AFR[0], HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOA->AFR[1] = 0x");
Serial1.print(GPIOA->AFR[1], HEX);
Serial1.println(";");

Serial1.print("STM32_GPIOB->MODER = 0x");
Serial1.print(GPIOB->MODER, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOB->OTYPER = 0x");
Serial1.print(GPIOB->OTYPER, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOB->OSPEEDR = 0x");
Serial1.print(GPIOB->OSPEEDR, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOB->PUPDR = 0x");
Serial1.print(GPIOB->PUPDR, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOB->ODR = 0x");
Serial1.print(GPIOB->ODR, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOB->AFR[0] = 0x");
Serial1.print(GPIOB->AFR[0], HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOB->AFR[1] = 0x");
Serial1.print(GPIOB->AFR[1], HEX);
Serial1.println(";");

Serial1.print("STM32_GPIOC->MODER = 0x");
Serial1.print(GPIOC->MODER, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOC->OTYPER = 0x");
Serial1.print(GPIOC->OTYPER, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOC->OSPEEDR = 0x");
Serial1.print(GPIOC->OSPEEDR, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOC->PUPDR = 0x");
Serial1.print(GPIOC->PUPDR, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOC->ODR = 0x");
Serial1.print(GPIOC->ODR, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOC->AFR[0] = 0x");
Serial1.print(GPIOC->AFR[0], HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOC->AFR[1] = 0x");
Serial1.print(GPIOC->AFR[1], HEX);
Serial1.println(";");

Serial1.print("STM32_GPIOD->MODER = 0x");
Serial1.print(GPIOD->MODER, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOD->OTYPER = 0x");
Serial1.print(GPIOD->OTYPER, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOD->OSPEEDR = 0x");
Serial1.print(GPIOD->OSPEEDR, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOD->PUPDR = 0x");
Serial1.print(GPIOD->PUPDR, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOD->ODR = 0x");
Serial1.print(GPIOD->ODR, HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOD->AFR[0] = 0x");
Serial1.print(GPIOD->AFR[0], HEX);
Serial1.println(";");
Serial1.print("STM32_GPIOD->AFR[1] = 0x");
Serial1.print(GPIOD->AFR[1], HEX);
Serial1.println(";");

Debug Simba
===========
    
std_printf(FSTR("STM32_RCC->CR = 0x%lx\r\n"), STM32_RCC->CR);
std_printf(FSTR("STM32_RCC->PLLCFGR = 0x%lx\r\n"), STM32_RCC->PLLCFGR);
std_printf(FSTR("STM32_RCC->CFGR = 0x%lx\r\n"), STM32_RCC->CFGR);
std_printf(FSTR("STM32_RCC->CIR = 0x%lx\r\n"), STM32_RCC->CIR);
std_printf(FSTR("STM32_RCC->AHB1RSTR = 0x%lx\r\n"), STM32_RCC->AHB1RSTR);
std_printf(FSTR("STM32_RCC->AHB2RSTR = 0x%lx\r\n"), STM32_RCC->AHB2RSTR);
std_printf(FSTR("STM32_RCC->AHB3RSTR = 0x%lx\r\n"), STM32_RCC->AHB3RSTR);
std_printf(FSTR("STM32_RCC->APB1RSTR = 0x%lx\r\n"), STM32_RCC->APB1RSTR);
std_printf(FSTR("STM32_RCC->APB2RSTR = 0x%lx\r\n"), STM32_RCC->APB2RSTR);
std_printf(FSTR("STM32_RCC->AHB1ENR = 0x%lx\r\n"), STM32_RCC->AHB1ENR);
std_printf(FSTR("STM32_RCC->AHB2ENR = 0x%lx\r\n"), STM32_RCC->AHB2ENR);
std_printf(FSTR("STM32_RCC->AHB3ENR = 0x%lx\r\n"), STM32_RCC->AHB3ENR);
std_printf(FSTR("STM32_RCC->APB1ENR = 0x%lx\r\n"), STM32_RCC->APB1ENR);
std_printf(FSTR("STM32_RCC->APB2ENR = 0x%lx\r\n"), STM32_RCC->APB2ENR);
std_printf(FSTR("STM32_RCC->AHB1LPENR = 0x%lx\r\n"), STM32_RCC->AHB1LPENR);
std_printf(FSTR("STM32_RCC->AHB2LPENR = 0x%lx\r\n"), STM32_RCC->AHB2LPENR);
std_printf(FSTR("STM32_RCC->AHB3LPENR = 0x%lx\r\n"), STM32_RCC->AHB3LPENR);
std_printf(FSTR("STM32_RCC->APB1LPENR = 0x%lx\r\n"), STM32_RCC->APB1LPENR);
std_printf(FSTR("STM32_RCC->APB2LPENR = 0x%lx\r\n"), STM32_RCC->APB2LPENR);
std_printf(FSTR("STM32_RCC->BDCR = 0x%lx\r\n"), STM32_RCC->BDCR);
std_printf(FSTR("STM32_RCC->CSR = 0x%lx\r\n"), STM32_RCC->CSR);
std_printf(FSTR("STM32_RCC->SSCGR = 0x%lx\r\n"), STM32_RCC->SSCGR);
std_printf(FSTR("STM32_RCC->PLLI2SCFGR = 0x%lx\r\n"), STM32_RCC->PLLI2SCFGR);

std_printf(FSTR("STM32_SDIO->POWER = 0x%lx\r\n"), STM32_SDIO->POWER);
std_printf(FSTR("STM32_SDIO->CLKCR = 0x%lx\r\n"), STM32_SDIO->CLKCR);
std_printf(FSTR("STM32_SDIO->DLEN = 0x%lx\r\n"), STM32_SDIO->DLEN);
std_printf(FSTR("STM32_SDIO->DCTRL = 0x%lx\r\n"), STM32_SDIO->DCTRL);
std_printf(FSTR("STM32_SDIO->STA = 0x%lx\r\n"), STM32_SDIO->STA);
std_printf(FSTR("STM32_SDIO->MASK = 0x%lx\r\n"), STM32_SDIO->MASK);

std_printf(FSTR("STM32_GPIOA->MODER = 0x%lx\r\n"), STM32_GPIOA->MODER);
std_printf(FSTR("STM32_GPIOA->OTYPER = 0x%lx\r\n"), STM32_GPIOA->OTYPER);
std_printf(FSTR("STM32_GPIOA->OSPEEDR = 0x%lx\r\n"), STM32_GPIOA->OSPEEDR);
std_printf(FSTR("STM32_GPIOA->PUPDR = 0x%lx\r\n"), STM32_GPIOA->PUPDR);
std_printf(FSTR("STM32_GPIOA->ODR = 0x%lx\r\n"), STM32_GPIOA->ODR);
std_printf(FSTR("STM32_GPIOA->AFR[0] = 0x%lx\r\n"), STM32_GPIOA->AFR[0]);
std_printf(FSTR("STM32_GPIOA->AFR[1] = 0x%lx\r\n"), STM32_GPIOA->AFR[1]);

std_printf(FSTR("STM32_GPIOB->MODER = 0x%lx\r\n"), STM32_GPIOB->MODER);
std_printf(FSTR("STM32_GPIOB->OTYPER = 0x%lx\r\n"), STM32_GPIOB->OTYPER);
std_printf(FSTR("STM32_GPIOB->OSPEEDR = 0x%lx\r\n"), STM32_GPIOB->OSPEEDR);
std_printf(FSTR("STM32_GPIOB->PUPDR = 0x%lx\r\n"), STM32_GPIOB->PUPDR);
std_printf(FSTR("STM32_GPIOB->ODR = 0x%lx\r\n"), STM32_GPIOB->ODR);
std_printf(FSTR("STM32_GPIOB->AFR[0] = 0x%lx\r\n"), STM32_GPIOB->AFR[0]);
std_printf(FSTR("STM32_GPIOB->AFR[1] = 0x%lx\r\n"), STM32_GPIOB->AFR[1]);

std_printf(FSTR("STM32_GPIOC->MODER = 0x%lx\r\n"), STM32_GPIOC->MODER);
std_printf(FSTR("STM32_GPIOC->OTYPER = 0x%lx\r\n"), STM32_GPIOC->OTYPER);
std_printf(FSTR("STM32_GPIOC->OSPEEDR = 0x%lx\r\n"), STM32_GPIOC->OSPEEDR);
std_printf(FSTR("STM32_GPIOC->PUPDR = 0x%lx\r\n"), STM32_GPIOC->PUPDR);
std_printf(FSTR("STM32_GPIOC->ODR = 0x%lx\r\n"), STM32_GPIOC->ODR);
std_printf(FSTR("STM32_GPIOC->AFR[0] = 0x%lx\r\n"), STM32_GPIOC->AFR[0]);
std_printf(FSTR("STM32_GPIOC->AFR[1] = 0x%lx\r\n"), STM32_GPIOC->AFR[1]);

std_printf(FSTR("STM32_GPIOD->MODER = 0x%lx\r\n"), STM32_GPIOD->MODER);
std_printf(FSTR("STM32_GPIOD->OTYPER = 0x%lx\r\n"), STM32_GPIOD->OTYPER);
std_printf(FSTR("STM32_GPIOD->OSPEEDR = 0x%lx\r\n"), STM32_GPIOD->OSPEEDR);
std_printf(FSTR("STM32_GPIOD->PUPDR = 0x%lx\r\n"), STM32_GPIOD->PUPDR);
std_printf(FSTR("STM32_GPIOD->ODR = 0x%lx\r\n"), STM32_GPIOD->ODR);
std_printf(FSTR("STM32_GPIOD->AFR[0] = 0x%lx\r\n"), STM32_GPIOD->AFR[0]);
std_printf(FSTR("STM32_GPIOD->AFR[1] = 0x%lx\r\n"), STM32_GPIOD->AFR[1]);

before
------

STM32_RCC->CR = 0x3037083
STM32_RCC->PLLCFGR = 0x5403c1a
STM32_RCC->CFGR = 0x940a
STM32_RCC->CIR = 0x0
STM32_RCC->AHB1RSTR = 0x0
STM32_RCC->AHB2RSTR = 0x0
STM32_RCC->AHB3RSTR = 0x0
STM32_RCC->APB1RSTR = 0x0
STM32_RCC->APB2RSTR = 0x0
STM32_RCC->AHB1ENR = 0x7e6411ff
STM32_RCC->AHB2ENR = 0xf1
STM32_RCC->AHB3ENR = 0x0
STM32_RCC->APB1ENR = 0x36fec9ff
STM32_RCC->APB2ENR = 0x75f33
STM32_RCC->AHB1LPENR = 0x7e6791ff
STM32_RCC->AHB2LPENR = 0xf1
STM32_RCC->AHB3LPENR = 0x1
STM32_RCC->APB1LPENR = 0x36fec9ff
STM32_RCC->APB2LPENR = 0x75f33
STM32_RCC->BDCR = 0x8103
STM32_RCC->CSR = 0x14000000
STM32_RCC->SSCGR = 0x0
STM32_RCC->PLLI2SCFGR = 0x20003000

STM32_SDIO->POWER = 0x0
STM32_SDIO->CLKCR = 0x0
STM32_SDIO->DLEN = 0x0
STM32_SDIO->DCTRL = 0x0
STM32_SDIO->STA = 0x0
STM32_SDIO->MASK = 0x0

STM32_GPIOA->MODER = 0xa82800a8
STM32_GPIOA->OTYPER = 0x0
STM32_GPIOA->OSPEEDR = 0xc0000fc
STM32_GPIOA->PUPDR = 0x64000000
STM32_GPIOA->ODR = 0x0
STM32_GPIOA->AFR[0] = 0x1110
STM32_GPIOA->AFR[1] = 0x770

STM32_GPIOB->MODER = 0x280
STM32_GPIOB->OTYPER = 0x0
STM32_GPIOB->OSPEEDR = 0xc0
STM32_GPIOB->PUPDR = 0x100
STM32_GPIOB->ODR = 0x0
STM32_GPIOB->AFR[0] = 0x0
STM32_GPIOB->AFR[1] = 0x0

STM32_GPIOC->MODER = 0x0
STM32_GPIOC->OTYPER = 0x0
STM32_GPIOC->OSPEEDR = 0x0
STM32_GPIOC->PUPDR = 0x4000
STM32_GPIOC->ODR = 0x0
STM32_GPIOC->AFR[0] = 0x0
STM32_GPIOC->AFR[1] = 0x0

STM32_GPIOD->MODER = 0x0
STM32_GPIOD->OTYPER = 0x0
STM32_GPIOD->OSPEEDR = 0x0
STM32_GPIOD->PUPDR = 0x0
STM32_GPIOD->ODR = 0x0
STM32_GPIOD->AFR[0] = 0x0
STM32_GPIOD->AFR[1] = 0x0

after
-----

STM32_RCC->CR = 0x3037083
STM32_RCC->PLLCFGR = 0x5403c1a
STM32_RCC->CFGR = 0x940a
STM32_RCC->CIR = 0x0
STM32_RCC->AHB1RSTR = 0x0
STM32_RCC->AHB2RSTR = 0x0
STM32_RCC->AHB3RSTR = 0x0
STM32_RCC->APB1RSTR = 0x0
STM32_RCC->APB2RSTR = 0x0
STM32_RCC->AHB1ENR = 0x7e6411ff
STM32_RCC->AHB2ENR = 0xf1
STM32_RCC->AHB3ENR = 0x0
STM32_RCC->APB1ENR = 0x36fec9ff
STM32_RCC->APB2ENR = 0x75f33
STM32_RCC->AHB1LPENR = 0x7e6791ff
STM32_RCC->AHB2LPENR = 0xf1
STM32_RCC->AHB3LPENR = 0x1
STM32_RCC->APB1LPENR = 0x36fec9ff
STM32_RCC->APB2LPENR = 0x75f33
STM32_RCC->BDCR = 0x8103
STM32_RCC->CSR = 0x14000000
STM32_RCC->SSCGR = 0x0
STM32_RCC->PLLI2SCFGR = 0x20003000

STM32_SDIO->POWER = 0x3
STM32_SDIO->CLKCR = 0x178
STM32_SDIO->DLEN = 0x0
STM32_SDIO->DCTRL = 0x0
STM32_SDIO->STA = 0x0
STM32_SDIO->MASK = 0xffffff

STM32_GPIOA->MODER = 0xa82800a8
STM32_GPIOA->OTYPER = 0x0
STM32_GPIOA->OSPEEDR = 0xc0000fc
STM32_GPIOA->PUPDR = 0x64000000
STM32_GPIOA->ODR = 0x0
STM32_GPIOA->AFR[0] = 0x1110
STM32_GPIOA->AFR[1] = 0x770

STM32_GPIOB->MODER = 0x280
STM32_GPIOB->OTYPER = 0x0
STM32_GPIOB->OSPEEDR = 0xc0
STM32_GPIOB->PUPDR = 0x100
STM32_GPIOB->ODR = 0x0
STM32_GPIOB->AFR[0] = 0x0
STM32_GPIOB->AFR[1] = 0x0

STM32_GPIOC->MODER = 0x2aa0004
STM32_GPIOC->OTYPER = 0x0
STM32_GPIOC->OSPEEDR = 0x0
STM32_GPIOC->PUPDR = 0x1554000
STM32_GPIOC->ODR = 0x2
STM32_GPIOC->AFR[0] = 0x0
STM32_GPIOC->AFR[1] = 0xccccc

STM32_GPIOD->MODER = 0x20
STM32_GPIOD->OTYPER = 0x0
STM32_GPIOD->OSPEEDR = 0x0
STM32_GPIOD->PUPDR = 0x10
STM32_GPIOD->ODR = 0x0
STM32_GPIOD->AFR[0] = 0xc00
STM32_GPIOD->AFR[1] = 0x0

Todo
====

RCC->CFGR.MCO1 = 1; # Micro Controller Output 1

STM32_SDIO->CLKCR = 0x900

wwd_management_wifi_on

00000000 <host_platform_init>:
   0:	b510      	push	{r4, lr}
   2:	4c06      	ldr	r4, [pc, #24]	; (1c <host_platform_init+0x1c>)
   4:	f7ff fffe 	bl	0 <host_platform_init>
   8:	2103      	movs	r1, #3
   a:	4620      	mov	r0, r4
   c:	f7ff fffe 	bl	0 <platform_gpio_init>
  10:	4620      	mov	r0, r4
  12:	f7ff fffe 	bl	0 <platform_gpio_output_low>
  16:	2000      	movs	r0, #0
  18:	bd10      	pop	{r4, pc}
  1a:	bf00      	nop
  1c:	00000008 	.word	0x00000008
