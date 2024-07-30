#include <SPI.h>
#include <Ethernet.h>
#include <EthernetICMP.h>

// MAC address for Ethernet shield
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Static IP address for the Ethernet shield
byte ip[] = {192, 168, 0, 8};

// IP addresses to ping
//change the ip's with the desired ip you want to ping (currently using 2, edit it if you want to use 1)
IPAddress pingAddr1(8, 8, 8, 8);
IPAddress pingAddr2(8, 8, 8, 8);

SOCKET pingSocket = 0;
EthernetICMPPing ping(pingSocket, (uint16_t)random(0, 255));
char buffer[256];

//relay pingAddr1
const int relay1Pin1 = 7; 
const int relay1Pin2 = 6;
const int relay1Pin3 = 5;

//relay pingAddr2
const int relay2Pin1 = 8; 
const int relay2Pin2 = 9; 
const int relay2Pin3 = 10; 

int failCount1 = 0;
int failCount2 = 0;

void setup() {
  // Initialize relay pins
  //pingAddr1
  pinMode(relay1Pin1, OUTPUT);
  pinMode(relay1Pin2, OUTPUT);
  pinMode(relay1Pin3, OUTPUT);
  //pingAddr2
  pinMode(relay2Pin1, OUTPUT);
  pinMode(relay2Pin2, OUTPUT);
  pinMode(relay2Pin3, OUTPUT);

  // Start Ethernet
  Ethernet.begin(mac, ip);
  Serial.begin(115200);
  Serial.println("Ethernet started");
}

void loop() {
  //pingAddr1
  Serial.print("Pinging ");
  Serial.print(pingAddr1);
  Serial.print("...");

  EthernetICMPEchoReply echoReply1 = ping(pingAddr1, 4);
  if (echoReply1.status == SUCCESS) {
    sprintf(buffer,
            "Reply[%d] from: %d.%d.%d.%d: bytes=%d time=%ldms TTL=%d",
            echoReply1.data.seq,
            echoReply1.addr[0],
            echoReply1.addr[1],
            echoReply1.addr[2],
            echoReply1.addr[3],
            REQ_DATASIZE,
            millis() - echoReply1.data.time,
            echoReply1.ttl);
    failCount1 = 0; // Reset fail count on success
  } else {
    sprintf(buffer, "Echo request failed; %d", echoReply1.status);
    failCount1++;
  }
  Serial.println(buffer);

  if (failCount1 >= 3) {
    //pingAddr1
    digitalWrite(relay1Pin1, HIGH);
    digitalWrite(relay1Pin2, HIGH);
    digitalWrite(relay1Pin3, HIGH); // Activate relay on fail
    Serial.println("Relay 1 activated");
  } else {
    digitalWrite(relay1Pin1, LOW);
    digitalWrite(relay1Pin2, LOW);
    digitalWrite(relay1Pin3, LOW); // Deactivate relay if not failing
  }

  // Wait 5 seconds between pings
  delay(5000);

  //pingAddr2
  Serial.print("Pinging ");
  Serial.print(pingAddr2);
  Serial.print("...");

  EthernetICMPEchoReply echoReply2 = ping(pingAddr2, 4);
  if (echoReply2.status == SUCCESS) {
    sprintf(buffer,
            "Reply[%d] from: %d.%d.%d.%d: bytes=%d time=%ldms TTL=%d",
            echoReply2.data.seq,
            echoReply2.addr[0],
            echoReply2.addr[1],
            echoReply2.addr[2],
            echoReply2.addr[3],
            REQ_DATASIZE,
            millis() - echoReply2.data.time,
            echoReply2.ttl);
    failCount2 = 0; // Reset fail count on success
  } else {
    sprintf(buffer, "Echo request failed; %d", echoReply2.status);
    failCount2++;
  }
  Serial.println(buffer);

  if (failCount2 >= 3) {
    digitalWrite(relay2Pin1, HIGH);
    digitalWrite(relay2Pin2, HIGH);
    digitalWrite(relay2Pin3, HIGH); // Activate relay on fail
    Serial.println("Relay 2 activated");
  } else {
    digitalWrite(relay2Pin1, LOW);
    digitalWrite(relay2Pin2, LOW);
    digitalWrite(relay2Pin3, LOW); // Deactivate relay if not failing
  }

  // Wait 30 seconds before pinging again
  delay(30000);
}
