//  Exemplo TTGO Tbeam
//  Comunicação LoRa entre placas, envio de uma estrutura e confirmação


//  Bibliotecas usadas (includes)

#include <SPI.h>
#include <LoRa.h>

// LoRa defines

#define SCK   5     //  GPIO5  -- SX1278's SCK
#define MISO  19    //  GPIO19 -- SX1278's Master In Slave Out (MISO)
#define MOSI  27    //  GPIO27 -- SX1278's Master Out Slave In (MOSI)
#define SS    18    //  GPIO18 -- SX1278's CS (Chip Select)
#define RST   14    //  GPIO14 -- SX1278's RESET (Reset Pin)
#define DI0   26    //  GPIO5  -- SX1278's IRQ (Interrupt Request)
#define BAND 915E6  //  RF     -- SX1278's Radio frequency

//  Variaveis globais


void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println();
  Serial.println("LoRa Node test");

  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed");
    while (1);
  }
  LoRa.onReceive(rcv);
  LoRa.onTxDone(txDone);
  LoRa_rxMode();
  //delay(1500);
}

void loop() 
{
    if (runEvery(1000)) { // repeat every 5000 millis

    String message = "HeLoRa World! ";
    message += "I'm a Node! ";
    message += millis();

    LoRa_sendMessage(message); // send a message

    Serial.println("Send Message!");
  }
}

void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void rcv(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }

  Serial.print("Node Receive: ");
  Serial.println(message);
}

void txDone() {
  Serial.println("TxDone");
  LoRa_rxMode();
}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
