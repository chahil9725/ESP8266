#include <pgmspace.h>
#include "ESPWebMQTT.h"

/*
 * ESPWebMQTTBase class implementation
 */

ESPWebMQTTBase::ESPWebMQTTBase() : ESPWebBase() {
  _espClient = new WiFiClient();
  pubSubClient = new PubSubClient(*_espClient);
}

void ESPWebMQTTBase::setupExtra() {
  if (*_mqttServer) {
    pubSubClient->setServer(_mqttServer, _mqttPort);
    pubSubClient->setCallback(std::bind(&ESPWebMQTTBase::mqttCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  }
}

void ESPWebMQTTBase::loopExtra() {
  if (*_mqttServer && ((WiFi.getMode() == WIFI_STA) && (WiFi.status() == WL_CONNECTED))) {
    if (! pubSubClient->connected())
      mqttReconnect();
    if (pubSubClient->connected())
      pubSubClient->loop();
  }
}

bool ESPWebMQTTBase::readConfig(uint16_t &offset) {
  if (! ESPWebBase::readConfig(offset))
    return false;

  uint16_t start = offset;

  if ((! readEEPROM(offset, (uint8_t*)_mqttServer, sizeof(_mqttServer))) || (! readEEPROM(offset, (uint8_t*)&_mqttPort, sizeof(_mqttPort))) ||
    (! readEEPROM(offset, (uint8_t*)_mqttUser, sizeof(_mqttUser))) || (! readEEPROM(offset, (uint8_t*)_mqttPassword, sizeof(_mqttPassword))) ||
    (! readEEPROM(offset, (uint8_t*)_mqttClient, sizeof(_mqttClient)))) {
    _log->println(F("Error reading from EEPROM!"));
    defaultConfig(1);
    return false;
  }

  uint8_t crc = crc8EEPROM(start, offset);
  if (readEEPROM(offset) != crc) {
    _log->println(F("CRC mismatch! Use default MQTT parameters."));
    defaultConfig(1);
    return false;
  }

  return true;
}

bool ESPWebMQTTBase::writeConfig(uint16_t &offset, bool commit) {
  if (! ESPWebBase::writeConfig(offset, false))
    return false;

  uint16_t start = offset;

  if ((! writeEEPROM(offset, (uint8_t*)_mqttServer, sizeof(_mqttServer))) || (! writeEEPROM(offset, (uint8_t*)&_mqttPort, sizeof(_mqttPort))) ||
    (! writeEEPROM(offset, (uint8_t*)_mqttUser, sizeof(_mqttUser))) || (! writeEEPROM(offset, (uint8_t*)_mqttPassword, sizeof(_mqttPassword))) ||
    (! writeEEPROM(offset, (uint8_t*)_mqttClient, sizeof(_mqttClient)))) {
    _log->println(F("Error writing to EEPROM!"));
    return false;
  }

  uint8_t crc = crc8EEPROM(start, offset);
  if (! writeEEPROM(offset, crc)) {
    _log->println(F("Error writing CRC!"));
    return false;
  }
  if (commit)
    commitConfig();

  return true;
}

void ESPWebMQTTBase::defaultConfig(uint8_t level) {
  if (level < 1)
    ESPWebBase::defaultConfig(level);

  if (level < 2) {
    memset(_mqttServer, 0, sizeof(_mqttServer));
    _mqttPort = defMQTTPort;
    memset(_mqttUser, 0, sizeof(_mqttUser));
    memset(_mqttPassword, 0, sizeof(_mqttPassword));
    String mqttClient = FPSTR(defMQTTClient);
    mqttClient += getBoardId();
    memset(_mqttClient, 0, sizeof(_mqttClient));
    strncpy(_mqttClient, mqttClient.c_str(), sizeof(_mqttClient) - 1);
  }
}

bool ESPWebMQTTBase::setConfigParam(const String &name, const String &value) {
  if (! ESPWebBase::setConfigParam(name, value)) {
    if (name.equals(FPSTR(paramMQTTServer))) {
      memset(_mqttServer, 0, sizeof(_mqttServer));
      strncpy(_mqttServer, value.c_str(), sizeof(_mqttServer) - 1);
    } else if (name.equals(FPSTR(paramMQTTPort)))
      _mqttPort = value.toInt();
    else if (name.equals(FPSTR(paramMQTTUser))) {
      memset(_mqttUser, 0, sizeof(_mqttUser));
      strncpy(_mqttUser, value.c_str(), sizeof(_mqttUser) - 1);
    } else if (name.equals(FPSTR(paramMQTTPassword))) {
      memset(_mqttPassword, 0, sizeof(_mqttPassword));
      strncpy(_mqttPassword, value.c_str(), sizeof(_mqttPassword) - 1);
    } else if (name.equals(FPSTR(paramMQTTClient))) {
      memset(_mqttClient, 0, sizeof(_mqttClient));
      strncpy(_mqttClient, value.c_str(), sizeof(_mqttClient) - 1);
    } else
      return false;
  }

  return true;
}

void ESPWebMQTTBase::setupHttpServer() {
  ESPWebBase::setupHttpServer();
  httpServer->on(String(FPSTR(pathMQTT)).c_str(), std::bind(&ESPWebMQTTBase::handleMQTTConfig, this));
}

void ESPWebMQTTBase::handleRootPage() {
  if (! userAuthenticate())
    return;

  String script = F("function refreshData() {\n\
var request = getXmlHttpRequest();\n\
request.open('GET', '");
  script += FPSTR(pathData);
  script += F("?dummy=' + Date.now(), true);\n\
request.onreadystatechange = function() {\n\
if (request.readyState == 4) {\n\
var data = JSON.parse(request.responseText);\n");
  script += FPSTR(getElementById);
  script += FPSTR(jsonMQTTConnected);
  script += F("').innerHTML = (data.");
  script += FPSTR(jsonMQTTConnected);
  script += F(" != true ? \"not \" : \"\") + \"connected\";\n");
  script += FPSTR(getElementById);
  script += FPSTR(jsonFreeHeap);
  script += F("').innerHTML = data.");
  script += FPSTR(jsonFreeHeap);
  script += F(";\n");
  script += FPSTR(getElementById);
  script += FPSTR(jsonUptime);
  script += F("').innerHTML = data.");
  script += FPSTR(jsonUptime);
  script += F(";\n");
  if (WiFi.getMode() == WIFI_STA) {
    script += FPSTR(getElementById);
    script += FPSTR(jsonRSSI);
    script += F("').innerHTML = data.");
    script += FPSTR(jsonRSSI);
    script += F(";\n");
  }
  script += F("}\n\
}\n\
request.send(null);\n\
}\n\
setInterval(refreshData, 1000);\n");

  String page = ESPWebBase::webPageStart(F("ESP8266"));
  page += ESPWebBase::webPageStdStyle();
  page += ESPWebBase::webPageStdScript();
  page += ESPWebBase::webPageScript(script);
  page += ESPWebBase::webPageBody();
  page += F("<h3>ESP8266</h3>\n\
<p>\n\
MQTT broker: <span id=\"");
  page += FPSTR(jsonMQTTConnected);
  page += F("\">?</span><br/>\n\
Heap free size: <span id=\"");
  page += FPSTR(jsonFreeHeap);
  page += F("\">0</span> bytes<br/>\n\
Uptime: <span id=\"");
  page += FPSTR(jsonUptime);
  page += F("\">0</span> seconds<br/>\n");
  if (WiFi.getMode() == WIFI_STA) {
    page += F("Signal strength: <span id=\"");
    page += FPSTR(jsonRSSI);
    page += F("\">?</span> dBm<br/>\n");
  }
  page += F("<p>\n");
  page += navigator();
  page += ESPWebBase::webPageEnd();

  httpServer->send(200, FPSTR(textHtml), page);
}

void ESPWebMQTTBase::handleMQTTConfig() {
  if (! adminAuthenticate())
    return;

  String script = F("function trimString(field) {\n\
field.value = field.value.trim();\n\
}\n\
function fixNumber(field, minvalue, maxvalue) {\n\
var val = parseInt(field.value);\n\
if (isNaN(val) || (val < minvalue))\n\
field.value = minvalue;\n\
else\n\
if (val > maxvalue)\n\
field.value = maxvalue;\n\
}\n");

  String page = ESPWebBase::webPageStart(F("MQTT Setup"));
  page += ESPWebBase::webPageStdStyle();
  page += ESPWebBase::webPageScript(script);
  page += ESPWebBase::webPageBody();
  page += F("<form name=\"mqtt\" method=\"GET\" action=\"");
  page += FPSTR(pathStore);
  page += F("\">\n\
<h3>MQTT Setup</h3>\n\
<label>Server:</label><br/>\n");
  page += ESPWebBase::tagInput(FPSTR(typeText), FPSTR(paramMQTTServer), _mqttServer, F("onblur=\"trimString(this)\""), MAX_STRING_LEN, MAX_STRING_LEN);
  page += F("\n(leave blank to ignore MQTT)<br/>\n\
<label>Port:</label><br/>\n");
  page += ESPWebBase::tagInput(FPSTR(typeText), FPSTR(paramMQTTPort), String(_mqttPort), F("onblur=\"fixNumber(this, 0, 65535)\""), 5, 5);
  page += F("<br/>\n\
<label>User:</label><br/>\n");
  page += ESPWebBase::tagInput(FPSTR(typeText), FPSTR(paramMQTTUser), _mqttUser, F("onblur=\"trimString(this)\""), MAX_STRING_LEN, MAX_STRING_LEN);
  page += F("\n(leave blank if authorization is not required)<br/>\n\
<label>Password:</label><br/>\n");
  page += ESPWebBase::tagInput(FPSTR(typePassword), FPSTR(paramMQTTPassword), _mqttPassword, MAX_STRING_LEN, MAX_STRING_LEN);
  page += F("<br/>\n\
<label>Client:</label><br/>\n");
  page += ESPWebBase::tagInput(FPSTR(typeText), FPSTR(paramMQTTClient), _mqttClient, F("onblur=\"trimString(this)\""), MAX_STRING_LEN, MAX_STRING_LEN);
  page += F("\n\
<p>\n");
  page += ESPWebBase::tagInput(FPSTR(typeSubmit), strEmpty, F("Save"));
  page += charLF;
  page += btnBack();
  page += ESPWebBase::tagInput(FPSTR(typeHidden), FPSTR(paramReboot), "1");
  page += F("\n\
</form>\n");
  page += ESPWebBase::webPageEnd();

  httpServer->send(200, FPSTR(textHtml), page);
}

String ESPWebMQTTBase::jsonData() {
  String result = ESPWebBase::jsonData();
  result += F(",\"");
  result += FPSTR(jsonMQTTConnected);
  result += F("\":");
  if (pubSubClient->connected())
    result += FPSTR(bools[1]);
  else
    result += FPSTR(bools[0]);

  return result;
}

String ESPWebMQTTBase::btnMQTTConfig() {
  String result = ESPWebBase::tagInput(FPSTR(typeButton), strEmpty, F("MQTT Setup"), String(F("onclick=\"location.href='")) + String(FPSTR(pathMQTT)) + String(F("'\"")));
  result += charLF;

  return result;
}

String ESPWebMQTTBase::navigator() {
  String result = btnWiFiConfig();
  result += btnTimeConfig();
  result += btnMQTTConfig();
  result += btnLog();
  result += btnReboot();

  return result;
}

bool ESPWebMQTTBase::mqttReconnect() {
  const uint32_t timeout = 30000;

  static uint32_t nextTime;
  bool result = false;

  if ((int32_t)(millis() - nextTime) >= 0) {
    _log->print(F("Attempting MQTT connection..."));
#ifdef LED_PIN
    enablePulse(PULSE);
#endif
    if (*_mqttUser)
      result = pubSubClient->connect(_mqttClient, _mqttUser, _mqttPassword);
    else
      result = pubSubClient->connect(_mqttClient);
#ifdef LED_PIN
//    enablePulse(WiFi.getMode() == WIFI_STA ? BREATH : FADEIN);
    enablePulse(BREATH);
#endif
    if (result) {
      _log->println(F(" connected"));
      mqttResubscribe();
    } else {
      _log->print(F(" failed, rc="));
      _log->println(pubSubClient->state());
    }
    nextTime = millis() + timeout;
  }

  return result;
}

void ESPWebMQTTBase::mqttCallback(char *topic, byte *payload, unsigned int length) {
  _log->print(F("MQTT message arrived ["));
  _log->print(topic);
  _log->print(F("] "));
  for (uint16_t i = 0; i < length; ++i) {
    _log->print((char)payload[i]);
  }
  _log->println();
}

void ESPWebMQTTBase::mqttResubscribe() {
  String topic;

  if (*_mqttClient) {
    topic += charSlash;
    topic += _mqttClient;
    topic += F("/#");
    mqttSubscribe(topic);
  }
}

bool ESPWebMQTTBase::mqttSubscribe(const String &topic) {
  _log->print(F("MQTT subscribe to topic \""));
  _log->print(topic);
  _log->println('\"');

  return pubSubClient->subscribe(topic.c_str());
}

bool ESPWebMQTTBase::mqttPublish(const String &topic, const String &value, bool retained) {
  _log->print(F("MQTT publish topic \""));
  _log->print(topic);
  _log->print(F("\" with value \""));
  _log->print(value);
  _log->println('\"');

  return pubSubClient->publish(topic.c_str(), value.c_str(), retained);
}
