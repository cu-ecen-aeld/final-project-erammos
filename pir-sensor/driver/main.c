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

#include <linux/fs.h> // file_operations
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
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
  dev = container_of(inode->i_cdev,pir_dev, cdev);
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

int pir_init_module(void) {
  dev_t dev = 0;
  int result;
  result = alloc_chrdev_region(&dev, pir_minor, 1, "pir_dev");
  pir_major = MAJOR(dev);
  if (result < 0) {
    printk(KERN_WARNING "Can't get major %d\n",pir_major);
    return result;
  }
  result = setup_cdev(&pirdev);

  if(gpio_is_valid(21) == false){
    pr_err("GPIO %d is not valid\n", 21);
    goto gt;
  }
  else printk(KERN_INFO "GPIO %d is valid\n",21);
  if (result) {
    unregister_chrdev_region(dev, 1);
  }
gt:
  return result;
}

void pir_cleanup_module(void) {
  dev_t devno = MKDEV(pir_major, pir_minor);
  cdev_del(&pirdev.cdev);

  unregister_chrdev_region(devno, 1);
}

module_init(pir_init_module);
module_exit(pir_cleanup_module);
