#include <linux/module.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shiva Teja");
MODULE_DESCRIPTION("Mock Camera Platform Device");

static struct platform_device camera_device = {
    .name = "mock_camera",
    .id = -1,
};

static int __init camera_device_init(void)
{
    pr_info("Camera device: registering...\n");
    return platform_device_register(&camera_device);
}

static void __exit camera_device_exit(void)
{
    pr_info("Camera device: unregistering...\n");
    platform_device_unregister(&camera_device);
}

module_init(camera_device_init);
module_exit(camera_device_exit);

