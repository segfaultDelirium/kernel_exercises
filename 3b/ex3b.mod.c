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
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x9a19ed29, "__register_chrdev" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xa916b694, "strnlen" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x41ed3709, "get_random_bytes" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "46CBEFE3F261E84AF3FB524");