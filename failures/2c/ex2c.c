#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>


#define MYMAJOR 112
#define NAME "ex2c_module"
MODULE_LICENSE("GPL");

static struct class *my_class;
static struct cdev my_device;
#define DRIVER_CLASS "device_class_ex2c"

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


static struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = driver_open,
  .release = driver_close,
  .read = driver_read,
  .write = driver_write
};


static int __init start(void){
  printk(KERN_INFO "Loading module...");
  printk(KERN_INFO "Hello world!");

  int retval = register_chrdev(MYMAJOR, NAME, &fops);
  if(retval != 0){
    printk("couldn not register device number!\n");
    return -1;
  }
  printk("dev_nr - registered device number major: %d, minor: %d\n", MYMAJOR, 0);

  // create a device class:
  my_class = class_create(THIS_MODULE, DRIVER_CLASS);
  if(my_class == NULL){
    printk("Device class cannot be created\n");
    unregister_chrdev(MYMAJOR, NAME);
    return -1;
  }

  // create a device file:
  if(device_create(my_class, NULL, MYMAJOR, NULL, NAME) == NULL){
    printk("Cannot create device file!\n");
    class_destroy(my_class);
    unregister_chrdev(MYMAJOR, NAME);
    return -1;
  }

  // initialize device file (these two steps will add deivce file into the /dev directory):
  cdev_init(&my_device, &fops);
  if(cdev_add(&my_device, MYMAJOR, 1) == -1){
    printk("registering device to kernel failed!\n");
    device_destroy(my_class, MYMAJOR);
    class_destroy(my_class);
    unregister_chrdev(MYMAJOR, NAME);
    return -1;
  }


  return 0;
}

static void __exit end(void){
  printk(KERN_INFO "Unloading module start\n");
  cdev_del(&my_device);
  device_destroy(my_class, MYMAJOR);
  class_destroy(my_class);
  unregister_chrdev(MYMAJOR, NAME);
  printk(KERN_INFO "Unloading module end\n");
}

module_init(start);
module_exit(end);
