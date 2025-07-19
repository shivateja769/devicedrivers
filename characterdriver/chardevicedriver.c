#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shiva Teja");
MODULE_DESCRIPTION("Simple Char Device Driver with IOCTL");

#define DEVICE_NAME "simple_char_driver"

#define MAGIC_NUM 100
#define IOCTL_PRINT       _IO(MAGIC_NUM, 0)
#define IOCTL_GET_VALUE   _IOR(MAGIC_NUM, 1, int *)
#define IOCTL_SET_VALUE   _IOW(MAGIC_NUM, 2, int)

static int major_number;
static int kernel_value = 0;

int chardev_open(struct inode *, struct file *);
ssize_t chardev_read(struct file *, char __user *, size_t, loff_t *);
ssize_t chardev_write(struct file *, const char __user *, size_t, loff_t *);
int chardev_release(struct inode *, struct file *);
long chardev_ioctl(struct file *, unsigned int, unsigned long);

static struct file_operations my_file_operations = {
    .owner = THIS_MODULE,
    .open = chardev_open,
    .read = chardev_read,
    .write = chardev_write,
    .release = chardev_release,
    .unlocked_ioctl = chardev_ioctl,
};

int chardev_open(struct inode *pinode, struct file *pfile) {
    printk(KERN_INFO "simple_char_driver: Open called\n");
    return 0;
}

ssize_t chardev_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) {
    const char *message = "Hello from Kernel!\n";
    size_t message_len = strlen(message);

    printk(KERN_INFO "simple_char_driver: Read called\n");

    if (*offset >= message_len)
        return 0;

    if (copy_to_user(buffer, message, message_len))
        return -EFAULT;

    *offset += message_len;
    return message_len;
}

ssize_t chardev_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) {
    printk(KERN_INFO "simple_char_driver: Write called\n");
    return length;
}

int chardev_release(struct inode *pinode, struct file *pfile) {
    printk(KERN_INFO "simple_char_driver: Release called\n");
    return 0;
}

long chardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case IOCTL_PRINT:
            printk(KERN_INFO "IOCTL_PRINT called\n");
            break;

        case IOCTL_GET_VALUE:
            printk(KERN_INFO "IOCTL_GET_VALUE called, returning %d\n", kernel_value);
            if (copy_to_user((int *)arg, &kernel_value, sizeof(kernel_value)))
                return -EFAULT;
            break;

        case IOCTL_SET_VALUE:
            printk(KERN_INFO "IOCTL_SET_VALUE called\n");
            if (copy_from_user(&kernel_value, (int *)arg, sizeof(kernel_value)))
                return -EFAULT;
            printk(KERN_INFO "New kernel_value: %d\n", kernel_value);
            break;

        default:
            return -EINVAL;
    }
    return 0;
}

static int __init ldd_init(void) {
    printk(KERN_INFO "simple_char_driver: Initializing...\n");

    major_number = register_chrdev(0, DEVICE_NAME, &my_file_operations);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register device\n");
        return major_number;
    }

    printk(KERN_INFO "Registered device with major number %d\n", major_number);
    return 0;
}

static void __exit ldd_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "simple_char_driver: Unregistered device and exiting\n");
}

module_init(ldd_init);
module_exit(ldd_exit);

