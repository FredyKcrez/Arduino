#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <Temboo.h>
#define TEMBOO_ACCOUNT "zholary"  // Your Temboo account name 
#define TEMBOO_APP_KEY_NAME "myFirstApp"  // Your Temboo app key name
#define TEMBOO_APP_KEY "PXuekkzqDLIpYEfTuArvFcxfX1p6yEqW"  // Your Temboo app key

#define ETHERNET_SHIELD_MAC {0x90, 0xA2, 0xDA, 0x0D, 0xDC, 0x81}
byte ethernetMACAddress[] = ETHERNET_SHIELD_MAC;
EthernetClient client;

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed

void setup() {
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);

  Serial.print("DHCP:");
  if (Ethernet.begin(ethernetMACAddress) == 0) {
    Serial.println("FAIL");
    while(true);
  }
  Serial.println("OK");
  delay(5000);

  Serial.println("Setup complete.\n");
}

void loop() {
  if (numRuns <= maxRuns) {
    Serial.println("Running FinalizeOAuth - Run #" + String(numRuns++));

    TembooChoreo FinalizeOAuthChoreo(client);

    // Invoke the Temboo client
    FinalizeOAuthChoreo.begin();

    // Set Temboo account credentials
    FinalizeOAuthChoreo.setAccountName(TEMBOO_ACCOUNT);
    FinalizeOAuthChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    FinalizeOAuthChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    String CallbackIDValue = "zholary/81c3524d-c3c3-4ad6-8fe7-0982d8a8facb";
    FinalizeOAuthChoreo.addInput("CallbackID", CallbackIDValue);

    // Identify the Choreo to run
    FinalizeOAuthChoreo.setChoreo("/Library/Facebook/OAuth/FinalizeOAuth");

    // Run the Choreo; when results are available, print them to serial
    FinalizeOAuthChoreo.run();

    while(FinalizeOAuthChoreo.available()) {
      char c = FinalizeOAuthChoreo.read();
      Serial.print(c);
    }
    FinalizeOAuthChoreo.close();
  }

  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between FinalizeOAuth calls
}
