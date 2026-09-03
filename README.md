# embedded_intern

Staj sirasinda yapilan gomulu sistem calismalari. Hedef donanim **ESP8266**
(DENSi `ESP8266_Wifi_260720` kontrol karti) ve **APS EM205 V2.0** termal yazici.
Derleme/yukleme icin `arduino-cli` kullanilir.

## Ortam

- Linux (Pop!_OS)
- `arduino-cli` + `esp8266:esp8266` platformu
- FQBN: `esp8266:esp8266:generic`, port: `/dev/ttyUSB0`

Her sketch klasorunde `sketch.yaml` varsa `arduino-cli compile --upload`
parametresiz calisir.

## Klasorler

| Klasor | Icerik |
|---|---|
| `0-Docs/` | Notlar: arduino-cli komutlari, flash prosedurü, LittleFS kullanimi |
| `basic_wifi_panel/` | Basit WiFi kontrol paneli |
| `medium_wifi_panel/` | Gelistirilmis WiFi panel |
| `buzzer_cancel/` | Buzzer kontrolu |
| `lcd_write/` | LCD'ye yazma |
| `lcd_scroll_buttons/` | LCD kaydirma + buton okuma |
| `tus_test/` | Tus/buton testi |
| `GPIO2_Deneme/` | GPIO2 pin denemesi |
| `dp200-printer-basic/` | DP200 termal yazici - temel surus |
| `dp200-printer-intermediate/` | DP200 termal yazici - gelismis surus |
| `littleFS-deneme/` | LittleFS dosya sistemi denemesi (`data/` + `littlefs.bin`) |
| `serial-to-wifi/` | Seri porttan gelen veriyi web arayuzune aktarma |
| `esp-wifi-communication/` | Iki ESP arasi WiFi haberlesme (`esp-a`, `esp-b`) |
| `esp-wifi-sayac/` | ESP-A sunucu + ESP-B LCD sayac uygulamasi |
| `esp-wifi-web-socket/` | WebSocket uzerinden ESP haberlesmesi |
| `C-Deneme/`, `vaarg-deneme/` | Saf C denemeleri (varargs vb.) |

## Referans

`ESP8266_Wifi_260720.PDF` - kontrol kartinin 4 sayfalik semasi.

## WiFi bilgileri

Ag bilgileri kaynakta placeholder olarak birakilmistir. Kendi degerlerinle
degistir:

```c
const char* AP_SSID = "YOUR_WIFI_SSID";
const char* AP_PASS = "YOUR_WIFI_PASSWORD";
```
