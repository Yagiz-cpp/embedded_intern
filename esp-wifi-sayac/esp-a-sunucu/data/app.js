async function guncelle(){
  try{
    const cevap = await fetch('/veri');
    const metin = await cevap.text();
    const parca = metin.split(';');

    document.getElementById('sayi').textContent = parca[0];
    document.getElementById('durum').textContent = parca[1]=== '1' ? 'calisiyor':'durdu';

  }catch(e){
    document.getElementById('durum').textContent = 'baglanilamadi';
  }
}

async function logGuncelle(){
  try{
    const cevap = await fetch('/log');
    const kutu = document.getElementById('log');

    kutu.textContent = await cevap.text();
  }catch(e){}
}

async function komut(c){
  await fetch('/komut?c=' +c);
  guncelle();
}

setInterval(guncelle, 500);
setInterval(logGuncelle,3000);
guncelle();
logGuncelle();