#include <SPI.h>
const byte INSTREAD  = B0000011;
const byte INSTWRITE = B0000010;
const byte INSTWREN  = B0000110;
const byte INSTWRDI  = B0000100;
const byte READID = B10011111;
const byte READSTATREG = B00000101;
const byte SECTOR_ERASE = 0x20;
const int chipSelectPin = 10;

//We can read 256 bytes at a time currently
int DAT_LEN = 256;
byte page_data[256];

void setup() 
{
  Serial.begin(115200);
  SPI.begin();
  SPI.setBitOrder( MSBFIRST );
  pinMode(chipSelectPin, OUTPUT);
  init_page();
}

void init_page()
{
  int x = 0;
  for(x=0;x<DAT_LEN;x++)
  {
       page_data[x] = x;
  } 
}

void get_chip_id(byte* id)
{
  digitalWrite(chipSelectPin, LOW);
  delay(10);
  SPI.transfer(READID);
  id[0] = SPI.transfer(0);
  id[1] = SPI.transfer(0);
  id[2] = SPI.transfer(0);
  digitalWrite(chipSelectPin, HIGH);
}

void read_page(byte high_byte, unsigned int addr)
{
  int x = 0;
  byte read_data[DAT_LEN];
  digitalWrite(chipSelectPin, LOW);
  SPI.transfer(INSTREAD); 
  SPI.transfer(high_byte);
  SPI.transfer((addr>>8) & 0xFF);
  SPI.transfer(addr & 0xFF);
    for(x=0;x<DAT_LEN;x++){
    read_data[x] = SPI.transfer(0); // <- clock SPI 8 bits
  }
  digitalWrite(chipSelectPin, HIGH);

  for(x=1;x<DAT_LEN+1;x++)
  {
    if(read_data[x-1] <= 0xF)
    {
        Serial.print(0);
    }
    Serial.print(read_data[x-1],HEX);
    /*
    Serial.print('-');
    if (x % 16 == 0 && x != 15)
    {
      Serial.println();
    }
    */
  }
  Serial.println();
}

void erase_sector(byte high_byte, unsigned int addr)
{
  digitalWrite(chipSelectPin, LOW);  
  SPI.transfer(INSTWREN);
  digitalWrite(chipSelectPin, HIGH);
  
  digitalWrite(chipSelectPin, LOW);  
  SPI.transfer(SECTOR_ERASE);
  SPI.transfer(high_byte);
  SPI.transfer((addr>>8) && 0xFF);
  SPI.transfer(addr && 0xFF);
  digitalWrite(chipSelectPin,HIGH);
  //Give the erase enough time to finish (should be checking status register until it says it's done though :/ (TODO)
  delay(100);
  
  digitalWrite(chipSelectPin, LOW);  
  SPI.transfer(INSTWRDI);
  digitalWrite(chipSelectPin, HIGH);

}
/*
 * write_page
 * Note: We can only write 256 bytes at a time, must write over previously erased bytes
*/
void write_page(byte high_byte,unsigned int addr)
{
  int x = 0;
  digitalWrite(chipSelectPin, LOW);  
  SPI.transfer(INSTWREN);
  digitalWrite(chipSelectPin, HIGH);

  digitalWrite(chipSelectPin, LOW);  
  SPI.transfer(INSTWRITE);
  SPI.transfer(high_byte);
  SPI.transfer((addr>>8) && 0xFF);
  SPI.transfer(addr && 0xFF);
  for(x=0;x<DAT_LEN;x++){
    SPI.transfer(page_data[x]);
    delay(1);
  }
  digitalWrite(chipSelectPin, HIGH);
  delay(100);
  
  digitalWrite(chipSelectPin, LOW);  
  SPI.transfer(INSTWRDI);
  digitalWrite(chipSelectPin, HIGH);
}

void chip_erase()
{
  //Enable writes
  digitalWrite(chipSelectPin, LOW);  
  SPI.transfer(INSTWREN);
  digitalWrite(chipSelectPin, HIGH);

  //Erase Chip
  digitalWrite(chipSelectPin,LOW);
  SPI.transfer(0x60);
  digitalWrite(chipSelectPin,HIGH);
  //Wait for chip to finish
  delay(2000);

  //Disable writes
  digitalWrite(chipSelectPin, LOW);  
  SPI.transfer(INSTWRDI);
  digitalWrite(chipSelectPin, HIGH);
}

void loop()
{
  byte cmd;
  byte id[3];
  unsigned int x;
  byte test = 0;
  int start_addr_low = 0;
  byte start_addr_high = 0;
  while(1){
    while(Serial.available() ==  0)
    {
        Serial.println("READY");
        delay(250);
    }
    for(test=0;test<0xFF;test++)
    {
        for(x=0;x<0xFFFF;x+=256)
        {
            read_page(test,x);
            if(x == 0xFF00)
            {
                break;
            }
        }
    }
    /*
    cmd = Serial.read();
    switch(cmd){
      case 'r':
          for(test=0;test<0xFF;test++)
          {
              for(x=0;x<0xFFFF;x+=256)
              {
                read_page(test,x);
                if(x == 0xFFFF-256){
                    break;
                }
                
              }  
          }
        break;
      case 'w':
            for(x=0;x<256;x++)
            {
                while(Serial.available() <= 0){}
                page_data[x] = Serial.read();
            }
            write_page(start_addr_high,start_addr_low);
            start_addr_low += 256;
            if(start_addr_low >= 0xFFFF)
            {
              start_addr_low = 0;
              start_addr_high +=1;
            }
            Serial.println("DONE");
            break; 
    }
        */
  }
}
