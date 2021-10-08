#include <linux/kernel.h>

asmlinkage long sys_mycall(void){
	printk("System Call 20171688\n");

	return 0;
}
