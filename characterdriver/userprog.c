#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define DEVICE_PATH "/dev/simple_char_driver"

#define MAGIC_NUM 100
#define IOCTL_PRINT       _IO(MAGIC_NUM, 0)
#define IOCTL_GET_VALUE   _IOR(MAGIC_NUM, 1, int *)
#define IOCTL_SET_VALUE   _IOW(MAGIC_NUM, 2, int)

int main() {
    int fd;
    char write_buf[] = "Hello from Userspace!";
    char read_buf[100];
    int ioctl_value = 42;

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return 1;
    }

    // Write to device
    if (write(fd, write_buf, strlen(write_buf)) < 0) {
        perror("Failed to write to the device");
        close(fd);
        return 1;
    }
    printf("Written to device: %s\n", write_buf);

    // Read from device
    if (read(fd, read_buf, sizeof(read_buf)) < 0) {
        perror("Failed to read from the device");
        close(fd);
        return 1;
    }
    printf("Read from device: %s\n", read_buf);

    // IOCTL SET
    if (ioctl(fd, IOCTL_SET_VALUE, &ioctl_value) < 0) {
        perror("IOCTL_SET_VALUE failed");
    } else {
        printf("Sent ioctl set value: %d\n", ioctl_value);
    }

    // IOCTL GET
    ioctl_value = 0;
    if (ioctl(fd, IOCTL_GET_VALUE, &ioctl_value) < 0) {
        perror("IOCTL_GET_VALUE failed");
    } else {
        printf("Received ioctl get value: %d\n", ioctl_value);
    }

    // IOCTL PRINT
    if (ioctl(fd, IOCTL_PRINT) < 0) {
        perror("IOCTL_PRINT failed");
    }

    close(fd);
    return 0;
}

