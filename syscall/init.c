
#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#include <sys/ucontext.h>

#include "init.h"

int cntr = 0;

void os_sighnd(int sig, siginfo_t *info, void *ctx) {
	ucontext_t *uc = (ucontext_t *) ctx;

	uint16_t instr_ptr = (*(uint16_t *) uc->uc_mcontext.gregs[REG_RIP]);
	
	int index = 0;
	int base_offset = 2;
	int extra_offset = 0;

	if ( !((instr_ptr & 0xffff)  == 0x138b || instr_ptr == 0x0b8b) ) {
		index += (((*(uint32_t *) uc->uc_mcontext.gregs[REG_RIP]) >> 16) & 0xff) >> 2;
		extra_offset = 1;
	}
		
	if ((instr_ptr & 0xff00) == 0x5500 || (instr_ptr & 0xff00) == 0x4d00) {
		index += ((uint32_t) uc->uc_mcontext.gregs[REG_RBP]) - ((uint32_t) uc->uc_mcontext.gregs[REG_RBX]) >> 2;
	} 
	int sgn_reg = ((instr_ptr & 0xffff) == 0x0b8b || (instr_ptr & 0xffff) == 0x4d8b) ? REG_RCX : REG_RDX;
		
	cntr++;

	uc->uc_mcontext.gregs[sgn_reg] = 100000 + 1000 * index + cntr;
	uc->uc_mcontext.gregs[REG_RIP] += base_offset + extra_offset;
}

void init(void *base) {
	struct sigaction act = {
		.sa_sigaction = os_sighnd,
		.sa_flags = SA_RESTART,
	};
	sigemptyset(&act.sa_mask);

	if (-1 == sigaction(SIGSEGV, &act, NULL)) {
		perror("signal set failed");
		exit(1);
	}
}