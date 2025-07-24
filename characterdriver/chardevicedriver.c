#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "simple_char_driver"
#define CLASS_NAME  "simple_class"

#define MAGIC_NUM 100
#define WR_DATA _IOW(MAGIC_NUM, 1, int)
#define RD_DATA _IOR(MAGIC_NUM, 2, int)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shiva Teja");
MODULE_DESCRIPTION("Char Driver using cdev_init with IOCTL support");

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

static int kernel_val = 0;

/* File operations function prototypes */
static int chardev_open(struct inode *, struct file *);
static int chardev_release(struct inode *, struct file *);
static ssize_t chardev_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t chardev_write(struct file *, const char __user *, size_t, loff_t *);
static long chardev_ioctl(struct file *, unsigned int, unsigned long);

/* File operations structure */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = chardev_open,
    .release = chardev_release,
    .read = chardev_read,
    .write = chardev_write,
    .unlocked_ioctl = chardev_ioctl,
};

/* Open */
static int chardev_open(struct inode *inode, struct file *file) {
    pr_info("Driver: Device opened\n");
    return 0;
}

/* Release */
static int chardev_release(struct inode *inode, struct file *file) {
    pr_info("Driver: Device closed\n");
    return 0;
}

/* Read */
static ssize_t chardev_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    pr_info("Driver: Read called\n");
    return 0;
}

/* Write */
static ssize_t chardev_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    pr_info("Driver: Write called\n");
    return len;
}

/* IOCTL */
static long chardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case WR_DATA:
            if (copy_from_user(&kernel_val, (int __user *)arg, sizeof(kernel_val)))
                return -EFAULT;
            pr_info("IOCTL WR_DATA: Received value = %d\n", kernel_val);
            break;

        case RD_DATA:
            if (copy_to_user((int __user *)arg, &kernel_val, sizeof(kernel_val)))
                return -EFAULT;
            pr_info("IOCTL RD_DATA: Sent value = %d\n", kernel_val);
            break;

        default:
            return -EINVAL;
    }
    return 0;
}

/* Module Init */
static int __init ldd_init(void) {
    int ret;

    /* Allocate major and minor number dynamically */
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("Failed to allocate major number\n");
        return ret;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(dev_num), MINOR(dev_num));

    /* Initialize cdev structure */
    cdev_init(&my_cdev, &fops);

    /* Add char device to system */
    ret = cdev_add(&my_cdev, dev_num, 1);
    if (ret < 0) {
        pr_err("Failed to add cdev\n");
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }

    /* Create device class */
    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        pr_err("Failed to create class\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(my_class);
    }

    /* Create device file in /dev/ */
    my_device = device_create(my_class, NULL, dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        pr_err("Failed to create device\n");
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(my_device);
    }

    pr_info("Driver: Inserted successfully\n");
    return 0;
}

/* Module Exit */
static void __exit ldd_exit(void) {
    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("Driver: Removed successfully\n");
}

module_init(ldd_init);
module_exit(ldd_exit);

