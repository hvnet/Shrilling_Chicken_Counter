// ピンの初期化
byte dataPin = 2;
byte latchPin = 3;
byte clockPin = 4;

byte chickenPin = 0;

// チキンが鳴いた回数の初期化 (初期の回数）
int count = 0;

// チキンの状態
boolean chicken = 0;

// チキンが鳴いたと判定するしきい値
int thrVal = 100;

#define dataNum 4
int data[dataNum];
int dataIndex = 0;


// 初期化処理
void setup() {
  Serial.begin(9600);
  
  // 各種ピンの動作モード設定
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  pinMode(chickenPin, INPUT);

  // data buf for moving ave
  for (byte i = 0; i < dataNum; i++) {
    data[i] = 0;
  }

}

// 7セグメントLEDを1桁出力する関数
void led_7seg_out(byte dec) {
  byte out[] = {  0b11111100,
                  0b01100000,
                  0b11011010,
                  0b11110010,
                  0b01100110,
                  0b10110110,
                  0b10111110,
                  0b11100100,
                  0b11111110,
                  0b11110110 };
  shiftOut(dataPin, clockPin, LSBFIRST, out[dec]);
}

// 7セグメントLEDを3桁全部出力する関数
void led_7seg_all_out (int dec) {
  digitalWrite(latchPin, LOW);
  byte dec3 = dec % 10;
  dec = dec / 10;
  byte dec2 = dec % 10;
  dec = dec / 10;
  byte dec1 = dec % 10;
    led_7seg_out(dec1);
    led_7seg_out(dec2);
    led_7seg_out(dec3);
  digitalWrite(latchPin, HIGH);
}


void checkChicken() {
  int raw = 0;
  int val = 0;

  raw = analogRead(chickenPin);

  val = raw - 512;
  if (val < 0) val = val * (-1);


  int sum = 0;
  int ave = 0;
  if (dataIndex == dataNum) dataIndex = 0;
  data[dataIndex] = val;
  dataIndex++;
  for (byte i = 0; i < dataNum; i++) {
    sum += data[i];
  }
  ave = sum / dataNum;
  Serial.println(ave);


  if (ave >= thrVal){
    chicken = 1;
  } else {
    if (chicken == 1) {
      count++;
      chicken = 0;
    }
    chicken = 0;
  }
}

// メインループ
void loop() {
  // チキンの状態を読み込む
  checkChicken();

  delay(10);
  //Serial.println(count);
  //Serial.println(chicken);

  // 現状の点数をディスプレイに出力する
  led_7seg_all_out(count);

}

