#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/ktime.h>

int counter;
spinlock_t counter_lock;
struct task_struct *writer_thread1, *writer_thread2;
ktime_t start,end;

static int writer_function(void*data){
    start = ktime_get();
    while(!kthread_should_stop() && counter<40){
        spin_lock(&counter_lock);
        counter++;
        printk("%s, counter : %d, pid : %u\n",__func__,counter,current->pid);
        spin_unlock(&counter_lock);
        msleep(500);
    }   
    end = ktime_get();
    printk("time : %lld ns\n",end-start);
    do_exit(0);
}

static int __init my_mod_init(void){
    printk("%s, Entering module\n",__func__);
    counter = 0;
    spin_lock_init(&counter_lock);
    writer_thread1 = kthread_run(writer_function,NULL,"writer_function");
    writer_thread2 = kthread_run(writer_function,NULL,"writer_function");
    return 0;
}

static void __exit my_mod_exit(void){
    kthread_stop(writer_thread1);
    kthread_stop(writer_thread2);
    printk("%s, Exiting module\n",__func__);
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");