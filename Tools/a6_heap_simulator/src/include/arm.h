#include <stdint.h>

unsigned int __clz(unsigned int val);

int enter_critical_section();
int exit_critical_section();

int panic(const char *func, const char *cause);
