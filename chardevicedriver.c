#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>  // For copy_to_user / copy_from_user

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shiva Teja");
MODULE_DESCRIPTION("Simple Hello World Char Device Driver");

#define DEVICE_NAME "simple_char_driver"

static int major_number;

int chardev_open(struct inode *, struct file *);
ssize_t chardev_read(struct file *, char __user *, size_t, loff_t *);
ssize_t chardev_write(struct file *, const char __user *, size_t, loff_t *);
int chardev_release(struct inode *, struct file *);

// File operations structure
static struct file_operations my_file_operations = {
    .owner = THIS_MODULE,
    .open = chardev_open,
    .read = chardev_read,
    .write = chardev_write,
    .release = chardev_release,
};

// Function Definitions

int chardev_open(struct inode *pinode, struct file *pfile){
    printk(KERN_INFO "simple_char_driver: Open called\n");
    return 0;
}

ssize_t chardev_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
    printk(KERN_INFO "simple_char_driver: Read called\n");
    return 0;
}

ssize_t chardev_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
    printk(KERN_INFO "simple_char_driver: Write called\n");
    return length;
}

int chardev_release(struct inode *pinode, struct file *pfile){
    printk(KERN_INFO "simple_char_driver: Release called\n");
    return 0;
}

static int __init ldd_init(void)
{
    printk(KERN_INFO "simple_char_driver: Initializing...\n");

    major_number = register_chrdev(0, DEVICE_NAME, &my_file_operations);
    if (major_number < 0) {
        printk(KERN_ALERT "simple_char_driver: Failed to register char device\n");
        return major_number;
    }

    printk(KERN_INFO "simple_char_driver: Registered successfully with major number %d\n", major_number);
    return 0;
}

static void __exit ldd_exit(void)
{
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "simple_char_driver: Unregistered device and exiting\n");
}

module_init(ldd_init);
module_exit(ldd_exit);

