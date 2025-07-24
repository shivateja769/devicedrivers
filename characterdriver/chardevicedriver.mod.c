#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

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



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x69b07fa1, "class_create" },
	{ 0x2f920287, "cdev_del" },
	{ 0xc8cf812f, "device_create" },
	{ 0x73ca058d, "class_destroy" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x281722e1, "device_destroy" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x92997ed8, "_printk" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xd0802696, "cdev_init" },
	{ 0xaf860c30, "cdev_add" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xbf1981cb, "module_layout" },
};

MODULE_INFO(depends, "");

