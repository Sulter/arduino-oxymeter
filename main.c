byte RED_LED_ON = 1;
 
//pins
const int RED_LED_WHEN_HIGH = 18;
const int IR_LED_WHEN_HIGH = 16;
const int VOLTAGE_DIVIDER = 7;//arduino analog pin
 
//heart beaths variables
int pulse =0;//just to show when a beat occurs
int beat_millis[60];//stores the duration of the last 60 beats
long unsigned int last_beat_time = 0; //stores the "millis()" of last beat
long unsigned int time = 0; //stores current "millis()"
byte beat_counter =0; //counter for the beat_millis container
byte bps = 0; //current calculated bps
 
//graph smoothing variables RED
const int AVERAGEOVER = 20; //how many to take avarge over
int red_storage[AVERAGEOVER]; 
float red_storage_avr[AVERAGEOVER]; 
int red_head=0; //counter for the container
 
//graph smoothing variables IR
int IR_storage[AVERAGEOVER]; 
float IR_storage_avr[AVERAGEOVER]; 
int IR_head=0; //counter for the container

//other globals
long unsigned int count = 0;
int measuring_red = 1;
int raw_IR_value=0;
float smooth_IR_value=0;
 
int raw_red_value =0;
float smooth_value = 0;
 
void calc_bps()
{
  int count = 0;
  long unsigned int total = 0;
  for(int a = 0; a<60; a++){
    if(beat_millis[a] < 2000 && beat_millis[a] > 300){//just make sure that the value makes sense (30-200bps)
      count++;
      total += beat_millis[a]; 
    }
  } 
 
  bps = 60000/(total/count);
 
}
 
float get_avr()
{
  float total = 0;
 
  for(int item = 0; item < AVERAGEOVER; item++){
    total+= red_storage[item];
  }
 
  red_storage_avr[red_head] = total / AVERAGEOVER;
  return float(red_storage_avr[red_head]);
 
}
void add_avr(int number)
{
  red_storage[red_head] = number;
  red_head+=1;
  if (red_head == AVERAGEOVER)
    red_head = 0;
}
 
float get_IR_avr()
{
  float total = 0;
 
  for(int item = 0; item < AVERAGEOVER; item++){
    total+= IR_storage[item];
  }
 
  IR_storage_avr[IR_head] = total / AVERAGEOVER;
  return float(IR_storage_avr[IR_head]);
 
}
 
void add_IR_avr(int number)
{
  IR_storage[IR_head] = number;
  IR_head+=1;
  if (IR_head == AVERAGEOVER)
    IR_head = 0;
}
 
 
void setup()
{  
  pinMode(RED_LED_WHEN_HIGH, OUTPUT);
  pinMode(IR_LED_WHEN_HIGH, OUTPUT);
  change_to_RED();
 
  Serial.begin(38400);
 
  for(int a =0; a<60; a++){//init the table to 0
    beat_millis[a] = 0;
  }
 
}

void find_pulse()
{
  int check_head =0;  
  if(red_head < 10){
    check_head = 14-red_head;
  }
  else
    check_head = red_head-10;
 
  if(red_storage_avr[red_head]+1 < red_storage_avr[check_head]){
 
    if(time - last_beat_time > 300){//make sure we don't get "doubles" - 300 miliseconds = pulse of ~200bps 
 
      pulse = 1;
      beat_millis[beat_counter] =  time - last_beat_time;
      last_beat_time = time;
      beat_counter++;
      if(beat_counter >=60)
        beat_counter = 0;
 
      calc_bps();
    }
 
  }
 
}
 
void change_to_IR()
{
  digitalWrite(RED_LED_WHEN_HIGH, LOW);
  digitalWrite(IR_LED_WHEN_HIGH, HIGH);
  RED_LED_ON = 0;
}
 
void change_to_RED()
{
  digitalWrite(RED_LED_WHEN_HIGH, HIGH);
  digitalWrite(IR_LED_WHEN_HIGH, LOW);
  RED_LED_ON = 1;
}

void loop()                     
{
  pulse =0; //make sure it's reset
  if(RED_LED_ON)
    raw_red_value = analogRead(VOLTAGE_DIVIDER);
  else
    raw_red_value = 1023-analogRead(VOLTAGE_DIVIDER);
 
  time = millis();
  smooth_value = get_avr();
  add_avr(raw_red_value);
 
  find_pulse();
  
  Serial.println(time);
  Serial.println(raw_red_value);
  Serial.println(smooth_value);
  Serial.println(pulse);
  Serial.println(beat_millis[beat_counter-1]);
  Serial.println(bps);
  Serial.println(raw_IR_value);
  
  delay(5);//measure at set /s , this should be more then enough for a good reading, without overflowing the COM
}
