#ifndef __CONFIG_H__
#define __CONFIG_H__

//#define CONFIG_STATIC
#ifdef CONFIG_STATIC

#define STA_SSID	"ssid2join"
#define STA_PASSWORD	"password"
#define AP_SSID		"myssid"
#define AP_PASSWORD	"password"
// over ride in this file
#include "config_wifi.h"

void config_execute(void);

#endif

#define CONFIG_DYNAMIC
#ifdef CONFIG_DYNAMIC

typedef struct config_commands {
	char *command;
	void(*function)(serverConnData *conn, uint8_t argc, char *argv[]);
} config_commands_t;


void config_parse(serverConnData *conn, char *buf, int len);

#endif


#include <gpio.h>

void config_gpio(void);

extern volatile char short_press_flag;
extern volatile char long_press_flag;
extern volatile char hardrest_press_flag;

#define TIMER_DURATION_MS 500

#define PWBTN_SHORT_PRESS_MS 500
#define PWBTN_LONG_PRESS_MS 6000
#define PWBTN_HARDRESET_PAUSE_MS 2000

#define LED_PIN 2
#define PWBTN_PIN 14

#endif /* __CONFIG_H__ */
