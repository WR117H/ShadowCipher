#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <RCSwitch.h>
#include <U8g2lib.h>
#include "graphics.h"
#include "utils.h"
// Pin definitions for the WeMos S2 Mini
#define RX_PIN D2  // GDO0 connected to GPIO4 (D2 on NodeMCU)
#define TX_PIN D8  // CSN connected to GPIO15 (D8 on NodeMCU)
#define OLED_SCL 0  // GPIO0
#define OLED_SDA 2  // GPIO2
#define BUTTON_PIN 14    // Pino do botão
#define CC1101_MOD_ASK   2
#define CC1101_MOD_2FSK  0
#define BUTTON_PIN_DIR 27    // Pino do botão
#define BUTTON_PIN_ESQ 12    // Pino do botão
int gdo0 = 5;     // GPIO 5
#define BUTTON_PIN_UP 26    // Pino do botão
#define FREQUENCY_SWITCH_PIN 13 // Pino do interruptor para mudar a frequência
#define SCREEN_WIDTH 128 // Largura da tela OLED
#define SCREEN_HEIGHT 64 // Altura da tela OLED
#define CCBUFFERSIZE 64
#define RECORDINGBUFFERSIZE 4096   // Buffer for recording the frames
#define EPROMSIZE 4096              // Size of EEPROM in your Arduino chip. For  ESP8266 size is 4096
#define BUF_LENGTH 128             // Buffer for the incoming command.
// buffer for receiving  CC1101
byte ccreceivingbuffer[CCBUFFERSIZE] = {0};
// buffer for sending  CC1101
byte ccsendingbuffer[CCBUFFERSIZE] = {0};
//char ccsendingbuffer[CCBUFFERSIZE] = {0};
// buffer for recording and replaying of many frames
byte bigrecordingbuffer[RECORDINGBUFFERSIZE] = {0};
// buffer for hex to ascii conversions 
byte textbuffer[BUF_LENGTH];
//char textbuffer[BUF_LENGTH];
String inputString = ""; // A string to hold incoming data
bool stringComplete = false; // Whether the string is complete
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ OLED_SCL, /* data=*/ OLED_SDA);
byte receivedSignal[100];  // Use byte array instead of unsigned int
int receivedLength = 0;    // Use int for length
#define WAVEFORM_SAMPLES 128
int waveform[WAVEFORM_SAMPLES] = {0};
int waveformIndex = 0;
#define scary_width 128
#define scary_height 64



RCSwitch mySwitch = RCSwitch();

unsigned long receivedValue = 0;
int receivedBitLength = 0;
int receivedProtocol = 0;
const int rssi_threshold = -75;

static const uint32_t subghz_frequency_list[] = {
    300000000, 303875000, 304250000, 310000000, 315000000, 318000000,
    390000000, 418000000, 433075000, 433420000, 433920000, 434420000, 434775000, 438900000,
    868350000, 915000000, 925000000
};

enum Modulation {
  AM270,
  AM650,
  FM238
};

Modulation currentModulation = AM650; // Default modulation

void configureModulation(Modulation mod) {
    switch (mod) {
        case AM270:
            ELECHOUSE_cc1101.setModulation(CC1101_MOD_ASK);
            ELECHOUSE_cc1101.setRxBW(270);
            Serial.println("Set to AM270");
            break;
        case AM650:
            ELECHOUSE_cc1101.setModulation(CC1101_MOD_ASK);
            ELECHOUSE_cc1101.setRxBW(650);
            Serial.println("Set to AM650");
            break;
        case FM238:
            ELECHOUSE_cc1101.setModulation(CC1101_MOD_2FSK);
            ELECHOUSE_cc1101.setRxBW(270);
            ELECHOUSE_cc1101.setDeviation(2380);
            Serial.println("Set to FM238");
            break;
    }
    ELECHOUSE_cc1101.SetRx();  // Start reception
}

void setup() {
  Serial.begin(115200);  

  u8g2.begin();
  u8g2.enableUTF8Print();  // Permite imprimir caracteres UTF-8
  ELECHOUSE_cc1101.Init();
  configureModulation(currentModulation);

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFlipMode(3); //rotaciona tela
  u8g2.setFont(u8g2_font_6x10_tf);
    
    u8g2.firstPage();
    do {
        u8g2.drawXBMP(0, 0, scary_width, scary_height, scary_bits);
    } while (u8g2.nextPage()); 


  ELECHOUSE_cc1101.setMHZ(433.92);

  ELECHOUSE_cc1101.SetRx();

  mySwitch.enableReceive(RX_PIN);
  mySwitch.enableTransmit(TX_PIN);
}



  
  
  
  

void loop() {


  if (Serial.available()) {
    char option = Serial.read();  // Read the option sent from Python
    


    if(option=='A') {
        while(true){    


           sniffing();
           yield();

        }
    }
    else if (option=='B'){

      while(true){
        scan();
      }
      
    }
    else if (option=='F'){
        graph();
    }

    else if(option=='D'){
            

       capture();

    }
  else if (option=='E'){
    
  

     replay();
  }
  else if (option=='J'){
    while (true){
      jammer();
    }
    
  }
 } 
}
void graph(){
      u8g2.clearBuffer();
      float mhz = 0;
      String string_mhz = ""; 
      for (int i = 1; i < SCREEN_WIDTH; i++) {
        int rssi = ELECHOUSE_cc1101.getRssi();
        waveform[i] = map(rssi, -100, -40, 0, 1023);
  
        if (rssi < -75) {
          if (i % 2 == 0) {
            ELECHOUSE_cc1101.setMHZ(433.92);
            ELECHOUSE_cc1101.SetRx();
            mhz = 433.92;
          } else {
            ELECHOUSE_cc1101.setMHZ(315);
            ELECHOUSE_cc1101.SetRx();
            mhz = 315.00;
          }
        } else {
          u8g2.setCursor(0, 7);
          string_mhz = String(mhz, 2) + " AM"; 
          u8g2.print(string_mhz);

          
        }
  
        int prevY = map(waveform[i - 1], 0, 1023, 64 - 13, 8);
        int currY = map(waveform[i], 0, 1023, 64 - 13, 8);
        u8g2.drawLine(i - 1, prevY, i, currY);
  
        u8g2.sendBuffer();
        delay(30);
  
        if (i == 105 && mhz != 0) {
          u8g2.clearBuffer();
          u8g2.setCursor(0, 7);
          u8g2.print(string_mhz);
          i = 1;
        }


      }
}
void sniffing(){
  if (mySwitch.available()) {
    long value = mySwitch.getReceivedValue();
 
    Serial.print("Received: ");
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("Key: 0x");
      Serial.println(value, HEX);
      Serial.print("Bit length: ");
      Serial.println(mySwitch.getReceivedBitlength());
      Serial.print("Protocol: ");
      Serial.println(mySwitch.getReceivedProtocol());
      Serial.print("Pulse length: ");
      Serial.println(mySwitch.getReceivedDelay());
    }

    Serial.print("Received: ");
    Serial.println(value);
    
    // Accessing raw timing data
    const int numPulses = mySwitch.getReceivedBitlength() * 2;
    Serial.print("Raw data: ");
    for (int i = 0; i < numPulses; i++) {
      Serial.print(mySwitch.getReceivedRawdata()[i]);
      Serial.print(" ");
    }
    Serial.println();

    mySwitch.resetAvailable();
  }

  // Modulation setting can be changed using buttons (not shown in this example)

  // Modulation setting can be changed using buttons (not shown in this example)
}
void capture(){
            // setup async mode on CC1101 with GDO0 pin processing
        ELECHOUSE_cc1101.setCCMode(0); 
        ELECHOUSE_cc1101.setPktFormat(3);
        ELECHOUSE_cc1101.SetRx();

        //start recording to the buffer with bitbanging of GDO0 pin state
        Serial.print(F("\r\nWaiting for radio signal to start RAW recording...\r\n"));
        pinMode(gdo0, INPUT);

        // this is only for ESP32 boards because they are getting some noise on the beginning

        delayMicroseconds(1000);  
        while ( digitalRead(gdo0) == LOW ) 
                {  yield(); 
                   // feed the watchdog in ESP8266
                   ESP.wdtFeed();                  
                };
                   
        // waiting for some data first or serial port signal
        // while (!Serial.available() ||  (digitalRead(gdo0) == LOW) ); 
        // feed the watchdog while waiting for the RF signal    

                   
            
        //start recording to the buffer with bitbanging of GDO0 pin state
        Serial.print(F("\r\nStarting RAW recording to the buffer...\r\n"));
        pinMode(gdo0, INPUT);

        // temporarly disable WDT for the time of recording
        // ESP.wdtDisable();
        // start recording RF signal        
        for (int i=0; i<RECORDINGBUFFERSIZE ; i++)  
           { 
             byte receivedbyte = 0;
             for(int j=7; j > -1; j--)  // 8 bits in a byte
               {
                 bitWrite(receivedbyte, j, digitalRead(gdo0)); // Capture GDO0 state into the byte
                 delayMicroseconds(100);                   // delay for selected sampling interval
               }; 
                 // store the output into recording buffer
             bigrecordingbuffer[i] = receivedbyte;
             ESP.wdtFeed();
             

           }
        Serial.print(F("\r\nRecording RAW data complete.\r\n\r\n"));
        // setting normal pkt format again
        ELECHOUSE_cc1101.setCCMode(1); 
        ELECHOUSE_cc1101.setPktFormat(0);
        ELECHOUSE_cc1101.SetRx();
        // feed the watchdog
        Serial.print(F("\r\nRecorded RAW data:\r\n"));
        for (int i = 0; i < RECORDINGBUFFERSIZE ; i = i + 32){ 
                    asciitohex(&bigrecordingbuffer[i], textbuffer,  32);
                    Serial.print((char *)textbuffer);
                    // feed the watchdog
                    ESP.wdtFeed();
                    // needed for ESP8266   
                    yield();                      
        };
       Serial.print(F("\r\n\r\n"));
       ESP.wdtFeed();        
       // needed for ESP8266
           
       yield();    
       Serial.print(F("\r\nEnd\r\n"));
}
void replay(){
        ELECHOUSE_cc1101.setCCMode(0); 
        ELECHOUSE_cc1101.setPktFormat(3);
        ELECHOUSE_cc1101.SetTx();
        //start replaying GDO0 bit state from data in the buffer with bitbanging 
        Serial.print(F("\r\nReplaying RAW data from the buffer...\r\n"));
        pinMode(gdo0, OUTPUT);

        // temporarly disable WDT for the time of recording
        // ESP.wdtDisable();
        // start RF replay
        for (int i=1; i<RECORDINGBUFFERSIZE ; i++)  
           { 
             byte receivedbyte = bigrecordingbuffer[i];
             for(int j=7; j > -1; j--)  // 8 bits in a byte
               {
                 digitalWrite(gdo0, bitRead(receivedbyte, j)); // Set GDO0 according to recorded byte
                 delayMicroseconds(100);                      // delay for selected sampling interval
               }; 
              // feed the watchdog
              ESP.wdtFeed();
           };
        // Enable WDT 
        // ESP.wdtEnable(5000);
           
        Serial.print(F("\r\nReplaying RAW data complete.\r\n\r\n"));
        // setting normal pkt format again
        ELECHOUSE_cc1101.setCCMode(1); 
        ELECHOUSE_cc1101.setPktFormat(0);
        ELECHOUSE_cc1101.SetTx();
        // pinMode(gdo0pin, INPUT);
        // feed the watchdog
        ESP.wdtFeed();        
        // needed for ESP8266   
        yield();      
}
void scan(){
  int rssi = ELECHOUSE_cc1101.getRssi();
  Serial.println(rssi);
}
void jammer(){
  randomSeed(analogRead(0));
  
  byte ccsendingbuffer[60];  // Ensure this buffer is declared appropriately

  for (int i = 0; i < 60; i++) {
    ccsendingbuffer[i] = (byte)random(255); 
    // feed the watchdog
    ESP.wdtFeed();
    // needed for ESP8266   
    yield();                   
  };        

  // send these data to radio over CC1101
  ELECHOUSE_cc1101.SendData(ccsendingbuffer, 60);

  // feed the watchdog
  ESP.wdtFeed();
  // needed for ESP8266   
  yield();               
}
