#include <linux/module.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shiva Teja");
MODULE_DESCRIPTION("Mock Camera Platform Driver");

// Called when platform_device is registered and matched
static int camera_probe(struct platform_device *pdev)
{
    pr_info("Camera driver: probe called. Device matched!\n");
    return 0;
}

// Called when platform_device is removed/unregistered
static void camera_remove(struct platform_device *pdev)
{
    pr_info("Camera driver: remove called. Device removed!\n");
}

static struct platform_driver camera_driver = {
    .probe = camera_probe,
    .remove = camera_remove,
    .driver = {
        .name = "mock_camera",
        .owner = THIS_MODULE,
    }
};

static int __init camera_driver_init(void)
{
    pr_info("Camera driver: registering...\n");
    return platform_driver_register(&camera_driver);
}

static void __exit camera_driver_exit(void)
{
    pr_info("Camera driver: unregistering...\n");
    platform_driver_unregister(&camera_driver);
}

module_init(camera_driver_init);
module_exit(camera_driver_exit);

