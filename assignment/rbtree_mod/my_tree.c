#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/rbtree.h>
#include <linux/slab.h>
#include <linux/ktime.h>

typedef struct {
    struct rb_node node;
    int value;
}my_node;

void insert_node(struct rb_root *root, my_node *node){
    struct rb_node **new = &root->rb_node, *parent = NULL;
    my_node *temp;
    while(*new){
        parent = *new;
        temp = rb_entry(*new, my_node, node);
        if(temp->value < node->value)
            new = &(*new)->rb_left;
        else if (temp->value > node->value)
            new = &(*new)->rb_right;
        else return;
    }
    rb_link_node(&node->node, parent,new);
    rb_insert_color(&node->node, root);
}

my_node* search_node(struct rb_root *root, int num){
    struct rb_node **new = &root->rb_node;
    my_node *temp;
    while(*new){
        temp = rb_entry(*new, my_node, node);
        if(temp->value < num)
            new = &(*new)->rb_left;
        else if (temp->value > num)
            new = &(*new)->rb_right;
        else 
            return temp;
    }
    return NULL;
}

void tree_example(int many){
    struct rb_root root = RB_ROOT;
    int i;

    ktime_t start,end;


    start = ktime_get();
    for(i = 1; i <= many;i++){
        my_node*new_node = kmalloc(sizeof(my_node),GFP_KERNEL);
        if(!new_node){ printk("kmalloc fail\n"); return; }
        new_node->value = i;
        insert_node(&root,new_node);
    }
    end = ktime_get();
    printk("insert %d node to tree : %lld ns\n",many,end-start);

    int count = 0;
    start = ktime_get();
    for(i = 1;i<=many;i++){
        my_node*target;
        target = search_node(&root,i);
        if (target)
            count++;
    }
    end = ktime_get();
    printk("search %d node in tree : %lld ns\n",count,end-start);

    start = ktime_get();
    for(i = 1;i<=many;i++){
        my_node*target;
        target = search_node(&root,i);
        rb_erase(&target->node,&root);
        if(i == many)
            printk("last node value : %d\n",i);
    }   
    end = ktime_get();
    printk("erase %d node : %lld ns\n",many,end-start);
}

int __init my_list_module_init(void){
    printk("tree module init\n");
    tree_example(1000);
    tree_example(10000);
    tree_example(100000);
    return 0;
}

void __exit my_list_module_cleanup(void){
    printk("Bye Module\n");
}

module_init(my_list_module_init);
module_exit(my_list_module_cleanup);
MODULE_LICENSE("GPL");