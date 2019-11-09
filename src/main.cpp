#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "PortIO.h"
#define SERVICE_UUID "80e90741-bc2c-04b4-300f-183eedb939dd"
#define CHARACTERISTIC_UUID "fcd8f491-9226-e196-c976-fa290b824f4a"
//cmdddd
const int ioportled[8] = {4,16,17,13,26,27,14,12};
const int ioportinter[4] = {39, 38, 37, 36};

BLEServer* pServer;
BLECharacteristic* pCaracteristic;

int strtovaleur(std::string chaine){
  int table[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
  int valeur =0x00;
  for(int i=0;i<8;i++){
    if(chaine[i]=='1'){
      valeur=valeur|table[i];
    }
  }
return valeur;
}

std::string valeurtostr(int valeur){
  int table[4]={0x08,0x04,0x02,0x01};
  std::string chaine ="";
  Serial.print("valeur:");
  Serial.println(valeur);
  for(int i=0;i<4;i++){
    Serial.print("table:");
    Serial.println(table[i]);
    if(valeur&0x01==0x01){   //00001010 & 00001000 == 1000
      chaine +='1';
    }
	else {
      chaine +='0';
    }
  Serial.println(chaine.c_str());
  }
return chaine;
}


class MyCallBack : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic* pCaracteristic){
    int valeur = readPort(ioportinter,4);
    std::string chaine = valeurtostr(valeur);
    Serial.print("valeur ");
    Serial.println(chaine.c_str());
    pCaracteristic->setValue(chaine);
  }
  void onWrite(BLECharacteristic* pCaracteristic){
    std::string message = pCaracteristic->getValue();
    Serial.print("message : ");
    Serial.println(message.c_str());
    int valeur=strtovaleur(message);
    writePort(ioportled,valeur,8);
  }
};

void setup() {
  pinMode(BUILTIN_LED,OUTPUT);
  Serial.begin(115200);
  Serial.println("init BLE");
  initPort(ioportled,0xff,8);
  initPort(ioportinter,0x00,4);
  BLEDevice::init("ESP32CMDCSY");
  pServer=BLEDevice::createServer();
  BLEService* pService=pServer->createService(SERVICE_UUID);
  pCaracteristic=pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE);
  pCaracteristic->setCallbacks(new MyCallBack());
  pService->start();
  BLEAdvertising* pAdvertising=pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  // put your main code here, to run repeatedly:
}
