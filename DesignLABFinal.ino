/*
기본 LED RGB LED  참고 => http://wiki.vctec.co.kr/opensource/arduino/rgbled

별  => 반복적으로 반짝 반짝    =>   RGB LED X => 핀 부족함.   ㅇ    // star()  -> 코드 완료 
달   => 조도센서에 의해서 불이 꺼지면 달이 켜지도록  ㅇ  // moon()     -> 코드 완료

==========================================================================================

물레방아 => 버튼 누르면 DC 모터가 돌아가는 형태 ㅇ    // motor()
폭포 => 물소리! (미디어) 

반딧불이 => 터치하면 불이 들어오는 것  - 적외선 센서. (2마리)

==========================================================================================

매화 => 손이 움직인 방향 부터 불이 들어오도록!(근접센서)    http://blog.naver.com/roboholic84/220522339575  
// flower()

*/
// 달 11개(하나로)   매화 1개 6개 7개   반딧불이 6개

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <CapacitiveSensor.h>
// 꽂을 핀
#define MPIN     22   //  LED 13개
#define WPINA     41    //  
#define WPINB     43    //  
#define FPINA    32    //  LED 3개 
#define FPINB    11    //  LED 2개 
#define FPINC    12
#define FPIND    13
#define FPINE    2


// LED 개수
#define MPIX     11   // 달
#define WPIX     6    // 벌레
#define FPIXA    1
#define FPIXB    1
#define FPIXC    1
#define FPIXD    1
#define FPIXE    1

// 조도 센서
int cd = A0;
//                                          LED 개수/ 핀번호
Adafruit_NeoPixel mpixels = Adafruit_NeoPixel(MPIX, MPIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel wpixels_a = Adafruit_NeoPixel(1, WPINA, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel wpixels_b = Adafruit_NeoPixel(1, WPINB, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel fpixels_a = Adafruit_NeoPixel(FPIXA, FPINA, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel fpixels_b = Adafruit_NeoPixel(FPIXB, FPINB, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel fpixels_c = Adafruit_NeoPixel(FPIXC, FPINC, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel fpixels_d = Adafruit_NeoPixel(FPIXD, FPIND, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel fpixels_e = Adafruit_NeoPixel(FPIXE, FPINE, NEO_GRB + NEO_KHZ800);


int on1, on2, i;
int ct = 0;
int lc = 0;
int rc = 0;

//   [on1]  22   23    24    25    26   [on2]
//    lc                                 rc
//    4핀                                7핀

// 물레방아 + 폭포
int count = 0;
int ccount = 0;
int mcount = 0;
int motorPin = 9;
int btPin = 8;

int stopcount = 0;
int AbtPin = 10;


int bt = 0;
int abt = 0;

//벌레

CapacitiveSensor cs1 = CapacitiveSensor(50, 51); // 신호를 보내는 핀 신호를 받는 핀
CapacitiveSensor cs2 = CapacitiveSensor(52, 53);

//         [연결선]
//    [ 저     항 ]
//    [52]     [53]
//    [50]     [51]

long value1;
long value2;
int cdv;

void color_b(int a, int b, int c) {
  wpixels_b.setPixelColor(0, wpixels_b.Color(a, b, c));
  wpixels_b.show();
}

void color_a(int a, int b, int c) {
  wpixels_a.setPixelColor(0, wpixels_a.Color(a, b, c));
  wpixels_a.show();
}

void f_color_a(int a, int x, int y, int z) {
  fpixels_a.setPixelColor(a, fpixels_a.Color(x, y, z));
  fpixels_a.show();
}


void f_color_b(int a, int x, int y, int z) {
  fpixels_b.setPixelColor(a, fpixels_b.Color(x, y, z));
  fpixels_b.show();
}


void f_color_c(int a, int x, int y, int z) {
  fpixels_c.setPixelColor(a, fpixels_c.Color(x, y, z));
  fpixels_c.show();
}



void f_color_d(int a, int x, int y, int z) {
  fpixels_d.setPixelColor(a, fpixels_d.Color(x, y, z));
  fpixels_d.show();
}



void f_color_e(int a, int x, int y, int z) {
  fpixels_e.setPixelColor(a, fpixels_e.Color(x, y, z));
  fpixels_e.show();
}



void setup()
{
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  Serial.begin (9600);
  // 벌레
  wpixels_a.begin();
  wpixels_b.begin();

  // 달
  mpixels.begin();
  // 꽃
  fpixels_a.begin();
  fpixels_b.begin();
  fpixels_c.begin();
  fpixels_d.begin();
  fpixels_e.begin();

  // 물레방아
  pinMode(motorPin, OUTPUT);
  pinMode(btPin, INPUT);
  mp3_set_serial (Serial);  //set Serial for DFPlayer-mini mp3 module
  delay(1);  //wait 1ms for mp3 module to set volume
  mp3_set_volume (30);

  f_color_a(0, 0, 0, 0);
  f_color_b(0, 0, 0, 0);
  f_color_c(0, 0, 0, 0);
  f_color_d(0, 0, 0, 0);   // On
  f_color_e(0, 0, 0, 0);   // On

}

void loop()

{
  // ======================================================
  // 물레방아 + 소리(완성)
  bt = digitalRead(btPin);
  abt = digitalRead(AbtPin);
  if (bt == 1) {
    if (stopcount == 0)    {
      stopcount = 1;
      count++;
    }
  }
  else {
    stopcount = 0;
  }

  if (count % 2 == 0)  {
    
    analogWrite(motorPin, 0);
  } else if (count % 2 == 1)  {
    
    analogWrite(motorPin, 255);  // 속도 조절 필요
  }
  // ======================================================
  // 꽃
  on1 = digitalRead(4);

  on2 = digitalRead(7);
  if (on1 == 0)
  {
    ct++;
    lc = ct;
  } else if (on2 == 0)
  {
    ct++;
    rc = ct;
  }

  if (lc != 0 && rc != 0 && ct != 0)  {
    if (lc < rc) // 손이 오른쪽에서 왼쪽으로 간 경우
    {

      f_color_c(0, 158, 21, 255);
      delay(500);

      f_color_a(0, 65, 255, 58);

      delay(500);

      f_color_b(0, 158, 21, 255);
      f_color_c(0, 0, 0, 0);

      delay(500);

      f_color_e(0, 65, 255, 58);
      f_color_a(0, 0, 0, 0);
      delay(500);

      f_color_d(0, 204, 61, 61);
      f_color_b(0, 0, 0, 0);
      delay(500);

      f_color_e(0, 0, 0, 0);
      delay(500);
      f_color_d(0, 0, 0, 0);
      delay(100);
      f_color_d(0, 204, 61, 61);
      f_color_e(0, 65, 255, 58);
      f_color_b(0, 158, 21, 255);
      f_color_c(0, 158, 21, 255);
      f_color_a(0, 65, 255, 58);
      delay(2000);

      f_color_d(0, 0, 0, 0);
      f_color_e(0, 0, 0, 0);
      f_color_b(0, 0, 0, 0);
      f_color_a(0, 0, 0, 0);
      f_color_c(0, 0, 0, 0);


      //   C A B E D

      lc = 0;
      rc = 0;
      ct = 0;
    } else {   // 손이 왼쪽에서 오른쪽으로 간 경우

      f_color_d(0, 204, 61, 61);
      delay(500);

      f_color_e(0, 65, 255, 58);

      delay(500);

      f_color_b(0, 158, 21, 255);
      f_color_d(0, 0, 0, 0);

      delay(500);

      f_color_a(0, 65, 255, 58);
      f_color_e(0, 0, 0, 0);
      delay(500);

      f_color_c(0, 158, 21, 255);
      f_color_b(0, 0, 0, 0);
      delay(500);

      f_color_a(0, 0, 0, 0);
      delay(500);
      f_color_c(0, 0, 0, 0);
      delay(100);
      f_color_c(0, 158, 21, 255);
      f_color_d(0, 204, 61, 61);
      f_color_e(0, 65, 255, 58);
      f_color_b(0, 158, 21, 255);
      f_color_a(0, 65, 255, 58);
      delay(2000);

      f_color_d(0, 0, 0, 0);
      f_color_e(0, 0, 0, 0);
      f_color_b(0, 0, 0, 0);
      f_color_a(0, 0, 0, 0);
      f_color_c(0, 0, 0, 0);

      lc = 0;
      rc = 0;
      ct = 0;

      //   D E B A C
    }
  }
  // ======================================================
  // 벌레
  value1 = cs1.capacitiveSensor(30);
  value2 = cs2.capacitiveSensor(30);

  if (value1 > 1000)  {
    color_b(65, 255, 58);
    delay(300);
    color_b(47, 237, 40);
    delay(300);
    color_b(29, 219, 22);
    delay(300);
    color_b(11, 201, 4);
    delay(300);
    color_b(0, 183, 0);
    delay(300);
    color_b(11, 201, 4);
    delay(300);
    color_b(29, 219, 22);
    delay(300);
    color_b(47, 237, 40);
    delay(300);
  } else  {
    wpixels_b.setPixelColor(0, wpixels_b.Color(0, 0, 0));
    wpixels_b.show();

  }

  if (value2 > 1000)  {
    color_a(65, 255, 58);
    delay(300);
    color_a(47, 237, 40);
    delay(300);
    color_a(29, 219, 22);
    delay(300);
    color_a(11, 201, 4);
    delay(300);
    color_a(0, 183, 0);
    delay(300);
    color_a(11, 201, 4);
    delay(300);
    color_a(29, 219, 22);
    delay(300);
    color_a(47, 237, 40);
    delay(300);

  } else  {
    wpixels_a.setPixelColor(0, wpixels_a.Color(0, 0, 0));
    wpixels_a.show();
  }

  // ======================================================
  // 달
  cdv = analogRead(cd);
  if (cdv < 550) {
    if (mcount == 0) {
      mcount = 1;
      for (int k = 0 ; k < MPIX; k++) {
        mpixels.setPixelColor(k, mpixels.Color(255, 228, 0));
        mpixels.show();
      }
      delay(2000);
    }
  } else {
    if (mcount == 1) {
      mcount = 0;
      for (int k = 0 ; k < MPIX; k++) {
        mpixels.setPixelColor(k, mpixels.Color(0, 0, 0));
        mpixels.show();
      }
    }
  }

  Serial.println(abt);
  if (abt == 1)
  {
    for (int k = 0 ; k < MPIX; k++) {
      mpixels.setPixelColor(k, mpixels.Color(255, 228, 0));
      mpixels.show();
    }
    color_a(65, 255, 58);
    color_b(65, 255, 58);
    f_color_c(0, 158, 21, 255);
    f_color_d(0, 204, 61, 61);
    f_color_e(0, 65, 255, 58);
    f_color_b(0, 158, 21, 255);
    f_color_a(0, 65, 255, 58);
    delay(3000);
  } 
  else{
    for (int k = 0 ; k < MPIX; k++) {
      mpixels.setPixelColor(k, mpixels.Color(0, 0, 0));
      mpixels.show();
    }
    color_a(0, 0, 0);
    color_b(0, 0, 0);
    f_color_c(0, 0, 0, 0);
    f_color_d(0, 0, 0, 0);
    f_color_e(0, 0, 0, 0);
    f_color_b(0, 0, 0, 0);
    f_color_a(0, 0, 0, 0);
    }
}

