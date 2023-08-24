/**
 * @file pir.c
 * @brief Functions and data related to the char driver implementation
 *
 * Based on the implementation of the "scull" device driver, found in
 * Linux Device Drivers example code.
 *
 * @author Dan Walkes
 * @date 2019-10-22
 * @copyright Copyright (c) 2019
 *
 */

#include <linux/cdev.h>
#include <linux/fs.h> // file_operations
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/interrupt.h>
int pir_major = 0; // use dynamic major
int pir_minor = 0;

MODULE_AUTHOR("erammos"); /** TODO: fill in your name **/
MODULE_LICENSE("Dual BSD/GPL");

typedef struct {
  struct cdev cdev;
} pir_dev;

pir_dev pirdev;
int pir_open(struct inode *inode, struct file *filp) {
  pir_dev *dev;
  dev = container_of(inode->i_cdev, pir_dev, cdev);
  filp->private_data = dev;
  return 0;
}

int pir_release(struct inode *inode, struct file *filp) { return 0; }

ssize_t pir_read(struct file *filp, char __user *buf, size_t count,
                 loff_t *f_pos) {
  ssize_t retval = 0;
  return retval;
}

long pir_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
  int retval = 0;
  return retval;
}
struct file_operations pir_fops = {.owner = THIS_MODULE,
                                   .read = pir_read,
                                   .open = pir_open,
                                   .release = pir_release,
                                   .unlocked_ioctl = pir_ioctl};

static int setup_cdev(pir_dev *dev) {
  int err, devno = MKDEV(pir_major, pir_minor);

  cdev_init(&dev->cdev, &pir_fops);
  dev->cdev.owner = THIS_MODULE;
  dev->cdev.ops = &pir_fops;
  err = cdev_add(&dev->cdev, devno, 1);
  if (err) {
    printk(KERN_ERR "Error %d adding pir cdev", err);
  }
  return err;
}
static irqreturn_t gpio_irq_handler(int irq,void *dev_id) 
{
  static unsigned long flags = 0;
  
#ifdef EN_DEBOUNCE
   unsigned long diff = jiffies - old_jiffie;
   if (diff < 20)
   {
     return IRQ_HANDLED;
   }
  
  old_jiffie = jiffies;
#endif  
  local_irq_save(flags);
  pr_info("Interrupt Occurred : GPIO_4 : %d ",gpio_get_value(4));
  local_irq_restore(flags);
  return IRQ_HANDLED;
}
int pir_init_module(void) {
  dev_t dev = 0;
  int result;
  result = alloc_chrdev_region(&dev, pir_minor, 1, "pir_dev");
  pir_major = MAJOR(dev);
  if (result < 0) {
    printk(KERN_WARNING "Can't get major %d\n", pir_major);
    return result;
  }
  result = setup_cdev(&pirdev);

  if (gpio_is_valid(4) == false) {
    printk(KERN_WARNING "GPIO %d is not valid\n", 4);
    goto gt;
  } else
    printk(KERN_INFO "GPIO %d is valid\n", 4);

  if (gpio_request(4, "GPIO_4") < 0) {
    printk(KERN_WARNING "ERROR: GPIO %d request\n", 4);
    goto gt;
  }

  gpio_direction_input(4);
 unsigned int GPIO_irqnumber = gpio_to_irq(4);
  if(request_irq(GPIO_irqnumber,(void *) gpio_irq_handler,IRQF_TRIGGER_RISING,"pir-sensor",NULL)) 
  {
    printk(KERN_WARNING "Cannot register %d IRQ", GPIO_irqnumber);
    goto gt;

  }
gt:
  if (result) {
    unregister_chrdev_region(dev, 1);
  }
  return result;
}

void pir_cleanup_module(void) {
  dev_t devno = MKDEV(pir_major, pir_minor);
  cdev_del(&pirdev.cdev);

  gpio_free(4);
  unregister_chrdev_region(devno, 1);
}

module_init(pir_init_module);
module_exit(pir_cleanup_module);
