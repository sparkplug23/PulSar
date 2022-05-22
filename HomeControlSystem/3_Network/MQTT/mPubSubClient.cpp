/*
  mPubSubClient.cpp - A simple client for MQTT.
  Nick O'Leary
  http://knolleary.net

  Edited by my_name_here
  List of changes:
  - Removed callback function (non static nature caused frequent exception errors)
  - Add "ppublish" which prefixes the topic using "prefixtopic". This is set using
    "setprefixtopic()" and is used to prefix the device name.

*/

#include "mPubSubClient.h"
#include "Arduino.h"

#ifdef USE_PUBSUB_V1

// #define TRACE2() \
// Serial.print(__FILE__); \
// Serial.print(":"); \
// Serial.println(__LINE__); 
//Serial.print(": "); 
//Serial.println(__PRETTY_FUNCTION__); 

mPubSubClient::mPubSubClient() {
    this->_state = MQTT_DISCONNECTED;
    this->_client = NULL;
    this->stream = NULL;
}

mPubSubClient::mPubSubClient(Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setClient(client);
    this->stream = NULL;
}

mPubSubClient::mPubSubClient(IPAddress addr, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr, port);
    setClient(client);
    this->stream = NULL;
}
mPubSubClient::mPubSubClient(IPAddress addr, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr,port);
    setClient(client);
    setStream(stream);
}
/*mPubSubClient::mPubSubClient(IPAddress addr, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr, port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
}
mPubSubClient::mPubSubClient(IPAddress addr, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
}*/

mPubSubClient::mPubSubClient(uint8_t *ip, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip, port);
    setClient(client);
    this->stream = NULL;
}
mPubSubClient::mPubSubClient(uint8_t *ip, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip,port);
    setClient(client);
    setStream(stream);
}
/*mPubSubClient::mPubSubClient(uint8_t *ip, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip, port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
}
mPubSubClient::mPubSubClient(uint8_t *ip, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
}*/

mPubSubClient::mPubSubClient(const char* domain, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setClient(client);
    this->stream = NULL;
}
mPubSubClient::mPubSubClient(const char* domain, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setClient(client);
    setStream(stream);
}
/*mPubSubClient::mPubSubClient(const char* domain, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
}
mPubSubClient::mPubSubClient(const char* domain, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
}*/
boolean mPubSubClient::connect(const char *id) {
    return connect(id,NULL,NULL,0,0,0,0);
}

boolean mPubSubClient::connect(const char *id, const char *user, const char *pass) {
    return connect(id,user,pass,0,0,0,0);
}

boolean mPubSubClient::connect(const char *id, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage) {
    return connect(id,NULL,NULL,willTopic,willQos,willRetain,willMessage);
}

boolean mPubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage) {
    if (!connected()) {

        ALOG_DBM( PSTR(D_LOG_PUBSUB D_ERROR " \"Not connected\""));

        stats.reconnects_counter++;

        int result = 0;

        if (domain != NULL) {
            //Serial.println("[PUBSUB] domain != NULL");
            #ifdef ESP8266
                result = _client->connect(this->domain, this->port);
            #else
                result = _client->connect(this->domain, this->port);//,10000);
            #endif
        } else {
            //Serial.println("[PUBSUB] domain ELSE NULL");
            
        // Serial.print("[PUBSUB] this->port="); Serial.println(this->port);
        // Serial.print("[PUBSUB] this->ip="); Serial.println(this->ip);
            result = _client->connect(this->ip, this->port);
            
        ALOG_DBM( PSTR(D_LOG_PUBSUB "_client->connect result=%d"),result);

        // Serial.print("[PUBSUB] this->result="); Serial.println(result);
        //delay(1000);

        }

        //Serial.println("[PUBSUB] !connected");
        //Serial.print("[PUBSUB] result="); Serial.println(result);
        //Serial.print("[PUBSUB] this->port="); Serial.println(this->port);
        //Serial.print("[PUBSUB] this->ip="); Serial.println(this->ip);

        // char message[100];
        // memset(message,0,sizeof(message));
        // sprintf(message,"[PUBSUB] NOT connected | IP[%s] | port[%d] | result[%d]",this->ip.toString().c_str(),this->port,result);
        // Serial.println(message);

        //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PUBSUB D_ERROR "\n\n\n\n\n\n\n\n\n\n\"Not connected\"  | IP[%s] | port[%d] | result[%d]"),this->ip.toString().c_str(),this->port,result);


        if (result == 1) {
            nextMsgId = 1;
            // Leave room in the buffer for header and variable length field
            uint16_t length = 5;
            unsigned int j;

#if MQTT_VERSION == MQTT_VERSION_3_1
            uint8_t d[9] = {0x00,0x06,'M','Q','I','s','d','p', MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 9
#elif MQTT_VERSION == MQTT_VERSION_3_1_1
            uint8_t d[7] = {0x00,0x04,'M','Q','T','T',MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 7
#endif
            for (j = 0;j<MQTT_HEADER_VERSION_LENGTH;j++) {
                buffer[length++] = d[j];
            }

            uint8_t v;
            if (willTopic) {
                v = 0x06|(willQos<<3)|(willRetain<<5);
            } else {
                v = 0x02;
            }

            if(user != NULL) {
                v = v|0x80;

                if(pass != NULL) {
                    v = v|(0x80>>1);
                }
            }

            buffer[length++] = v;

            buffer[length++] = ((MQTT_KEEPALIVE) >> 8);
            buffer[length++] = ((MQTT_KEEPALIVE) & 0xFF);
            length = writeString(id,buffer,length);
            if (willTopic) {
                length = writeString(willTopic,buffer,length);
                length = writeString(willMessage,buffer,length);
            }

            if(user != NULL) {
                length = writeString(user,buffer,length);
                if(pass != NULL) {
                    length = writeString(pass,buffer,length);
                }
            }
            
            write(MQTTCONNECT,buffer,length-5);

            lastInActivity = lastOutActivity = millis();

            uint32_t tShowInfo = millis();

            while (!_client->available()) {
                unsigned long t = millis();

                // if(abs(millis()-tShowInfo)>=1000){ tShowInfo = millis();
                //   Serial.println("[PUBSUB] Awaiting broker...");
                // }

                //yield();
                if (t-lastInActivity >= ((int32_t) MQTT_SOCKET_TIMEOUT*1000UL)) { //Serial.println("[PUBSUB] MQTT_SOCKET_TIMEOUT");
                    _state = MQTT_CONNECTION_TIMEOUT;
                    _client->stop();
                    return false;
                }
            }
            uint8_t llen;
            uint16_t len = readPacket(&llen);

            if (len == 4) {
                if (buffer[3] == 0) {
                    lastInActivity = millis();
                    pingOutstanding = false;
                    _state = MQTT_CONNECTED;
                    //Serial.println("[PUBSUB] MQTT_CONNECTED");

                    ALOG_DBM( PSTR(D_LOG_PUBSUB D_CONNECTED));

                    return true;
                } else {
                    _state = buffer[3];
                }
            }
            _client->stop();
        } else {
            _state = MQTT_CONNECT_FAILED;
            
            ALOG_DBM( PSTR(D_LOG_PUBSUB D_ERROR " _state = %s"),state_ctr());
            //Serial.println("[PUBSUB] _state = MQTT_CONNECT_FAILED");
            _client->stop(); //ADDED BY ME
            //delay(500);
        }
        return false;
    }
    return true;
}

// reads a byte into result
boolean mPubSubClient::readByte(uint8_t * result) {
   uint32_t previousMillis = millis();
   while(!_client->available()) {


    //  #ifdef ESP8266
    //    ESP.wdtFeed();
    //  #endif
     uint32_t currentMillis = millis();
     if(currentMillis - previousMillis >= ((int32_t) MQTT_SOCKET_TIMEOUT * 1000)){
       return false;
     }
   }
   *result = _client->read();
   return true;
}

// reads a byte into result[*index] and increments index
boolean mPubSubClient::readByte(uint8_t * result, uint16_t * index){
  uint16_t current_index = *index;
  uint8_t * write_address = &(result[current_index]);
  if(readByte(write_address)){
    *index = current_index + 1;
    return true;
  }
  return false;
}

uint16_t mPubSubClient::readPacket(uint8_t* lengthLength) {
    uint16_t len = 0;
    if(!readByte(buffer, &len)) return 0;
    bool isPublish = (buffer[0]&0xF0) == MQTTPUBLISH;
    uint32_t multiplier = 1;
    uint16_t length = 0;
    uint8_t digit = 0;
    uint16_t skip = 0;
    uint8_t start = 0;

    do {
        if(!readByte(&digit)) return 0;
        buffer[len++] = digit;
        length += (digit & 127) * multiplier;
        multiplier *= 128;
    } while ((digit & 128) != 0);
    *lengthLength = len-1;

    if (isPublish) {
        // Read in topic length to calculate bytes to skip over for Stream writing
        if(!readByte(buffer, &len)) return 0;
        if(!readByte(buffer, &len)) return 0;
        skip = (buffer[*lengthLength+1]<<8)+buffer[*lengthLength+2];
        start = 2;
        if (buffer[0]&MQTTQOS1) {
            // skip message id
            skip += 2;
        }
    }

    for (uint16_t i = start;i<length;i++) {
        if(!readByte(&digit)) return 0;
        if (this->stream) {
            if (isPublish && len-*lengthLength-2>skip) {
                this->stream->write(digit);
            }
        }
        if (len < MQTT_MAX_PACKET_SIZE) {
            buffer[len] = digit;
        }
        len++;
    }

    if (!this->stream && len > MQTT_MAX_PACKET_SIZE) {
        len = 0; // This will cause the packet to be ignored.
    }

    return len;
}

boolean mPubSubClient::loop() {

    // Serial.println("mPubSubClient::loop()");

    if(abs(millis()-stats.tSavedPacketsSent)>=60000){ stats.tSavedPacketsSent=millis();
      stats.packets_sent_per_minute += (stats.packets_sent_counter-stats.packets_sent_per_minute_saved);
      stats.packets_sent_per_minute /= 2; // averaging last two minutes
      stats.packets_sent_per_minute_saved = stats.packets_sent_counter; // update
    }

    if(abs(millis()-stats.tSavedUpDown)>1000){stats.tSavedUpDown=millis();
      if(connected()){
        stats.connection_uptime++;
        stats.connection_downtime = 0;
      }else{
        stats.connection_downtime++;
        stats.connection_uptime = 0;
        ALOG_DBM( PSTR(D_LOG_PUBSUB "Connection_downtime>> %d"),stats.connection_downtime);
        //Serial.print("[MQTT] connection_downtime>> "); Serial.println(stats.connection_downtime);
      }
    }

    #ifdef ENABLE_RESTART_ON_MQTT_LOST
      if(stats.connection_downtime>(60*10)){
        //ESP.restart();
        #ifdef ESP8266
            ESP.reset();
        #endif
      }
    #endif

    if (connected()) {
        unsigned long t = millis();
        if ((t - lastInActivity > MQTT_KEEPALIVE*1000UL) || (t - lastOutActivity > MQTT_KEEPALIVE*1000UL)) {
            
            ALOG_DBM( PSTR(D_LOG_PUBSUB D_SENDING " \"MQTT_KEEPALIVE\""));
            if (pingOutstanding) {
                this->_state = MQTT_CONNECTION_TIMEOUT;
                _client->stop();
                ALOG_DBM( PSTR(D_LOG_PUBSUB D_ERROR " \"MQTT_CONNECTION_TIMEOUT\""));
                return false;
            } else { 
                ALOG_DBM( PSTR(D_LOG_PUBSUB D_SENDING " \"MQTTPINGREQ\""));
                buffer[0] = MQTTPINGREQ;
                buffer[1] = 0;
                _client->write(buffer,2);
                lastOutActivity = t;
                lastInActivity = t;
                pingOutstanding = true;
            }

        }
        if (_client->available()) {
            ALOG_DBM( PSTR(D_LOG_PUBSUB D_SENDING " \"_client->available()\""));
            uint8_t llen;
            uint16_t len = readPacket(&llen);
            uint16_t msgId = 0;
            uint8_t *payload;
            if (len > 0) {

                D_DATA_BUFFER_CLEAR();

                lastInActivity = t;
                uint8_t type = buffer[0]&0xF0;
                if (type == MQTTPUBLISH) {
                    ALOG_DBM( PSTR(D_LOG_PUBSUB D_SENDING " \"type == MQTTPUBLISH\""));
                  
                    uint16_t tl = (buffer[llen+1]<<8)+buffer[llen+2];
                    char topic[tl+1];
                    for (uint16_t i=0;i<tl;i++) {
                        topic[i] = buffer[llen+3+i];
                    }
                    topic[tl] = 0;
                    // msgId only present for QOS>0
                    if ((buffer[0]&0x06) == MQTTQOS1) {
                        msgId = (buffer[llen+3+tl]<<8)+buffer[llen+3+tl+1];
                        payload = buffer+llen+3+tl+2;

                        data_buffer.payload.len = len-llen-3-tl-2;

                        buffer[0] = MQTTPUBACK;
                        buffer[1] = 2;
                        buffer[2] = (msgId >> 8);
                        buffer[3] = (msgId & 0xFF);
                        _client->write(buffer,4);
                        lastOutActivity = t;

                    } else {
                        payload = buffer+llen+3+tl;
                        data_buffer.payload.len = len-llen-3-tl;
                    }

                    data_buffer.topic.len=tl;
                    memcpy(data_buffer.topic.ctr,topic,data_buffer.topic.len*sizeof(char));
                    memcpy(data_buffer.payload.ctr,(char*)payload,data_buffer.payload.len*sizeof(char));
                    data_buffer.flags.waiting = true;
                    AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_PUBSUB "\"data_buffer.flags.waiting = true;\""));

                } else if (type == MQTTPINGREQ) {
                    buffer[0] = MQTTPINGRESP;
                    buffer[1] = 0;
                    _client->write(buffer,2);
                    ALOG_DBM( PSTR(D_LOG_PUBSUB D_SENDING " \"MQTTPINGREQ\""));
                } else if (type == MQTTPINGRESP) {
                    pingOutstanding = false;
                    ALOG_DBM( PSTR(D_LOG_PUBSUB  "\"MQTTPINGRESP\""));
                }
            }
        }
        return true;
    }

    return false;
}

boolean mPubSubClient::publish(const char* topic, const char* payload) {
    return publish(topic,(const uint8_t*)payload,strlen(payload),false);
}

boolean mPubSubClient::publish(const char* topic, const char* payload, boolean retained) {
    return publish(topic,(const uint8_t*)payload,strlen(payload),retained);
}


boolean mPubSubClient::publish(const char* topic, const uint8_t* payload, unsigned int plength) {
    return publish(topic, payload, plength, false);
}

boolean mPubSubClient::publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {
 
//TRACE();
DEBUG_LINE;
   if (connected()) {
       
// AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "connected()"));
DEBUG_LINE;
//TRACE();
        if (MQTT_MAX_PACKET_SIZE < 5 + 2+strlen(topic) + plength) {
//TRACE();
DEBUG_LINE;
            // Too long
//    Serial.println("ERROR: MQTT_MAX_PACKET_SIZE reached");
AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "ERROR: MQTT_MAX_PACKET_SIZE reached"));
        

            #ifndef SEND_PAYLOAD_LENGTH_EXCEEDED
                return false;
            #endif
            #ifdef SEND_PAYLOAD_LENGTH_EXCEEDED
              int ii=0;

              memcpy(&payload,"Message too long ",sizeof("Message too long "));ii = sizeof("Message too long ")-1;
              itoa(plength,&payload[ii+1],10);
              ii+=mSupport::NumDigits(plength);

              plength = ii;
              retained = false;
            #endif

        }

//TRACE();
stats.packets_sent_counter++;

        // Leave room in the buffer for header and variable length field
        uint16_t length = 5;
    DEBUG_LINE;
        length = writeString(topic,buffer,length);
        uint16_t i;
        for (i=0;i<plength;i++) {
            buffer[length++] = payload[i];
        }
        uint8_t header = MQTTPUBLISH;
        if (retained) {
            header |= 1;
        }
    DEBUG_LINE;
        return write(header,buffer,length-5);
    }else{
        
// AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "NOT connected()"));
    }
//TRACE();
    DEBUG_LINE;
    return false;
}

boolean mPubSubClient::publish_P(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {
    uint8_t llen = 0;
    uint8_t digit;
    unsigned int rc = 0;
    uint16_t tlen;
    unsigned int pos = 0;
    unsigned int i;
    uint8_t header;
    unsigned int len;
    DEBUG_LINE;

    if (!connected()) {
        return false;
    }

    tlen = strlen(topic);

    header = MQTTPUBLISH;
    if (retained) {
        header |= 1;
    }
    buffer[pos++] = header;
    len = plength + 2 + tlen;
    do {
        digit = len % 128;
        len = len / 128;
        if (len > 0) {
            digit |= 0x80;
        }
        buffer[pos++] = digit;
        llen++;
    } while(len>0);

    pos = writeString(topic,buffer,pos);

    rc += _client->write(buffer,pos);

    for (i=0;i<plength;i++) {
        rc += _client->write((char)pgm_read_byte_near(payload + i));
    }

    lastOutActivity = millis();
    DEBUG_LINE;

    return rc == tlen + 4 + plength;
}

boolean mPubSubClient::write(uint8_t header, uint8_t* buf, uint16_t length) { //Serial.println("write");
    
    DEBUG_LINE;
    if (!connected()) {
        AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "NOT CONNECTED \"write\" failed!"));
        
    DEBUG_LINE;
        return 0;
    }

    DEBUG_LINE;
    if (!length) {
        AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "length is ZERO"));
        //return 0;
    }else{
        //AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "%s,%d"),buf,length);
    }
//     Serial.print("BEFORE TRACE2");
    
// Serial.flush();
//  delay(1000);
//     //TRACE2();
//     Serial.print("AFTER TRACE2");
    
// Serial.flush();
//  delay(1000);

// Serial.println(WiFi.localIP());
// Serial.println(static_cast<uint32_t>(WiFi.localIP()));

    DEBUG_LINE;
    if ((WL_CONNECTED != WiFi.status()) || (static_cast<uint32_t>(WiFi.localIP()) == 0)) {
        AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "Unable to publish no connection -- Exiting early"));
        return false;
    }else{
        //AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "WiFi.status()=%d"),WiFi.status());
    }

    DEBUG_LINE;
// Serial.print(__LINE__);
// Serial.flush();
// delay(1000);

// return 0;

// //TRACE();
// DUMP(length);
uint8_t lenBuf[4];
    uint8_t llen = 0;
    uint8_t digit;
    uint8_t pos = 0;
    uint16_t rc;
    uint16_t len = length;
   // Serial.println("HERE4");
    // delay(200);
    do {
        digit = len % 128;
        len = len / 128;
        if (len > 0) {
            digit |= 0x80;
        }
        lenBuf[pos++] = digit;
        llen++;
        //Serial.printf("len=%d\n\r",len);
    } while(len>0);

// Serial.printf("buf[4-llen] =%d\n\r",llen);
// Serial.flush();
    buf[4-llen] = header;
    for (int i=0;i<llen;i++) {
        buf[5-llen+i] = lenBuf[i];
    }

#ifdef MQTT_MAX_TRANSFER_SIZE
    uint8_t* writeBuf = buf+(4-llen);
    uint16_t bytesRemaining = length+1+llen;  //Match the length type
    uint8_t bytesToWrite;
    boolean result = true;
    while((bytesRemaining > 0) && result) {

      #ifdef ESP8266
        ESP.wdtFeed();
      #endif
        bytesToWrite = (bytesRemaining > MQTT_MAX_TRANSFER_SIZE)?MQTT_MAX_TRANSFER_SIZE:bytesRemaining;
        rc = _client->write(writeBuf,bytesToWrite);
        result = (rc == bytesToWrite);
        bytesRemaining -= rc;
        writeBuf += rc;
    }
    return result;
#else

    //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PUBSUB "\"_client->write\""));//buf\"=%s, \"llen\"=%d"),buf,llen);

// if(_client == NULL){
//     Serial.print("_client == NULL");
// Serial.flush();
// // delay(1000);
// }else{
    
//     Serial.print("_client != NULL");
// }

// delay(1000);
if(_client->connected()){

    // Serial.printf("\"length\"=%d",length);
    // Serial.flush();
    // delay(200);
    // Serial.printf("\"llen\"=%d",llen);
    // Serial.flush();
    // delay(200);
    // Serial.printf("\"buf\"=%c",(char)buf[0]);
    // Serial.flush();
    // delay(200);

    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PUBSUB "\"buf\"=%c,\"length\"=%d,\"llen\"=%d"),(char)buf[0],length,llen);
    // delay(1000);
    
    rc = _client->write(buf+(4-llen),length+1+llen);
    ALOG_DBM( PSTR(D_LOG_PUBSUB "\"_client->write is connected\""));
}else{
    ALOG_DBM( PSTR(D_LOG_PUBSUB "\"_client->write NOT CONNECTED\""));
}

DEBUG_LINE;
    
    lastOutActivity = millis();
    return (rc == 1+llen+length);
#endif
}

boolean mPubSubClient::subscribe(const char* topic) {
    return subscribe(topic, 0);
}

boolean mPubSubClient::subscribe(const char* topic2, uint8_t qos) {

    // if(strlen(topic)){}

    char topic[100];
    memset(topic,0,sizeof(topic));
    sprintf(topic,topic2);
    
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_PUBSUB "Subscribed to \"%s\",%d"),topic,strlen(topic));
// return 0;
    //Serial.printf("Subscribed to \"%s\"\n\r",topic);

    if (qos < 0 || qos > 1) {
        return false;
    }
    if (MQTT_MAX_PACKET_SIZE < 9 + strlen(topic)) {
        // Too long
        return false;
    }
    if (connected()) {
        // Leave room in the buffer for header and variable length field
        uint16_t length = 5;
        nextMsgId++;
        if (nextMsgId == 0) {
            nextMsgId = 1;
        }
        buffer[length++] = (nextMsgId >> 8);
        buffer[length++] = (nextMsgId & 0xFF);
        length = writeString((char*)topic, buffer,length);
        buffer[length++] = qos;
        return write(MQTTSUBSCRIBE|MQTTQOS1,buffer,length-5);
    }
    return false;
}

boolean mPubSubClient::unsubscribe(const char* topic) {
    if (MQTT_MAX_PACKET_SIZE < 9 + strlen(topic)) {
        // Too long
        return false;
    }
    if (connected()) {
        uint16_t length = 5;
        nextMsgId++;
        if (nextMsgId == 0) {
            nextMsgId = 1;
        }
        buffer[length++] = (nextMsgId >> 8);
        buffer[length++] = (nextMsgId & 0xFF);
        length = writeString(topic, buffer,length);
        return write(MQTTUNSUBSCRIBE|MQTTQOS1,buffer,length-5);
    }
    return false;
}

void mPubSubClient::disconnect() {
    buffer[0] = MQTTDISCONNECT;
    buffer[1] = 0;
    _client->write(buffer,2);
    _state = MQTT_DISCONNECTED;
    _client->stop();
    lastInActivity = lastOutActivity = millis();
}

uint16_t mPubSubClient::writeString(const char* string, uint8_t* buf, uint16_t pos) {
    const char* idp = string;
    uint16_t i = 0;
    pos += 2;
    while (*idp) {
        buf[pos++] = *idp++;
        i++;
    }
    buf[pos-i-2] = (i >> 8);
    buf[pos-i-1] = (i & 0xFF);
    return pos;
}


boolean mPubSubClient::connected() {
    boolean rc;
    if (_client == NULL ) {
        rc = false;
    } else {
        rc = (int)_client->connected();
        if (!rc) {
            if (this->_state == MQTT_CONNECTED) {
                this->_state = MQTT_CONNECTION_LOST;
                _client->flush();
                _client->stop();
            }
        }
    }
    return rc;
}

mPubSubClient& mPubSubClient::setServer(uint8_t * ip, uint16_t port) {
    IPAddress addr(ip[0],ip[1],ip[2],ip[3]);
    return setServer(addr,port);
}

mPubSubClient& mPubSubClient::setServer(IPAddress ip, uint16_t port) {
    this->ip = ip;
    this->port = port;
    this->domain = NULL;
    return *this;
}

mPubSubClient& mPubSubClient::setServer(const char * domain, uint16_t port) {
    this->domain = domain;
    this->port = port;
    return *this;
}

mPubSubClient& mPubSubClient::setClient(Client& client){
    this->_client = &client;
    return *this;
}

mPubSubClient& mPubSubClient::setStream(Stream& stream){
    this->stream = &stream;
    return *this;
}

int mPubSubClient::state() {
    return this->_state;
}

const char* mPubSubClient::state_ctr(void){
    switch(this->_state){
        case MQTT_CONNECTION_TIMEOUT:       return PSTR("Connection Timeout");
        case MQTT_CONNECTION_LOST:          return PSTR("Connecttion Lost");
        case MQTT_CONNECT_FAILED:           return PSTR("Connect Failed");
        case MQTT_DISCONNECTED:             return PSTR("Disconnected");
        case MQTT_CONNECTED:                return PSTR("Connected");
        case MQTT_CONNECT_BAD_PROTOCOL:     return PSTR("Bad Protocol");
        case MQTT_CONNECT_BAD_CLIENT_ID:    return PSTR("Bad Client ID");
        case MQTT_CONNECT_UNAVAILABLE:      return PSTR("Unavailable");
        case MQTT_CONNECT_BAD_CREDENTIALS:  return PSTR("Bad Credentials");
        case MQTT_CONNECT_UNAUTHORIZED:     return PSTR("Unauthorized");
    }
}


#endif // #ifdef USE_PUBSUB_V1