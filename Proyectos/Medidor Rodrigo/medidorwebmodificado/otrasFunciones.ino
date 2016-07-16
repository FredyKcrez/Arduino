/*
	Funciones que no han sido creadas por mi
	pero son dependencia de de las otras funciones
*/

// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;
}


byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;

  while(!mailCliente.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      mailCliente.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  respCode = mailCliente.peek();

  while(mailCliente.available())
  {  
    thisByte = mailCliente.read();    
    Serial.write(thisByte);
  }

  if(respCode >= '4')
  {
    efail();
    return 0;  
  }

  return 1;
}


void efail()
{
  byte thisByte = 0;
  int loopCount = 0;

  mailCliente.println(F("QUIT"));

  while(!mailCliente.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      mailCliente.stop();
      Serial.println(F("\r\nTimeout"));
      return;
    }
  }

  while(mailCliente.available())
  {  
    thisByte = mailCliente.read();    
    Serial.write(thisByte);
  }

  mailCliente.stop();

  Serial.println(F("disconnected"));
}
