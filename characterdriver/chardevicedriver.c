/***************************************************************************//**
*  \file       driver.c
*  \details    Simple Character Device Driver Example
*******************************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>           // file_operations
#include <linux/cdev.h>         // cdev struct
#include <linux/uaccess.h>      // copy_to_user(), copy_from_user()
#include <linux/device.h>       // device create
#include <linux/slab.h>         // kmalloc

#define DEVICE_NAME "my_char_device"
#define BUFFER_SIZE 1024

dev_t dev_num;                          // Holds major + minor number
struct cdev my_cdev;                    // Character device structure
struct class *my_class;                 // Device class
char *kernel_buffer;                    // Data buffer

// ------------ Function Declarations -------------------
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *user_buf, size_t len, loff_t *offset);
static ssize_t my_write(struct file *file, const char __user *user_buf, size_t len, loff_t *offset);

// ------------- File Operations Structure --------------
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

// ------------- File Operation Functions ---------------
static int my_open(struct inode *inode, struct file *file) {
    pr_info("Device Opened!\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file) {
    pr_info("Device Closed!\n");
    return 0;
}

static ssize_t my_read(struct file *file, char __user *user_buf, size_t len, loff_t *offset) {
    pr_info("Reading from device\n");
    return copy_to_user(user_buf, kernel_buffer, len) ? -EFAULT : len;
}

static ssize_t my_write(struct file *file, const char __user *user_buf, size_t len, loff_t *offset) {
    pr_info("Writing to device\n");
    return copy_from_user(kernel_buffer, user_buf, len) ? -EFAULT : len;
}

// ------------- Module Init Function -------------------
static int __init char_driver_init(void) {
    // 1. Allocate device number
    if (alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME) < 0) {
        pr_err("Failed to allocate device number\n");
        return -1;
    }

    // 2. Initialize cdev
    cdev_init(&my_cdev, &fops);

    // 3. Add cdev to system
    if (cdev_add(&my_cdev, dev_num, 1) < 0) {
        pr_err("Failed to add cdev\n");
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }

    // 4. Create class
    my_class = class_create(THIS_MODULE, "my_class");
    if (IS_ERR(my_class)) {
        pr_err("Failed to create class\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(my_class);
    }

    // 5. Create device
    if (IS_ERR(device_create(my_class, NULL, dev_num, NULL, DEVICE_NAME))) {
        pr_err("Failed to create device\n");
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }

    // 6. Allocate kernel buffer
    kernel_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!kernel_buffer) {
        pr_err("Failed to allocate memory\n");
        device_destroy(my_class, dev_num);
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }

    pr_info("Character Driver Loaded Successfully\n");
    return 0;
}

// ------------- Module Exit Function -------------------
static void __exit char_driver_exit(void) {
    kfree(kernel_buffer);
    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("Character Driver Removed\n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shiva Teja");
MODULE_DESCRIPTION("Simple Character Device Driver");
MODULE_VERSION("1.0");
