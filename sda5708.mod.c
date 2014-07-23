#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xda9e78e9, "module_layout" },
	{ 0x74738f6d, "cdev_del" },
	{ 0x85305f03, "class_destroy" },
	{ 0x4b8b420c, "device_destroy" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xab9ed595, "kobject_put" },
	{ 0xba26c2b6, "_dev_info" },
	{ 0x148887b7, "device_create" },
	{ 0xb4c75863, "__class_create" },
	{ 0xe3321d40, "cdev_add" },
	{ 0x9d0d02cf, "cdev_alloc" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xfe990052, "gpio_free" },
	{ 0xa8f59416, "gpio_direction_output" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xb81960ca, "snprintf" },
	{ 0x11089ac7, "_ctype" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x27e1a049, "printk" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x432fd7f6, "__gpio_set_value" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "3DABF639481EF9B82CE3E53");
