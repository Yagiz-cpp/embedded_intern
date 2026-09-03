/*
 * printf AILESI -- snprintf ve vsnprintf farki
 *
 * Derle:  gcc -Wall -Wextra -o aile printf_ailesi.c && ./aile
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* ==================================================================
 * 1) AILENIN TAMAMI -- isimler harf harf anlam tasir
 *
 *   s = string   -> ekrana degil, char[] dizisine yaz
 *   n = number   -> en fazla n bayt yaz (tasma korumasi)
 *   v = va_list  -> argumanlari '...' yerine hazir listeden al
 *
 *   fonksiyon      nereye        sinir   argumanlar
 *   -----------    ----------    -----   -----------
 *   printf         ekrana         yok     ...
 *   sprintf        char[]         YOK     ...      <- TEHLIKELI
 *   snprintf       char[]         var     ...      <- gunluk kullanim
 *   vsnprintf      char[]         var     va_list  <- fonksiyon ICINDEN
 * ================================================================== */

/* ------------------------------------------------------------------
 * vsnprintf'e neden ihtiyac var: '...' aktarilamaz.
 * ------------------------------------------------------------------ */
void logla(const char *fmt, ...)
{
    char govde[64];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(govde, sizeof(govde), fmt, ap);   /* v'li surum SART */
    va_end(ap);

    printf("  [LOG] %s\n", govde);
}

/* Ayni isi snprintf ile yapmaya calisirsan:
 *
 *   void logla(const char *fmt, ...) {
 *       snprintf(govde, sizeof(govde), fmt, ...);   // DERLENMEZ
 *   }
 *
 * C'de '...' tasinabilir bir sey degil. Once va_list'e cevirmen lazim,
 * va_list kabul eden surumun adi da vsnprintf.
 */

/* ------------------------------------------------------------------
 * Tasmayi gozle gormek icin: iki komsu dizi
 * ------------------------------------------------------------------ */
struct Komsu {
    char birinci[16];
    char ikinci[16];
};

int main(void)
{
    char tampon[32];

    /* ============================================================== */
    printf("=== 1) AYNI CIKTI, FARKLI HEDEF ===\n\n");

    printf("  printf   -> ");
    printf("sayac=%d\n", 42);

    sprintf(tampon, "sayac=%d", 42);
    printf("  sprintf  -> tampon = \"%s\"\n", tampon);

    snprintf(tampon, sizeof(tampon), "sayac=%d", 42);
    printf("  snprintf -> tampon = \"%s\"\n", tampon);

    printf("  vsnprintf-> ");
    logla("sayac=%d", 42);

    /* ============================================================== */
    printf("\n=== 2) n PARAMETRESI NE YAPIYOR ===\n\n");

    char kucuk[10];
    snprintf(kucuk, sizeof(kucuk), "cok uzun bir metin burada");
    printf("  10 baytlik diziye 25 karakter yazdik:\n");
    printf("    sonuc = \"%s\"  (uzunluk %zu)\n", kucuk, strlen(kucuk));
    printf("    ^ 9 karakter + '\\0'  -- kesildi ama TASMADI\n");

    /* ============================================================== */
    printf("\n=== 3) snprintf NE DONDURUR ===\n\n");

    int n = snprintf(kucuk, sizeof(kucuk), "cok uzun bir metin burada");
    printf("  donen deger = %d\n", n);
    printf("  dizi boyutu = %zu\n", sizeof(kucuk));
    printf("  ^ 'sigsaydi %d karakter olurdu' der.\n", n);
    printf("    Bu sayi dizi boyutundan BUYUKSE metin kesilmis demektir:\n\n");

    if (n >= (int)sizeof(kucuk))
        printf("    if (n >= sizeof(dizi)) -> KESILDI  (%d >= %zu)\n",
               n, sizeof(kucuk));

    /* ============================================================== */
    printf("\n=== 4) sprintf NEDEN TEHLIKELI ===\n\n");

    struct Komsu k;
    strcpy(k.birinci, "AAAA");
    strcpy(k.ikinci,  "SAGLAM-VERI");

    printf("  Once:\n");
    printf("    birinci = \"%s\"\n", k.birinci);
    printf("    ikinci  = \"%s\"\n", k.ikinci);

    /* birinci 16 bayt ama 30 karakter yaziyoruz -- komsuya tasar */
    sprintf(k.birinci, "%s", "BU-METIN-ONALTI-BAYTTAN-UZUN!!");

    printf("\n  sprintf ile 30 karakter yazdiktan sonra:\n");
    printf("    birinci = \"%s\"\n", k.birinci);
    printf("    ikinci  = \"%s\"   <-- BOZULDU\n", k.ikinci);
    printf("\n  ^ Program cokmedi. Sessizce baska bir degiskeni ezdi.\n");
    printf("    Hata cok sonra, alakasiz bir yerde ortaya cikar.\n");

    /* Ayni is snprintf ile: */
    struct Komsu g;
    strcpy(g.birinci, "AAAA");
    strcpy(g.ikinci,  "SAGLAM-VERI");
    snprintf(g.birinci, sizeof(g.birinci), "%s", "BU-METIN-ONALTI-BAYTTAN-UZUN!!");

    printf("\n  Ayni is snprintf ile:\n");
    printf("    birinci = \"%s\"  (kesildi)\n", g.birinci);
    printf("    ikinci  = \"%s\"   <-- SAGLAM\n", g.ikinci);

    /* ============================================================== */
    printf("\n=== 5) KURAL ===\n\n");
    printf("  Normal kod icinde        -> snprintf\n");
    printf("  '...' alan fonksiyon ici -> vsnprintf\n");
    printf("  sprintf                  -> HIC KULLANMA\n");

    return 0;
}