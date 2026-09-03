



void satirIsle (String s){
    int p = s.indexOf('|');
    String komut = (p < 0) ? s : s.substring( 0 , p );
    String arg = (p < 0) ? "" : s.substring(p+1);

    arg.replace("{NO}", String(fisNo)) ; 
    
    if (komut == "METIN"){
        Serial1.println(arg);
    } 
    else if (komut == "CIZGI"){
        char c = arg.length() ? arg[0] : '-';
        for (uint8_t i = 0 ; i<32 ; i++) Serial1.write(c);
        Serial1.write('\n');
    }
    else if (komut == "BASLIK"){
        
    }
}