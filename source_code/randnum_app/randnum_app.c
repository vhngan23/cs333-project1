#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define DEVICE_NODE "/dev/Project1_device"

/* Check entry point open cua of character driver */
int open_chardrv() {
    int temp = open(DEVICE_NODE, O_RDWR);
    if(temp < 0) {
        printf("Can not open the device file\n");
        exit(1);
    }
    return temp;
}

/* Check entry point release of character driver */
void close_chardrv(int temp) {
    close(temp);
}

/* Get the random number from character device */
void getnum_chardrv(){
	int randnum;
	int temp = openat(temp, DEVICE_NODE, O_RDONLY);
	if(temp < 0){
		printf("Failed to open device file\n");
		return;
	}
	read(temp, &randnum, sizeof(randnum));
	close_chardrv(temp);
	printf("The random number is: %d\n", randnum);
}

int main() {
    int ret = 0;
    char option = 'q';
    int temp = -1;
    printf("Select below options:\n");
    printf("\to (to open a device node)\n");
    printf("\tr (to read a random number)\n");
    printf("\tc (to close the device node)\n");
    printf("\tq (to quit the application)\n");
    while (1) {
        printf("Enter your option: ");
        scanf(" %c", &option);

        switch (option) {
            case 'o':
                if (temp < 0)
                    temp = open_chardrv();
                else
                    printf("%s has already opened.\n", DEVICE_NODE);
                break;
            case 'r':
                if (temp < 0)
                    printf("%s has not opened yet! You need to open the device node first.\n", DEVICE_NODE);
                else
                    getnum_chardrv();
                break;
            case 'c':
                if (temp > -1)
                    close_chardrv(temp);
                else
                    printf("%s has not opened yet! Can not close.\n", DEVICE_NODE);
                temp = -1;
                break;
            case 'q':
                if (temp > -1)
                    close_chardrv(temp);
                printf("Quit the application.\n");
                return 0;
            default:
                printf("Invalid option %c\n", option);
                break;
        }
    };
}
