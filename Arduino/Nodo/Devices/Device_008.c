//#######################################################################################################
//##################################### Device-08 AlectoV1  #############################################
//#######################################################################################################

/*********************************************************************************************\
 * Dit protocol zorgt voor ontvangst van Alecto weerstation buitensensoren met protocol V1
 * 
 * Auteur             : Nodo-team (Martinus van den Broek) www.nodo-domotica.nl
 * Support            : www.nodo-domotica.nl
 * Datum              : Mrt.2013
 * Versie             : 1.0
 * Nodo productnummer : n.v.t. meegeleverd met Nodo code.
 * Compatibiliteit    : Vanaf Nodo build nummer 508
 * Syntax             : "AlectoV1 <Par1:Sensor ID>, <Par2:Basis Variabele>"
 *********************************************************************************************
 * Technische informatie:
 * Message Format: (9 nibbles, 36 bits):
 *
 * Format for Temperature Humidity
 *   AAAAAAAA BBBB CCCC CCCC CCCC DDDDDDDD EEEE
 *   RC       Type Temperature___ Humidity Checksum
 *   A = Rolling Code
 *   B = Message type (xyyx = temp/humidity if yy <> '11')
 *   C = Temperature (two's complement)
 *   D = Humidity BCD format
 *   E = Checksum
 *
 * Format for Rain
 *   AAAAAAAA BBBB CCCC DDDD DDDD DDDD DDDD EEEE
 *   RC       Type      Rain                Checksum
 *   A = Rolling Code
 *   B = Message type (xyyx = NON temp/humidity data if yy = '11')
 *   C = fixed to 1100
 *   D = Rain (bitvalue * 0.25 mm)
 *   E = Checksum
 *
 * Format for Windspeed
 *   AAAAAAAA BBBB CCCC CCCC CCCC DDDDDDDD EEEE
 *   RC       Type                Windspd  Checksum
 *   A = Rolling Code
 *   B = Message type (xyyx = NON temp/humidity data if yy = '11')
 *   C = Fixed to 1000 0000 0000
 *   D = Windspeed  (bitvalue * 0.2 m/s, correction for webapp = 3600/1000 * 0.2 * 100 = 72)
 *   E = Checksum
 *
 * Format for Winddirection & Windgust
 *   AAAAAAAA BBBB CCCD DDDD DDDD EEEEEEEE FFFF
 *   RC       Type      Winddir   Windgust Checksum
 *   A = Rolling Code
 *   B = Message type (xyyx = NON temp/humidity data if yy = '11')
 *   C = Fixed to 111
 *   D = Wind direction
 *   E = Windgust (bitvalue * 0.2 m/s, correction for webapp = 3600/1000 * 0.2 * 100 = 72)
 *   F = Checksum
 \*********************************************************************************************/
 
#ifdef DEVICE_008
#define DEVICE_ID 8
#define DEVICE_NAME "AlectoV1"

#define WS3500_PULSECOUNT 74

boolean Device_008(byte function, struct NodoEventStruct *event, char *string)
{
  boolean success=false;

  switch(function)
  {
#ifdef DEVICE_CORE_008
  case DEVICE_RAWSIGNAL_IN:
    {
      if (RawSignal.Number != WS3500_PULSECOUNT) return false;

      const byte PTMF=50;
      unsigned long bitstream=0;
      byte nibble0=0;
      byte nibble1=0;
      byte nibble2=0;
      byte nibble3=0;
      byte nibble4=0;
      byte nibble5=0;
      byte nibble6=0;
      byte nibble7=0;
      byte checksum=0;
      int temperature=0;
      byte humidity=0;
      unsigned int rain=0;
      byte windspeed=0;
      byte windgust=0;
      int winddirection=0;
      byte checksumcalc = 0;
      byte rc=0;
      byte basevar=0;

      for(byte x=2; x<=64; x=x+2)
        {
        if(RawSignal.Pulses[x]*PTMF > 0xA00) bitstream = ((bitstream >> 1) |(0x1L << 31)); 
        else bitstream = (bitstream >> 1);
        }

      for(byte x=66; x<=72; x=x+2)
        {
        if(RawSignal.Pulses[x]*PTMF > 0xA00) checksum = ((checksum >> 1) |(0x1L << 3)); 
        else checksum = (checksum >> 1);
        }

      nibble7 = (bitstream >> 28) & 0xf;
      nibble6 = (bitstream >> 24) & 0xf;
      nibble5 = (bitstream >> 20) & 0xf;
      nibble4 = (bitstream >> 16) & 0xf;
      nibble3 = (bitstream >> 12) & 0xf;
      nibble2 = (bitstream >> 8) & 0xf;
      nibble1 = (bitstream >> 4) & 0xf;
      nibble0 = bitstream & 0xf;

      // checksum calculations
      if ((nibble2 & 0x6) != 6) {
        checksumcalc = (0xf - nibble0 - nibble1 - nibble2 - nibble3 - nibble4 - nibble5 - nibble6 - nibble7) & 0xf;
      }
      else
        {
        // Alecto checksums are Rollover Checksums by design!
        if (nibble3 == 3)
          checksumcalc = (0x7 + nibble0 + nibble1 + nibble2 + nibble3 + nibble4 + nibble5 + nibble6 + nibble7) & 0xf;
        else
          checksumcalc = (0xf - nibble0 - nibble1 - nibble2 - nibble3 - nibble4 - nibble5 - nibble6 - nibble7) & 0xf;
        }

      if (checksum != checksumcalc) return false;
      rc = bitstream & 0xff;

      basevar = ProtocolAlectoCheckID(rc);

      event->Par1=rc;
      event->Par2=basevar;
      event->SourceUnit    = 0;                     // Komt niet van een Nodo unit af, dus unit op nul zetten
      event->Port          = VALUE_SOURCE_RF;

      if (basevar == 0) return true;

      if ((nibble2 & 0x6) != 6) {

        temperature = (bitstream >> 12) & 0xfff;
        //fix 12 bit signed number conversion
        if ((temperature & 0x800) == 0x800) temperature = temperature - 0x1000;
        UserVar[basevar -1] = (float)temperature/10;

        humidity = (10 * nibble7) + nibble6;
        UserVar[basevar +1 -1] = (float)humidity;
        RawSignal.Number=0;
        return true;
      }
      else
      {
        if (nibble3 == 3)
        {
          rain = ((bitstream >> 16) & 0xffff);
          // check if rain unit has been reset!
          if (rain < ProtocolAlectoRainBase) ProtocolAlectoRainBase=rain;
          if (ProtocolAlectoRainBase > 0)
          {
            UserVar[basevar +2 -1] += ((float)rain - ProtocolAlectoRainBase) * 0.25;
          }
          ProtocolAlectoRainBase = rain;
          return true;
        }

        if (nibble3 == 1)
        {
          windspeed = ((bitstream >> 24) & 0xff);
          UserVar[basevar +3 -1] = (float)windspeed * 0.72;
          RawSignal.Number=0;
          return true;
        }

        if ((nibble3 & 0x7) == 7)
        {
          winddirection = ((bitstream >> 15) & 0x1ff) / 45;
          UserVar[basevar +4 -1] = (float)winddirection;

          windgust = ((bitstream >> 24) & 0xff);
          UserVar[basevar +5 -1] = (float)windgust * 0.72;

          RawSignal.Number=0;
          return true;
        }
      }
      success = true;
      break;
    }
  case DEVICE_COMMAND:
    {
    if ((event->Par2 > 0) && (ProtocolAlectoCheckID(event->Par1) == 0))
      {
      for (byte x=0; x<5; x++)
        {
        if (ProtocolAlectoValidID[x] == 0)
          {
          ProtocolAlectoValidID[x] = event->Par1;
          ProtocolAlectoVar[x] = event->Par2;
          break;
          }
        }
      }
    break;
    }
#endif // DEVICE_CORE_008

#if NODO_MEGA
  case DEVICE_MMI_IN:
    {
    char *TempStr=(char*)malloc(26);
    string[25]=0;

    if(GetArgv(string,TempStr,1))
      {
      if(strcasecmp(TempStr,DEVICE_NAME)==0)
        {
        if(event->Par1>0 && event->Par1<255 && event->Par2>0 && event->Par2<=USER_VARIABLES_MAX)
          success=true;
        }
      }
      free(TempStr);
      break;
    }

  case DEVICE_MMI_OUT:
    {
    strcpy(string,DEVICE_NAME);            // Eerste argument=het commando deel
    strcat(string," ");
    strcat(string,int2str(event->Par1));
    strcat(string,",");
    strcat(string,int2str(event->Par2));
    break;
    }
#endif //NODO_MEGA
  }      
  return success;
}

byte ProtocolAlectoValidID[5];
byte ProtocolAlectoVar[5];
unsigned int ProtocolAlectoRainBase=0;

/*********************************************************************************************\
 * Calculates CRC-8 checksum
 * reference http://lucsmall.com/2012/04/29/weather-station-hacking-part-2/
 *           http://lucsmall.com/2012/04/30/weather-station-hacking-part-3/
 *           https://github.com/lucsmall/WH2-Weather-Sensor-Library-for-Arduino/blob/master/WeatherSensorWH2.cpp
 \*********************************************************************************************/
uint8_t ProtocolAlectoCRC8( uint8_t *addr, uint8_t len)
{
  uint8_t crc = 0;
  // Indicated changes are from reference CRC-8 function in OneWire library
  while (len--) {
    uint8_t inbyte = *addr++;
    for (uint8_t i = 8; i; i--) {
      uint8_t mix = (crc ^ inbyte) & 0x80; // changed from & 0x01
      crc <<= 1; // changed from right shift
      if (mix) crc ^= 0x31;// changed from 0x8C;
      inbyte <<= 1; // changed from right shift
    }
  }
  return crc;
}

/*********************************************************************************************\
 * Check for valid sensor ID
 \*********************************************************************************************/
byte ProtocolAlectoCheckID(byte checkID)
{
  for (byte x=0; x<5; x++) if (ProtocolAlectoValidID[x] == checkID) return ProtocolAlectoVar[x];
  return 0;
}


#endif //DEVICE_08

