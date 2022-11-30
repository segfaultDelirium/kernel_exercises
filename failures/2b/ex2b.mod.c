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
	{ 0xdd8f8694, "module_layout" },
	{ 0x22b90774, "cdev_del" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xb65e5a32, "class_destroy" },
	{ 0x5e04e317, "device_destroy" },
	{ 0xc4952f09, "cdev_add" },
	{ 0x2064fa56, "cdev_init" },
	{ 0xebaeddf7, "device_create" },
	{ 0x2871e975, "__class_create" },
	{ 0x9a19ed29, "__register_chrdev" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "7C31339380A1C84DDE1EF79");
