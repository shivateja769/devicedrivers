#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "toy_device"

static int major_number;
static char device_buffer[256];

static int toy_open(struct inode *inode, struct file *file) {
    pr_info("Toy Driver: Device opened\n");
    return 0;
}

static int toy_release(struct inode *inode, struct file *file) {
    pr_info("Toy Driver: Device closed\n");
    return 0;
}

static ssize_t toy_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    return simple_read_from_buffer(buf, len, offset, device_buffer, sizeof(device_buffer));
}

static ssize_t toy_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    return simple_write_to_buffer(device_buffer, sizeof(device_buffer), offset, buf, len);
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = toy_open,
    .release = toy_release,
    .read = toy_read,
    .write = toy_write,
};

static int toy_probe(struct platform_device *pdev) {
    pr_info("Toy Driver: probe() called! Device matched\n");

    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        pr_err("Toy Driver: Failed to register char device\n");
        return major_number;
    }

    pr_info("Toy Driver: Registered char device with major number %d\n", major_number);
    return 0;
}

static void toy_remove(struct platform_device *pdev) {
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("Toy Driver: Char device unregistered. remove() called!\n");
}

static struct platform_driver toy_driver = {
    .probe = toy_probe,
    .remove = toy_remove,
    .driver = {
        .name = "toy_phone",
        .owner = THIS_MODULE,
    },
};

static int __init toy_driver_init(void) {
    pr_info("Toy Driver: Registering platform driver\n");
    return platform_driver_register(&toy_driver);
}

static void __exit toy_driver_exit(void) {
    pr_info("Toy Driver: Unregistering platform driver\n");
    platform_driver_unregister(&toy_driver);
}

module_init(toy_driver_init);
module_exit(toy_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shiva Teja");
MODULE_DESCRIPTION("Platform Driver with Char Device");

