#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

struct my_list_node {
    struct list_head list;
    int data;
};

void list_example(int);

int __init my_list_module_init(void){
    printk("list module init\n");
    list_example(1000);
    list_example(10000);
    list_example(100000);
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
    
    ktime_t start,end;
    int temp;

    printk("create_list\n");
    int i;
    start = ktime_get();
    for(i=0;i<length;i++){
        struct my_list_node *new_node = kmalloc(sizeof(struct my_list_node), GFP_KERNEL);
	new_node->data = i+1;
	list_add(&new_node->list, &my_list);
    }
    end = ktime_get();
    printk("insert %d node : %lld ns\n",length,end-start);

    struct my_list_node *current_node;
    struct list_head *p, *pos;

    start = ktime_get();
    list_for_each(p, &my_list){
	current_node = list_entry(p,struct my_list_node, list);
	if (current_node->data == length)
	    printk("last value : %d\n",current_node->data);
    }
    end = ktime_get();
    printk("search %d node : %lld ns\n",length,end-start);


    start = ktime_get();
    list_for_each_safe(pos, p, &my_list){
        current_node = list_entry(pos,struct my_list_node, list);
        list_del(pos);
        kfree(current_node);
    }
    end = ktime_get();
    printk("delete %d node : %lld ns\n",length,end-start);

}

