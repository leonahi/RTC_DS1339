#include <Arduino.h>
#include "RTC_DS1339.h"
//#include <Wire.h>


const unsigned int monthdays[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

RTC_DS1339::RTC_DS1339()
{
}

// Read current time and date from the DS1339 chip in BCD format
void RTC_DS1339::read_time(void)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write(0);
    Wire.endTransmission();
    
    // request 7 bytes (secs, min, hour(24), dow, date, month, year)
    Wire.requestFrom(DS1339_ADDR, 7);
    rtc_bcd[DS1339_SEC]   = Wire.read();
    rtc_bcd[DS1339_MIN]   = Wire.read();
    rtc_bcd[DS1339_HOUR]  = Wire.read();
    rtc_bcd[DS1339_DOW]   = Wire.read();
    rtc_bcd[DS1339_DATE]  = Wire.read();
    rtc_bcd[DS1339_MONTH] = Wire.read();
    rtc_bcd[DS1339_YEAR]  = Wire.read();
}

// Read the current alarm 1 value in BCD format.
void RTC_DS1339::read_alarm1(void)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write( (uint8_t)DS1339_ALARM1_ADDR );
    Wire.endTransmission();
    
    // Request 4 bytes of alarm 1 values (sec : min : hour : date)
    Wire.requestFrom(DS1339_ADDR, 4);    
    rtc_bcd[DS1339_SEC]  = Wire.read() & B01111111;
    rtc_bcd[DS1339_MIN]  = Wire.read() & B01111111;
    rtc_bcd[DS1339_HOUR] = Wire.read() & B00111111;  // Only 24 hour format is supported other bits are discarted
    rtc_bcd[DS1339_DATE] = Wire.read() & B00111111;    
}

// Read the current alarm 2 value in BCD format.
void RTC_DS1339::read_alarm2(void)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write( (uint8_t)DS1339_ALARM2_ADDR );
    Wire.endTransmission();

    // Request 3 bytes of alarm 2 values (min : hour : date)
    Wire.requestFrom(DS1339_ADDR, 3);
    rtc_bcd[DS1339_MIN]  = Wire.read() & B01111111;
    rtc_bcd[DS1339_HOUR] = Wire.read() & B00111111; // Only 24 hour format is supported other bits are discarted
    rtc_bcd[DS1339_DATE] = Wire.read() & B00111111;
}

// update the data on the IC from the bcd formatted data in the buffer
void RTC_DS1339::set_time(void)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write(0);
    Wire.write(rtc_bcd[DS1339_SEC]);
    Wire.write(rtc_bcd[DS1339_MIN]);
    Wire.write(rtc_bcd[DS1339_HOUR]);
    Wire.write(rtc_bcd[DS1339_DOW]);
    Wire.write(rtc_bcd[DS1339_DATE]);
    Wire.write(rtc_bcd[DS1339_MONTH]);
    Wire.write(rtc_bcd[DS1339_YEAR]);    
    Wire.endTransmission();
    
    // clear Oscillator Stop Flag
    set_register(REG_DS1339_CONTROL, get_register(REG_DS1339_CONTROL) & ~BITM_DS1339_CONTROL_EOSC);
}

void RTC_DS1339::set_alarm1(alarm1_rate opt)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write( (uint8_t)DS1339_ALARM1_ADDR );
    
    Wire.write( rtc_bcd[DS1339_SEC]  | ((opt & B00000001 ) << 7) ); // A1M1
    Wire.write( rtc_bcd[DS1339_MIN]  | ((opt & B00000010 ) << 6) ); // A1M2
    Wire.write( rtc_bcd[DS1339_HOUR] | ((opt & B00000100 ) << 5) ); // A1M3
    Wire.write( rtc_bcd[DS1339_DATE] | ((opt & B00001000 ) << 4) ); // A1M4
    Wire.write( rtc_bcd[DS1339_DATE] | ((opt & B00010000 ) << 2) ); // DY/DT

    Wire.endTransmission();
}

void RTC_DS1339::set_alarm2(alarm2_rate opt)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write( (uint8_t)DS1339_ALARM2_ADDR );
    
    Wire.write( rtc_bcd[DS1339_MIN]  | ((opt & B00000001 ) << 7)); // A2M1
    Wire.write( rtc_bcd[DS1339_HOUR] | ((opt & B00000010 ) << 6)); // A2M2
    Wire.write( rtc_bcd[DS1339_DATE] | ((opt & B00000100 ) << 5)); // A2M3
    Wire.write( rtc_bcd[DS1339_DATE] | ((opt & B00001000 ) << 3)); // DY/DT

    
    Wire.endTransmission();
}

unsigned char RTC_DS1339::get_register(unsigned char reg_addr)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write( (uint8_t)reg_addr );
    Wire.endTransmission();
    
    Wire.requestFrom(DS1339_ADDR, 1);
    
    return Wire.read();
}

void RTC_DS1339::set_register(unsigned char reg_addr, unsigned char reg_value)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write( (uint8_t)reg_addr );  // set register pointer
    
    Wire.write(reg_value);
    
    Wire.endTransmission();
}

void RTC_DS1339::enable_interrupt(alarm a)
{
    clear_interrupt(a);
    set_register(REG_DS1339_CONTROL, get_register(REG_DS1339_CONTROL) | BITM_DS1339_CONTROL_INTCN | ALARM_INDEX(a));
}

void RTC_DS1339::disable_interrupt(alarm a)
{
    clear_interrupt(a);
    set_register(REG_DS1339_CONTROL, get_register(REG_DS1339_CONTROL) & ~ALARM_INDEX(a));            
}

void RTC_DS1339::clear_interrupt(alarm a)
{
    set_register(REG_DS1339_STATUS, get_register(REG_DS1339_STATUS) & ~ALARM_INDEX(a));
}

unsigned char RTC_DS1339::get_seconds()
{
    return bcd2bin(rtc_bcd[DS1339_SEC]);
}

unsigned char RTC_DS1339::get_minutes()
{
    return bcd2bin(rtc_bcd[DS1339_MIN]);
}
unsigned char RTC_DS1339::get_hours()
{
    return bcd2bin(rtc_bcd[DS1339_HOUR]);
}
unsigned char RTC_DS1339::get_days()
{
    return bcd2bin(rtc_bcd[DS1339_DATE]);
}
unsigned char RTC_DS1339::get_dayofweek()
{
    return bcd2bin(rtc_bcd[DS1339_DOW]);
}
unsigned char RTC_DS1339::get_months()
{
    return bcd2bin(rtc_bcd[DS1339_MONTH]);
}
unsigned int RTC_DS1339::get_years()
{
    return 2000 + bcd2bin(rtc_bcd[DS1339_YEAR]);
}

void RTC_DS1339::set_seconds(unsigned char v)
{
    rtc_bcd[DS1339_SEC] = bin2bcd(v);

}
void RTC_DS1339::set_minutes(unsigned char v)
{
    rtc_bcd[DS1339_MIN] = bin2bcd(v);
}

void RTC_DS1339::set_hours(unsigned char v)
{
    rtc_bcd[DS1339_HOUR] = bin2bcd(v);
}

void RTC_DS1339::set_dayofweek(unsigned char v)
{
    rtc_bcd[DS1339_DOW] = bin2bcd(v);
}

void RTC_DS1339::set_days(unsigned char v)
{
    rtc_bcd[DS1339_DATE] = bin2bcd(v);
}

void RTC_DS1339::set_months(unsigned char v)
{
    rtc_bcd[DS1339_MONTH] = bin2bcd(v);
}

void RTC_DS1339::set_years(unsigned int v)
{
    if(v>1999)
        v -= 2000;
    rtc_bcd[DS1339_YEAR] = bin2bcd(v);
}

byte RTC_DS1339::bcd2bin(byte v)
{
   return (v&0x0F) + ((v>>4)*10);
}

byte RTC_DS1339::bin2bcd(byte v)
{
   return ((v / 10)<<4) + (v % 10);
}

void RTC_DS1339::rtc_stop(void)
{
    set_register(REG_DS1339_CONTROL, get_register(REG_DS1339_CONTROL) | BITM_DS1339_CONTROL_EOSC);
}

void RTC_DS1339::rtc_start(void)
{
    set_register(REG_DS1339_CONTROL, get_register(REG_DS1339_CONTROL) & ~BITM_DS1339_CONTROL_EOSC);
}

unsigned short int RTC_DS1339::set_time_serial()
{
  unsigned short int num_bytes=19;
  unsigned int i=0;
  unsigned char j=0;
  char c;

  Serial.begin(9600);
  
  for(int yr=0; yr<4; ++yr)
  {
    while (!Serial.available());
    c = Serial.read();
    if( !isDigit(c) )
    {
      Serial.println("Error: Check date and time format - YYYY-MM-DD W hh:mm:ss");
      return -1;
    }
    i = i * 10 + c - '0';
  }

  set_years(i);

  while (!Serial.available());
  c = Serial.read();

  for(int mt=0; mt<2; ++mt)
  {
    while (!Serial.available());
    c = Serial.read();
    if( !isDigit(c) )
    {
      Serial.println("Error: Check date and time format - YYYY-MM-DD W hh:mm:ss");
      return -1;
    }  
    j = j * 10 + c - '0';
  }

  set_months(j);

  while (!Serial.available());
  c = Serial.read();

  j=0;
  for(int dy=0; dy<2; ++dy)
  {
    while (!Serial.available());
    c = Serial.read();
    if( !isDigit(c) )
    {
      Serial.println("Error: Check date and time format - YYYY-MM-DD W hh:mm:ss");
      return -1;
    }
    j = j * 10 + c - '0';
  }

  set_days(j);

  while (!Serial.available());
  c = Serial.read();
  if( !isWhitespace(c) )
  {
    Serial.println("Error: Check date and time format - Whitespace missing -  YYYY-MM-DD W hh:mm:ss");
    return -1;
  }

  while (!Serial.available());
  c = Serial.read();
  if( !isDigit(c) )
  {
    Serial.println("Error: Check date and time format - YYYY-MM-DD W hh:mm:ss");
    return -1;
  }
  c = c - '0';

  set_dayofweek(c);

  while (!Serial.available());
  c = Serial.read();
  if( !isWhitespace(c) )
  {
    Serial.println("Error: Check date and time format - Whitespace missing -  YYYY-MM-DD W hh:mm:ss");
    return -1;
  }

  j=0;
  for(int hh=0; hh<2; ++hh)
  {
    while (!Serial.available());
    c = Serial.read();
    if( !isDigit(c) )
    {
      Serial.println("Error: Check date and time format - YYYY-MM-DD W hh:mm:ss");
      return -1;
    }
    j = j * 10 + c - '0';
  }

  set_hours(j);

  while (!Serial.available());
  c = Serial.read();

  j=0;
  for(int mm=0; mm<2; ++mm)
  {
    while (!Serial.available());
    c = Serial.read();
    if( !isDigit(c) )
    {
      Serial.println("Error: Check date and time format - YYYY-MM-DD W hh:mm:ss");
      return -1;
    }
    j = j * 10 + c - '0';
  }

  set_minutes(j);

  while (!Serial.available());
  c = Serial.read();

  j=0;
  for(int ss=0; ss<2; ++ss)
  {
    while (!Serial.available());
    c = Serial.read();
    if( !isDigit(c) )
    {
      Serial.println("Error: Check date and time format - YYYY-MM-DD W hh:mm:ss");
      return -1;
    }
    j = j * 10 + c - '0';
  }

  set_seconds(j);

  set_time();
  
  Serial.end();

  return 0;
}
