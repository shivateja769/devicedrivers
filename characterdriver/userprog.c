#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/simple_char_driver"

int main() {
    int fd;
    char write_buf[] = "Hello from Userspace!";
    char read_buf[100];

    // Open the device
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return 1;
    }

    // Write to the device
    if (write(fd, write_buf, strlen(write_buf)) < 0) {
        perror("Failed to write to the device");
        close(fd);
        return 1;
    }
    printf("Data written to device: %s\n", write_buf);

    // Read from the device
    if (read(fd, read_buf, sizeof(read_buf)) < 0) {
        perror("Failed to read from the device");
        close(fd);
        return 1;
    }

    printf("Data read from device: %s\n", read_buf);

    // Close the device
    close(fd);
    return 0;
}
