/* SIMPLE EXAMPLE OF HOW TO READ-WRITE */
/* TO THE TEENSY 4.1 BUIT-IN SD CARD */
/* Tested on a Lexar E-Series 128GB Micro SD Card LMSESXX128G-B3AAU*/
/* losely based on the TeensySdioDemo.ino example*/

#include "SdFat.h"

SdFs    sd;                           /* DF file system */                
FsFile  file;                         /* file object */
const int LEDPIN       = 13;          /* pin number for Teensy 4/4.1 LED */
const unsigned long DT = 200;         /* SOP blink dt */
bool xread             = true;        /* use to limit number of read/writes to SD card */
const char* fname      = "test.bin";  /* test file name */

/* cleaer serial input */
void clearSerialInput() {while(Serial.read() >= 0){}}

/* write 100-byte array to sdcard/fname */
void testwrite(const char* fname)
{
  uint8_t buff[100];
  uint8_t xi = 0;
  if(!file.open(fname, O_WRONLY | O_CREAT)){Serial.print("file.open failed");}
  for(uint8_t i = 0; i < 10; ++i)
  {
    for(uint8_t j = 0; j < 10; ++j)
    {
      buff[xi++] = j;
    }
  }
  file.write(buff, 100);
  file.close();
}

/* read contents of fname and print them to serial port*/
void testread(const char* fname)
{
  uint8_t buff[100];
  if(!file.open(fname, O_RDONLY)){Serial.print("file.open failed");}
  file.read(buff, 100);
  file.close();
  uint8_t xi = 0;
  for(uint8_t i = 0; i < 10; ++i)
  {
    for(uint8_t j = 0; j < 10; ++j)
    {
      Serial.print(buff[xi++], 1);      /* print it to serial port */
      Serial.print(j < 9 ? "," : ""); /* print comma */
    }
    Serial.print('\n');
  }
}

void setup()
{
  pinMode(LEDPIN, OUTPUT);    /* set up LED for output */
  Serial.begin(9600);         /* open serial port */
  while (!Serial){}           /* wait for serial port connection */
  clearSerialInput();
}

void loop()
{
  if(!sd.begin(SdioConfig(FIFO_SDIO))){Serial.print("sd.begin failed");}

  /* test read-write */
  if(xread)
  {
    Serial.println("writing to SD card");
    testwrite(fname);
    Serial.println("reading from SD card");
    testread(fname);
    xread = false;
  }
    
  sd.end();

 /* after read-write, go to blink */
 digitalWrite(LEDPIN, HIGH);    /* set the LED on  */
 delay(DT);                     /* wait for DT ms  */
 digitalWrite(LEDPIN, LOW);     /* set the LED off */
 delay(DT);                     /* wait for DT ms  */
}