#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shiva Teja");
MODULE_DESCRIPTION("Char Driver with IOCTL WR_DATA and RD_DATA");

#define DEVICE_NAME "simple_char_driver"
#define MAGIC_NUM 100
#define WR_DATA _IOW(MAGIC_NUM, 1, int)
#define RD_DATA _IOR(MAGIC_NUM, 2, int)

static int major_number;
static int kernel_val = 0;

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
    printk(KERN_INFO "Driver: Open\n");
    return 0;
}

ssize_t chardev_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) {
    printk(KERN_INFO "Driver: Read called\n");
    return 0;
}

ssize_t chardev_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) {
    printk(KERN_INFO "Driver: Write called\n");
    return length;
}

int chardev_release(struct inode *pinode, struct file *pfile) {
    printk(KERN_INFO "Driver: Release\n");
    return 0;
}

long chardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case WR_DATA:
            if (copy_from_user(&kernel_val, (int *)arg, sizeof(kernel_val)))
                return -EFAULT;
            printk(KERN_INFO "IOCTL WR_DATA: Received value = %d\n", kernel_val);
            break;

        case RD_DATA:
            if (copy_to_user((int *)arg, &kernel_val, sizeof(kernel_val)))
                return -EFAULT;
            printk(KERN_INFO "IOCTL RD_DATA: Sent value = %d\n", kernel_val);
            break;

        default:
            return -EINVAL;
    }
    return 0;
}

static int __init ldd_init(void) {
    printk(KERN_INFO "Driver: Initializing...\n");

    major_number = register_chrdev(0, DEVICE_NAME, &my_file_operations);
    if (major_number < 0) {
        printk(KERN_ALERT "Driver: Register failed\n");
        return major_number;
    }

    printk(KERN_INFO "Driver: Registered with major number %d\n", major_number);
    return 0;
}

static void __exit ldd_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Driver: Unregistered\n");
}

module_init(ldd_init);
module_exit(ldd_exit);

