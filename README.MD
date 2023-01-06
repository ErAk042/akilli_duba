# Akıllı Duba

Izmir Fen Lisesi öğrencileri tarafından 2204-A Lise Öğrencileri Araştırma Projeleri Yarışması için tasarlanan ve üretilen, içinde bulunduğu su ortamından
alg miktarı, iletkenlik, bulanıklık ve sıcaklık gibi parametrelerin ölçümünü yaparak bu verileri gsm aracılığı ile bir web veritabanına yükleyen duba.

## Genel Mekanik Yapı

Dubamızın ana gövdesi olarak elektronik sistemlerin içinde durduğu bir adet su geçirmez kutu kullanılmıştır. Fotodiyot ve LEDlerin bulunduğu optik sensör
bloğu kutunun içinde yer almaktadır. Bu seyede karanlık odanın mekanik yapısı daha basite indirgenebilmektedir ve su geçirmezlik açıısndan daha az muhtamel
hata noktası oluşmaktadır. Yapılacak ölçümlerin gün ışığından etkilenmemesi için kutunun içindeki bir servoya mıknatıslar ile bağlanan ve bu servo aracılığıyla döndürülerek örneğin gireceği delikleri açıp kapayan bir alt mekanizma tasarlandı. Bu sayede ölçümlerde gün ışığından gelen gürültü miktarının en aza indirilmesi amaçlanmıştır. 

## Optik ve Analog Kısım

Dubada, sudaki klrofil miktarı ölçümü için bir adet BPW34 fotodiyot kullanılmıştır. Bunun seçilmesinin temel sebeplerinden biri yüksek hassasiyete ve geiş bir ışığa duyarlı alana sahip olmasıdır. Bulanıklık sensöründe ise üstünde kızılötesi filtreleyen reçineden yapılma lens olması ve yüksek hassasiyet sebebiyle ------- kullanılmıştır. Bu iki fotodiyodun da çıkışına sinyali yükseltme amaçlı düşük gürültülü sabit kazançlı(24x) ADPD2210 fotodiyot yükseltici entegresi kullanılmıştır. Bu fotodiyotlardan gelen sinyalleri arduinonun okuyabileceği bir dijital veriye çevirmesi için küçük ayak izine sahip ve I2C bus ile kolayca kullanılabilen ADS1115 analogtan dijitale çevirici entegresi kullanılmıştır. Sensörlerin durması gereken pozisyon neeniyle BPW34 ve ona ait yükseltici ayrı bir karta monte edilmiş ve bu kart ana karta bir adet 90° pin header ile bağlanmıştır. Ana kartta ayrıca kızılötesi fotodiyot için bir adet arduinonun dijital pinleri tarafından kontrol edilebilen sabit akım kaynağı kullanılmıştır. UV ledler için pilin gerilimi ile ledlerin çalışma gerilimi arasındaki yakınlık sebebiyle sadce arduinonun dijital pinleri ile açıp kapayabilmeyi sağlamak amacıyla bir mosfet konmuştur.
![kart](/resimler/Ekran Alıntısı.jpg "kart")
