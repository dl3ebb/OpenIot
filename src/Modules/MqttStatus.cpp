#include "MqttStatus.h"
#include <Connector/MqttConnector.h>
#include <OpenIot.h>
#include <Ticker.h>
#include <WiFi.h>

Ticker mqttUpdateStatusTicker;

void updateMqttStatus() {
    mqttStatus->updateStatus();
}

MqttStatus::MqttStatus() {

    loopCounterElement = new LongElement("SYS_STATUS_LPS");
    mqttConnector->registerPublish(loopCounterElement, "/sys/LoopsPerSecond");

    upTimeElement = new StringElement("SYS_STATUS_UPTIME");
    mqttConnector->registerPublish(upTimeElement, "/sys/Uptime");

    localTimeElement = new StringElement("SYS_STATUS_LOCAL_TIME");
    mqttConnector->registerPublish(localTimeElement, "/sys/Localtime");

    ipAddressElement = new StringElement("SYS_STATUS_IP_ADDRESS");
    mqttConnector->registerPublish(ipAddressElement, "/sys/IpAdress");

    wifiSsidElement = new StringElement("SYS_STATUS_WIFI_SSID");
    mqttConnector->registerPublish(wifiSsidElement, "/sys/Wifi-Ssid");

    wifiRssiElement = new LongElement("SYS_STATUS_WIFI_RSSI");
    mqttConnector->registerPublish(wifiRssiElement, "/sys/Wifi-Rssi");

    heapSizeElement = new LongElement("SYS_STATUS_HEAP");
    mqttConnector->registerPublish(heapSizeElement, "/sys/Heapsize");

    openIot.registerModule(this);
}

void MqttStatus::setup() {
    mqttUpdateStatusTicker.attach(5, updateMqttStatus);
    updateMqttStatus();
}

void MqttStatus::loop(bool firstLoop) {
    ++loopCounter;
}

void MqttStatus::updateStatus() {
    upTimeSeconds += 5;
    long lps = static_cast<int>(1.0f * loopCounter / 5);
    loopCounter = 0;
    uint32_t heap = esp_get_free_heap_size();

    IPAddress ip = WiFi.localIP();
    sprintf(ipBuffer, "%u.%u.%u.%u ", ip[0], ip[1], ip[2], ip[3]);

    getLocalTime(&timeinfo);
    strftime(timebuffer, 32, "%d.%m.%Y %H:%M:%S", &timeinfo);

    unsigned long seconds = upTimeSeconds;
    unsigned long minutes = seconds / 60;
    seconds %= 60;
    unsigned long hours = minutes / 60;
    minutes %= 60;
    unsigned long days = hours / 24;
    hours %= 24;

    snprintf(uptimebuffer, 32, "%ld d, %02ld:%02ld:%02ld", days, hours, minutes, seconds);

    loopCounterElement->setValue(lps);
    localTimeElement->setValue(timebuffer);
    upTimeElement->setValue(uptimebuffer);
    ipAddressElement->setValue(ipBuffer);
    wifiSsidElement->setValue(WiFi.SSID());
    wifiRssiElement->setValue(WiFi.RSSI());
    heapSizeElement->setValue(heap);
}

MqttStatus *_mqttStatus = nullptr;

MqttStatus *getMqttStatus() {
    if (!_mqttStatus) {
        _mqttStatus = new MqttStatus();
    }
    return _mqttStatus;
}
