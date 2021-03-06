/***************************** WiFiClient helper ******************************
This file is part of the Ewings Esp8266 Stack.

This is free software. you can redistribute it and/or modify it but without any
warranty.

Author          : Suraj I.
created Date    : 1st June 2019
******************************************************************************/

#include "WiFiClientHelper.h"

/*   WiFi client support functions */

bool connectToServer( WiFiClient* client, char*host, uint16_t port, uint16_t timeout ){

  #ifdef EW_SERIAL_LOG
  Log(F("Client: Connecting to "));Log(host);Log(F(" : "));Logln(port);
  #endif
  client->setTimeout(timeout);
  // client->keepAlive();
  int result = client->connect( host, port);
  #ifdef EW_SERIAL_LOG
  Log(F("Client: Connect result -: ")); Logln(result);
  #endif
  return result != 0;
}

bool isConnected( WiFiClient* client ) {

  return client ? client->connected() : false;
}

bool disconnect( WiFiClient* client ) {

  return isConnected(client) ? client->stop(500) : true;
}

bool sendPacket( WiFiClient* client, uint8_t *buffer, uint16_t len ) {

  uint16_t sentBytes = 0;
  uint16_t _buf_len = strlen((char*)buffer);
  len =  _buf_len < len ? _buf_len : len;

  while (len > 0) {

    #ifdef EW_SERIAL_LOG
    Log(F("Client: Sending to "));Log(client->remoteIP());Log(F(" : "));Logln(client->remotePort());
    #endif
    if ( isConnected(client) ) {
      // send 250 bytes at most at a time, can adjust this later based on Client

      uint16_t sendlen = len > 250 ? 250 : len;
      uint8_t* _buff_pointer = buffer+sentBytes;
      uint16_t _sent = client->write( _buff_pointer, sendlen);
      sentBytes += _sent;
      len -= _sent;

      #ifdef EW_SERIAL_LOG
      Log(F("Client: sending packets : "));
      for (int i = 0; i < sendlen; i++) {
        Log((char)_buff_pointer[i]);
      }
      Logln();
      Log(F("Client: sent ")); Log(sentBytes); Log(F("/")); Logln(_buf_len);
      #endif

      if (len == 0) {
	      return true;
      }
      if (_sent != sendlen) {
        #ifdef EW_SERIAL_LOG
        Logln(F("Client: send packet - failed to send"));
        #endif
	      return false;
      }
    } else {
      #ifdef EW_SERIAL_LOG
      Logln(F("Client: send packet - connection failed"));
      #endif
      return false;
    }
    delay(0);
  }
  return true;
}

uint16_t readPacket( WiFiClient* client, uint8_t *buffer, uint16_t maxlen, int16_t timeout ) {

  uint16_t len = 0;
  int16_t t = timeout;

  // #ifdef EW_SERIAL_LOG
  // Log(F("Client: Reading from "));Log(client->remoteIP());Log(F(" : "));Logln(client->remotePort());
  // Log(F("Client: Reading packets : "));
  // #endif
  while ( isConnected(client) && ( timeout >= 0 ) ) {

    while ( client->available() ) {

      char c = client->read();
      timeout = t;  // reset the timeout
      buffer[len] = c;
      len++;
      // #ifdef EW_SERIAL_LOG
      // Log(c);
      // #endif

      if( maxlen == 0 ){
        // #ifdef EW_SERIAL_LOG
        // Logln();
        // #endif
        return 0;
      }

      if( len == maxlen ){
        // #ifdef EW_SERIAL_LOG
        // Logln();
        // #endif
        return len;
      }
    }
    timeout -= 1;
    delay(1);
  }
  // #ifdef EW_SERIAL_LOG
  // Logln();
  // #endif
  return len;
}
