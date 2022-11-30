#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>


#define MYMAJOR 91
#define NAME "ex1_module"

static int driver_open(struct inode *device_file, struct file *instance){
  printk("%s - open was called\n", NAME);
  return 0;
}

static int driver_close(struct inode *device_file, struct file *instance){
  printk("%s - close was called\n", NAME);
  return 0;
}

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = driver_open,
  .release = driver_close
};


static int __init start(void){
  printk(KERN_INFO "Loading module...");
  printk(KERN_INFO "Hello world!");

  int retval = register_chrdev(MYMAJOR, NAME, &fops);
  if(retval == 0){
    printk("dev_nr - registered device number major: %d, minor: %d\n", MYMAJOR, 0);
  }
  else{
    printk("couldn not register device number!\n");
    return -1;
  }

  return 0;
}

static void __exit end(void){
  printk(KERN_INFO "Unloading module");
  unregister_chrdev(MYMAJOR, NAME);
}

module_init(start);
module_exit(end);
