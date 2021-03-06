// tinyTouch for M5Paper w/o M5EPD library V.001 / Pingi-Soft®2021
#include"Wire.h"
#include"SPI.h"
uint8_t f;uint32_t g;uint16_t h;
unsigned long z=millis();
bool o1=1;bool o2=1;bool o3=1;
bool xm;bool ym;
uint8_t fn;uint8_t nm;uint8_t sz;uint8_t id;
uint16_t ax=0;uint16_t bx=0;
uint8_t bf[8];
uint16_t xx[512];uint16_t yy[512];
uint16_t xd;uint16_t yd;uint16_t px;uint16_t py;
uint16_t po;uint16_t st;
uint8_t di;

void ww(uint8_t rg,uint8_t dt){Wire.beginTransmission(81);Wire.write(rg);Wire.write(dt);Wire.endTransmission();}
void sw(bool wr,uint16_t dt){while(digitalRead(27)!=1){};digitalWrite(15,0);SPI.write16(24576*wr);while(digitalRead(27)!=1){}SPI.write16(dt);digitalWrite(15,1);}
void cl(){sw(1,33);sw(0,32);sw(0,0);sw(0,0);sw(0,960);sw(0,540);for(g=0;g<129600;g++){digitalWrite(15,0);SPI.write32(65535);digitalWrite(15,1);}sw(1,34);}
void sc(uint8_t upm){sw(1,55);sw(0,0);sw(0,0);sw(0,960);sw(0,540);sw(0,upm);sw(0,14048);sw(0,18);delay(3e2);}

void setup(){
  Serial.begin(115200);
  pinMode(2,OUTPUT);digitalWrite(2,1);delay(1e2);pinMode(5,OUTPUT);digitalWrite(5,1);pinMode(23,OUTPUT);digitalWrite(23,1);
  Wire.begin(21,22);ww(0,0);ww(1,0);ww(13,0);delay(1e2);SPI.begin(14,13,12,4);digitalWrite(15,1);pinMode(15,OUTPUT);pinMode(27,INPUT);
  SPI.beginTransaction(SPISettings(1e7,MSBFIRST,SPI_MODE0));
  sw(1,1);sw(1,17);sw(0,4);sw(0,1);sw(1,57);sw(0,1);sw(0,2300);sw(1,17);sw(0,522);sw(0,18);sw(1,17);sw(0,520);sw(0,14048);
  cl();sc(0);delay(1e3);
  while(1){
    po=0;
    while(!po||o1){
      if(millis()-z>6e4){delay(1e3);digitalWrite(2,0);}
      Wire.beginTransmission(93);Wire.write(129);Wire.write(78);Wire.endTransmission(0);Wire.requestFrom(93,1);fn=Wire.read();
      nm=fn&15;if(nm==2){cl();sc(0);o2=0;o3=0;z=millis();}
      if(fn&128){
        if(nm&&o3){
          po++;o1=1;o2=1;
          Wire.beginTransmission(93);Wire.write(129);Wire.write(80);Wire.endTransmission(0);Wire.requestFrom(93,8);Wire.readBytes(bf,8);
          yy[po-1]=540-((bf[1]<<8)|bf[0]);xx[po-1]=((bf[3]<<8)|bf[2]);sz=(bf[5]<<8)|bf[4];id=bf[7];
          if(yy[po-1]==bx&&xx[po-1]==ax)po--;else{bx=yy[po-1];ax=xx[po-1];}
        }
        else o1=0;o3=1;
        Wire.beginTransmission(93);Wire.write(129);Wire.write(78);Wire.write(0);Wire.endTransmission(1);
        }
      }
      if(o2)for(f=0;f<po;f++){
        di=(sz-2)/4;
        if(f+1<po){
          xm=xx[f]<xx[f+1];ym=yy[f]<yy[f+1];
          xd=abs(xx[f]-xx[f+1]);yd=abs(yy[f]-yy[f+1]);
          st=sqrt(sq(xd+yd))/(int(di/2)+1);
        }else st=1;
        for(h=0;h<st;h++){
          if(f+1<po){
            bool abc=!(h+1==st&&f+1==po)/2;
            px=(h+1)*xd/st*((xm-2)*2+3)+xx[f];py=(h+1)*yd/st*((ym-2)*2+3)+yy[f];
          }else{px=ax;py=bx;}
          sw(1,0x21);sw(0,0x20);sw(0,px);sw(0,py);sw(0,di);sw(0,di);
          for(g=0;g<sq(di);g++){digitalWrite(15,0);SPI.write32(0);digitalWrite(15,1);}
        sw(1,0x22);
      }
    }
    sc(1);o2=0;
  }
}
   
void loop(){}
