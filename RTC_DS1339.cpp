/*
 Arduino RTC Library for Maxim Integrated's DS1337 & DS1339 RTC
 by Sridhar Rajagopal (sridhar.rajagopal at google's mail)
 This is a fork of
 Yet-Another DS1337 RTC Library
 By Tim Gipson (drmn4ea at google's mail)
 
 which is 
 Based loosely on mattt and xSmurf's RTC library at (http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1191209057/0)
 epoch_seconds_to_date() function based on this Dallas/Maxim application note: http://www.maxim-ic.com/app-notes/index.mvp/id/517
 
 This library is written for use with any Arduino board with no/minimal changes 
 (modifying the alarm functions to work without interrupts if needed).
 The DS1337 library is suited for Mosquino, and you have to make some
 modifications to make it work elsewhere. I saw that the DS1337 and DS1339 
 both have similar functionality and data format (the DS1339 has an extra 
 trickle charger in case you want to use a super cap instead of a coin cell 
 battery. I've modified the library to allow for the configuration settings
 to be specified by the caller
 
*/

extern "C" {

	#include <avr/power.h>
	#include <avr/sleep.h>

	// Dummy "interrupt handler" for sleep to wake up to on alarm interrupt
	void _dummy_int_handler(void)
	{

	}
	
}

#include <Arduino.h>
#include "RTC_DS1339.h"
#include <Wire.h>

// NOTE: To keep the math from getting even more lengthy/annoying than it already is, the following constraints are imposed:
//   1) All times are in 24-hour format (military time)
//   2) DayOfWeek field is not used internally or checked for validity. Alarm functions may optionally set alarms repeating on DayOfWeek, but this feature has not been tested yet.
//   3) This library's buffer stores all times in raw BCD format, just as it is sent from the RTC.
//      It is not converted to/from 'real' (binary) values until needed via get...() and set...() functions.
//      In other words, don't go hacking around and reading from the rtc_bcd[] buffer directly, unless you want the raw BCD results.


// Cumulative number of days elapsed at the start of each month, assuming a normal (non-leap) year.
const unsigned int monthdays[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

RTC_DS1339::RTC_DS1339() : _rtc_int_pin(2), _rtc_int_number(0)
{
  init();
}

RTC_DS1339::RTC_DS1339(int int_pin, int int_number) : _rtc_int_pin(int_pin), _rtc_int_number(int_number)
{
  init();
}

void RTC_DS1339::init() 
{
    Wire.begin();
    pinMode(_rtc_int_pin, INPUT);
    digitalWrite(_rtc_int_pin, HIGH);	// enable software pullup resistor on RTC interrupt pin
}

// Read current time and date from the DS1339 chip in BCD format
void RTC_DS1339::read_time(void)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write(0);
    Wire.endTransmission();
    
    // request 7 bytes (secs, min, hour(24), dow, date, month, year)
    Wire.requestFrom(DS1339_ADDR, 7);
    rtc_bcd[DS1339_SEC] = Wire.read();
    rtc_bcd[DS1339_MIN] = Wire.read();
    rtc_bcd[DS1339_HOUR] = Wire.read();
    rtc_bcd[DS1339_DOW] = Wire.read();
    rtc_bcd[DS1339_DATE] = Wire.read();
    rtc_bcd[DS1339_MONTH] = Wire.read();
    rtc_bcd[DS1339_YEAR] = Wire.read();
}

// Read the current alarm 1 value in BCD format.
void RTC_DS1339::read_alarm1(void)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write( (uint8_t)DS1339_ALARM1_ADDR );
    Wire.endTransmission();
    
    // Request 4 bytes of alarm 1 values (sec : min : hour : date)
    Wire.requestFrom(DS1339_ADDR, 4);    
    rtc_bcd[DS1339_SEC] = Wire.read() & B01111111;
    rtc_bcd[DS1339_MIN] = Wire.read() & B01111111;
    rtc_bcd[DS1339_HOUR] = Wire.read() & B01111111;  // Only 24 hour format is supported other bits are discarted
    rtc_bcd[DS1339_DATE] = Wire.read() & B01111111;  // Only Date of month is supported other bits are discarted    
}

// Read the current alarm 2 value in BCD format.
void RTC_DS1339::read_alarm2(void)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write( (uint8_t)DS1339_ALARM2_ADDR );
    Wire.endTransmission();

    // Request 3 bytes of alarm 2 values (min : hour : date)
    Wire.requestFrom(DS1339_ADDR, 3);
    rtc_bcd[DS1339_MIN] = Wire.read() & B01111111;
    rtc_bcd[DS1339_HOUR] = Wire.read() & B00111111; // Only 24 hour format is supported other bits are discarted
    rtc_bcd[DS1339_DATE] = Wire.read() & B00111111; // Only Date of month is supported other bits are discarted
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
    set_register(REG_DS1339_CONTROL, get_register(REG_DS1339_CONTROL) & !BITM_DS1339_CONTROL_EOSC);
}

// FIXME: automatically set alarm interrupt after writing new alarm? Nah...

// Write the BCD alarm value in the buffer to the alarm registers.
// If an alarm repeat mode has been specified, poke those bytes into the buffer before sending.
void RTC_DS1339::set_alarm1(alarm1_rate opt)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write( (uint8_t)DS1339_ALARM1_ADDR );
    
    Wire.write( rtc_bcd[DS1339_SEC]  | ((opt & B00000001 ) << 7)); // A1M1
    Wire.write( rtc_bcd[DS1339_MIN]  | ((opt & B00000010 ) << 6)); // A1M2
    Wire.write( rtc_bcd[DS1339_HOUR] | ((opt & B00000100 ) << 5)); // A1M3
    Wire.write( rtc_bcd[DS1339_DATE] | ((opt & B00001000 ) << 4)); // A1M4
    
    Wire.endTransmission();
}

void RTC_DS1339::set_alarm2(alarm2_rate opt)
{
    Wire.beginTransmission(DS1339_ADDR);
    Wire.write( (uint8_t)DS1339_ALARM2_ADDR );
    
    Wire.write( rtc_bcd[DS1339_MIN]  | ((opt & B00000001 ) << 7)); // A2M1
    Wire.write( rtc_bcd[DS1339_HOUR] | ((opt & B00000010 ) << 6)); // A2M2
    Wire.write( rtc_bcd[DS1339_DATE] | ((opt & B00000100 ) << 5)); // A2M3
    
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
    set_register(REG_DS1339_CONTROL, get_register(REG_DS1339_CONTROL) & !ALARM_INDEX(a));            
}

void RTC_DS1339::clear_interrupt(alarm a)
{
    set_register(REG_DS1339_STATUS, get_register(REG_DS1339_STATUS) & !ALARM_INDEX(a));
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

void RTC_DS1339::set_second(unsigned char v)
{
    rtc_bcd[DS1339_SEC] = bin2bcd(v);

}
void RTC_DS1339::set_minute(unsigned char v)
{
    rtc_bcd[DS1339_MIN] = bin2bcd(v);
}

void RTC_DS1339::set_hour(unsigned char v)
{
    rtc_bcd[DS1339_HOUR] = bin2bcd(v);
}

void RTC_DS1339::set_dayofweek(unsigned char v)
{
    rtc_bcd[DS1339_DOW] = bin2bcd(v);
}

void RTC_DS1339::set_day(unsigned char v)
{
    rtc_bcd[DS1339_DATE] = bin2bcd(v);
}

void RTC_DS1339::set_month(unsigned char v)
{
    rtc_bcd[DS1339_MONTH] = bin2bcd(v);
}

void RTC_DS1339::set_year(unsigned int v)
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

void RTC_DS1339::Stop(void)
{
	set_register(REG_DS1339_CONTROL, Get_Register(REG_DS1339_CONTROL) | BITM_DS1339_CONTROL_EOSC);
}

void RTC_DS1339::Start(void)
{
	set_register(REG_DS1339_CONTROL, Get_Register(REG_DS1339_CONTROL) & !BITM_DS1339_CONTROL_EOSC);
}

DS1339::DS1339() : DSRTCLib(2, 0)
{
}

DS1339::DS1339(int int_pin, int int_number) : DSRTCLib(int_pin, int_number)
{
}
