/*
 * main.h
 *
 *  Created on: Dec 7, 2020
 *      Author: dell
 */

#ifndef MAIN_H_
#define MAIN_H_
#define SRAM_START 				0x20000000U
#define SRAM_SIZE  				(96 * 1024)
#define SRAM_END  				( (SRAM_START) + (SRAM_SIZE) )
#define STACK_START SRAM_END

#define STACK_START_T1 (SRAM_END)
#define STACK_START_T2 ((SRAM_END) - (1024))
#define STACK_START_MSP ((SRAM_END) - (2*1024))


#define SYSTICK_CLK 16000000U
typedef struct {
	uint32_t ENABLE:1;
	uint32_t TICKINIT:1;
	uint32_t CLKSOURCE:1;
	uint32_t res1:13;
	uint32_t COUNTFLAG:1;
	uint32_t res2:15;
} SYST_CSR;
typedef struct{
	uint32_t RELOAD_VALUE:24;
	uint32_t res1:8;
}SYST_RVR;

typedef struct {
	uint32_t MEMFAULTACT :1;
	uint32_t BUSFAULTACT :1;
	uint32_t res1 :1;
	uint32_t USGFAULTACT :1;
	uint32_t res2 :3;
	uint32_t SVCALLACT :1;
	uint32_t MONITORACT :1;
	uint32_t res3 :1;
	uint32_t PENDSVACT :1;
	uint32_t SYSTICKACT :1;
	uint32_t USGFAULTPENDED :1;
	uint32_t MEMFAULTPENDED :1;
	uint32_t BUSFAULTPENDED :1;
	uint32_t SVCALLPENDED :1;
	uint32_t MEMFAULTENA :1;
	uint32_t BUSFAULTENA :1;
	uint32_t USGFAULTENA :1;
	uint32_t res4:13;
} SHCSR;

typedef struct{
	uint32_t MMFSR:8;
	uint32_t BFSR:8;
	uint32_t UFSR:16;
}CFSR;
#endif /* MAIN_H_ */
