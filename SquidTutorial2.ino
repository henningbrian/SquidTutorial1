//  Send sample SIGFOX messages and wait for downlink with UnaBiz UnaShield V2S Arduino Shield.
//  This sketch includes diagnostics functions in the UnaShield.
//  For a simpler sample sketch, see examples/send-light-level.
#include "SIGFOX.h"

//  IMPORTANT: Check these settings with UnaBiz to use the SIGFOX library correctly.
static const String device = "g88pi";  //  Set this to your device name if you're using UnaBiz Emulator.
static const bool useEmulator = false;  //  Set to true if using UnaBiz Emulator.
static const bool echo = true;  //  Set to true if the SIGFOX library should display the executed commands.
static const Country country = COUNTRY_SA;  //  Set this to your country to configure the SIGFOX transmission frequencies.
static UnaShieldV2S transceiver(country, useEmulator, device, echo);  //  Downlink supported only for UnaShield V2S.
static String response;  //  Will store the downlink response from SIGFOX.

void setup() {  //  Will be called only once.
  //  Initialize console so we can see debug messages (9600 bits per second).
  Serial.begin(9600);  Serial.println(F("Running setup..."));  
  //  Check whether the SIGFOX module is functioning.
  if (!transceiver.begin()) stop(F("Unable to init SIGFOX module, may be missing"));  //  Will never return.

  //  Send a raw 12-byte message payload to SIGFOX.  In the loop() function we will use the Message class, which sends structured messages.
  //  If you don't wish to wait for downlink response from SIGFOX, call sendMessage() instead.
  //  Warning: This may delay for about a minute.
 // transceiver.sendMessageAndGetResponse("0102030405060708090a0b0c", response);
 // Serial.print(F("Downlink response is "));  Serial.println(response);
  
  //  Delay 10 seconds before sending next message.
 // Serial.println(F("Waiting 10 seconds..."));
 // delay(10000);
}

void loop() {  //  Will be called repeatedly.
  //  Send message counter, temperature and voltage as a structured SIGFOX message, up to 10 times.
  static int counter = 0, successCount = 0, failCount = 0;  //  Count messages sent and failed.
  Serial.print(F("\nRunning loop #")); Serial.println(counter);

  //  Get temperature and voltage of the SIGFOX module.
  float temperature;  float voltage;
  transceiver.getTemperature(temperature);
  transceiver.getVoltage(voltage);

  //  Convert the numeric counter, temperature and voltage into a compact message with binary fields.
  Message msg(transceiver);  //  Will contain the structured sensor data.
  msg.addField("ctr", counter);  //  4 bytes for the counter.
  msg.addField("tmp", temperature);  //  4 bytes for the temperature.
  msg.addField("vlt", voltage);  //  4 bytes for the voltage.
  //  Total 12 bytes out of 12 bytes used.

  //  Send the message. If you don't wish to wait for downlink response from SIGFOX, call send() instead.
  //  Warning: This may delay for about a minute.
  if (msg.sendAndGetResponse(response)) {
    Serial.print(F("Downlink response is "));  Serial.println(response);    
    successCount++;  //  If successful, count the message sent successfully.
  } else {
    failCount++;  //  If failed, count the message that could not be sent.
  }
  counter++;

  //  Send only 2 messages.
  if (counter >= 2) {
    //  If more than 10 times, display the results and hang here forever.
    stop(String(F("Messages sent successfully: ")) + successCount +
                   F(", failed: ") + failCount);  //  Will never return.
  }

  //  Delay 10 seconds before sending next message.
  Serial.println("Waiting 10 seconds...");
  delay(10000);
}

/* Expected output for UnaShield V2S:
Running setup...
 - Disabling emulation mode...
 - Checking emulation mode (expecting 0)...
 - Getting SIGFOX ID...
 - Wisol.sendBuffer: AT$I=10
>> AT$I=10
<< 002C30EB0x0d
 - Wisol.sendBuffer: response: 002C30EB
 - Wisol.sendBuffer: AT$I=11
>> AT$I=11
<< A8664B5523B5405D0x0d
 - Wisol.sendBuffer: response: A8664B5523B5405D
 - Wisol.getID: returned id=002C30EB, pac=A8664B5523B5405D
 - SIGFOX ID = 002C30EB
 - PAC = A8664B5523B5405D
 - Wisol.setFrequencySG
 - Set frequency result = OK
 - Getting frequency (expecting 3)...
 - Frequency (expecting 3) = 52
 - Wisol.sendMessageAndGetResponse: 002C30EB,0102030405060708090a0b0c
 - Wisol.sendBuffer: AT$GI?
>> AT$GI?
<< 1,5
 - Wisol.sendBuffer: response: 1,5
 - Wisol.sendBuffer: AT$SF=0102030405060708090a0b0c,1
>> AT$SF=0102030405060708090a0b0c,1
<< OK0x0d
RX=01 23 45 67 89 AB CD EF
 - Wisol.sendBuffer: response: OK
RX=01 23 45 67 89 AB CD EF
OK
RX=01 23 45 67 89 AB CD EF
Downlink response is 0123456789ABCDEF
Waiting 10 seconds...
Running loop #0
 - Wisol.sendBuffer: AT$T?
>> AT$T?
<< 287
 - Wisol.sendBuffer: response: 287
 - Wisol.getTemperature: returned 28.70
 - Wisol.sendBuffer: AT$V?
>> AT$V?
<< 33500x0d
 - Wisol.sendBuffer: response: 3350
 - Wisol.getVoltage: returned 3.35
 - Message.addField: ctr=0
 - Message.addField: tmp=287.7
 - Message.addField: vlt=33.3
 - Wisol.sendMessageAndGetResponse: 002C30EB,920e0000b0511f0194592100
Warning: Should wait 10 mins before sending the next message
 - Wisol.sendBuffer: AT$GI?
>> AT$GI?
<< 1,5
 - Wisol.sendBuffer: response: 1,5
 - Wisol.sendBuffer: AT$SF=920e0000b0511f0194592100,1
>> AT$SF=920e0000b0511f0194592100,1
<< OK0x0d
RX=01 23 45 67 89 AB CD EF
 - Wisol.sendBuffer: response: OK
RX=01 23 45 67 89 AB CD EF
OK
RX=01 23 45 67 89 AB CD EF
Downlink response is 0123456789ABCDEF
Waiting 10 seconds...
Running loop #1
 - Wisol.sendBuffer: AT$T?
>> AT$T?
<< 290
 - Wisol.sendBuffer: response: 290
 - Wisol.getTemperature: returned 29.00
 - Wisol.sendBuffer: AT$V?
>> AT$V?
<< 33500x0d
 - Wisol.sendBuffer: response: 3350
 - Wisol.getVoltage: returned 3.35
 - Message.addField: ctr=1
 - Message.addField: tmp=290.0
 - Message.addField: vlt=33.3
 - Wisol.sendMessageAndGetResponse: 002C30EB,920e0a00b051220194592100
Warning: Should wait 10 mins before sending the next message
 - Wisol.sendBuffer: AT$GI?
>> AT$GI?
<< 1,5
 - Wisol.sendBuffer: response: 1,5
 - Wisol.sendBuffer: AT$SF=920e0a00b051220194592100,1
*/
