#include <stdio.h>
#include <stdint.h>
#include "serial_port.h"
#include <string.h>

void swap(uint8_t& a,uint8_t& b){
    uint8_t c = b;
    b = a;
    a = c;
}
int main(int argc,char** argv) {
    if(argc != 4){
        printf("./ZoonUartUpload /dev/ttyYourSerialPort baudRate filePath\n");
        printf("Example: ./ZoonUartUpload /dev/ttyAMA0 115200 Firmware.v3.bin\n");
        return 0;
    }

    union{
        uint8_t bytes[4];
        uint32_t value;
    }length;
    bool isLittleEndian = true;
    length.value = 1;
    if(length.bytes[0] != 1){
        isLittleEndian = false;
    }


    int baudRate = 115200;
    baudRate = atoi(argv[2]);
    SerialPort serial(argv[1], baudRate);

    FILE* file = fopen(argv[3],"rb");


    fseek(file,0L,SEEK_END);
    length.value = ftell(file);
    if(isLittleEndian){
        swap(length.bytes[0],length.bytes[3]);
        swap(length.bytes[1],length.bytes[2]);
    }
    fseek(file,0L,SEEK_SET);
    printf("File: %s size %d bytes\n",argv[3],length.value);
    serial.connect();
    serial.send((uint8_t*)"DRONEE-BOOTLOADX",strlen("DRONEE-BOOTLOADX"));
    sleep(1);
    serial.send(length.bytes,sizeof(length));
    uint8_t buffer[1024];
    int l = 0;
    while( (l=fread(buffer,1,1024,file)) > 0){
        serial.send(buffer,l);
    }
    serial.disconnect();

    return 0;
}
