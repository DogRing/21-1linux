#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/time.h>

struct my_list_node {
    struct list_head list;
    int data;
};

void list_example(int);

int __init my_list_module_init(void){
    list_example(1000);
    list_example(10000);
    list_example(100000);
    printk("list module init\n");
    return 0;
}

void __exit my_list_module_cleanup(void){
    printk("Bye Module\n");
}

module_init(my_list_module_init);
module_exit(my_list_module_cleanup);
MODULE_LICENSE("GPL");

void list_example(int length){
    struct list_head my_list;
    INIT_LIST_HEAD(&my_list);
    
    struct timespec64 begin, end;
    long diff;

    printk("create_list\n");

    get_timespec64(&begin, NULL);
    int i;
    for(i=0;i<length;i++){
        struct my_list_node *new_node = kmalloc(sizeof(struct my_list_node), GFP_KERNEL);
	new_node->data = i+1;
	list_add(&new_node->list, &my_list);
    }
    get_timespec64(&end, NULL);
    diff = timespec64_to_ns(&end) - timespec64_to_ns(&begin);
    printk("insert %d node : %ld ns\n",length,diff);

    struct my_list_node *current_node;
    struct list_head *p, *pos;

    get_timespec64(&begin, NULL);
    list_for_each(p, &my_list){
	current_node = list_entry(p,struct my_list_node, list);
	if (current_node->data == length)
	    printk("last value : %d\n",current_node->data);
    }
    get_timespec64(&end, NULL);
    diff = timespec64_to_ns(&end) - timespec64_to_ns(&begin);
    printk("search %d node : %ld ns\n",length,diff);
    
    get_timespec64(&begin, NULL);
    list_for_each_safe(pos, p, &my_list){
        current_node = list_entry(pos,struct my_list_node, list);
        list_del(pos);
        kfree(current_node);
    }
    get_timespec64(&end, NULL);
    diff = timespec64_to_ns(&end) - timespec64_to_ns(&begin);
    printk("delete %d node : %ld ns\n",length,diff);
}

