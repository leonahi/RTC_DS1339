/*
  Author : Nahit Pawar 
  RTC_DS1339.h - Library for Maxim Integrated's DS1339 RTC
  Note : The supported format of date and time is - (YEAR : MONTH : DATE : HOUR : MINUTE : SECONDS) where HOUR is in 24 hour format
         Day of week (DOW) is not implemented in this version of library "YYYY-MM-DD hh:mm:ss" 19 characters
*/

// Ensure this library description is only included once
#ifndef RTC_DS1339_H
#define RTC_DS1339_H

// Include types & constants of Wiring core API
#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h> 
#define I2C_READ    Wire.read
#define I2C_WRITE   Wire.write
#endif

// include types & constants of Wire ic2 lib
#include <Wire.h>

//Indices within the rtc_bcd[] buffer
#define DS1339_SEC    0
#define DS1339_MIN    1
#define DS1339_HOUR   2
#define DS1339_DOW    3
#define DS1339_DATE   4
#define DS1339_MONTH  5
#define DS1339_YEAR   6

#define DS1339_BASE_YR        2000

#define DS1339_ADDR  B1101000  // DS1339 slave address

// DS1339 Register Address
#define REG_DS1339_SECONDS          0x00
#define REG_DS1339_MINUTES          0x01
#define REG_DS1339_HOURS            0x02
#define REG_DS1339_DAY              0x03
#define REG_DS1339_DATE             0x04
#define REG_DS1339_MONTH            0x05
#define REG_DS1339_YEAR             0x06
#define REG_DS1339_ALARM1_SECONDS   0x07
#define REG_DS1339_ALARM1_MINUTES   0x08
#define REG_DS1339_ALARM1_HOURS     0x09
#define REG_DS1339_ALARM1_DAYDATE   0x0A
#define REG_DS1339_ALARM2_MINUTES   0x0B
#define REG_DS1339_ALARM2_HOURS     0x0C
#define REG_DS1339_ALARM2_DAYDATE   0x0D
#define REG_DS1339_CONTROL          0x0E
#define REG_DS1339_STATUS           0x0F
#define REG_DS1339_TRICKLE_CHARGER  0x10

#define DS1339_ALARM1_ADDR 0x07
#define DS1339_ALARM2_ADDR 0x0B

/* 
 * DS1339 Register Bit map (BITM) and Bit position (BITP)
*/

// REG_DS1339_SECONDS - Seconds
#define BITM_DS1339_SECONDS B01111111
#define BITP_DS1339_SECONDS 1

// REG_DS1339_MINUTES - Minutes 
#define BITM_DS1339_MINUTES B01111111
#define BITP_DS1339_MINUTES 1

// REG_DS1339_HOURS - Hours 
// 12HOUR
#define BITM_DS1339_HOURS_12HOUR B00011111
#define BITP_DS1339_HOURS_24HOUR 1
// 24HOUR
#define BITM_DS1339_HOURS_24HOUR B00111111
#define BITP_DS1339_HOURS_24HOUR 1
// AMPM
#define BITM_DS1339_HOURS_AMPM B00100000
#define BITP_DS1339_HOURS_AMPM 5
// 12/24
#define BITM_DS1339_HOURS_1224 B01000000
#define BITP_DS1339_HOURS_1224 6

// REG_DS1339_DAY - Day
#define BITM_DS1339_DAY B00000111
#define BITP_DS1339_DAY 1

// REG_DS1339_DATE Date
#define BITM_DS1339_DATE B00111111
#define BITP_DS1339_DATE 1

// REG_DS1339_MONTH Month
// MTH
#define BITM_DS1339_MONTH_MTH B00011111
#define BITP_DS1339_MONTH_MTH 1
// CEN
#define BITM_DS1339_MONTH_CEN B10000000
#define BITP_DS1339_MONTH_CEN 7

// REG_DS1339_YEAR Year 
#define BITM_DS1339_YEAR B11111111
#define BITP_DS1339_YEAR 1

// REG_DS1339_ALARM1_SECONDS - Alarm 1 Seconds
// SEC
#define BITM_DS1339_ALARM1_SECONDS_SEC B01111111
#define BITP_DS1339_ALARM1_SECONDS_SEC 1
// A1M1
#define BITM_DS1339_ALARM1_SECONDS_A1M1 B10000000
#define BITP_DS1339_ALARM1_SECONDS_A1M1 7

// REG_DS1339_ALARM1_MINUTES - Alarm 1 Minutes
// MIN
#define BITM_DS1339_ALARM1_MINUTES_MIN B01111111
#define BITP_DS1339_ALARM1_MINUTES_MIN 1
// A1M2
#define BITM_DS1339_ALARM1_MINUTES_A1M2 B10000000
#define BITP_DS1339_ALARM1_MINUTES_A1M2 7

// REG_DS1339_ALARM1_HOURS - Alarm 1 Hours
// 12HOURS
#define BITM_DS1339_ALARM1_HOURS_12HOUR B00011111
#define BITP_DS1339_ALARM1_HOURS_12HOUR 1
// 24HOURS
#define BITM_DS1339_ALARM1_HOURS_24HOUR B00111111
#define BITP_DS1339_ALARM1_HOURS_24HOUR 1
// AMPM
#define BITM_DS1339_ALARM1_HOURS_AMPM B00100000
#define BITP_DS1339_ALARM1_HOURS_AMPM 5
// 12/24
#define BITM_DS1339_ALARM1_HOURS_1224 B01000000
#define BITP_DS1339_ALARM1_HOURS_1224 6
// A1M3
#define BITM_DS1339_ALARM1_HOURS_A1M3 B10000000
#define BITP_DS1339_ALARM1_HOURS_A1M3 7

// REG_DS1339_ALARM1_DAYDATE - Alarm 1 Day, Alarm 1 Date
// DAY
#define BITM_DS1339_ALARM1_DAYDATE_DAY B00001111
#define BITP_DS1339_ALARM1_DAYDATE_DAY 1
// DATE
#define BITM_DS1339_ALARM1_DAYDATE_DATE B00111111
#define BITP_DS1339_ALARM1_DAYDATE_DATE 1
// DYDT
#define BITM_DS1339_ALARM1_DAYDATE_DYDT B01000000
#define BITP_DS1339_ALARM1_DAYDATE_DYDT 6
// A1M4
#define BITM_DS1339_ALARM1_DAYDATE_A1M4 B10000000
#define BITP_DS1339_ALARM1_DAYDATE_A1M4 7

// REG_DS1339_ALARM2_MINUTES - Alarm 2 Minutes
// MIN
#define BITM_DS1339_ALARM2_MINUTES_MIN B01111111
#define BITP_DS1339_ALARM2_MINUTES_MIN 1
// A2M2
#define BITM_DS1339_ALARM2_MINUTES_A2M2 B10000000
#define BITP_DS1339_ALARM2_MINUTES_A2M2 7

// REG_DS1339_ALARM2_HOURS - Alarm 2 Hours
// 12HOURS
#define BITM_DS1339_ALARM2_HOURS_12HOUR B00011111
#define BITP_DS1339_ALARM2_HOURS_12HOUR 1
// 24HOURS
#define BITM_DS1339_ALARM2_HOURS_24HOUR B00111111
#define BITP_DS1339_ALARM2_HOURS_24HOUR 1
// AMPM
#define BITM_DS1339_ALARM2_HOURS_AMPM B00100000
#define BITP_DS1339_ALARM2_HOURS_AMPM 5
// 12/24
#define BITM_DS1339_ALARM2_HOURS_1224 B01000000
#define BITP_DS1339_ALARM2_HOURS_1224 6
// A2M3
#define BITM_DS1339_ALARM2_HOURS_A2M3 B10000000
#define BITP_DS1339_ALARM2_HOURS_A2M3 7

// REG_DS1339_ALARM2_DAYDATE - Alarm 2 Day, Alarm 2 Date
// DAY
#define BITM_DS1339_ALARM2_DAYDATE_DAY B00001111
#define BITP_DS1339_ALARM2_DAYDATE_DAY 1
// DATE
#define BITM_DS1339_ALARM2_DAYDATE_DATE B00111111
#define BITP_DS1339_ALARM2_DAYDATE_DATE 1
// DYDT
#define BITM_DS1339_ALARM2_DAYDATE_DYDT B01000000
#define BITP_DS1339_ALARM2_DAYDATE_DYDT 6
// A2M4
#define BITM_DS1339_ALARM2_DAYDATE_A2M4 B10000000
#define BITP_DS1339_ALARM2_DAYDATE_A2M4 7

// REG_DS1339_CONTROL - Control
// A1IE
#define BITM_DS1339_CONTROL_A1IE B00000001
#define BITP_DS1339_CONTROL_A1IE 1
// A2IE
#define BITM_DS1339_CONTROL_A2IE B00000010
#define BITP_DS1339_CONTROL_A2IE 2
// INTCN
#define BITM_DS1339_CONTROL_INTCN B00000100
#define BITP_DS1339_CONTROL_INTCN 3
// RS1
#define BITM_DS1339_CONTROL_RS1 B00001000
#define BITP_DS1339_CONTROL_RS1 4
// RS2
#define BITM_DS1339_CONTROL_RS2 B00010000
#define BITP_DS1339_CONTROL_RS2 5
// BBSQI
#define BITM_DS1339_CONTROL_BBSQI B00100000
#define BITP_DS1339_CONTROL_BBSQI 6
// !EOSC!
#define BITM_DS1339_CONTROL_EOSC B10000000
#define BITP_DS1339_CONTROL_EOSC 7

// REG_DS1339_STATUS - Status
// A1F
#define BITM_DS1339_STATUS_A1F B00000001
#define BITP_DS1339_STATUS_A1F B00000001
// A2F
#define BITM_DS1339_STATUS_A2F B00000010
#define BITP_DS1339_STATUS_A2F B00000010
// OSF
#define BITM_DS1339_STATUS_OSF B10000000
#define BITP_DS1339_STATUS_OSF B10000000


#define ALARM_INDEX(a) (1<<a)  //  Interrupt Enable and Flag bit position for Alarm 1 and Alarm 2 in Status and Control register

typedef unsigned long time_t;

// Library interface
class RTC_DS1339
{
public:
    RTC_DS1339();
    
    enum alarm {A1=0, A2=1};

    // Alarm 1 repeat bit mask : [x  x  x  DY/DT  A1M4  A1M3  A1M2  A1M1] 
    enum alarm1_rate {A1_EVERY_SEC=B00001111, A1_EVERY_MIN=B00001110, A1_EVERY_HR=B00001100, A1_EVERY_DY=B00001000, A1_EVERY_WK=B00010000, A1_EVERY_MON=B00000000};
    
    // Alarm 2 repear bit mask : [x  x  x  x  DY/DT  A2M4  A2M3  A2M2]
    enum alarm2_rate {A2_EVERY_0_SEC=B00000111, A2_EVERY_MIN=B00000110, A2_EVERY_DY=B00000100, A2_EVERY_WK=B00001000, A2_EVERY_MON=B00000000};

    unsigned char time_is_set();
    unsigned char alarm_is_set();
    
    void enable_interrupt(alarm a);
    void disable_interrupt(alarm a);
    void clear_interrupt(alarm a);
    
    void read_time();
    void set_time();
    
    void read_alarm1();
    void read_alarm2();
    
    void set_alarm1(alarm1_rate opt);
    void set_alarm2(alarm2_rate opt);
        
    void time_stamp();
    
    unsigned short int set_time_serial();
    
    unsigned char get_seconds();
    unsigned char get_minutes();
    unsigned char get_hours();
    unsigned char get_days();
    unsigned char get_dayofweek();
    unsigned char get_months();
    unsigned int get_years();
    
    void set_seconds(unsigned char);
    void set_minutes(unsigned char);
    void set_hours(unsigned char);
    void set_days(unsigned char);
    void set_dayofweek(unsigned char);
    void set_months(unsigned char);
    void set_years(unsigned int);
    
    void rtc_start(void);
    void rtc_stop(void);
    
    unsigned char get_register(unsigned char reg_addr);
    void set_register(unsigned char reg_addr, unsigned char reg_value);

private:
    byte time_set;
    byte alarm_repeat;
    byte rtc_bcd[7]; // used prior to read/set DS1337 & DS1339 registers;
    void read(void);
    void save(void);
    byte bcd2bin(byte);
    byte bin2bcd(byte);    
};

#endif
