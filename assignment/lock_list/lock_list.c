#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/ktime.h>
#include <linux/mutex.h>

#define NUM_OF_THREAD 4

struct my_node {
    struct list_head list;
    int data;
};

struct list_head my_list;
spinlock_t sl_list;
struct rw_semaphore rw_list;
struct mutex mt_list;

int counter;
ktime_t start,end;

static void insert_node(int data){
    struct my_node *new_node = kmalloc(sizeof(struct my_node), GFP_KERNEL);
	new_node->data = data;
	list_add(&new_node->list, &my_list);
}

struct list_head* search_node(struct list_head* pos){
    pos = pos->next->next->next->next;
    return pos;
}

static int spin_insert(void*data){
    while(!kthread_should_stop()){
        spin_lock(&sl_list);
        if(counter == 100000){
            spin_unlock(&sl_list);
            break;
        }
        insert_node(++counter);
        spin_unlock(&sl_list);
    }   
    do_exit(0);
}

static int spin_search(void*data){
    struct list_head *p = &my_list;

    while(!kthread_should_stop()){
        spin_lock(&sl_list);
        if(counter == 100000){
            spin_unlock(&sl_list);
            break;
        }
        p = search_node(p);
        counter++;
        spin_unlock(&sl_list);
    } 
    do_exit(0);
}

static int spin_delete(void*data){
    struct my_node *current_node;
    struct list_head *p,*pos; 

    for(p = &my_list; p->next != &my_list;p = p->next){
        spin_lock(&sl_list);
        if(counter == 100000){
            spin_unlock(&sl_list);
            break;
        }
        pos = p->next;
        current_node = list_entry(pos,struct my_node,list);
        list_del(pos);
        kfree(current_node);
        counter++;
        spin_unlock(&sl_list);
    } 
    do_exit(0);
}

static int rw_insert(void*data){
    while(!kthread_should_stop()){
        down_write(&rw_list);
        if(counter == 100000){
            up_write(&rw_list);
            break;
        }
        insert_node(++counter);
        up_write(&rw_list);
    }   
    do_exit(0);
}

static int rw_search(void*data){
    struct list_head *p = &my_list;

    while(!kthread_should_stop()){
        down_write(&rw_list);
        if(counter == 100000){
            up_write(&rw_list);
            break;
        }
        p = search_node(p);
        counter++;
        up_write(&rw_list);
    } 
    do_exit(0);
}

static int rw_delete(void*data){
    struct my_node *current_node;
    struct list_head *p,*pos; 

    for(p = &my_list; p->next != &my_list;p = p->next){
        down_write(&rw_list);
        if(counter == 100000){
            up_write(&rw_list);
            break;
        }
        pos = p->next;
        current_node = list_entry(pos,struct my_node,list);
        list_del(pos);
        kfree(current_node);
        counter++;
        up_write(&rw_list);
    } 
    do_exit(0);
}


static int mt_insert(void*data){
    while(!kthread_should_stop()){
        mutex_lock(&mt_list);
        if(counter == 100000){
            mutex_unlock(&mt_list);
            break;
        }
        insert_node(++counter);
        mutex_unlock(&mt_list);
    }   
    do_exit(0);
}

static int mt_search(void*data){
    struct list_head *p = &my_list;

    while(!kthread_should_stop()){
        mutex_lock(&mt_list);
        if(counter == 100000){
            mutex_unlock(&mt_list);
            break;
        }
        p = search_node(p);
        counter++;
        mutex_unlock(&mt_list);
    } 
    do_exit(0);
}

static int mt_delete(void*data){
    struct my_node *current_node;
    struct list_head *p,*pos; 

    for(p = &my_list; p->next != &my_list;p = p->next){
        mutex_lock(&mt_list);
        if(counter == 100000){
            mutex_unlock(&mt_list);
            break;
        }
        pos = p->next;
        current_node = list_entry(pos,struct my_node,list);
        list_del(pos);
        kfree(current_node);
        counter++;
        mutex_unlock(&mt_list);
    } 
    do_exit(0);
}

static int __init my_mod_init(void){
    printk("%s, Entering module\n",__func__);
    INIT_LIST_HEAD(&my_list);
    int i;

    spin_lock_init(&sl_list);
    counter = 0;
    start = ktime_get();    
    for(i = 0 ;i < NUM_OF_THREAD;i++)
        kthread_run(spin_insert,NULL,"spin_insert");
    while(counter<100000){msleep(50);}
    end = ktime_get();
    printk("Spinlock linked list insert time : %lld ns\n",end-start);

    counter = 0;
    start = ktime_get();    
    for(i = 0 ;i < NUM_OF_THREAD;i++)
        kthread_run(spin_search,i,"spin_search");
    while(counter<100000){msleep(50);}
    end = ktime_get();
    printk("Spinlock linked list search time : %lld ns\n",end-start);

    counter = 0;
    start = ktime_get();    
    for(i = 0 ;i < NUM_OF_THREAD;i++)
        kthread_run(spin_search,i,"spin_search");
    while(counter<100000){msleep(50);}
    end = ktime_get();
    printk("Spinlock linked list delete time : %lld ns\n",end-start);


    init_rwsem(&rw_list);
    counter = 0;
    start = ktime_get();    
    for(i = 0 ;i < NUM_OF_THREAD;i++)
        kthread_run(rw_insert,NULL,"rw_insert");
    while(counter<100000){msleep(50);}
    end = ktime_get();
    printk("RWlock linked list insert time : %lld ns\n",end-start);

    counter = 0;
    start = ktime_get();    
    for(i = 0 ;i < NUM_OF_THREAD;i++)
        kthread_run(rw_search,i,"rw_search");
    while(counter<100000){msleep(50);}
    end = ktime_get();
    printk("RWlock linked list search time : %lld ns\n",end-start);

    counter = 0;
    start = ktime_get();    
    for(i = 0 ;i < NUM_OF_THREAD;i++)
        kthread_run(rw_search,i,"rw_search");
    while(counter<100000){msleep(50);}
    end = ktime_get();
    printk("RWlock linked list delete time : %lld ns\n",end-start);


    mutex_init(&mt_list);
    counter = 0;
    start = ktime_get();    
    for(i = 0 ;i < NUM_OF_THREAD;i++)
        kthread_run(mt_insert,NULL,"mt_insert");
    while(counter<100000){msleep(50);}
    end = ktime_get();
    printk("Mutexlock linked list insert time : %lld ns\n",end-start);

    counter = 0;
    start = ktime_get();    
    for(i = 0 ;i < NUM_OF_THREAD;i++)
        kthread_run(mt_search,i,"mt_search");
    while(counter<100000){msleep(50);}
    end = ktime_get();
    printk("Mutexlock linked list search time : %lld ns\n",end-start);

    counter = 0;
    start = ktime_get();    
    for(i = 0 ;i < NUM_OF_THREAD;i++)
        kthread_run(mt_search,i,"mt_search");
    while(counter<100000){msleep(50);}
    end = ktime_get();
    printk("Mutexlock linked list delete time : %lld ns\n",end-start);

    return 0;
}



static void __exit my_mod_exit(void){
    printk("%s, Exiting module\n",__func__);
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");