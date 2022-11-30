#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>


#define MYMAJOR 103
#define NAME "ex3_module"
MODULE_LICENSE("GPL");
#define BUFFER_SIZE 50

static int driver_open(struct inode *device_file, struct file *instance){
  printk("%s - open was called\n", NAME);
  return 0;
}

static int driver_close(struct inode *device_file, struct file *instance){
  printk("%s - close was called\n", NAME);
  return 0;
}

static ssize_t driver_read(struct file *filep, char *buffer, size_t len, loff_t *offset){

  int rand = 0;
  printk("before get_random_bytes gets called\n");
  get_random_bytes(&rand, sizeof(int)-1);
  printk("randomly generated number: %d\n", rand);
  char message[BUFFER_SIZE] = "";
  snprintf(message, BUFFER_SIZE, "%d\n", rand);
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
