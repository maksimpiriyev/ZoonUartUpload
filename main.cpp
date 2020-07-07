#include <stdio.h>
#include "serial_port.h"
int main(int argc,char** argv) {
    if(argc != 4){
        printf("./ZoonUartUpload /dev/ttyYourSerialPort baudRate filePath\n");
        printf("Example: ./ZoonUartUpload /dev/ttyAMA0 115200 Firmware.v3.bin\n");
        return 0;
    }

    int baudRate = 115200;
    baudRate = atoi(argv[2]);
    SerialPort serial(argv[1], baudRate);
    FILE* file = fopen(argv[3],"rb");
    union{
        uint_t bytes[4];
        int value;
    }length;
    fseel(file,0L,SEEK_END);
    length.value = ftell(file);
    fseel(file,0L,SEEK_SET);
    serial.write((uint8_t*)"BOOTLOADX",strlen("BOOTLOADX"));
    sleep(1);
    serial.write(length.bytes,sizeof(length));
    while( (l=fread(buffer,1,1024,file)) > 0){
        serial.write(buffer,l);
    }
    serial.close();

    return 0;
}
