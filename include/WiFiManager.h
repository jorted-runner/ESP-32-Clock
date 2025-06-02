#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

void connectToWiFi(const char* ssid, const char* password);
bool checkWiFiAndReconnect();

#endif