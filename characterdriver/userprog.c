#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DEVICE_PATH "/dev/simple_char_driver"
#define MAGIC_NUM 100
#define WR_DATA _IOW(MAGIC_NUM, 1, int)
#define RD_DATA _IOR(MAGIC_NUM, 2, int)

int main() {
    int fd, value;

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    // Take input from console
    printf("Enter an integer to send to the kernel: ");
    scanf("%d", &value);

    // Send value using ioctl WR_DATA
    if (ioctl(fd, WR_DATA, &value) < 0) {
        perror("WR_DATA ioctl failed");
    } else {
        printf("Sent to kernel: %d\n", value);
    }

    // Read back using ioctl RD_DATA
    value = 0;
    if (ioctl(fd, RD_DATA, &value) < 0) {
        perror("RD_DATA ioctl failed");
    } else {
        printf("Received from kernel: %d\n", value);
    }

    close(fd);
    return 0;
}

