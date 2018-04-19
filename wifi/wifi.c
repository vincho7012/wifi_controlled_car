#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"

#include "espconn.h"
#include "eagle_soc.h"



static const int fwd_pin = 2;
static const int bwd_pin = 4;
static const int left_pin = 0;
static const int right_pin = 5;



const char *forward_command = "FF";
const char *backward_command = "BB";
const char *left_command = "LL";
const char *right_command = "RR";
const char *stop_turning_command = "UU";
const char *stop_moving_command = "SS";

/*---------------------------------------------------------------------------*/
LOCAL struct espconn ptrespconn;

/******************************************************************************
 * FunctionName : user_devicefind_recv
 * Description  : Processing the received udp packet
 * Parameters   : arg -- Additional argument to pass to the callback function
 *                pusrdata -- The received data (or NULL when the connection has been closed!)
 *                length -- The length of received data
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
user_udp_recv(void *arg, char *pusrdata, unsigned short length)
{
    char DeviceBuffer[40] = {0};
    char Device_mac_buffer[60] = {0};
    char hwaddr[6];

    struct ip_info ipconfig;

    if (wifi_get_opmode() != STATION_MODE)
   {
        wifi_get_ip_info(SOFTAP_IF, &ipconfig);
        wifi_get_macaddr(SOFTAP_IF, hwaddr);

        if (!ip_addr_netcmp((struct ip_addr *)ptrespconn.proto.udp->remote_ip, &ipconfig.ip, &ipconfig.netmask))
      {
         //udp packet is received from ESP8266 station
            wifi_get_ip_info(STATION_IF, &ipconfig);
            wifi_get_macaddr(STATION_IF, hwaddr);
        }
      else
      {
         //udp packet is received from ESP8266 softAP
        }

    }
   else
   {
      //udp packet is received from ESP8266 station
        wifi_get_ip_info(STATION_IF, &ipconfig);
        wifi_get_macaddr(STATION_IF, hwaddr);
    }

    if (pusrdata == NULL)
        return;

    if (os_strncmp(pusrdata, forward_command, os_strlen(forward_command)) == 0)
    {
      os_printf("123\n");
        GPIO_OUTPUT_SET(fwd_pin, 1);
        GPIO_OUTPUT_SET(bwd_pin, 0);
    }
    if (os_strncmp(pusrdata, backward_command, os_strlen(backward_command)) == 0)
    {
      os_printf("123\n");
        GPIO_OUTPUT_SET(bwd_pin, 1);
        GPIO_OUTPUT_SET(fwd_pin, 0);
    }
    if (os_strncmp(pusrdata, left_command, os_strlen(left_command)) == 0)
    {
      os_printf("123\n");
        GPIO_OUTPUT_SET(left_pin, 1);
        GPIO_OUTPUT_SET(right_pin, 0);
    }
    if (os_strncmp(pusrdata, right_command, os_strlen(right_command)) == 0)
    {
      os_printf("123\n");
        GPIO_OUTPUT_SET(right_pin, 1);
        GPIO_OUTPUT_SET(left_pin, 0);
    }
    if (os_strncmp(pusrdata, stop_moving_command, os_strlen(stop_moving_command)) == 0)
    {
        GPIO_OUTPUT_SET(fwd_pin, 0);
        GPIO_OUTPUT_SET(bwd_pin, 0);

    }
    if (os_strncmp(pusrdata, stop_turning_command, os_strlen(stop_turning_command)) == 0)
    {
        GPIO_OUTPUT_SET(right_pin, 0);
        GPIO_OUTPUT_SET(left_pin, 0);
    }


}

/******************************************************************************
 * FunctionName : user_devicefind_init
 * Description  : create a udp listening
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
user_udp_init(void)
{
    ptrespconn.type = ESPCONN_UDP;
    ptrespconn.proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
    ptrespconn.proto.udp->local_port = 4210;  // ESP8266 udp port
    espconn_regist_recvcb(&ptrespconn, user_udp_recv); // register a udp packet receiving callback
    espconn_create(&ptrespconn);   // create udp
}



/******************************************************************************
 * FunctionName : user_set_softap_config
 * Description  : set SSID and password of ESP8266 softAP
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
user_set_softap_config(void)
{
   struct softap_config config;

   wifi_softap_get_config(&config); // Get config first.

   os_memset(config.ssid, 0, 32);
   os_memset(config.password, 0, 64);
   os_memcpy(config.ssid, "ESP8266", 7);
   os_memcpy(config.password, "12345678", 8);
   config.authmode = AUTH_WPA_WPA2_PSK;
   config.ssid_len = 0;// or its actual length
   config.max_connection = 1; // how many stations can connect to ESP8266 softAP at most.

   wifi_softap_set_config(&config);// Set ESP8266 softap config .

}


/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
    os_printf("SDK version:%s\n", system_get_sdk_version());

    wifi_set_opmode(STATIONAP_MODE);

    // ESP8266 softAP set config.
    user_set_softap_config();

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);

    gpio_init();



    user_udp_init();

}
