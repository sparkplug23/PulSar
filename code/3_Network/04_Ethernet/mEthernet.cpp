#include "mEthernet.h"

#ifdef USE_MODULE_NETWORK_ETHERNET

int8_t mEthernet::Tasker(uint8_t function, JsonParserObject obj)
{

  int8_t function_result = 0;

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    case TASK_UPTIME_1_MINUTES:
      Ethernet_Begin();
      eth.delayed_start = false;
    break;
    /************
     * STORAGE SECTION * 
    *******************/  
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * RULES SECTION * 
    *******************/
    // #ifdef USE_MODULE_CORE_RULES
    // case TASK_EVENT_SET_POWER_ID: 
    //   RulesEvent_Set_Power();
    // break;
    // #endif// USE_MODULE_CORE_RULES
    /************
     * MQTT SECTION * 
    *******************/
    // For Networks, telemetry shows connection status
  } // end switch

  return FUNCTION_RESULT_UNKNOWN_ID;

} // END function


void mEthernet::Pre_Init()
{
  //   if (!Settings->flag4.network_ethernet) { return; }
//   if (!PinUsed(GPIO_ETH_PHY_MDC) && !PinUsed(GPIO_ETH_PHY_MDIO)) {
//     ALOG_DBG(PSTR(D_LOG_ETH "No ETH MDC and/or ETH MDIO GPIO defined"));
//     return;
//   }

//   eth_config_change = 0;

//   if (WT32_ETH01 == TasmotaGlobal.module_type) {
//     Settings->eth_address = 1;                    // EthAddress
//     Settings->eth_type = ETH_PHY_LAN8720;         // EthType
//     Settings->eth_clk_mode = ETH_CLOCK_GPIO0_IN;  // EthClockMode
//   }

// //  snprintf_P(Eth.hostname, sizeof(Eth.hostname), PSTR("%s-eth"), TasmotaGlobal.hostname);
//   strlcpy(eth_hostname, TasmotaGlobal.hostname, sizeof(eth_hostname) -5);  // Make sure there is room for "-eth"
//   strcat(eth_hostname, "-eth");

//   WiFi.onEvent(EthernetEvent);

//   int eth_power = Pin(GPIO_ETH_PHY_POWER);
//   int eth_mdc = Pin(GPIO_ETH_PHY_MDC);
//   int eth_mdio = Pin(GPIO_ETH_PHY_MDIO);
// //#if CONFIG_IDF_TARGET_ESP32
//   // fix an disconnection issue after rebooting Olimex POE - this forces a clean state for all GPIO involved in RMII
// //  gpio_reset_pin((gpio_num_t)GPIO_ETH_PHY_POWER);
// //  gpio_reset_pin((gpio_num_t)GPIO_ETH_PHY_MDC);
// //  gpio_reset_pin((gpio_num_t)GPIO_ETH_PHY_MDIO);
// //  gpio_reset_pin(GPIO_NUM_19);    // EMAC_TXD0 - hardcoded
// //  gpio_reset_pin(GPIO_NUM_21);    // EMAC_TX_EN - hardcoded
// //  gpio_reset_pin(GPIO_NUM_22);    // EMAC_TXD1 - hardcoded
// //  gpio_reset_pin(GPIO_NUM_25);    // EMAC_RXD0 - hardcoded
// //  gpio_reset_pin(GPIO_NUM_26);    // EMAC_RXD1 - hardcoded
// //  gpio_reset_pin(GPIO_NUM_27);    // EMAC_RX_CRS_DV - hardcoded
// //  switch (Settings->eth_clk_mode) {
// //    case 0:   // ETH_CLOCK_GPIO0_IN
// //    case 1:   // ETH_CLOCK_GPIO0_OUT
// //      gpio_reset_pin(GPIO_NUM_0);
// //      break;
// //    case 2:   // ETH_CLOCK_GPIO16_OUT
// //      gpio_reset_pin(GPIO_NUM_16);
// //      break;
// //    case 3:   // ETH_CLOCK_GPIO17_OUT
// //      gpio_reset_pin(GPIO_NUM_17);
// //      break;
// //  }
// //  delay(1);
// //#endif // CONFIG_IDF_TARGET_ESP32
//   if (!ETH.begin(Settings->eth_address, eth_power, eth_mdc, eth_mdio, (eth_phy_type_t)Settings->eth_type, (eth_clock_mode_t)Settings->eth_clk_mode)) {
//     ALOG_DBG(PSTR(D_LOG_ETH "Bad PHY type or init error"));
//     return;
//   };

//   if (Settings->eth_ipv4_address[0]) {
//     EthernetSetIp();                             // Set static IP
//   }
}


void mEthernet::Init(void)
{
#ifdef ESP32
  // If you want “Ethernet-only”, hard-disable WiFi early.
  // (If later you want AP fallback, remove this and let WiFi2 manage AP/STA.)
  // WiFi.mode(WIFI_OFF);

  // Ethernet_Begin();
#endif


  const char* originalName = tkr_set->Settings.system_name.device;
  // Define a temporary buffer (max 64 bytes: 63 + null terminator)
  char hostname[64];
  strncpy(hostname, originalName, 63);
  hostname[63] = '\0';
  // Replace invalid underscores with dashes
  for (char* p = hostname; *p; ++p){ if (*p == '_') *p = '-';  }
  strlcpy(eth_hostname, hostname, sizeof(eth_hostname) -5);  // Make sure there is room for "-eth"
  strcat(eth_hostname, "-eth");
  ALOG_INF(PSTR(D_LOG_ETHERNET "Hostname %s"),eth_hostname);

  module_state.mode = ModuleStatus::Running;

}



// -----------------------------------------------------------------------------
// Status helpers
// -----------------------------------------------------------------------------
bool mEthernet::Ethernet_HasLink() const
{
#ifdef ESP32
  return ETH.linkUp();
#else
  return false;
#endif
}

bool mEthernet::Ethernet_HasIP() const
{
#ifdef ESP32
  IPAddress ip = ETH.localIP();
  return (ip[0] != 0); // cheap check for 0.0.0.0
#else
  return false;
#endif
}

bool mEthernet::Ethernet_IsRoutable() const
{
#ifdef ESP32
  if (!Ethernet_HasIP()) return false;
  IPAddress gw = ETH.gatewayIP();
  return (gw[0] != 0); // if gateway is 0.0.0.0, treat as not-routable
#else
  return false;
#endif
}


IPAddress mEthernet::EthernetLocalIP(void) {
  return ETH.localIP();
}


// void EthernetSetIp(void) {
//   // Set static IP
//   ETH.config(Settings->eth_ipv4_address[0],       // IPAddress local_ip
//              Settings->eth_ipv4_address[1],       // IPAddress gateway
//              Settings->eth_ipv4_address[2],       // IPAddress subnet
//              Settings->eth_ipv4_address[3],       // IPAddress dns1
//              Settings->eth_ipv4_address[4]);      // IPAddress dns2
// }

// Check to see if we have any routable IP address
// IPv4 has always priority
// Copy the value of the IP if pointer provided (optional)
bool mEthernet::EthernetGetIP(IPAddress *ip) {
#ifdef USE_IPV6
  if ((uint32_t)ETH.localIP() != 0) {
    if (ip != nullptr) { *ip = ETH.localIP(); }
    return true;
  }
  IPAddress lip;
  if (EthernetGetIPv6(&lip)) {
    if (ip != nullptr) { *ip = lip; }
    return true;
  }
  if (ip != nullptr) { *ip = IPAddress(); }
  return false;
#else
  // IPv4 only
  if (ip != nullptr) { *ip = ETH.localIP(); }
  return (uint32_t)ETH.localIP() != 0;
#endif // USE_IPV6
}

bool mEthernet::EthernetHasIP(void) {
  return EthernetGetIP(nullptr);
}

String mEthernet::EthernetGetIPStr(void) {
  IPAddress ip;
  if (EthernetGetIP(&ip)) {
    return ip.toString();
  } else {
    return String();
  }
}

char* mEthernet::EthernetHostname(void) {
  return eth_hostname;
}

String mEthernet::EthernetMacAddress(void) {
  return ETH.macAddress();
}

void mEthernet::EthernetConfigChange(void) {
  // if (eth_config_change) {
  //   eth_config_change--;
  //   if (!eth_config_change) {
  //     EthernetSetIp();
  //   }
  // }
}
// -----------------------------------------------------------------------------
// ETH bring-up + events
// -----------------------------------------------------------------------------
#ifdef ESP32

void mEthernet::Ethernet_Begin()
{
  ALOG_INF(PSTR("Calling Ethernet_Begin"));
  if (eth.begun) return;

  // Register event callback (Arduino-ESP32 Ethernet is delivered via WiFi.onEvent)
  WiFi.onEvent(&mEthernet::EthernetEvent);

  // Start ETH
  // Signature (Arduino-ESP32): ETH.begin(phy_addr, power_pin, mdc, mdio, type, clk_mode);
  const bool ok = ETH.begin(
    PULSAR_ETH_PHY_ADDR,
    PULSAR_ETH_POWER_PIN,
    PULSAR_ETH_MDC_PIN,
    PULSAR_ETH_MDIO_PIN,
    PULSAR_ETH_PHY_TYPE,
    PULSAR_ETH_CLK_MODE
  );
  ALOG_INF(PSTR("Calling Ethernet_Begin ok =%d"),ok);

  eth.begun = ok;
  eth.last_change = millis();

  // You can log ok/failed here using your logger if desired.
}

void mEthernet::Ethernet_Stop()
{
  if (!eth.begun) return;
  // ETH.end();
  eth = {}; // reset all flags
  eth.link_up = false;
eth.got_ip  = false;
eth.connected = false;

}

void mEthernet::EthernetEvent(arduino_event_t* event)
{
  if (!event) return;

  switch (event->event_id)
  {
    case ARDUINO_EVENT_ETH_START:{
      ALOG_INF(PSTR(D_LOG_ETHERNET D_ATTEMPTING_CONNECTION));
      ETH.setHostname(tkr_eth->eth_hostname);

      tkr_eth->eth.last_change = millis();
    }break;

    case ARDUINO_EVENT_ETH_CONNECTED:
    
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ETHERNET D_CONNECTED " at %dMbps%s, Mac %s, Hostname %s"),
        ETH.linkSpeed(), (ETH.fullDuplex()) ? " Full Duplex" : "",
        ETH.macAddress().c_str(), tkr_eth->eth_hostname
        );
        

      tkr_eth->eth.link_up = true;
      tkr_eth->eth.last_change = millis();
      break;

    case ARDUINO_EVENT_ETH_GOT_IP:
      tkr_eth->eth.got_ip = true;
      tkr_eth->eth.last_change = millis();
      break;

    case ARDUINO_EVENT_ETH_DISCONNECTED:
      ALOG_INF(PSTR(D_LOG_ETHERNET "Disconnected"));
      tkr_set->runtime.global_state.eth_down = false;

      tkr_eth->eth.link_up = false;
      tkr_eth->eth.got_ip  = false;
      tkr_eth->eth.last_change = millis();
      break;

    case ARDUINO_EVENT_ETH_STOP:
      ALOG_INF(PSTR(D_LOG_ETHERNET "Stopped"));
      tkr_set->runtime.global_state.eth_down = false;

      tkr_eth->eth.link_up = false;
      tkr_eth->eth.got_ip  = false;
      tkr_eth->eth.last_change = millis();
    break;

    default:
      break;
  }
}

void mEthernet::EverySecond()
{
  if(eth.delayed_start) return;
ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  if (!eth.begun)
  {
    Ethernet_Begin();
    return;
  }

IPAddress ip = ETH.localIP();
ALOG_INF(PSTR("ethernet ip== %d.%d.%d.%d"),ip[0],ip[1],ip[2],ip[3]);

  // Re-evaluate “connected” based on routable IP, not just link.
  const bool want_connected = Ethernet_IsRoutable();
ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  if (want_connected && !eth.connected)
  {
    eth.connected = true;
    OnBecameConnected();
  }
  else if (!want_connected && eth.connected)
  {
    eth.connected = false;
    OnBecameDisconnected();
  }ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
}

void mEthernet::OnBecameConnected()
{

IPAddress ip = ETH.localIP();
ALOG_INF(PSTR("ethernet connected? %d.%d.%d.%d"),ip[0],ip[1],ip[2],ip[3]);


  // Optional: mDNS
  #ifdef USE_NETWORK_MDNS
    // If you have an mDNS module, call into it here.
    // (Do not reuse WiFi module’s mdns code unless you deliberately unify it.)
  #endif

  // Switch MQTT transport to ETH if your manager supports it
  // Your earlier skeleton used: tkr_mqtt->connection_maintainer.mqtt_client_type == ...ETHERNET...
  // if (tkr_mqtt && (tkr_mqtt->connection_maintainer.mqtt_client_type == mMQTTManager::CLIENT_TYPE_ETHERNET_ID))
  // {
  //   if (!mqtt_client) mqtt_client = new WiFiClient(); // ETH uses this client type
  //   tkr_mqtt->SetPubSubClient(mqtt_client);
  // }

  // If you want to reuse your Tasker event pattern:
  // Tasker(TASK_NETWORK_CONNECTED__ETHERNET);  // only if your Tasker manager expects it
}

void mEthernet::OnBecameDisconnected()
{
  // Let MQTT reconnect logic do its thing; you can also force a disconnect if desired.
}





/*********************************************************************************************\
 * Commands
\*********************************************************************************************/


#define D_CMND_ETHADDRESS   "Address"
#define D_CMND_ETHTYPE      "Type"
#define D_CMND_ETHCLOCKMODE "ClockMode"
#define D_CMND_ETHIPADDRESS D_CMND_IPADDRESS
#define D_CMND_ETHGATEWAY   D_GATEWAY
#define D_CMND_ETHNETMASK   D_SUBNETMASK
#define D_CMND_ETHDNS       D_DNSSERVER

void mEthernet::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken tok;

  JsonParserObject jobj = obj[GetModuleName()];

  if(!jobj) return; // Only allow commands for this module

  if(tok = jobj[D_CMND_ETHADDRESS])
  {
    
  }
  

}




// #define D_CMND_ETHADDRESS   "Address"
// #define D_CMND_ETHTYPE      "Type"
// #define D_CMND_ETHCLOCKMODE "ClockMode"
// #define D_CMND_ETHIPADDRESS D_CMND_IPADDRESS
// #define D_CMND_ETHGATEWAY   D_GATEWAY
// #define D_CMND_ETHNETMASK   D_SUBNETMASK
// #define D_CMND_ETHDNS       D_DNSSERVER

// const char kEthernetCommands[] PROGMEM = "Eth|"  // Prefix
//   "ernet|" D_CMND_ETHADDRESS "|" D_CMND_ETHTYPE "|" D_CMND_ETHCLOCKMODE "|"
//   D_CMND_ETHIPADDRESS "|" D_CMND_ETHGATEWAY "|" D_CMND_ETHNETMASK "|" D_CMND_ETHDNS ;

// void (* const EthernetCommand[])(void) PROGMEM = {
//   &CmndEthernet, &CmndEthAddress, &CmndEthType, &CmndEthClockMode,
//   &CmndEthSetIpConfig, &CmndEthSetIpConfig, &CmndEthSetIpConfig, &CmndEthSetIpConfig };

// #define ETH_PARAM_OFFSET 4                       // Offset of command index in above table of first CmndEthIpConfig

// void CmndEthernet(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     Settings->flag4.network_ethernet = XdrvMailbox.payload;
//     TasmotaGlobal.restart_flag = 2;
//   }
//   ResponseCmndStateText(Settings->flag4.network_ethernet);
// }

// void CmndEthAddress(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 31)) {
//     Settings->eth_address = XdrvMailbox.payload;
//     TasmotaGlobal.restart_flag = 2;
//   }
//   ResponseCmndNumber(Settings->eth_address);
// }

// void CmndEthType(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 8)) {
//     Settings->eth_type = XdrvMailbox.payload;
//     TasmotaGlobal.restart_flag = 2;
//   }
//   ResponseCmndNumber(Settings->eth_type);
// }

// void CmndEthClockMode(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
//     Settings->eth_clk_mode = XdrvMailbox.payload;
//     TasmotaGlobal.restart_flag = 2;
//   }
//   ResponseCmndNumber(Settings->eth_clk_mode);
// }

// void CmndEthSetIpConfig(void) {
//   uint32_t param_id = XdrvMailbox.command_code -ETH_PARAM_OFFSET;

//   char cmnd_idx[2] = { 0 };
//   if (3 == param_id) {                           // EthDnsServer
//     if ((XdrvMailbox.index < 1) || (XdrvMailbox.index > 2)) {
//       XdrvMailbox.index = 1;
//     }
//     cmnd_idx[0] = '0' + XdrvMailbox.index;
//     param_id += XdrvMailbox.index -1;            // EthDnsServer2
//   }

//   if (XdrvMailbox.data_len) {
//     uint32_t ipv4_address;
//     if (ParseIPv4(&ipv4_address, XdrvMailbox.data)) {
//       Settings->eth_ipv4_address[param_id] = ipv4_address;
//       eth_config_change = 2;
//     }
//   }

//   char network_address[22] = { 0 };
//   if (0 == param_id) {
//     if (!Settings->eth_ipv4_address[0]) {
//       ext_snprintf_P(network_address, sizeof(network_address), PSTR(" (%_I)"), (uint32_t)ETH.localIP());
//     }
//   }
//   Response_P(PSTR("{\"%s%s\":\"%_I%s\"}"), XdrvMailbox.command, cmnd_idx, Settings->eth_ipv4_address[param_id], network_address);
// }



#endif // ESP32

#endif // USE_MODULE_NETWORK_ETHERNET


