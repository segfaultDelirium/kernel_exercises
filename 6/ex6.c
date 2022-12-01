#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include "ioctl_test.h"

#define MYMAJOR 96
#define NAME "ex6_module"
MODULE_LICENSE("GPL");

static int driver_open(struct inode *device_file, struct file *instance){
  printk("%s - open was called\n", NAME);
  return 0;
}

static int driver_close(struct inode *device_file, struct file *instance){
  printk("%s - close was called\n", NAME);
  return 0;
}

static ssize_t driver_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
  char *message = "My custom message!\n";
  int message_len = strlen(message);
  int errors = copy_to_user(buffer, message, message_len);

  return errors == 0 ? message_len : -EFAULT;

}

static ssize_t driver_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
  printk("Sorry, this device is read-only.\n");
  return -EFAULT;
}

int32_t answer = 42;


static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg){
  struct mystruct test;
  switch(cmd){
    case WR_VALUE:
      if(copy_from_user(&answer, (int32_t *) arg, sizeof(answer))){
        printk("%s - error copying data from user.\n", NAME);
        return -1;
      }
      printk("%s - update the answer to %d.\n", NAME, answer);
      break;
      
    case RD_VALUE:
      if(copy_to_user((int32_t *) arg, &answer, sizeof(answer))){
        printk("%s - error copying data to user.\n", NAME);
        return -1;
      }
      printk("%s - the answer was copied %d.\n", NAME, answer);
      break;

    case GREETER:
      if(copy_from_user(&test, (struct mystruct *) arg, sizeof(test))){
        printk("%s - error copying data from user.\n", NAME);
        return -1;
      }
      printk("%s - %d greets to %s.\n", NAME, test.repeat, test.name);
      break;
       
  }
  return 0;
}


static struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = driver_open,
  .release = driver_close,
  .read = driver_read,
  .write = driver_write,
  .unlocked_ioctl = my_ioctl
};


static int __init start(void){
  printk(KERN_INFO "Loading module...");
  printk(KERN_INFO "Hello world!");

  int retval = register_chrdev(MYMAJOR, NAME, &fops);
  if(retval == 0){
    printk("%s - registered device number major: %d, minor: %d\n", NAME, MYMAJOR, 0);
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
