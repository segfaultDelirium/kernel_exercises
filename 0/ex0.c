#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int __init start(void){
  printk(KERN_INFO "Loading module...");
  printk(KERN_INFO "Hello world!");
  return 0;
}

static void __exit end(void){
  printk(KERN_INFO "Unloading module");
}

module_init(start);
module_exit(end);
