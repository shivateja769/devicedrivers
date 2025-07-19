#include <linux/module.h>
#include <linux/platform_device.h>

static struct platform_device toy_device = {
    .name = "toy_phone", // This name should match the driver's name
    .id = -1,
};

static int __init toy_device_init(void) {
    pr_info("Toy Device: Registering\n");
    return platform_device_register(&toy_device);
}

static void __exit toy_device_exit(void) {
    pr_info("Toy Device: Unregistering\n");
    platform_device_unregister(&toy_device);
}

module_init(toy_device_init);
module_exit(toy_device_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Toy Platform Device");
MODULE_AUTHOR("Shiva Teja");

