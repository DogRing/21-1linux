#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xeeab4c1e, "module_layout" },
	{ 0x977f511b, "__mutex_init" },
	{ 0x7b4da6ff, "__init_rwsem" },
	{ 0xf9a482f9, "msleep" },
	{ 0x2fbefc53, "wake_up_process" },
	{ 0xe554678a, "kthread_create_on_node" },
	{ 0xb43f9365, "ktime_get" },
	{ 0xc5850110, "printk" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0x1a81ff9c, "pv_ops" },
	{ 0xce807a25, "up_write" },
	{ 0x57bc19d2, "down_write" },
	{ 0xd60e87c0, "kmem_cache_alloc_trace" },
	{ 0x6aa9e858, "kmalloc_caches" },
	{ 0x952664c5, "do_exit" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x409bcb62, "mutex_unlock" },
	{ 0x2ab7989d, "mutex_lock" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "AD0075CF32B5159C3C0E93A");
