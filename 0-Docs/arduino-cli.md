# Arduino Cli Komutlari

## Kart ve Platform Isleri

- arduino-cli core list                        # kurulu platformlar
- arduino-cli core search esp8266              # platform ara
- arduino-cli core install esp8266:esp8266     # kur
- arduino-cli board listall esp8266            # o platformdaki tüm kartlar + FQBN'leri
- arduino-cli board details --fqbn <fqbn>      # kartın derleme seçenekleri
- arduino-cli board list                       # takılı kartları ve portlarını göster

## Derleme ve Yukleme
- arduino-cli compile                  # sketch.yaml varsa parametresiz yeter
- arduino-cli compile --upload         # derle + yükle
- arduino-cli compile --warnings all   # tüm uyarıları göster
- arduino-cli compile -v               # ayrıntılı çıktı, hata ayıklarken
- arduino-cli upload                   # sadece yükle
- arduino-cli monitor -c baudrate=115200

## Kutuphane Isler
- arduino-cli lib search <kelime>      # kütüphane ara
- arduino-cli lib install "<isim>"     # kur
- arduino-cli lib list                 # kurulu olanları listele
- arduino-cli lib update-index         # kütüphane listesini tazele
- arduino-cli lib upgrade              # hepsini güncelle

### Genel
- arduino-cli config dump              # ayarları ve klasör yollarını gör
- arduino-cli sketch new <isim>        # yeni sketch iskeleti
- arduino-cli version

