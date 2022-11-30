#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");

static dev_t device_numbers;
static struct class *my_class;
static struct cdev my_device;
#define NAME "ex4_module"
#define DEVICE_CLASS "ex4_module_class"



#define BUFFER_SIZE 255


static char buffer[BUFFER_SIZE] = "";
static int buffer_counter = 0;



static int driver_open(struct inode *device_file, struct file *instance){
  printk("%s - open was called\n", NAME);
  return 0;
}

static int driver_close(struct inode *device_file, struct file *instance){
  printk("%s - close was called\n", NAME);
  return 0;
}

static ssize_t driver_read(struct file *File, char *user_buffer, size_t len, loff_t *offset){
  int amount_to_copy = 0;
  int amount_not_copied = 0;
  int delta = 0;

  printk("buffer content: %s\n", buffer);
  amount_to_copy = min(len, buffer_counter);

  // copy to user(TO, FROM, amount);
  amount_not_copied = copy_to_user(user_buffer, buffer, amount_to_copy);

  delta = amount_to_copy - amount_not_copied;

  printk("amount_to_copy = %d\n", amount_to_copy);
  printk("amount_not_copied = %d\n", amount_to_copy);
  printk("delta = %d\n", delta);
  return delta;
}

static ssize_t driver_write(struct file *filep, const char *user_buffer, size_t len, loff_t *offset){
  int amount_to_copy = 0;
  int amount_not_copied = 0;
  int delta = 0;

  amount_to_copy = min(len, sizeof(buffer));
  buffer_counter  =  amount_to_copy;

  // copy_to_user(TO, FROM, amount);
  // copy_from_user(TO, FROM, amount);
  amount_not_copied = copy_from_user(buffer, user_buffer, amount_to_copy);

  delta = amount_to_copy - amount_not_copied;

  printk("amount_to_copy = %d\n", amount_to_copy);
  printk("amount_not_copied = %d\n", amount_to_copy);
  printk("delta = %d\n", delta);
  return delta;
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

  // allocation of character device:
  if(alloc_chrdev_region(&device_numbers, 0, 1, NAME) < 0 ){
    printk("device numbers could not be allocated!\n");
    return -1;
  }
  printk("device major: %d, minor: %d was registered!n", device_numbers >> 20, device_numbers && 0xfffff);
  
  // create a device class
  if((my_class = class_create(THIS_MODULE, DEVICE_CLASS)) == NULL){
    printk("device class cannot be created");
    unregister_chrdev(device_numbers, NAME);
    return -1;
  }
  // create a device file:
  if(device_create(my_class, NULL, device_numbers, NULL, NAME) == NULL){
    printk("cannot create device file\n");
    class_destroy(my_class);
    unregister_chrdev(device_numbers, NAME);
    return -1;
  }
  cdev_init(&my_device, &fops);
  if(cdev_add(&my_device, device_numbers, 1) == -1){
    printk("registering device to kernel failed\n");
    device_destroy(my_class, device_numbers);
    class_destroy(my_class);
    unregister_chrdev(device_numbers, NAME);
    return -1;
  }
  return 0;
  


}

static void __exit end(void){
  printk(KERN_INFO "Unloading module");
  cdev_del(&my_device);
  device_destroy(my_class, device_numbers);
  class_destroy(my_class);
  unregister_chrdev(device_numbers, NAME);
}

module_init(start);
module_exit(end);
