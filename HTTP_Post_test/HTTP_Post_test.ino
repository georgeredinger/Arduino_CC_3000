/*************************************************** 
 * This is an example for the Adafruit CC3000 Wifi Breakout & Shield
 * 
 * Designed specifically to work with the Adafruit WiFi products:
 * ----> https://www.adafruit.com/products/1469
 * 
 * Adafruit invests time and resources providing this open source code, 
 * please support Adafruit and open-source hardware by purchasing 
 * products from Adafruit!
 * 
 * Written by Limor Fried & Kevin Townsend for Adafruit Industries.  
 * BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

// These are the interrupt and control pins/
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
SPI_CLOCK_DIV2); // you can change this clock speed

#define WLAN_SSID       "OfficeSandpoint" //cannot be longer than 32 characters!
#define WLAN_PASS       "506office1419"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

// What page to grab!
#define WEBSITE      "/"



#define WEBPAGE "/collect"

/***********************************************************************
 ***/
/*!
 @brief  Sets up the HW and the CC3000 module (called automatically
 on startup)
 */
/**************************************************************************/

uint32_t ip;

void setup(void)
{
  int i = 0;
  char count[5];
  Serial.begin(115200);
  Serial.println(F("Hello, CC3000!\n")); 

  displayFreeRam();

  /* Initialise the module */
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }

  // Optional SSID scan
  // listSSIDResults();

  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);

  Serial.println(F("Connected!"));

  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  /* Display the IP address DNS, Gateway, etc. */
  while (! displayConnectionDetails()) {
    delay(1000);
  }

  ip = 0;
  // Try looking up the website's IP address
  Serial.print(WEBSITE); 
  Serial.print(F(" -> "));
  //while (ip == 0) {
  //  if (! cc3000.getHostByName(WEBSITE, &ip)) {
  //    Serial.println(F("Couldn't resolve!"));
  //  }

  //  delay(500);
  // }

  cc3000.printIPdotsRev(ip);


  // Optional: Do a ping test on the website
  /*
  Serial.print(F("\n\rPinging ")); cc3000.printIPdotsRev(ip); Serial.print("...");  
   replies = cc3000.ping(ip, 5);
   Serial.print(replies); Serial.println(F(" replies"));
   */
  // 192.168.1.100  
  ip = 3232235876UL; 
http://www.webtoolhub.com/tn561377-ip-address-number-converter.aspx
  //10.10.50.94
  //curl --data "param1=value1&param2=value2" http://echo.200please.com
  for(;;){
    ip = 168440414;
    Serial.println(ip);
    /* Try connecting to the website */
    itoa(i++,count,10);

    //char *json = "{\"some\":\"json\"}";
    char json[20];
    sprintf(json,"{\"count\":%d}",i++);
    char len_buf[5];
    itoa(strlen(json),len_buf,10);
    Adafruit_CC3000_Client www = cc3000.connectTCP(ip, 3000);
    if (www.connected()) {
      www.print(F("POST "));
      www.print(WEBPAGE);
      www.println(" HTTP/1.1");
      www.print(F("HOST: "));
      www.println(WEBPAGE);
      www.println(F("From: george@georgeredinger.com"));
      www.println(F("User-Agent: CC3000"));
      www.println(F("Connection: close"));
      www.println(F("Content-Type: application/x-www-form-urlencoded"));
      www.print(F("Content-Length: "));
      www.println(len_buf);
      www.println();
      www.println(json);
    } 
    else {
      Serial.println(F("Connection failed"));    
    }
   Serial.println("disconnecting socket");
    cc3000.disconnect();
    delay(1000);
  }  
}

void loop(void)
{
  delay(1000);
}

/**************************************************************************/
/*!
 @brief  Begins an SSID scan and prints out all the visible networks
 */
/**************************************************************************/

void listSSIDResults(void)
{
  uint8_t valid, rssi, sec, index;
  char ssidname[33]; 

  index = cc3000.startSSIDscan();

  Serial.print(F("Networks found: ")); 
  Serial.println(index);
  Serial.println(F("================================================"));

  while (index) {
    index--;

    valid = cc3000.getNextSSID(&rssi, &sec, ssidname);

    Serial.print(F("SSID Name    : ")); 
    Serial.print(ssidname);
    Serial.println();
    Serial.print(F("RSSI         : "));
    Serial.println(rssi);
    Serial.print(F("Security Mode: "));
    Serial.println(sec);
    Serial.println();
  }
  Serial.println(F("================================================"));

  cc3000.stopSSIDscan();
}

/**************************************************************************/
/*!
 @brief  Tries to read the IP address and other connection details
 */
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); 
    cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); 
    cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); 
    cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); 
    cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); 
    cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

