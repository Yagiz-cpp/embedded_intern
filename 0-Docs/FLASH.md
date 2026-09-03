# dp200-printer

DENSi ESP8266_Wifi_260720 kontrol karti + APS EM205 V2.0 termal yazici.
Kaynak: `ESP8266_Wifi_260720.PDF` (4 sayfa sema) + fiziksel olcum.

---

## 1. Flash / Yukleme

**FQBN:** `esp8266:esp8266:generic`

`sketch.yaml`:
```yaml
default_fqbn: esp8266:esp8266:generic
default_port: /dev/ttyUSB0
```

**Flash moduna girme:**
GPIO0 = PRINT butonu (soldan 4.). Butona basili tutarken reset -> birak -> upload.
Ayri jumper/pad kisa devresine gerek yok.

```bash
arduino-cli compile --upload
arduino-cli monitor -c baudrate=115200
```

Seri monitor acikken upload "Resource busy" verir; once monitoru kapat.

---

## 2. Pin haritasi

| GPIO | Baglanti | Not |
|------|----------|-----|
| 0 | PRINT butonu (SW604) | Boot pini - acilista basili = flash modu |
| 1 (TX0) | RS232 / MAX232 (U101) | USB debug hatti |
| 2 (TX1) | **Yazici veri hatti** | Sadece TX, `Serial1`, 9600 baud |
| 3 (RX0) | RS232 / MAX232 (U101) | USB debug hatti |
| 4 | I2C SDA | LCD + RTC ortak hat |
| 5 | I2C SCL | LCD + RTC ortak hat |
| 6-11 | Dahili SPI flash | KULLANMA |
| 12 | Buzzer (Q101 uzerinden) | Cikis, aktif HIGH |
| 13 | Kagit kontrol | Interrupt pini - buton DEGIL |
| 14 | M+ butonu (SW603) | |
| 15 | TM1638 SDI / DP3_15 | Dahili pull-down var |
| 16 | SET butonu (SW602) | Dahili pull-up YOK, kesme YOK |
| A0 | Besleme voltaji olcumu | R201 150K / R202 100K bolucu, VCC_IN'den |

**FEED butonu (SW601, soldan 1.) hicbir GPIO'ya bagli degil** -
`PRN_FEED` sinyali dogrudan EM205'in tus girisine (CN106) gidiyor.
Yazilimdan okunamaz.

---

## 3. Yazici (EM205 V2.0)

| | |
|---|---|
| Baud | 9600 |
| Protokol | ESC/POS |
| Karakter seti | CP437 (Turkce karakter YOK) |
| Cozunurluk | 8 nokta/mm, 384 nokta/satir, 48mm baski genisligi |
| Firmware | S1.06 (2021-08-10) |
| Besleme | 6.7V olculdu (sema: VCC_6.8 Thermal hatti) |

**Self-test:** KEY butonuna basili tutarken guc ver.
Cikti baud, voltaj, kafa sicakligi ve karakter tablosunu basar.

**Baglanti:** ESP GPIO2 -> JP105 jumper -> CN104 -> yazici J3 (TTL)
CN104 5-pin: GND(siyah), EM_TX(kirmizi), EM_RX(sari), EM_DSR(yesil), EM_DTR(mavi)

> **JP105 jumper'i onemli:** TXD1 sinyalini ya CN104'e (TTL, yazici) ya da
> MAX232'ye (`TXD1_#`, RS232) yonlendiriyor. Yanlis konumdaysa GPIO2'den
> gonderilen hicbir sey yaziciya ulasmaz.

**Bilinen sorun:** ESP her acilista GPIO2'den 74880 baud bootloader ciktisi
basar. Yazici 9600 dinledigi icin bunu cop karakter olarak kagida doker.
ROM'dan geldigi icin yazilimla susturulamaz. Fis basmadan once
`ESC @` (0x1B 0x40) ile tamponu temizle.

### Kullanilan ESC/POS komutlari

| Komut | Bayt | Anlam |
|-------|------|-------|
| ESC @ | `1B 40` | Reset / init |
| ESC a n | `1B 61 n` | Hizalama: 0=sol 1=orta 2=sag |
| GS ! n | `1D 21 n` | Boyut: 0x00 normal, 0x11 cift boy+en |
| ESC E n | `1B 45 n` | Kalin: 1 acik, 0 kapali |
| ESC d n | `1B 64 n` | n satir kagit ilerlet |
| GS V m | `1D 56 00` | Kagit kesme |

Komutlar `write()` ile ham bayt olarak gonderilir, `print()` ile DEGIL.

---

## 4. LCD

| | |
|---|---|
| Modul | RX1602A3 (CN603), 2x16 |
| Kontrolcu | ST7032 / AiP31068 ailesi |
| I2C adresi | 0x3E (sabit, kutuphane iceride tutuyor) |
| Kutuphane | `LCD_ST7032` v1.0.2 |

```cpp
LCD_ST7032 lcd;          // constructor parametre ALMAZ
lcd.begin();             // parametresiz
lcd.setcontrast(30);     // kucuk c - 0-63 arasi, deneyerek ayarla
lcd.setCursor(satir, sutun);   // LiquidCrystal'in TERSI sira
```

`backlight()` / `noBacklight()` YOK - arka isik I2C uzerinden kontrol
edilmiyor. J619 jumper'i LCD beslemesini seciyor: kapali = 5V, acik = 3.3V.

---

## 5. RTC

| | |
|---|---|
| Cip | DS1307Z (U102) |
| I2C adresi | 0x68 |
| Kristal | Y101, 32768 Hz |
| Pil | BS-1220 (B101), D101 + R101 uzerinden sarj hatti |

LCD ile ayni I2C hattinda (GPIO4/5). Kutuphane: `RTClib`.

---

## 6. Guc

- Giris: VCC_IN 6.8V - 12V (etiket: DC 7.1V / 5A)
- Buck: XL4005 (U205) -> VCC7
- 5V: XC62FP5002 (U203)
- 3.3V: XC62FP3302 (U202) -> ESP
- Sigorta: FUS201 3A
- Filtreleme: C204 680uF, C207/C212 220uF

Termal kafa baski aninda yuksek akim ceker; kart bunun icin tasarlanmis.

---

## 7. Diger / kullanilmayan

- **Micro SD** (P201): sema uzerinde var, VCC_SD hatti ayri regulatorlu.
  Kartta populate edilmis mi kontrol edilmedi.
- **CN105** (SIP4-F-2MM): OLED cikisi, SCL/SDA
- **CN110** (TER-BLOCK 5P): opsiyonel 3 tuslu harici klavye
- **CN101** (SIP3-F-2MM): opsiyonel harici ADC girisi
- **P101 / P102** (RJ/TEL6HP): RS232 seri portlar, MAX232 uzerinden

---

## 8. Kutuphaneler

```bash
arduino-cli lib install "LCD_ST7032"
arduino-cli lib install "RTClib"
```

---

## 9. Acik konular

- [ ] SD kart soketi kartta mevcut mu, kullanilacak mi
- [ ] GPIO13 kagit sensoru mantigi (kagit varken HIGH mi LOW mu)
- [ ] GPIO15 / TM1638 hatti bu uygulamada kullanilacak mi
- [ ] CP857 (Turkce) kod sayfasi firmware tarafindan destekleniyor mu
      (`ESC t 13` = `1B 74 0D` ile denenebilir)