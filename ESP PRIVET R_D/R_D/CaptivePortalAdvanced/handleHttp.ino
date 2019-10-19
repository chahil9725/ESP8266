/** Handle root or redirect to captive portal */
void handleRoot() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.sendContent(
    "<html><head></head><body>"
    "<h1><center>WIFI SMART CAR AKA PROJECTXBOT!!</center></h1>"
  );
  if (server.client().localIP() == apIP) {
    server.sendContent(String("<center><p>XBOT IS Connected through The Hotspot ") + softAP_ssid + "</p></center>");
        server.sendContent(String("<p><center>This PROJECTXBOT is Fully Managed By Chahil9725@gmail.com<br></p> </center> "));
    
  } else {
    server.sendContent(String("<p><center>XBOT IS Connected through The  Wifi network: ") + ssid + "</p></center>");
  }
  server.sendContent(
    "<center><p>You may want to <a href='/wifi'>config the wifi connection</a>.</p></center>"
    "</body></html>"
  );
  server.client().stop(); // Stop is needed because we sent no content length
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname)+".local")) {
    Serial.print("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

/** Wifi config page handler */
void handleWifi() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.sendContent(
    "<html><head></head><body>"
    "<h1><center>Wifi config</center></h1>"
  );
  if (server.client().localIP() == apIP) {
    server.sendContent(String("<p><center>You are connected through the soft AP: ") + softAP_ssid + "</p></center>");
  } else {
    server.sendContent(String("<center><p>You are connected through the wifi network: ") + ssid + "</p></center>");
  }
  server.sendContent(
    "\r\n<br />"
    "<center><table><tr><th align='center'>SoftAP config</th></tr></center>"
  );
  server.sendContent(String() + "<center><tr><td>SSID " + String(softAP_ssid) + "</td></tr></center>");
  server.sendContent(String() + "<center><tr><td>IP " + toStringIp(WiFi.softAPIP()) + "</td></tr></center>");
  server.sendContent(
    "</table>"
    "\r\n<br />"
    "<table><tr><th align='center'>WLAN config</th></tr>"
  );
  server.sendContent(String() + "<center><tr><td>SSID " + String(ssid) + "</td></tr></center>");
  server.sendContent(String() + "<center><tr><td>IP " + toStringIp(WiFi.localIP()) + "</td></tr></center>");
  server.sendContent(
    "</table>"
    "\r\n<br />"
    "<table><tr><th align='center'>WLAN list (refresh if any missing)</th></tr>"
  );
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      server.sendContent(String() + "\r\n<center><tr><td>SSID " + WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":" *") + " (" + WiFi.RSSI(i) + ")</td></tr></center>");
    }
  } else {
    server.sendContent(String() + "<tr><td><center>No WLAN found</center></td></tr>");
  }
  server.sendContent(
    "</table>"
    "\r\n<br /><form method='POST' action='wifisave'><h4><center>Connect to network:</center></h4>"
    "<center><input type='text' placeholder='network' name='n'/></center>"
    "<center><br /><input type='password' placeholder='password' name='p'/></center>"
    "<center><br /><input type='submit' value='Connect/Disconnect'/></form></center>"
    "<center><p>You may want to <a href='/'>return to the home page</a>.</p></center>"
    "</body></html>"
  );
  server.client().stop(); // Stop is needed because we sent no content length
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  Serial.println("wifi save");
  server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  saveCredentials();
  connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}

void handleNotFound() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 404, "text/plain", message );
}

