/************************ Adafruit IO Config *******************************/
#define IO_USERNAME  "Alejandroxd"
#define IO_KEY       "aio_fIfe35UKm26zTAUCz1MWDv9vO6Xy"
/******************************* WIFI **************************************/

/*
#define WIFI_SSID "CLARO1_DADC01"
#define WIFI_PASS "H06BrsREGw"
*/
#define WIFI_SSID "Pancho"
#define WIFI_PASS "MARLEN13"

#include "AdafruitIO_WiFi.h"


AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
