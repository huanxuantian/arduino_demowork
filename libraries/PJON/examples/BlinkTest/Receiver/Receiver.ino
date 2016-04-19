#include <PJON.h>

// network(Arduino pin used, selected device id)
PJON network(12, 44);

void setup() {
  pinModeFast(13, OUTPUT);
  digitalWriteFast(13, LOW);
  Serial.begin(9600);
  network.set_receiver(receiver_function);
};

void receiver_function(uint8_t length, uint8_t *payload) {
  if(payload[0] == 'B') {
    Serial.println("BLINK");
    digitalWrite(13, HIGH);
    delay(30);
    digitalWrite(13, LOW);
  }
}

void loop() {
  network.receive(1000);
};
