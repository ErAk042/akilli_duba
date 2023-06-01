# Akıllı Duba

Izmir Fen Lisesi öğrencileri tarafından 2204-A Lise Öğrencileri Araştırma Projeleri Yarışması için tasarlanan ve üretilen, içinde bulunduğu su ortamından
alg miktarı, iletkenlik, bulanıklık ve sıcaklık gibi parametrelerin ölçümünü yaparak bu verileri gsm aracılığı ile bir web veritabanına yükleyen duba.

## Genel Mekanik Yapı

Dubamızın ana gövdesi olarak elektronik sistemlerin içinde durduğu bir adet su geçirmez kutu kullanılmıştır. Fotodiyot ve LEDlerin bulunduğu optik sensör
bloğu kutunun içinde yer almaktadır. Bu seyede karanlık odanın mekanik yapısı daha basite indirgenebilmektedir ve su geçirmezlik açıısndan daha az muhtamel
hata noktası oluşmaktadır. Yapılacak ölçümlerin gün ışığından etkilenmemesi için kutunun içindeki bir servoya mıknatıslar ile bağlanan ve bu servo aracılığıyla döndürülerek örneğin gireceği delikleri açıp kapayan bir alt mekanizma tasarlandı. Bu sayede ölçümlerde gün ışığından gelen gürültü miktarının en aza indirilmesi amaçlanmıştır. 

## Optik ve Analog Kısım

Dubada, sudaki klrofil miktarı ölçümü için bir adet BPW34 fotodiyot kullanılmıştır. Bunun seçilmesinin temel sebeplerinden biri yüksek hassasiyete ve geiş bir ışığa duyarlı alana sahip olmasıdır. Bulanıklık sensöründe ise üstünde kızılötesi filtreleyen reçineden yapılma lens olması ve yüksek hassasiyet sebebiyle SFH 203-FA kullanılmıştır. Bu iki fotodiyodun da çıkışına sinyali yükseltme amaçlı düşük gürültülü sabit kazançlı(24x) ADPD2210 fotodiyot yükseltici entegresi kullanılmıştır. Bu fotodiyotlardan gelen sinyalleri arduinonun okuyabileceği bir dijital veriye çevirmesi için küçük ayak izine sahip ve I2C bus ile kolayca kullanılabilen ADS1115 analogtan dijitale çevirici entegresi kullanılmıştır. Sensörlerin durması gereken pozisyon neeniyle BPW34 ve ona ait yükseltici ayrı bir karta monte edilmiş ve bu kart ana karta bir adet 90° pin header ile bağlanmıştır. Ana kartta ayrıca kızılötesi fotodiyot için bir adet arduinonun dijital pinleri tarafından kontrol edilebilen mosfet ve bir sabit akım kaynağı kullanılmıştır. UV ledler için pilin gerilimi ile ledlerin çalışma gerilimi arasındaki yakınlık sebebiyle sadce arduinonun dijital pinleri ile açıp kapayabilmeyi sağlamak amacıyla bir mosfet konmuştur.

![A](https://github.com/ErAk042/akilli_duba/blob/main/resimler/Ekran%20Al%C4%B1nt%C4%B1s%C4%B12.PNG?raw=true)

Optik sensörler ve ADC yi içeren kartın şematiği


![A](https://github.com/ErAk042/akilli_duba/blob/main/resimler/Ekran%20Al%C4%B1nt%C4%B1s%C4%B1.PNG?raw=true)

Optik sensörler ve ADC yi içeren kartın dağılım şeması


![A](https://github.com/ErAk042/akilli_duba/blob/main/resimler/Ekran%20Al%C4%B1nt%C4%B1s%C4%B11.PNG?raw=true)

Hassas fotodiyot ve yükselticisini içeren kartın dağılım şeması


## İletkenlik Sensörü

Dubamız aynı zamanda suyun iletkenliğini ölçerek alg patlamalarının ana sebeplerden biri olan suya kirletici madde atılmasını tespit edebilmektedir. Bu ölçümü alırken sinyallerin su içinden geçerken ne kadar zayıfladığını ölçerek suyun ne düzeyde iletken olduğunu tespit eder. Bunu yaparken suyyun hidrolizini önlemek amacıyla suyun içinden CD4060 tarafından üretilen bir kare dalga geçirilerek bu durum önlenmektedir. Ardından bu kare dalga opamplar aracılığıyla doğrultularak Arduino tarafından analog olarak okunmaktadır. 

![A](https://github.com/ErAk042/akilli_duba/blob/main/resimler/Ekran%20Al%C4%B1nt%C4%B1s%C4%B12.PNG?raw=true)

TDS sensör kartının şematiği


![A](https://github.com/ErAk042/akilli_duba/blob/main/resimler/Ekran%20g%C3%B6r%C3%BCnt%C3%BCs%C3%BC%202023-06-01%20095348.png?raw=true)

TDS sensör kartının dağılım şeması



------------------------------------------------------------------------------------------------------------------------------------------------------------------

# Smart Buoy

Designed and produced by the students of Izmir Science High School for the 2204-A High School Students Research Projects Competition.
A buoy that measures parameters such as algae amount, conductivity, turbidity and temperature and uploads these data to a web database via gsm.

## General Mechanical Structure

As the main body of our buoy, a waterproof box was used in which the electronic systems are kept. Optical sensor with photodiodes and LEDs
block is located inside the box. This simplifies the mechanical structure of the darkroom and makes it less likely to be non watertight.
by reducing failure points from which water can get in. In order to prevent the measurements from being affected by daylight, a sub-mechanism was designed that is connected to a servo inside the box with magnets which are rotated by this servo to open and close the compartment where the sample will enter from. Whith this mechanism, our goal is to to minimize the amount of noise from daylight during measurements. 

## Optical and Analog Part

On the buoy, a BPW34 photodiode was used to measure the amount of chlorophyll in the water. This was chosen because it has high sensitivity and a large photosensitive area. For the turbidity sensor, SFH 203-FA was used due to its high sensitivity and the fact that it has a lens made out of infrared filtering resin. A low-noise fixed gain (24x) ADPD2210 photodiode amplifier ic was used to amplify the signal at the output of both photodiodes. In order to convert the signals from these photodiodes into digital data which the arduino can read, the ADS1115 analog-to-digital converter integration was used, which has a small footprint and can be easily used with the I2C bus. Due to the position where the sensors should be at, the BPW34 and its amplifier are mounted on a separate board and this board is connected to the main board with a 90° pin header. The main board also has a constant current source for the infrared photodiode and a mosfet which can be controlled by the digital pins of the arduino. For the UV LEDs, due to the proximity between the voltage of the battery and the operating voltage of the LEDs,only a mosfet was placed to enable on/off only with the digital pins of the arduino.


## Conductivity Sensor

Our buoy can also measure the conductivity of the water to detect contaminants in the water, one of the main causes of algae blooms. It does this by measuring how much the signal attenuates as it passes through the water to determine how conductive the water is. This is done by passing a square wave generated by CD4060 through the water to prevent hydrolysis of the water. This square wave is then rectified through opamps and read as analog value by Arduino. 


