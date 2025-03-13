#include <Arduino.h>
#include<SPI.h>
#include<MFRC522.h>
#include <StandardCplusplus.h>
#include <vector>
#include <Servo.h>
using namespace std;
#define led 8
// Khai báo MFRC522
#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);

// khai báo servo
Servo servo1; 
Servo servo2;

//
// khai báo hồng ngoại
#define hongngoai1 2
#define hongngoai2 3
int giatrihongngoai2;
int giatrihongngoai1;

// khai báo cảm biến siêu âm
#define trig 4
#define echo 5
unsigned long thoigian;
int khoangcach;

unsigned long ID1 = 4294949378;
unsigned long ID2 = 4294967007;
unsigned long ID3 = 4294966239;

// khai báo các trạng thái của cảm biến phát hiện xe
bool flag_sieuam=false;
bool flag_hongngoai1=false;
bool flag_hongngoai2=false;

//
vector<pair<unsigned long, unsigned long>> danhsachthe = {
  {4294949378, 0},
  {4294967007, 0},
  {4294966239, 0}
};// khai báo danh sách các cặp id thẻ và xe
unsigned long IDThe();
bool checkUID(unsigned long UID, unsigned long ID);
void setup() {
  Serial.begin(9600); // Mở Serial
  SPI.begin();        // Khởi động SPI
  mfrc522.PCD_Init(); // Khởi động module
  pinMode(led, OUTPUT);
  pinMode(hongngoai1,INPUT);
  pinMode(hongngoai2,INPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  servo1.attach(6);
  servo2.attach(7);
  servo1.write(0);  // Đặt vị trí ban đầu của servo1
  servo2.write(0);  // Đặt vị trí ban đầu của servo2
}

void loop() {
  giatrihongngoai1=digitalRead(hongngoai1);// hồng ngoại 1
  giatrihongngoai2=digitalRead(hongngoai2);// hồng ngoại 2
  digitalWrite(trig,LOW);// tắt xung chân trig
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);// bật xung chân trig
  delayMicroseconds(10);
  digitalWrite(trig,LOW);// tắt xung chân trig
  thoigian=pulseIn(echo,HIGH);// trả về thời gian gặp vật
  khoangcach=int(thoigian *0.034 / 2 );//tính khoảng cách
  
  static bool xe_da_duoc_quet = false;
  if(khoangcach<8 && xe_da_duoc_quet==false){
    unsigned long idthe1 = IDThe();
    if(idthe1!=0){
      unsigned long idthe2 = IDThe();Serial.println(idthe1);
      if(idthe2!=0){
        Serial.println(idthe2);
        for(int i=0;i<danhsachthe.size();i++){
          if(danhsachthe[i].first==idthe1){
            danhsachthe[i].second=idthe2;
            servo1.write(90);
            xe_da_duoc_quet=true;
          }
        }
      }
    }
  }
  
}


unsigned long IDThe() {
  if (!mfrc522.PICC_IsNewCardPresent()) return 0;
  if (!mfrc522.PICC_ReadCardSerial()) return 0;

  unsigned long cardID = (mfrc522.uid.uidByte[0] << 24) |
                         (mfrc522.uid.uidByte[1] << 16) |
                         (mfrc522.uid.uidByte[2] << 8)  |
                         mfrc522.uid.uidByte[3];

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1(); 
  return cardID;
}

bool checkUID(unsigned long UID, unsigned long ID) {
  return UID == ID;
}
