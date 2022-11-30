#include <linux/init.h>
#include <linux/module.h>

static char *message = "default message";
#define PERMISSIONS 0664

module_param(message, charp, PERMISSIONS);

MODULE_PARM_DESC(message, "this is a message that will be displayed when inserting the module\n");

static int __init start(void){
  printk(KERN_INFO "Loading module...");
  printk("message: %s\n", message);
  return 0;
}

static void __exit end(void){
  printk(KERN_INFO "Unloading module");
}

module_init(start);
module_exit(end);
