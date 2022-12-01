#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>

#define MYMAJOR 106
#define NAME "ex6_module"
MODULE_LICENSE("GPL");


struct mystruct {
  int repeat;
  char name[64];
};

#define WR_VALUE _IOW('a', 'a', int32_t *)
#define RD_VALUE _IOR('a', 'b', int32_t *)
#define GREETER _IOR('a', 'c', struct mystruct *)

int32_t my_number = 42;

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

static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg){
  struct mystruct test;
  switch(cmd){
    case WR_VALUE:
      if(copy_from_user(&my_number, (int32_t *) arg, sizeof(my_number))){
        printk("%s - error copying data from user.\n", NAME);
        return -1;
      }
      printk("%s - update the my_number to %d.\n", NAME, my_number);
      break;
      
    case RD_VALUE:
      if(copy_to_user((int32_t *) arg, &my_number, sizeof(my_number))){
        printk("%s - error copying data to user.\n", NAME);
        return -1;
      }
      printk("%s - the my_number was copied %d.\n", NAME, my_number);
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
