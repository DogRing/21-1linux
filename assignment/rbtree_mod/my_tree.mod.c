#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
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
__used __section(__versions) = {
	{ 0x8938a48, "module_layout" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x4d9b652b, "rb_erase" },
	{ 0xc5850110, "printk" },
	{ 0xf5ae3ef, "kmem_cache_alloc_trace" },
	{ 0x8af8507, "kmalloc_caches" },
	{ 0xb43f9365, "ktime_get" },
	{ 0xa5526619, "rb_insert_color" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "EFD5CD0380C0F49A9F9FD4C");
