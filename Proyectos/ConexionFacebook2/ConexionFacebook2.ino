#include <Bridge.h>
#include <Temboo.h>
/*
IMPORTANT NOTE about TembooAccount.h

TembooAccount.h contains your Temboo account information and must be included
alongside your sketch. To do so, make a new tab in Arduino, call it TembooAccount.h,
and copy this content into it. 
*/

#define TEMBOO_ACCOUNT "zholary"  // Your Temboo account name 
#define TEMBOO_APP_KEY_NAME "myFirstApp"  // Your Temboo app key name
#define TEMBOO_APP_KEY "PXuekkzqDLIpYEfTuArvFcxfX1p6yEqW"  // Your Temboo app key

/* 
The same TembooAccount.h file settings can be used for all Temboo SDK sketches.  
Keeping your account information in a separate file means you can share the 
main .ino file without worrying that you forgot to delete your credentials.
*/
int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed

void setup() {
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);
  Bridge.begin();
}

void loop() {
  if (numRuns <= maxRuns) {
    Serial.println("Running InitializeOAuth - Run #" + String(numRuns++));
    
    TembooChoreo InitializeOAuthChoreo;

    // Invoke the Temboo client
    InitializeOAuthChoreo.begin();

    // Set Temboo account credentials
    InitializeOAuthChoreo.setAccountName(TEMBOO_ACCOUNT);
    InitializeOAuthChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    InitializeOAuthChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // Set Choreo inputs
    InitializeOAuthChoreo.addInput("AppID", "1718317955096590");
    InitializeOAuthChoreo.addInput("Scope", "publish_actions");
    
    // Identify the Choreo to run
    InitializeOAuthChoreo.setChoreo("/Library/Facebook/OAuth/InitializeOAuth");
    
    // Run the Choreo; when results are available, print them to serial
    InitializeOAuthChoreo.run();
    
    while(InitializeOAuthChoreo.available()) {
      char c = InitializeOAuthChoreo.read();
      Serial.print(c);
    }
    InitializeOAuthChoreo.close();
  }

  Serial.println("Waiting...");
  delay(30000); // wait 30 seconds between InitializeOAuth calls
}
