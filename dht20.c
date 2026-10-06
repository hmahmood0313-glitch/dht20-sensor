#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int main() {

    int fd;
    unsigned char command[3] = {0xAC, 0x33, 0x00};
    unsigned char data[7];

    // Open Raspberry Pi I2C
    fd = open("/dev/i2c-1", O_RDWR);

    if (fd < 0) {
        printf("Could not open I2C\n");
        return 1;
    }

    // Connect to DHT20 address 0x38
    if (ioctl(fd, I2C_SLAVE, 0x38) < 0) {
        printf("Could not connect to DHT20\n");
        return 1;
    }

    while (1) {

        // it Tells DHT20 to take a measurement
        write(fd, command, 3);

        // Wait for sensor
        usleep(100000);

        // Read 7 bytes
        read(fd, data, 7);

        // Build 20-bit humidity value
        uint32_t raw_humidity =
            ((uint32_t)data[1] << 12) |
            ((uint32_t)data[2] << 4) |
            (data[3] >> 4);

        // Build 20-bit temperature value
        uint32_t raw_temperature =
            ((uint32_t)(data[3] & 0x0F) << 16) |
            ((uint32_t)data[4] << 8) |
            data[5];

        // Converting ther = raw values
        double humidity =
            ((double)raw_humidity / 1048576.0) * 100.0;

        double temperature =
            ((double)raw_temperature / 1048576.0) * 200.0 - 50.0;

        printf("Temperature: %.2f C\n", temperature);
        printf("Humidity: %.2f %%\n\n", humidity);

        sleep(2);
    }

    close(fd);

    return 0;
}