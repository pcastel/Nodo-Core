
/*********************************************************************************************\
 * Eenvoudige check of event een Nodo commando is die voor deze Nodo bestemd is.
 * Test NIET op geldigheid van de parameters
 * geeft de waarde 0, NODO_TYPE_EVENT of NODO_TYPE_COMMAND terug.
 \*********************************************************************************************/
byte NodoType(unsigned long Content)
  {
  byte x;

  // als het geen Nodo event of commando was dan zowieso geen commando
  if(((Content>>28)&0xf)!=SIGNAL_TYPE_NODO)
    return false;
  
  // als het een UserEvent was, dan is die altijd voor deze Nodo
  if(((Content>>16)&0xff)==CMD_USEREVENT)
    return NODO_TYPE_EVENT;

  // als Unit deel ongelijk is aan eigen adres en ongelijk aan wildcard unit=0, dan was het een event voor een andere Nodo
  x=(Content>>24)&0xf;
  if(x!=S.Unit && x!=0)
    return false;

  x=(Content>>16)&0xff;

  if(x>=FIRST_EVENT && x<=LAST_EVENT)
    return NODO_TYPE_EVENT;

  if(x>=FIRST_COMMAND && x<=LAST_COMMAND)
    return NODO_TYPE_COMMAND;

  return false;
  }  
  

/*********************************************************************************************\
 * Deze functie checked of de code die ontvangen is een geldige uitvoerbare opdracht is
 * Als het een correct commando is wordt een false teruggegeven 
 * in andere gevallen een foutcode
 \*********************************************************************************************/

byte CommandError(unsigned long Content)
  {
  byte x;
  
  x=NodoType(Content);
  if(x!=NODO_TYPE_COMMAND && x!=NODO_TYPE_EVENT)
    return ERROR_01;
  
  byte Command      = (Content>>16)&0xff;
  byte Par1         = (Content>>8)&0xff;
  byte Par2         = Content&0xff;
 


  switch(Command)
    {
    //test; geen, altijd goed
    case CMD_VARIABLE_SET:   //??? nog afvangen bij foute invoer
    case CMD_FILE_GET_HTTP:
    case CMD_IP_SETTINGS:
    case CMD_RAWSIGNAL_SAVE:
    case CMD_RAWSIGNAL_SEND:
    case CMD_FILE_SHOW:
    case CMD_FILE_ERASE:
    case CMD_FILE_EXECUTE:
    case CMD_FILE_WRITE:
    case CMD_FILE_LIST:
    case CMD_FILE_LOG:
    case CMD_PORT:
    case CMD_SEND: //??? t.b.v. test/ontwikkeling. nog niet operationeel.
    case CMD_ERROR:
    case CMD_REBOOT:
    case CMD_VARIABLE_SAVE:
    case CMD_WAITFREERF: 
    case CMD_USERPLUGIN: 
    case CMD_VARIABLE_EVENT:    
    case CMD_NODO_IP:    
    case CMD_DLS_EVENT:
    case CMD_CLOCK_EVENT_DAYLIGHT:
    case CMD_CLOCK_EVENT_ALL:
    case CMD_CLOCK_EVENT_SUN:
    case CMD_CLOCK_EVENT_MON:
    case CMD_CLOCK_EVENT_TUE:
    case CMD_CLOCK_EVENT_WED:
    case CMD_CLOCK_EVENT_THU:
    case CMD_CLOCK_EVENT_FRI:
    case CMD_CLOCK_EVENT_SAT:
    case CMD_STATUS:
    case CMD_STATUS_SEND:
    case CMD_DELAY:
    case CMD_SOUND: 
    case CMD_USEREVENT:
    case CMD_SEND_USEREVENT:
    case CMD_ANALYSE_SETTINGS:
    case CMD_KAKU:
    case CMD_SEND_KAKU:
    case CMD_EVENTGHOST_SERVER:
      return false;
 
    case CMD_SEND_KAKU_NEW:
    case CMD_KAKU_NEW:    
      if(Par2==VALUE_ON || Par2==VALUE_OFF || Par2<=16)return false;
      return ERROR_02;

    // Generiek: geen par1 of par2 ingevuld.
    case CMD_BOOT_EVENT:
    case CMD_EVENTLIST_WRITE:
    case CMD_EVENTLIST_SHOW:
    case CMD_RESET:
    case CMD_EVENTLIST_ERASE: 
      if(Par1!=0)return ERROR_02;    
      if(Par2!=0)return ERROR_02;    
      return false; 
      
    case CMD_TIMER_SET_SEC:
    case CMD_TIMER_SET_MIN:
      if(Par1>USER_VARIABLES_MAX)return ERROR_02;
      return false;

    // test:Par1 binnen bereik maximaal beschikbare variabelen
    case CMD_VARIABLE_INC: 
    case CMD_VARIABLE_DEC: 
    case CMD_BREAK_ON_VAR_NEQU:
    case CMD_BREAK_ON_VAR_MORE:
    case CMD_BREAK_ON_VAR_LESS:
    case CMD_BREAK_ON_VAR_EQU:
//      if(Par1<1 || Par1>USER_VARIABLES_MAX)return ERROR_02; nog afvangen n.a.v. analoog maken.
      return false;
      
    // test:Par1 en Par2 binnen bereik maximaal beschikbare variabelen
    case CMD_VARIABLE_VARIABLE:
      if(Par1<1 || Par1>USER_VARIABLES_MAX)return ERROR_02;
      if(Par2<1 || Par2>USER_VARIABLES_MAX)return ERROR_02;
      return false;
        
    // test:Par1 binnen bereik maximaal beschikbare timers
    case CMD_TIMER_EVENT:
    case CMD_TIMER_RANDOM:
      if(Par1<1 || Par1>TIMER_MAX)return ERROR_02;
      return false;

    // Par1 alleen 0,1 of 7
    case CMD_SIMULATE_DAY:
      if(Par1!=0 && Par1!=1 && Par1!=7)return ERROR_02;
      return false;
      
    // geldig jaartal
    case CMD_CLOCK_YEAR:
      if(Par1>21)return ERROR_02;
      return false;
    
    // geldige tijd    
    case CMD_CLOCK_TIME:
      if(Par1>23)return ERROR_02;
      if(Par2>59)return ERROR_02;
      return false;

    // geldige datum
    case CMD_CLOCK_DATE: // data1=datum, data2=maand
      if(Par1>31 || Par1<1)return ERROR_02;
      if(Par2>12 || Par2<1)return ERROR_02;
      return false;

    case CMD_CLOCK_DOW:
      if(Par1<1 || Par1>7)return ERROR_02;
      return false;

    case CMD_WIREDANALOG_VARIABLE:
      if(Par1<1 || Par1>WIRED_PORTS)return ERROR_02;
      if(Par2<1 || Par2>USER_VARIABLES_MAX)return ERROR_02;
      return false;
      
    // test:Par1 binnen bereik maximaal beschikbare wired poorten.
    case CMD_WIRED_IN_EVENT:
    case CMD_WIRED_ANALOG:
    case CMD_WIRED_THRESHOLD:
    case CMD_WIRED_SMITTTRIGGER:
      if(Par1<1 || Par1>WIRED_PORTS)return ERROR_02;
      return false;

    // test:Par1 binnen bereik maximaal beschikbare wired poorten EN Par2 is ON of OFF
    case CMD_WIRED_OUT:
    case CMD_WIRED_PULLUP:
      if(Par1<1 || Par1>WIRED_PORTS)return ERROR_02;
      if(Par2!=VALUE_ON && Par2!=VALUE_OFF)return ERROR_02;
      return false;

//    case CMD_RAWSIGNAL_COPY: //??? kan dit weg of herstellen wat verpunkt is?
//      if(Par1!=VALUE_IP && Par1!=VALUE_EVENTGHOST && Par1!=VALUE_OFF)return ERROR_02;
//      return false;

    case CMD_WAITBUSY:
    case CMD_SENDBUSY:
      if(Par1!=VALUE_OFF && Par1!=VALUE_ON && Par1!=VALUE_ALL)return ERROR_02;
      return false;

    case CMD_TRANSMIT_IP:
      if(Par1!=VALUE_OFF && Par1!=VALUE_SOURCE_HTTP && Par1!=VALUE_SOURCE_EVENTGHOST)return ERROR_02;
      return false;

    case CMD_WIRED_ANALOG_CALIBRATE:
      if(Par1!=VALUE_HIGH && Par1!=VALUE_LOW)return ERROR_02;
      return false;

    case CMD_SEND_EVENT:// ??? opties niet gellijk aan wildcard! moet EG hier bij?
      switch(Par1)
        {
        case VALUE_ALL:
        case VALUE_SOURCE_IR:
        case VALUE_SOURCE_RF:
        case VALUE_SOURCE_EVENTGHOST:
        case VALUE_SOURCE_HTTP:
        case VALUE_SOURCE_SERIAL:
          break;
        default:
          return ERROR_02;
        }
      return false;

    case CMD_COMMAND_WILDCARD:
      switch(Par1)
        {
        case VALUE_ALL:
        case VALUE_SOURCE_IR:
        case VALUE_SOURCE_EVENTGHOST:
        case VALUE_SOURCE_HTTP:
        case VALUE_SOURCE_RF:
        case VALUE_SOURCE_SERIAL:
        case VALUE_SOURCE_WIRED:
        case VALUE_SOURCE_EVENTLIST:
        case VALUE_SOURCE_SYSTEM:
        case VALUE_SOURCE_TIMER:
        case VALUE_SOURCE_VARIABLE:
        case VALUE_SOURCE_CLOCK:
          break;
        default:
          return ERROR_02;
        }

      switch(Par2)
        {
        case VALUE_ALL:
        case CMD_ERROR:
        case CMD_KAKU:
        case CMD_KAKU_NEW:
        case CMD_USEREVENT:
          break;
        default:
          return ERROR_02;
        } 
      return false;

     // par1 alleen On of Off.
     // par2 alleen 0, On of Off.
    case CMD_TRACE:
    case CMD_TRANSMIT_RF:
    case CMD_TRANSMIT_IR:
      if(Par1!=VALUE_OFF && Par1!=VALUE_ON)return ERROR_02;
      if(Par2!=VALUE_OFF && Par2!=VALUE_ON && Par2!=0)return ERROR_02;
      return false;

//    case CMD_DIVERT:   //???
//     if(Par1>UNIT_MAX)return ERROR_02;
//     return false;

    case CMD_UNIT:
      if(Par1<1 || Par1>UNIT_MAX)return ERROR_02;
      return false;

    default:
      return ERROR_01;
    }
  }

 
/*********************************************************************************************\
 * Deze functie checked of de code die ontvangen is een uitvoerbare opdracht is/
 * Als het een correct commando is wordt deze uitgevoerd en 
 * true teruggegeven. Zo niet dan wordt er een 'false' retour gegeven.
 * 
 \*********************************************************************************************/

boolean ExecuteCommand(unsigned long Content, int Src, unsigned long PreviousContent, int PreviousSrc)
  {
  unsigned long Event, Action;  
  int x,y,z;

  byte error        = false;
  byte Command      = (Content>>16)&0xff;
  byte Par1         = (Content>>8)&0xff;
  byte Par2         = Content&0xff;
  byte Type         = (Content>>28)&0x0f;
  byte PreviousType = (PreviousContent>>28)&0x0f;

  // Serial.print("*** debug: ExecuteCommand(); ");Serial.println(Content,HEX); //??? Debug

  error=CommandError(Content);
  if(error)
    {
    if(error==ERROR_01) // commando bestaat niet 
      RaiseError(ERROR_01);    
    else // commando bestaat maar parameters niet correct
      RaiseError(ERROR_02);

    // als er een error is, dan stoppen met verwerken van de gehele regel. Dit om stapelen van errors te voorkomen.
    return false;
    } 
  else // geen fouten, dan verwerken
    {        
    switch(Command)
      {   
      case CMD_SEND_KAKU:
        TransmitCode(command2event(CMD_KAKU,Par1,Par2),VALUE_ALL);
        break;
        
      case CMD_SEND_KAKU_NEW:
        TransmitCode(command2event(CMD_KAKU_NEW,Par1,Par2),VALUE_ALL);
        break;
        
      case CMD_VARIABLE_INC:
        Par2PortAnalog(Par1, Par2, &y, &x);// y=variabele, x=waarde
        z=UserVar[y]+x;
        if(abs(z)<=10000)
          UserVar[y]+=x;
        break;        
  
      case CMD_VARIABLE_DEC: 
        Par2PortAnalog(Par1, Par2, &y, &x);// y=variabele, x=waarde
        z=UserVar[y]-x;
        if(abs(z)<=10000)
          UserVar[y]-=x;
        break;        
  
      case CMD_VARIABLE_SAVE:   
        if(Par1==0)
          for(x=0;x<USER_VARIABLES_MAX;x++)
            S.UserVar[x]=UserVar[x];
        else
          S.UserVar[Par1]=UserVar[Par1];            
        SaveSettings();
        break;        
    
      case CMD_VARIABLE_SET:
        Par2PortAnalog(Par1, Par2, &y, &x);// y=variabele, x=waarde
        if(y==0)
          for(z=0;z<USER_VARIABLES_MAX;z++)
            UserVar[z]=x;
        else
          UserVar[y-1]=x;
        break;        
    
      case CMD_WIREDANALOG_VARIABLE:
        UserVar[Par2-1]=map(analogRead(PIN_WIRED_IN_1+Par1-1),S.WiredInput_Calibration_IL[Par1-1],S.WiredInput_Calibration_IH[Par1-1],S.WiredInput_Calibration_OL[Par1-1],S.WiredInput_Calibration_OH[Par1-1]);
        break;
        
      case CMD_VARIABLE_VARIABLE:
        UserVar[Par1-1]=UserVar[Par2-1];
        break;        
  
      case CMD_BREAK_ON_VAR_EQU:
        Par2PortAnalog(Par1, Par2, &y, &x);// y=variabele, x=waarde
        if(UserVar[y-1]==x)
          error=true;
        break;
        
      case CMD_BREAK_ON_VAR_NEQU:
        Par2PortAnalog(Par1, Par2, &y, &x);// y=variabele, x=waarde
        if(UserVar[y-1]!=x)
          error=true;
        break;
  
      case CMD_BREAK_ON_VAR_MORE:
        Par2PortAnalog(Par1, Par2, &y, &x);// y=variabele, x=waarde
        if(UserVar[y-1]>x)
          error=true;
        break;

      case CMD_BREAK_ON_VAR_LESS:
        Par2PortAnalog(Par1, Par2, &y, &x);// y=variabele, x=waarde
        if(UserVar[y-1]<x)
          error=true;
        break;
  
      case CMD_SEND_USEREVENT:
        // Voeg Unit=0 want een UserEvent is ALTIJD voor ALLE Nodo's. Verzend deze vervolgens.
        TransmitCode(command2event(CMD_USEREVENT,Par1,Par2),VALUE_ALL);// Maak Unit=0 want een UserEvent is ALTIJD voor ALLE Nodo's.;
        break;
  
      case CMD_SIMULATE_DAY:
        if(Par1==0)Par1=1;
        SimulateDay(Par1); 
        break;     
  
      case CMD_RAWSIGNAL_SEND:
        if(Par1!=0)
          RawSignalGet(Par1);
        //??? PrintRawSignal();
        TransmitCode(AnalyzeRawSignal(),VALUE_ALL);
        break;        
  
      case CMD_CLOCK_YEAR:
        x=Par1*100+Par2;
        Time.Year=x;
        ClockSet();
        ClockRead();
        break;
      
      case CMD_CLOCK_TIME:
        Time.Hour=Par1;
        Time.Minutes=Par2;
        Time.Seconds=0;
        ClockSet();
        break;
  
      case CMD_CLOCK_DATE: // data1=datum, data2=maand
        Time.Date=Par1;
        Time.Month=Par2;
        ClockSet();
        ClockRead();
        break;
  
      case CMD_CLOCK_DOW:
        Time.Day=Par1;
        ClockSet();
        break;

      case CMD_TIMER_SET_MIN:
        // Par1=timer, Par2=minuten. Timers werken op een resolutie van seconden maar worden door de gebruiker ingegeven in minuten        
        TimerSet(Par1,int(Par2)*60);
        break;
        
      case CMD_TIMER_SET_SEC:
        // Par1=timer, Par2=seconden. Timers werken op een resolutie van seconden.            
        TimerSet(Par1,Par2);
        break;

      case CMD_TIMER_RANDOM:
        UserTimer[Par1-1]=millis()+random(Par2)*60000;// Par1=timer, Par2=maximaal aantal minuten
        break;
  
      case CMD_DELAY:
        if(Par1)
          {
          HoldTimer=millis()+((unsigned long)(Par1))*1000;
          if(Par2==VALUE_OFF) // Niet opslaan in de queue, maar direct een 'dode' pause uitvoeren.
            {
            while(HoldTimer>millis())        
              digitalWrite(PIN_LED_RGB_R,(millis()>>7)&0x01);
            }
          else
            {
            // start een nieuwe recursieve loop() om zo de events die voorbij komen te plaatsen in de queue.
            // deze recursieve aanroep wordt beëindigd als HoldTimer==0L
            Hold=CMD_DELAY;
            loop();
            }
          }        
        else
          HoldTimer=0L; //  Wachttijd is afgelopen;
        break;        
        
      case CMD_SEND_EVENT:
        TransmitCode(PreviousContent,Par1);
        break;        

      case CMD_SOUND: 
        Alarm(Par1,Par2);
        break;     
    
      case CMD_WIRED_PULLUP:
        S.WiredInputPullUp[Par1-1]=Par2==VALUE_ON; // Par1 is de poort[1..4], Par2 is de waarde [0..1]
        digitalWrite(14+PIN_WIRED_IN_1+Par1-1,S.WiredInputPullUp[Par1-1]==VALUE_ON);// Zet de pull-up weerstand van 20K voor analoge ingangen. Analog-0 is gekoppeld aan Digital-14 
        // ??? uitzoeken hoe bovenstaande werkt voor de ATMega2260 !
        SaveSettings();
        break;
                   
      case CMD_WIRED_OUT:
        digitalWrite(PIN_WIRED_OUT_1+Par1-1,Par2==VALUE_ON);
        WiredOutputStatus[Par1-1]=Par2==VALUE_ON;
        PrintEvent(Content,VALUE_SOURCE_WIRED,VALUE_DIRECTION_OUTPUT);
        break;
                           
      case CMD_WAITFREERF: 
        S.WaitFreeRF_Delay=Par1;
        S.WaitFreeRF_Window=Par2;
        SaveSettings();
        break;

      case CMD_TRACE: 
        S.Debug=Par1;
        SaveSettings();
        break;
  
      case CMD_RAWSIGNAL_COPY:
        if(Par1==VALUE_RF_2_IR)CopySignalRF2IR(Par2);      
        if(Par1==VALUE_IR_2_RF)CopySignalIR2RF(Par2);
        break;        
  
      case CMD_SENDBUSY:
        if(Par1==VALUE_ALL)
          {
          S.SendBusy=Par1;
          SaveSettings();
          }
        else
          if(S.SendBusy==VALUE_ALL)
            {
            S.SendBusy=VALUE_OFF;
            SaveSettings();
            }
          else
            TransmitCode(command2event(CMD_BUSY,Par1,0),VALUE_ALL);
        break;
        
      case CMD_TRANSMIT_IP:
        S.TransmitIP=Par1;        
        SaveSettings();
        break;
        
      case CMD_WAITBUSY:
        if(Par1==VALUE_ALL)
          S.WaitBusy=Par1;
        else
          if(S.WaitBusy==VALUE_ALL)
            S.WaitBusy=VALUE_OFF;
          else
            {
            Hold=CMD_BUSY;
            loop(); // deze recursieve aanroep wordt beëindigd als BusyNodo==0
            }
        break;

      case CMD_TRANSMIT_IR:
        S.TransmitIR=Par1;
        if(Par2)S.TransmitRepeatIR=Par2;
        SaveSettings();
        break;

      case CMD_TRANSMIT_RF:
        S.TransmitRF=Par1;
        if(Par2)S.TransmitRepeatRF=Par2;
        SaveSettings();
        break;
        
      case CMD_USERPLUGIN:
        #ifdef USER_PLUGIN
          UserPlugin_Command(Par1,Par2);
        #endif
        break;        


      case CMD_WIRED_THRESHOLD:
        // Dit commando wordt ook afgevangen in ExecuteLine(). Hier opgenomen i.g.v. ontvangst via RF of IR
        S.WiredInputThreshold[Par1-1]=event2AnalogInt(Content);
        SaveSettings();
        break;                  
        
      case CMD_WIRED_SMITTTRIGGER:
        // Dit commando wordt ook afgevangen in ExecuteLine(). Hier opgenomen i.g.v. ontvangst via RF of IR
        S.WiredInputSmittTrigger[Par1-1]=event2AnalogInt(Content);
        SaveSettings();
        break;                  

      case CMD_STATUS:
        Status(Par1, Par2, false);
        break;

      case CMD_STATUS_SEND:
        Status(Par1, Par2, true);
        break;
                
      case CMD_REBOOT:
        delay(1000);
        Reset();
        break;        

      case CMD_ANALYSE_SETTINGS:
        S.AnalyseTimeOut=Par1;
        S.AnalyseSharpness=Par2*1000;
        SaveSettings();
        break;

      }
    }
  return error?false:true;
  }

 /*******************************************************************************************************\
 * Deze funktie parsed een string en voert de commando uit die daarin voorkomen. Commandos
 * worden gescheiden met een puntkomma.
 \*******************************************************************************************************/
void ExecuteLine(char *Line, byte Port)
  {
  const int MaxCommandLength=80; // maximaal aantal tekens van een commando
  char Command[MaxCommandLength]; 
  char TmpStr[INPUT_BUFFER_SIZE+1];
  int PosCommand;
  int PosLine;
  int L=strlen(Line);
  int x,y;
  int EventlistWriteLine=0;
  byte Error=0,Par1,Par2,Cmd;
  byte State_EventlistWrite=0;
  unsigned long v,event,action; 

  // verwerking van commando's is door gebruiker tijdelijk geblokkeerd door FileWrite commando
  if(FileWriteMode>0)
    {
    if(StringFind(Line,cmd2str(CMD_FILE_WRITE))!=-1)// string gevonden!
      {
      FileWriteMode=0;
      TempLogFile[0]=0;
      Serial.print("*** Debug: FileWrite ready.");Serial.println(); //??? Debug
      return;
      }
    else if(TempLogFile[0]!=0)
      {
      AddFileSDCard(TempLogFile,Line); // Extra logfile op verzoek van gebruiker
      return;
      }
    }
    
  // geef invoer regel weer 
  strcpy(TmpStr,">");
  strcat(TmpStr,Line);
  PrintTerminal(TmpStr);

  PosCommand=0;
  for(PosLine=0;PosLine<=L && Error==0 ;PosLine++)
    {
    x=Line[PosLine];

    // Comment teken. hierna verdier niets meer doen.
    if(x=='!') 
      PosLine=L+1; // ga direct naar einde van de regel.
       
    // als puntkomma (scheidt opdachten) of einde string(0), en het commando groter dan drie tekens
    if((x=='!' || x==';' || x==0) && PosCommand>3)
      {
      Command[PosCommand]=0;
      PosCommand=0;
      Cmd=0;
      Par1=0;
      Par2=0;
      v=0;
      // string met commando compleet
      
      // maak van de string een commando waarde. Het kan ook zijn dat het een hex-event is.
      if(GetArgv(Command,TmpStr,1));
        v=str2int(TmpStr);

      // kleiner of gelijk aan een bestaand commando. Dan behandelen als een commando. Anders is het een hex-event.
      if(v<=COMMAND_MAX)
        {
        Cmd=(byte)v;
        v=0;          
        
        if(GetArgv(Command,TmpStr,2))
          Par1=str2int(TmpStr);
  
        if(GetArgv(Command,TmpStr,3))
          Par2=str2int(TmpStr);
  
        switch(Cmd)
          {
          // Hier worden de commando's verwerkt die een afwijkende MMI hebben of die uitsluitend mogen worden uitgevoerd
          // als ze via Serial of ethernet wrden verzonden. (i.v.m. veiligheid)
          
          case CMD_UNIT:
            S.Unit=Par1;
            if(Par1>1)
               {
               S.WaitFreeRF_Delay=3 + Par1*3;
               S.WaitFreeRF_Window=3; // 1 eenheid = 100 ms.
               }
            else
               {
               S.WaitFreeRF_Delay=0;
               S.WaitFreeRF_Window=0;
               }
            SaveSettings();
            FactoryEventlist();
            Reset();
        
          case CMD_FILE_LOG:
            if(GetArgv(Command,TmpStr,2))
              {
              strcat(TmpStr,".dat");
              strcpy(TempLogFile,TmpStr);
              }
            else
              TempLogFile[0]=0;
            break;
          
          case CMD_FILE_ERASE:      
            if(GetArgv(Command,TmpStr,2))
              {
              strcat(TmpStr,".dat");

              // SDCard en de W5100 kunnen niet gelijktijdig werken. Selecteer SDCard chip
              digitalWrite(Ethernetshield_CS_W5100, HIGH);
              digitalWrite(EthernetShield_CS_SDCard,LOW);
              SD.remove(TmpStr);
              // SDCard en de W5100 kunnen niet gelijktijdig werken. Selecteer W5100 chip
              digitalWrite(EthernetShield_CS_SDCard,HIGH);
              digitalWrite(Ethernetshield_CS_W5100, LOW);
              }
            break;
    
          case CMD_IP_SETTINGS:
            PrintIPSettings();
            break;

          case CMD_FILE_GET_HTTP:
            {
            if(GetArgv(Command,TempString,2))
              {
              strcpy(TmpStr,"&file=");
              strcat(TmpStr,TempString);
              xSendHTTPRequestStr(TmpStr);
              }
            else
              Error=ERROR_02;            
            break; 
            }
            
          case CMD_FILE_SHOW:
            {
            if(GetArgv(Command,TmpStr,2))
              {
              PrintTerminal(ProgmemString(Text_22));
              strcat(TmpStr,".dat");
              // SDCard en de W5100 kunnen niet gelijktijdig werken. Selecteer SDCard chip
              digitalWrite(Ethernetshield_CS_W5100, HIGH);
              digitalWrite(EthernetShield_CS_SDCard,LOW);
              File dataFile=SD.open(TmpStr);
              if(dataFile) 
                {
                y=0;       
                while(dataFile.available())
                  {
                  x=dataFile.read();
                  if(isprint(x) && y<INPUT_BUFFER_SIZE)
                    {
                    TmpStr[y++]=x;
                    }
                  else
                    {
                    TmpStr[y]=0;
                    y=0;
                    PrintTerminal(TmpStr);
                    }
                  }
                dataFile.close();
                }  
              else 
                TransmitCode(command2event(CMD_ERROR,ERROR_03,0),VALUE_ALL);
      
              // SDCard en de W5100 kunnen niet gelijktijdig werken. Selecteer W5100 chip
              digitalWrite(Ethernetshield_CS_W5100, LOW);
              digitalWrite(EthernetShield_CS_SDCard,HIGH);
              PrintTerminal(ProgmemString(Text_22));
              }
            break;
            }
    
          case CMD_FILE_EXECUTE:
            {
            if(GetArgv(Command,TmpStr,2))
              {
              strcat(TmpStr,".dat");

              // zet (eventueel) de extra logging aan
              GetArgv(Command,TempString,3);
              strcat(TempString,".dat");
              TempString[14]=0; // voor het geval de gebruiker een te lange naam heeft ingegeven
              strcpy(TempLogFile,TempString);

              // SDCard en de W5100 kunnen niet gelijktijdig werken. Selecteer SDCard chip
              digitalWrite(Ethernetshield_CS_W5100, HIGH);
              digitalWrite(EthernetShield_CS_SDCard,LOW);
              File dataFile=SD.open(TmpStr);
              if(dataFile) 
                {
                y=0;       
                while(dataFile.available())
                  {
                  x=dataFile.read();
                  if(isprint(x) && y<INPUT_BUFFER_SIZE)
                    TmpStr[y++]=x;
                  else
                    {
                    TmpStr[y]=0;
                    y=0;
                    //Serial.print("*** debug: Regel uit file voor execute=");Serial.println(TmpStr);//??? Debug
                    digitalWrite(EthernetShield_CS_SDCard,HIGH);
                    digitalWrite(Ethernetshield_CS_W5100, LOW);
                    ExecuteLine(TmpStr,VALUE_SOURCE_FILE);
                    digitalWrite(EthernetShield_CS_SDCard,LOW);
                    digitalWrite(Ethernetshield_CS_W5100, HIGH);
                    }
                  }
                dataFile.close();
                }  
              else 
                TransmitCode(command2event(CMD_ERROR,ERROR_03,0),VALUE_ALL);
      
              // SDCard en de W5100 kunnen niet gelijktijdig werken. Selecteer W5100 chip
              digitalWrite(EthernetShield_CS_SDCard,HIGH);
              digitalWrite(Ethernetshield_CS_W5100, LOW);
              }
            TempLogFile[0]=0;
            break;
            }
    
          case CMD_EVENTGHOST_SERVER:
            if(Par1==VALUE_AUTO)
              { 
              if(Par2==VALUE_ON)
                S.AutoSaveEventGhostIP=VALUE_AUTO;  // Automatisch IP adres opslaan na ontvangst van een EG event of niet.
              else
                S.AutoSaveEventGhostIP=0;
              }
            else
              {
              if(GetArgv(Command,TmpStr,2))
                if(!str2ip(TmpStr,S.EventGhostServer_IP))
                  Error=ERROR_02;
              }
            SaveSettings();
            break;
            
          case CMD_NODO_IP:
            if(GetArgv(Command,TmpStr,2))
              if(!str2ip(TmpStr,S.Nodo_IP))
                Error=ERROR_02;
            SaveSettings();
            break;
            
          case CMD_EVENTLIST_WRITE:
            EventlistWriteLine=Par1;
            State_EventlistWrite=1;
            break;
            
          case CMD_SEND_KAKU_NEW:
          case CMD_KAKU_NEW:
            {
            if(GetArgv(Command,TmpStr,2))
              v=str2int(TmpStr);
              
            if(v>255)
              {
              v=(v&0x0fffffff) | (((unsigned long)SIGNAL_TYPE_NEWKAKU)<<28); //  // Niet Par1 want NewKAKU kan als enige op de Par1 plaats een 28-bit waarde hebben. Hoogste nible wissen en weer vullen met type NewKAKU        
              v|=(Par2==VALUE_ON)<<4; // Stop on/off commando op juiste plek in NewKAKU code
              }
            else
              {
              v=command2event(Cmd,v,Par2);      // verwerk binnengekomen event.
              Error=CommandError(v);
              }
            break;
            }

          case CMD_WIRED_THRESHOLD:
            if(GetArgv(Command,TmpStr,3))
              {
              S.WiredInputThreshold[Par1-1]=str2AnalogInt(TmpStr);
              SaveSettings();
              // Serial.print("*** debug: Threshold ingesteld op ");Serial.println(AnalogInt2str(S.WiredInputThreshold[Par1-1]));//??? Debug
              }
            break;                  
            
          case CMD_WIRED_SMITTTRIGGER:
            if(GetArgv(Command,TmpStr,3))
              {
              S.WiredInputSmittTrigger[Par1-1]=str2AnalogInt(TmpStr);
              SaveSettings();
              // Serial.print("*** debug: SmittTrigger ingesteld op ");Serial.println(AnalogInt2str(S.WiredInputSmittTrigger[Par1-1]));//??? Debug
              }
            break;            
                  
          case CMD_WIRED_ANALOG_CALIBRATE:
            {
            int t;              
            t=analogRead(PIN_WIRED_IN_1+Par1-1);
            if(GetArgv(Command,TmpStr,4))
              {
              if(Par2==VALUE_HIGH)
                {
                S.WiredInput_Calibration_IH[Par1-1]=t;
                S.WiredInput_Calibration_OH[Par1-1]=str2AnalogInt(TmpStr);
                //??? Serial.print("*** debug: Calibratie voltooid. IH=");Serial.print(t,DEC);Serial.print(", OH=");Serial.println(AnalogInt2str(S.WiredInput_Calibration_OH[Par1-1]));//??? Debug
                }
              if(Par2==VALUE_LOW)
                {
                S.WiredInput_Calibration_IL[Par1-1]=t;
                S.WiredInput_Calibration_OL[Par1-1]=str2AnalogInt(TmpStr);
                //??? Serial.print("*** debug: Calibratie voltooid. IL=");Serial.print(t,DEC);Serial.print(", OL=");Serial.println(AnalogInt2str(S.WiredInput_Calibration_OL[Par1-1]));//??? Debug
                }
              }
            v=0;
            SaveSettings();
            break;
            }
            
          case CMD_RESET:
            ResetFactory();
              
          case CMD_RAWSIGNAL_SAVE:
            PrintTerminal(ProgmemString(Text_07));
            GetArgv(Command,TmpStr,2);
            RawSignal.Key=str2int(TmpStr);
            break;        

          case CMD_EVENTLIST_SHOW:
            PrintTerminal(ProgmemString(Text_22));
            if(Par1==VALUE_ALL || Par1==0)
              {
              for(x=1;x<=EVENTLIST_MAX;x++)
                {
                if(EventlistEntry2str(x,0,TmpStr, false))
                  PrintTerminal(TmpStr);
                }
              }
            else
              {
              EventlistEntry2str(Par1,0,TmpStr, false);
              PrintTerminal(TmpStr);
              }
            PrintTerminal(ProgmemString(Text_22));
            break;

          case CMD_EVENTLIST_FILE:
            if(GetArgv(Command,TmpStr,2))
              {
              strcat(TmpStr,".dat");
              if(!SaveEventlistSDCard(TmpStr))
                TransmitCode(command2event(CMD_ERROR,ERROR_03,0),VALUE_ALL);
              }
            break;

          case CMD_EVENTLIST_ERASE:
            if(Par1==VALUE_ALL || Par1==0)
              {
              for(x=1;x<=EVENTLIST_MAX;x++)
                Eventlist_Write(x,0L,0L);
              }
            else
                Eventlist_Write(Par1,0L,0L);

            break;        

          case CMD_FILE_LIST:
            if(!FileList())
              Error=ERROR_03;
            break;

          case CMD_FILE_WRITE:
            if(GetArgv(Command,TmpStr,2))
              {
              strcat(TmpStr,".dat");
              strcpy(TempLogFile,TmpStr);
              FileWriteMode=60;
              Serial.print("*** FileWrite: Filename=");Serial.println(TempLogFile); //??? Debug
              }
            else
              Error=ERROR_02;
            break;

          case CMD_HTTP_REQUEST:
            // zoek in de regel waar de string met het http request begint.
            x=StringFind(Line,cmd2str(CMD_HTTP_REQUEST))+strlen(cmd2str(CMD_HTTP_REQUEST));
            while(Line[x]==32)x++;
            strcpy(S.HTTPRequest,&Line[0]+x);
            SaveSettings(); 
            break;

          case CMD_SEND:
            // zoek in de regel waar de string met het http request begint.
            x=StringFind(Line,cmd2str(CMD_SEND))+strlen(cmd2str(CMD_SEND));
            while(Line[x]==32)x++;
            SendStringRF(&Line[0]+x);
            break;

          case CMD_PASSWORD:
            {
            if(GetArgv(Command,S.Password,2))
              SaveSettings();
            break;
            }  

          case CMD_ID:
            {
            if(GetArgv(Command,S.ID,2))
              SaveSettings();
            break;
            }  

          case CMD_PORT:
            {
            if(GetArgv(Command,TmpStr,2))
              {
              S.Port=str2int(TmpStr);
              SaveSettings();
              }
            break;
            }  

          case CMD_SEND_KAKU:
          case CMD_KAKU:
            {
            if(GetArgv(Command,TmpStr,2))
              {
              byte z=0;
              Par1=HA2address(TmpStr,&z); // Parameter-1 bevat [A1..P16]. Omzetten naar absolute waarde. z=groep commando
              if(GetArgv(Command,TmpStr,3))
                {
                Par2=(str2int(TmpStr)==VALUE_ON) | (z<<1); // Parameter-2 bevat [On,Off]. Omzetten naar 1,0. tevens op bit-2 het groepcommando zetten.
                v=command2event(Cmd,Par1,Par2);
                Error=CommandError(v);
                }
              }
            break;
            }
            
          case CMD_BREAK_ON_VAR_EQU:
          case CMD_BREAK_ON_VAR_LESS:
          case CMD_BREAK_ON_VAR_MORE:
          case CMD_BREAK_ON_VAR_NEQU:
          case CMD_VARIABLE_DEC:
          case CMD_VARIABLE_SET:
          case CMD_VARIABLE_INC:
            if(GetArgv(Command,TmpStr,3));        
              v=AnalogInt2event(str2AnalogInt(TmpStr), Par1, Cmd);      // verwerk binnengekomen event.            
            break;
            
          default:
            {              
            //Serial.print("*** debug: Cmd =");Serial.println(Cmd,DEC);//??? Debug
            //Serial.print("*** debug: Par1=");Serial.println(Par1,DEC);//??? Debug
            //Serial.print("*** debug: Par2=");Serial.println(Par2,DEC);//??? Debug

            // standaard commando volgens gewone syntax
            v=command2event(Cmd,Par1,Par2);
            Error=CommandError(v);
            }
          }
        }

      if(v && Error==0)
        {
        if(State_EventlistWrite==0)// Gewoon uitvoeren
          {
          // Serial.print("*** debug: ProcesEvent(); Event=");Serial.println(v,HEX);//??? Debug
          ProcessEvent(v,VALUE_DIRECTION_INPUT,Port,0,0);      // verwerk binnengekomen event.
          continue;
          }
  
        if(State_EventlistWrite==2)
          {
          action=v;
          if(!Eventlist_Write(EventlistWriteLine,event,action))
            {
            RaiseError(ERROR_06);    
            return;
            }
          State_EventlistWrite=0;
          continue;
          }
  
        if(State_EventlistWrite=1)
          {
          event=v;
          State_EventlistWrite=2;
          }
        }

      if(Error) // er heeft zich een fout voorgedaan
        {
        strcpy(TmpStr,Command);
        strcat(TmpStr, " ?");
        PrintTerminal(TmpStr);
        RaiseError(Error);
        Line[0]=0;
        }          
      // Serial.println("*** debug: Einde behandeling commando;");
      }

    // printbare tekens toevoegen aan commando zolang er nog ruimte is in de string
    if(isprint(x) && x!=';' && PosCommand<MaxCommandLength)
      Command[PosCommand++]=x;      
    }
  }
  
  