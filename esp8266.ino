#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <RCSwitch.h>
#include <U8g2lib.h>

#define RX_PIN D2  // GDO0 connected to GPIO4 (D2 on NodeMCU)
#define TX_PIN D8  // CSN connected to GPIO15 (D8 on NodeMCU)
#define OLED_SCL D3   // GPIO0 (D3)
#define OLED_SDA D4   // GPIO2 (D4)

#define SCREEN_WIDTH 128 // width
#define SCREEN_HEIGHT 64 // height
String inputString = ""; // A string to hold incoming data
bool stringComplete = false; // Whether the string is complete
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ OLED_SCL, /* data=*/ OLED_SDA);




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

#define WAVEFORM_SAMPLES 128
int waveform[WAVEFORM_SAMPLES] = {0};
int waveformIndex = 0;

void setup() {
  Serial.begin(115200);


  u8g2.begin();
  u8g2.enableUTF8Print();  // Permite imprimir caracteres UTF-8
  ELECHOUSE_cc1101.Init();


  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFlipMode(3); //rotaciona tela
  u8g2.setFont(u8g2_font_6x10_tf);
    
    u8g2.firstPage();
    do {
        null
    } while (u8g2.nextPage()); 


  ELECHOUSE_cc1101.setMHZ(433.92);

  ELECHOUSE_cc1101.SetRx();

  mySwitch.enableReceive(RX_PIN);
  mySwitch.enableTransmit(TX_PIN);
}


void graph(){
  int rssi = ELECHOUSE_cc1101.getRssi();
  Serial.println(rssi);
}

void loop() {


  if (Serial.available()) {
    char option = Serial.read();  // Read the option sent from Python
    


    if(option=='A') {
        while(true){    


          if (mySwitch.available()) {

            u8g2.clearBuffer();
        
            receivedValue = mySwitch.getReceivedValue();
            receivedBitLength = mySwitch.getReceivedBitlength();
            unsigned int* rawSignal = mySwitch.getReceivedRawdata();
            receivedProtocol = mySwitch.getReceivedProtocol();
            
        
            if (receivedValue != 0) {
              u8g2.clearBuffer();
              Serial.println("[*] Received rxraw: ");
              
              for (int i = 0; i < receivedBitLength; ++i) {
                Serial.print(rawSignal[i]);
                Serial.print(" ");  // Print a space between each element for readability
              }
              Serial.println();  // Print a newline at the end
              u8g2.setCursor(0, 10);
              u8g2.print("[!] Received Signal:");
              u8g2.setCursor(0, 20);
              u8g2.print(receivedValue);
              u8g2.setCursor(0, 30);
              u8g2.printf("[*] Bit:%d", receivedBitLength);
              u8g2.setCursor(0, 40);
              u8g2.printf("[*] Ptc:%d", receivedProtocol);
              u8g2.sendBuffer();
        
              mySwitch.resetAvailable();
            }
          }

        }
    }
    else if (option=='B'){
      while(true){
        graph();
      }
      
    }
  
  }

}
