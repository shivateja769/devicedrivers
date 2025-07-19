#include <linux/module.h>
#include <linux/platform_device.h>

static int toy_probe(struct platform_device *pdev) {
    pr_info("Toy Driver: probe() called! Device matched\n");
    return 0;
}

static void toy_remove(struct platform_device *pdev) {
    pr_info("Toy Driver: remove() called!\n");
}

static struct platform_driver toy_driver = {
    .probe = toy_probe,
    .remove = toy_remove,
    .driver = {
        .name = "toy_phone",  // Must match the platform_device name
        .owner = THIS_MODULE,
    },
};

static int __init toy_driver_init(void) {
    pr_info("Toy Driver: Registering driver\n");
    return platform_driver_register(&toy_driver);
}

static void __exit toy_driver_exit(void) {
    pr_info("Toy Driver: Unregistering driver\n");
    platform_driver_unregister(&toy_driver);
}

module_init(toy_driver_init);
module_exit(toy_driver_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Toy Platform Driver");
MODULE_AUTHOR("Shiva Teja");
