#include <pgmspace.h>

#define SECRET
#define THINGNAME "CatFeeder"

const char WIFI_SSID[] = "SOMETHING";
const char WIFI_PASSWORD[] = "SOMETHING";
const char AWS_IOT_ENDPOINT[] = "SOMETHING.iot.us-east-1.amazonaws.com";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
SOMETHING
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
SOMETHING
-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
SOMETHING
-----END RSA PRIVATE KEY-----
)KEY";
