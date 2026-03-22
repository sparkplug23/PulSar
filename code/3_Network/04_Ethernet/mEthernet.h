#ifndef _M_ETHERNET_H
#define _M_ETHERNET_H

#define D_UNIQUE_MODULE_NETWORK_ETHERNET_ID  3004 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_ETHERNET


#include "1_TaskerManager/mTaskerInterface.h"

#ifdef ESP32
  #include <ETH.h>
  #include <WiFi.h>   // for WiFi.onEvent + WiFi.mode(WIFI_OFF)
#endif

class mEthernet : public mTaskerInterface
{
public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mEthernet(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_NETWORK_ETHERNET_CTR = D_MODULE_NETWORK_ETHERNET_CTR;
    PGM_P   GetModuleName()      { return PM_MODULE_NETWORK_ETHERNET_CTR; }
    uint16_t GetModuleUniqueID() { return D_UNIQUE_MODULE_NETWORK_ETHERNET_ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

  // High-level status
  bool Ethernet_HasLink() const;
  bool Ethernet_HasIP() const;
  bool Ethernet_IsRoutable() const;

  // Optional: reuse your existing naming if other code expects it
  bool Link_IsIpRoutable() const { return Ethernet_IsRoutable(); }

private:
#ifdef ESP32
  // ---- Board / PHY config (override with -D in build flags as needed) ----
  #ifndef PULSAR_ETH_PHY_TYPE
    #define PULSAR_ETH_PHY_TYPE   ETH_PHY_LAN8720
  #endif
  #ifndef PULSAR_ETH_PHY_ADDR
    #define PULSAR_ETH_PHY_ADDR   0
  #endif
  #ifndef PULSAR_ETH_MDC_PIN
    #define PULSAR_ETH_MDC_PIN    23
  #endif
  #ifndef PULSAR_ETH_MDIO_PIN
    #define PULSAR_ETH_MDIO_PIN   18
  #endif
  #ifndef PULSAR_ETH_POWER_PIN
    #define PULSAR_ETH_POWER_PIN  -1
  #endif
  #ifndef PULSAR_ETH_CLK_MODE
    // LilyGO T-Internet-POE family often needs GPIO17 clock out (Tasmota EthClockMode 3)
    #define PULSAR_ETH_CLK_MODE   ETH_CLOCK_GPIO17_OUT
  #endif

  char eth_hostname[64]; // max 63 + null char

  // ---- State ----
  struct {
    bool delayed_start = true;
    bool     begun        = false;
    bool     link_up      = false;
    bool     got_ip       = false;
    bool     connected    = false;   // “routable up” (what you want for MQTT/NTP)
    uint32_t last_change  = 0;
  } eth;

  WiFiClient* mqtt_client = nullptr;

  static void EthernetEvent(arduino_event_t* event);



bool EthernetGetIP(IPAddress *ip);
bool EthernetHasIP(void);
String EthernetGetIPStr(void);
char* EthernetHostname(void);
String EthernetMacAddress(void);
void EthernetConfigChange(void);
IPAddress EthernetLocalIP(void);


  void Ethernet_Begin();
  void Ethernet_Stop();
  void EverySecond();    

  void OnBecameConnected();
  void OnBecameDisconnected();

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
  void    parse_JSONCommand(JsonParserObject obj);

#endif
};

#endif // USE_MODULE_NETWORK_ETHERNET
#endif // _M_ETHERNET_H

/*********************************************************************************************\
 * Ethernet support for ESP32
 *
 * Dedicated fixed Phy pins
 * GPIO17 - EMAC_CLK_OUT_180
 * GPIO19 - EMAC_TXD0(RMII)
 * GPIO21 - EMAC_TX_EN(RMII)
 * GPIO22 - EMAC_TXD1(RMII)
 * GPIO25 - EMAC_RXD0(RMII)
 * GPIO26 - EMAC_RXD1(RMII)
 * GPIO27 - EMAC_RX_CRS_DV
 *
 * {"NAME":"Olimex ESP32-PoE","GPIO":[1,1,1,1,1,1,0,0,5536,1,1,1,1,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
 * GPIO12 = ETH POWER
 * GPIO18 = ETH MDIO
 * GPIO23 = ETH MDC
 * #define ETH_TYPE          ETH_PHY_LAN8720
 * #define ETH_CLKMODE       ETH_CLOCK_GPIO17_OUT
 * #define ETH_ADDRESS       0
 *
 * {"NAME":"wESP32","GPIO":[0,0,1,0,1,1,0,0,1,1,1,1,5568,5600,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
 * GPIO16 = ETH MDC
 * GPIO17 = ETH MDIO
 * #define ETH_TYPE          ETH_PHY_LAN8720
 * #define ETH_CLKMODE       ETH_CLOCK_GPIO0_IN
 * #define ETH_ADDRESS       0
 *
 * {"NAME":"WT32-ETH01","GPIO":[1,1,1,1,1,1,0,0,1,0,1,1,3840,576,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1],"FLAG":0,"BASE":1}
 * GPIO16 = Force Hi
 * GPIO18 = ETH MDIO
 * GPIO23 = ETH MDC
 * #define ETH_TYPE          ETH_PHY_LAN8720
 * #define ETH_CLKMODE       ETH_CLOCK_GPIO0_IN
 * #define ETH_ADDRESS       1
 *
\*********************************************************************************************/
