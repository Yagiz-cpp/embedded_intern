#include <stdio.h>
#include <stdarg.h>

void logla(const char *fmt, ...){
    char govde[128];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(govde, sizeof(govde), fmt, ap);
    va_end(ap);

    printf("[LOG], %s\n", govde);
}

int main(void){
    logla("Merhaba");
    logla("sayac=%d", 42);
    logla("ad=%s, yas=%d", "yagiz", 22);
    return 0;
}