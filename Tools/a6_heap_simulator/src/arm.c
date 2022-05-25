#include <stdio.h>
#include <stdlib.h>
#include <arm.h>


unsigned int __attribute__((naked)) __clz(unsigned int val) {
	__asm__("clz r0, r0");
	__asm__("bx lr");
}

int panic(const char *func, const char *cause) {
	printf("panic: %s: %s\n", func, cause);
	abort();
}

int enter_critical_section() {
	return 0;
}

int exit_critical_section() {
	return 0;
}
