# ESP8266 + LittleFS (arduino-cli ile)

Linux / Pop!_OS · arduino-cli · esptool v5 · ESP8266 generic (4MB flash)

---

## 0. Özet

arduino-cli'nin dosya sistemi yükleme komutu **yoktur**. IDE'deki eklentinin yaptığı işi elle iki adımda yaparsın:

1. `mklittlefs` ile `data/` klasöründen bir imaj dosyası (`littlefs.bin`) üret
2. `esptool` ile bu imajı flash'ın FS bölgesine yaz

Sketch ve dosya sistemi flash'ın **ayrı bölgelerinde** durur. Birini yüklemek diğerini silmez.

```
0x000000 ┌──────────────────┐
         │  bootloader      │
         │  sketch (.bin)   │  ← arduino-cli upload
0x200000 ├──────────────────┤
         │  LittleFS        │  ← esptool write-flash
0x3FA000 ├──────────────────┤
         │  RF cal / SDK    │
0x400000 └──────────────────┘   (4MB, eesz=4M2M)
```

---

## 1. Ön hazırlık

### 1.1 esptool kurulumu

`esptool.py: command not found` alıyorsan sistemde kurulu değil demektir. arduino-cli çekirdeğin içindeki kendi kopyasını kullandığı için `upload` çalışır ama terminalden erişemezsin.

Pop!_OS / Ubuntu'da `pip install esptool` PEP 668 yüzünden reddedilir, `pipx` kullan:

```bash
sudo apt install pipx
pipx install esptool
pipx ensurepath
source ~/.bashrc
```

Doğrula:

```bash
esptool version
```

> **esptool v5 notu:** Komut adı artık `esptool` (`.py` yok) ve alt komutlar tire ile yazılır: `write-flash`, `flash-id`, `erase-flash`. Eski `write_flash` biçimi hâlâ çalışır ama "deprecated" uyarısı verir.

Kurmak istemezsen çekirdekle gelen kopyayı tam yolla çağırabilirsin:

```bash
find ~/.arduino15/packages/esp8266 -name "esptool.py"
```

### 1.2 mklittlefs

ESP8266 çekirdeğiyle birlikte gelir, PATH'te değildir:

```bash
ls ~/.arduino15/packages/esp8266/tools/mklittlefs/*/mklittlefs
```

Yolu not al, her seferinde tam yolla çağıracaksın.

> `mkspiffs` **değil**. SPIFFS imajı LittleFS ile okunmaz.

---

## 2. Modülün flash boyutunu öğren

```bash
esptool --port /dev/ttyUSB0 flash-id
```

Örnek çıktı:

```
esptool v5.3.1
Connected to ESP8266 on /dev/ttyUSB0:
Chip type:          ESP8266EX
Crystal frequency:  26MHz
MAC:                c4:d8:d5:1a:51:90

Flash Memory Information:
Manufacturer: 5e
Device: 4016
Detected flash size: 4MB
```

`Detected flash size` değerine göre bir sonraki adımda `eesz` seçersin.

---

## 3. FQBN'i sabitle

**En kritik nokta.** `esp8266:esp8266:generic` seçenek verilmezse `eesz=1M64` ile derlenir (1MB flash, 64KB FS) — çünkü boards.txt'te menünün ilk öğesi odur. 4MB'lık bir modülde bu hem alanı boşa harcar hem de adresler tutmaz.

4MB modül için:

```
esp8266:esp8266:generic:eesz=4M2M,xtal=80
```

Bunu **derlerken de yazarken de birebir aynı** kullan.

Tüm seçenekleri ve varsayılanları görmek için:

```bash
arduino-cli board details -b esp8266:esp8266:generic
```

### FS adres tablosu (`generic`)

| `eesz` | FS başlangıç | FS bitiş | Boyut (bayt) | blocksize | pagesize |
|---|---|---|---|---|---|
| `1M64` *(varsayılan)* | `0xEB000` | `0xFB000` | 65536 | 4096 | 256 |
| `1M128` | `0xDB000` | `0xFB000` | 131072 | 4096 | 256 |
| `1M256` | `0xBB000` | `0xFB000` | 262144 | 4096 | 256 |
| `1M512` | `0x7B000` | `0xFB000` | 524288 | 8192 | 256 |
| `2M1M` | `0x100000` | `0x1FA000` | 1024000 | 8192 | 256 |
| `4M1M` | `0x300000` | `0x3FA000` | 1024000 | 8192 | 256 |
| **`4M2M`** | **`0x200000`** | **`0x3FA000`** | **2072576** | **8192** | **256** |
| `4M3M` | `0x100000` | `0x3FA000` | 3121152 | 8192 | 256 |

`1M`, `2M`, `4M` (FS:none) seçeneklerinde FS bölgesi hiç ayrılmaz, LittleFS kullanılamaz.

---

## 4. Proje yapısı

```
littleFS-deneme/
├── littleFS-deneme.ino     ← klasör adıyla aynı olmalı
├── sketch.yaml             ← opsiyonel, profil tanımı
└── data/                   ← FS'e gidecek dosyalar
    ├── sayfa.html
    ├── style.css
    └── script.js
```

`data/` klasörü sketch klasörünün **içinde** olmalı.

### sketch.yaml (opsiyonel ama önerilir)

```yaml
profiles:
  esp:
    fqbn: esp8266:esp8266:generic:eesz=4M2M,xtal=80,baud=230400
    platforms:
      - platform: esp8266:esp8266
```

Sonra `arduino-cli compile --profile esp .` yeterli olur, FQBN'i her seferinde yazmazsın.

---

## 5. FS parametrelerini doğrula

Tabloyu ezberlemek yerine arduino-cli'ye sor:

```bash
arduino-cli compile -b esp8266:esp8266:generic:eesz=4M2M,xtal=80 --show-properties . | grep spiffs
```

```
build.spiffs_start=0x200000
build.spiffs_end=0x3FA000
build.spiffs_pagesize=256
build.spiffs_blocksize=8192
```

Boyut = `end - start`:

```bash
echo $(( 0x3FA000 - 0x200000 ))   # 2072576
```

> Property adları tarihsel nedenle `spiffs_*` şeklindedir ama LittleFS için de aynılarını kullanırsın.

---

## 6. İmajı oluştur

```bash
~/.arduino15/packages/esp8266/tools/mklittlefs/*/mklittlefs \
  -c data -p 256 -b 8192 -s 2072576 littlefs.bin
```

| Parametre | Anlamı |
|---|---|
| `-c data` | kaynak klasör |
| `-p 256` | pagesize |
| `-b 8192` | blocksize |
| `-s 2072576` | toplam imaj boyutu (bayt) |

Kontrol — boyut tam `-s` değeri kadar olmalı:

```bash
ls -l littlefs.bin
```

---

## 7. Flash'a yaz

```bash
esptool --chip esp8266 --port /dev/ttyUSB0 --baud 230400 \
  write-flash 0x200000 littlefs.bin
```

Adres, 5. adımdaki `spiffs_start` değeri.

Başarılı çıktı:

```
Compressed 2072576 bytes to 2411...
Wrote 2072576 bytes (2411 compressed) at 0x00200000 in 10.9 seconds
Hash of data verified.
```

### Baud tuzağı

`--baud 460800` çoğu CH340 çeviricide şu hatayı verir:

```
Changing baud rate to 460800... Changed.
A fatal error occurred: Unable to verify flash chip connection
(No more data to read from the serial port)
```

Çözüm: **230400** kullan (test edilmiş, çalışıyor). O da olmazsa 115200'e in.

Başka olası nedenler: kötü USB kablosu, USB hub, portu tutan başka bir program (`arduino-cli monitor` açıksa kapat).

---

## 8. Sketch'i derle ve yükle

```bash
arduino-cli compile -b esp8266:esp8266:generic:eesz=4M2M,xtal=80 .
arduino-cli upload  -b esp8266:esp8266:generic:eesz=4M2M,xtal=80 -p /dev/ttyUSB0 .
```

Yükleme baud hatası verirse FQBN'e `baud=230400` ekle.

---

## 9. Örnek kod

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

ESP8266WebServer server(80);

void setup() {
    Serial.begin(115200);
    delay(500);

    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount edilemedi!");
        return;
    }

    FSInfo i;
    LittleFS.info(i);
    Serial.printf("FS toplam: %u  kullanilan: %u\n", i.totalBytes, i.usedBytes);

    WiFi.mode(WIFI_AP);
    WiFi.softAP("LittleFS", "12345678");
    Serial.println(WiFi.softAPIP());

    // "/" -> data/sayfa.html
    server.serveStatic("/", LittleFS, "/sayfa.html");
    // data/ altindaki diger dosyalar (css, js, resim)
    server.serveStatic("/static", LittleFS, "/");

    server.onNotFound([]() {
        server.send(404, "text/plain", "yok");
    });

    server.begin();
}

void loop() {
    server.handleClient();
}
```

### Sık kullanılan LittleFS API

```cpp
LittleFS.begin();                       // mount
LittleFS.format();                      // tumunu sil
LittleFS.exists("/x.txt");
LittleFS.remove("/x.txt");
LittleFS.rename("/a.txt", "/b.txt");
LittleFS.mkdir("/klasor");

File f = LittleFS.open("/x.txt", "w");  // "r" "w" "a" "r+" "w+" "a+"
f.println("satir");
f.close();

f = LittleFS.open("/x.txt", "r");
String s = f.readString();
f.close();

Dir d = LittleFS.openDir("/");
while (d.next()) {
    Serial.printf("%s - %u bayt\n", d.fileName().c_str(), d.fileSize());
}

FSInfo info;
LittleFS.info(info);                    // totalBytes, usedBytes, blockSize...
```

---

## 10. Doğrulama

```bash
arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200
```

Beklenen:

```
FS toplam: 2072576  kullanilan: 24576
192.168.4.1
```

Sonra `LittleFS` ağına bağlan (şifre `12345678`), tarayıcıda `http://192.168.4.1`.

> Açılışta gelen bozuk karakterler normaldir — ESP8266'nın ROM bootloader'ı 74880 baud'da konuşur, sen 115200'de dinlersin. `Serial.begin()` çalıştıktan sonrası düzgün gelir.

---

## 11. Günlük kullanım

| Ne değişti | Ne yapmalı |
|---|---|
| `data/` içeriği | 6. + 7. adım (imaj + write-flash) |
| `.ino` kodu | 8. adım (compile + upload) |
| `eesz` değeri | 5-8 arası hepsi (adres değişti) |
| Hiçbiri tutmuyor | `esptool erase-flash` sonra baştan |

---

## 12. Sık yapılan hatalar

- **`eesz` derleme ile yazma arasında farklı** → sketch bir adrese bakar, FS başka adrestedir. En sık hata.
- **FQBN'de `eesz` hiç yazılmamış** → `generic` varsayılanı `1M64`'tür.
- **`mkspiffs` kullanmak** → LittleFS imajı değildir, mount edilmez.
- **`-s` parametresini unutmak** → imaj boyutu bölgeyle uyuşmaz.
- **`data/` boş veya dosyalar `touch` ile boş bırakılmış** → imaj oluşur ama sayfa boş gelir.
- **Seri monitör açıkken yazmaya çalışmak** → port meşgul.
- **460800 baud** → CH340'ta kopar, 230400 kullan.
- **`server.on(...)` / `serveStatic` tanımlamayı unutmak** → FS doğru olsa bile her istek 404 döner.

---

## 13. Faydalı komutlar

```bash
# Portu bul
arduino-cli board list

# Tum kart secenekleri ve varsayilanlari
arduino-cli board details -b esp8266:esp8266:generic

# Tum build property'leri
arduino-cli compile -b <FQBN> --show-properties .

# Flash'i tamamen sil (sketch + FS gider)
esptool --chip esp8266 --port /dev/ttyUSB0 erase-flash

# Flash'tan FS'i geri oku (yedek)
esptool --chip esp8266 --port /dev/ttyUSB0 \
  read-flash 0x200000 2072576 yedek.bin

# Imajin icerigini listele
~/.arduino15/packages/esp8266/tools/mklittlefs/*/mklittlefs -l littlefs.bin

# Imaji bir klasore acmak
~/.arduino15/packages/esp8266/tools/mklittlefs/*/mklittlefs -u cikti/ littlefs.bin
```

---

## 14. .gitignore

```gitignore
littlefs.bin
build/
```

---

## Kaynaklar

- [ESP8266 Arduino Core — Filesystem](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html)
- [esp8266/Arduino — boards.txt](https://github.com/esp8266/Arduino/blob/master/boards.txt)
- [esptool dokümantasyonu](https://docs.espressif.com/projects/esptool/en/latest/)
- [earlephilhower/arduino-littlefs-upload](https://github.com/earlephilhower/arduino-littlefs-upload) (Arduino IDE 2.x eklentisi, arduino-cli için gerekmez)
