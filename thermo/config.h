#ifndef CONFIG_H 
#define CONFIG_H


// device identification (for various purposes)
#define DEVICE_ID "thermo";

// data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 0

// define interval for reading data from sensors (in miliseconds)
#define INTERVAL_READINGS 5000

// LCD display settings
#define LCD_ADDR 0x27
#define LCD_COLS 20
#define LCD_ROWS 4

// push URL
#define PUSH_URL "http://192.168.0.12:9200/api/readings"

// password used for encyption of 
#define PUSH_ENCRYPTION_PASSWORD "mysecret"

#define PUSH_AES_BUFFER_SIZE 1024

#endif
