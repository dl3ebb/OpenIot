
#include "Ntp.h"
#include <OpenIot.h>
/**
 * Set up a connection to a NTP server
 */
#include "esp_sntp.h"
#include "time.h"

//--- Update every 60 Minutes -----------------------------
#define sntpUpdateInterval 60L * 60 * 1000

/**
 * Callback if an NTP update is received
 */
void timeavailable(struct timeval *t) {
    char timebuffer[32];
    ntp->printLocalTime(timebuffer, 32, "%d.%m.%Y %H:%M:%S");
    logger->info(F("NTP update : %s"), timebuffer);
}

Ntp::Ntp() : Module ()  {
    openIot.registerModule (this);
}

void Ntp::setup() {
    if (enable->getValue()) {
        sntp_set_sync_interval(sntpUpdateInterval);
        sntp_set_time_sync_notification_cb(timeavailable);
        configTime(
            offsetGmt->getValue(),
            offsetDst->getValue(),
            strdup(server1->getValue().c_str()),
            strdup(server2->getValue().c_str()),
            strdup(server3->getValue().c_str()));
    }
    logSettings();
}

void Ntp::printLocalTime(char *buf, int maxlen, const char *format) {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    strftime(buf, maxlen, format, &timeinfo);
}

void Ntp::logSettings() {
    logger->info(F("---- Ntp settings : ------"));
    logger->info(F("Enabled               : %s"), (enable->getValue() ? "true" : "false"));
    logger->info(F("Server1               : %s"), server1->getValue().c_str());
    logger->info(F("Server2               : %s"), server2->getValue().c_str());
    logger->info(F("Server3               : %s"), server3->getValue().c_str());
    logger->info(F("Offset Gmt (sec)      : %d"), offsetGmt->getValue());
    logger->info(F("Offset Dst (sec)      : %d"), offsetDst->getValue());
}

Ntp *_ntp = nullptr;

Ntp *getNtp() {
    if (!_ntp) {
        _ntp = new Ntp();
    }
    return _ntp;
}
