/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
#include<stdio.h>
#include<stdint.h>
#include "main.h"
#define MAX_TASKS 2
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* Performing a single task but in delayed fashion using systick handler
 * In this program we will print increasing number from 0 but with delay.
 */

/* Global Variables */
SHCSR *pSHCSR = (uint32_t *) 0xE000ED24;
CFSR *pCFSR = (uint32_t *) 0xE000ED28;
uint32_t taskPSP[MAX_TASKS] = { STACK_START_T1, STACK_START_T2 };
uint32_t task_handlers[MAX_TASKS];
uint8_t curr_task = 0;
/*End Global Variables */

/*Useful functions*/
__attribute__((naked)) void init_msp_stack(uint32_t mspVal) {
	__asm volatile("MSR MSP,R0");
	__asm volatile("BX LR");
}
uint32_t get_psp_of_curtask(void){
	return taskPSP[curr_task];
}
void save_psp_of_curtask(uint32_t currpsp){
	taskPSP[curr_task] = currpsp;
}

__attribute__((naked)) void switch_sp_to_psp(void){
	__asm volatile("PUSH {LR}");
	__asm volatile("BL get_psp_of_curtask");
	__asm volatile("MSR PSP,R0");
	__asm volatile("POP {LR}");
	__asm volatile("MOV R0,#0x02");
	__asm volatile("MSR CONTROL,R0");
	__asm volatile("BX LR");
}
void tasks_stack_init(void) {
	uint32_t *psp;
	for (int i = 0; i < 2; i++) {
		psp = (uint32_t*) taskPSP[i];
		psp--;

		*psp = 0x01000000;

		psp--;

		*psp = task_handlers[i];

		psp--;

		*psp = 0xFFFFFFFD;


		for (int j = 0; j < 13; j++) {
			psp--;
			*psp = 0x0;

		}
		taskPSP[i] = (uint32_t)psp;
	}
}
void update_next_task(void) {
	curr_task++;
	curr_task %= 2;
}
/*End Useful Functions*/
void task1(void) {
	while (1) {
		printf("Hello Task1\n");

	}
}
void task2(void) {
	while (1) {
		printf("Hello Task2\n");
	}
}
int main(void) {
	/* Enabling the exceptions */

		pSHCSR->BUSFAULTENA = 1;
		pSHCSR->MEMFAULTENA = 1;
		pSHCSR->USGFAULTENA = 1;
		/* End enabling exception */
	/*Initializations*/
		init_msp_stack(STACK_START_MSP);

		task_handlers[0] =  (uint32_t)task1;
		task_handlers[1] =  (uint32_t)task2;

		tasks_stack_init();

		/*End initializations*/
	/*Systick enable*/
	uint32_t tick_value = 2;
	SYST_CSR *pCSR = (uint32_t *) 0xE000E010;
	SYST_RVR *pRVR = (uint32_t *) 0xE000E014;

	pCSR->ENABLE = 0;
	pCSR->CLKSOURCE = 1;
	pCSR->TICKINIT = 1;
	pRVR->RELOAD_VALUE = SYSTICK_CLK / (tick_value - 1);
	//Trigger the enable for systick
	pCSR->ENABLE = 1;
	/*end systick enable*/
	switch_sp_to_psp();
	task1();

	for (;;)
		;
}

__attribute__((naked))void SysTick_Handler(void) {
	//printf("Hi Entered handler\n");
	__asm volatile("MRS R0,PSP");

	__asm volatile("STMDB R0!,{R4-R11}");

	__asm volatile("PUSH {LR}");
	__asm volatile("BL save_psp_of_curtask");

	__asm volatile("BL update_next_task");

	__asm volatile("BL get_psp_of_curtask");

	__asm volatile("LDMIA R0!,{R4-R11}");

	__asm volatile("MSR PSP,R0");
	__asm volatile("POP {LR}");
	__asm volatile("BX LR");
}
void MemManage_Handler(void) {
	printf("Handling mem manage\n");
	while(1);
}
void BusFault_Handler(void) {
	printf("Handling bus fault\n");
	while(1);
}
void UsageFault_Handler(void) {
	printf("Handling Usagefault\n");
	while(1);
}
