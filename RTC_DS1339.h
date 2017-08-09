/*
  RTC_DS1339.h - library for Maxim Integrated's DS1339 RTC
*/

// ensure this library description is only included once
#ifndef DSRTCLib_h
#define DSRTCLib_h

// include types & constants of Wiring core API
#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h> 
#define I2C_READ	Wire.read
#define I2C_WRITE	Wire.write
#else
#include <WProgram.h> 
#include <WConstants.h>
#define I2C_READ	Wire.receive
#define I2C_WRITE	Wire.send
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

#define DSRTCLib_BASE_YR		2000

#define DS1339_ADDR  B1101000  // DS1339 slave address

 // Define register bit masks
#define DSRTCLib_CLOCKHALT	B10000000

#define DSRTCLib_LO_BCD		B00001111
#define DSRTCLib_HI_BCD		B11110000

#define DSRTCLib_HI_SEC		B01110000
#define DSRTCLib_HI_MIN		B01110000
#define DSRTCLib_HI_HR		B00110000
#define DSRTCLib_LO_DOW		B00000111
#define DSRTCLib_HI_DATE		B00110000
#define DSRTCLib_HI_MTH		B00110000
#define DSRTCLib_HI_YR		B11110000

#define DSRTCLib_ARLM1		0x07
#define DSRTCLib_ARLM1_LO_SEC	B00001111
#define DSRTCLib_ARLM1_HI_SEC	B01110000
#define DSRTCLib_ARLM1_LO_MIN	B01110000
#define DSRTCLib_ARLM1_HI_MIN	B00001111

#define DSRTCLib_SP			0x0E
#define	DSRTCLib_SP_EOSC		B10000000
#define	DSRTCLib_SP_RS2		B00010000
#define	DSRTCLib_SP_RS1		B00001000
#define	DSRTCLib_SP_INTCN		B00000100
#define	DSRTCLib_SP_A2IE		B00000010
#define	DSRTCLib_SP_A1IE		B00000001

#define DSRTCLib_STATUS		0x0F
#define DSRTCLib_STATUS_OSF	B10000000
#define DSRTCLib_STATUS_A2F	B00000010
#define DSRTCLib_STATUS_A1F	B00000001


// DS1339 Register Definition
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
#define BITM_DS1339_ALARM1_MINUTES_SEC B01111111
#define BITP_DS1339_ALARM1_MINUTES_SEC 1
// A1M1
#define BITM_DS1339_ALARM1_MINUTES_A1M1 B10000000
#define BITP_DS1339_ALARM1_MINUTES_A1M1 7

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


/* Definitions for alarm repeat */
/* The private variable alarm_repeat holds the user's alarm repeat preference. However, the DS1337 & DS1339 encode these in the topmost bit(s) of the 4 alarm registers. */
/* Splattering these bits across the alarm regs is handled in the writeAlarm() function. */
/* If DY/DT is set, the day field is interpreted as a DayOfWeek (1 ~ 7), else it is interpreted as a DayOfMonth.*/

/* user alarm_repeat bit mask:
       7   6   5    4       3      2       1     0
      [x   x   x   A1M4   DY/DT   A1M3   A1M2   A1M1]
*/

#define EVERY_SECOND       B00010111
#define EVERY_MINUTE       B00010110
#define EVERY_HOUR         B00010100
#define EVERY_DAY          B00010000
#define EVERY_WEEK         B00001000
#define EVERY_MONTH        B00000000

#define ALARM_INDEX(a) (1<<a)  //  Interrupt Enable and Flag bit position for Alarm 1 and Alarm 2 in Status and Control register


typedef unsigned long time_t;


// Library interface
class RTC_DS1339
{
	// user-accessible "public" interface
public:
    enum alarm {A1, A2};
    
    RTC_DS1339();
    RTC_DS1339(int int_pin, int int_number);
    
    unsigned char time_is_set();
    unsigned char alarm_is_set();
    //unsigned char time_is_valid();
    
    void Enable_Interrupt();
    void Disable_Interrupt();
    void Clear_Interrupt();

		void    readTime();
		void    readAlarm();
		void    writeTime();
                void    writeTime(unsigned long);
		void    writeAlarm();
                void    writeAlarm(unsigned long);
                void    setAlarmRepeat(byte repeat);

                unsigned long date_to_epoch_seconds(unsigned int year, byte month, byte day, byte hour, byte minute, byte second);
                unsigned long date_to_epoch_seconds();
                void epoch_seconds_to_date(unsigned long);								
                void snooze(unsigned long secondsToSnooze);
				void custom_snooze(unsigned long secondsToSnooze);

        unsigned char Get_Seconds();
        unsigned char Get_Minutes();
        unsigned char Get_Hours();
        unsigned char Get_Days();
        unsigned char Get_DayOfWeek();
        unsigned char Get_Months();
        unsigned int Get_Years();
                        
        void Set_Seconds(unsigned char);
        void Set_Minutes(unsigned char);
        void Set_Hours(unsigned char);
        void Set_Days(unsigned char);
        void Set_DayOfWeek(unsigned char);
        void Set_Months(unsigned char);
        void Set_Years(unsigned int);

        void Start(void);
        void Stop(void);
        
        unsigned char Get_Register(unsigned char reg_address);
        void Set_Register(unsigned char reg_address, unsigned char reg_value);
private:
    void init();
    byte time_set;
    byte alarm_repeat;
    byte rtc_bcd[7]; // used prior to read/set DS1337 & DS1339 registers;
    void read(void);
    void save(void);
    byte bcd2bin(byte);
    byte bin2bcd(byte);
    int _rtc_int_number;
    int _rtc_int_pin;
};

#endif
