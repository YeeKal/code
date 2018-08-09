//pin definition
int pwm_pin=4;
int dir_pin=5;
int enax_pin=6;
int enay_pin=7;
int enaz_pin=8;
int enajia_pin=9;
int relay_x=10;//electromagnetic relay
int relay_y=11;//lower is valid
int relay_z=12;
int enaqie_pin=13;
int read_pin=A2;

//variant
float stepx_to_pulse=0.13125;//step length each pulse 
float stepy_to_pulse=0.2625;
float stepz_to_pulse=0.015625;
float stepjia_to_pulse=0.00625;
boolean dir=true;//change this to define the positive direction
String cmd;//2 as the item and 4 as the num
String item;// 2 bytes
float disp;
int rate=500;

#define min_steps 300
int periods[min_steps];
float constant_k=4;
int min_fre=100;

//function declaration
void stepper(int fre, float disp,float step_to_pulse);
void stepperj(int fre, float disp,float step_to_pulse);
void moveQie();
boolean judge(int pin);
void smodeLine(int min_fre,int max_fre,int len,float k);

void setup()
{
  Serial.begin(9600);
  pinMode(pwm_pin,OUTPUT);
  pinMode(dir_pin,OUTPUT);
  pinMode(enax_pin,OUTPUT);
  pinMode(enay_pin,OUTPUT);
  pinMode(enaz_pin,OUTPUT);
  pinMode(enajia_pin,OUTPUT);
  pinMode(relay_x,OUTPUT);
  pinMode(relay_y,OUTPUT);
  pinMode(relay_z,OUTPUT);
  pinMode(enaqie_pin,OUTPUT);
  digitalWrite(enax_pin,HIGH);//always disabled
  digitalWrite(enay_pin,HIGH);
  digitalWrite(enaz_pin,HIGH);
  digitalWrite(enajia_pin,HIGH);
  digitalWrite(relay_x,HIGH);//always lock motion
  digitalWrite(relay_y,HIGH);
  digitalWrite(relay_z,HIGH);
  digitalWrite(enaqie_pin,HIGH);
}
void loop(){
  if(Serial.available() > 0){
    cmd =Serial.readStringUntil('.');
    if(cmd.length()<=1){
      Serial.println("Failed");
      return;
    }
    int cmd_len=cmd.length();
    item=cmd.substring(0,1);
    disp=cmd.substring(1,cmd_len).toInt();
    switch(item[0]){
      case 'x':
        //Serial.println("x start.");
        digitalWrite(enax_pin,LOW);
        digitalWrite(relay_x,LOW);
        stepper(1600,disp,stepx_to_pulse);
        digitalWrite(relay_x,HIGH);
        digitalWrite(enax_pin,HIGH);
      	Serial.println("111111");
        break;
      case 'y':
        //Serial.println("y start.");
        digitalWrite(enay_pin,LOW);
        digitalWrite(relay_y,LOW);
        stepper(500,disp,stepy_to_pulse);
        digitalWrite(relay_y,HIGH);
        digitalWrite(enay_pin,HIGH);
	      Serial.println("111111");
        break;
      case 'z':
        //Serial.println("z start.");
        digitalWrite(enaz_pin,LOW);
        digitalWrite(relay_z,LOW);
        stepper(2700,disp,stepz_to_pulse);
        digitalWrite(relay_z,HIGH);
        digitalWrite(enaz_pin,HIGH);
	      Serial.println("111111");
        break;
      case 'j':
        //Serial.println("j start.");
        digitalWrite(enajia_pin,LOW);
        stepperj(3000,disp,stepjia_to_pulse);
        digitalWrite(enajia_pin,HIGH);
	      Serial.println("111111");
        break;
      case 'q':
        //Serial.println("q start.");
        moveQie();
      	Serial.println("111111");
        break;
      default:
        Serial.println("hello world"+cmd);//"not a valid command");
        break;
    }//switch end.
  }//if end

}//loop end

void stepper(int fre, float disp,float step_to_pulse){
  smodeLine(min_fre,fre,min_steps,constant_k);
  if(disp<0){
    dir=!dir;
    disp=-disp;      
  }
  digitalWrite(dir_pin,dir);//direction 
  int steps=int(disp/step_to_pulse);
  if(steps<=2*min_steps){
    int start_steps=int(steps/2);
    int finish_steps=steps-start_steps;
    for(int i=0;i<start_steps;i++){
      digitalWrite(pwm_pin,LOW);
      delayMicroseconds(periods[i]);
      digitalWrite(pwm_pin,HIGH);
      delayMicroseconds(periods[i]);
    }
    for(int i=0;i<finish_steps;i++){
      digitalWrite(pwm_pin,LOW);
      delayMicroseconds(periods[finish_steps-1-i]);
      digitalWrite(pwm_pin,HIGH);
      delayMicroseconds(periods[finish_steps-1-i]);
    }
  }
  else{
    //low
    for(int i=0;i<min_steps;i++){
      digitalWrite(pwm_pin,LOW);
      delayMicroseconds(periods[i]);
      digitalWrite(pwm_pin,HIGH);
      delayMicroseconds(periods[i]);
    }
    //true
    int time_gap=1000000/(2*fre);
    for(int k=0;k<steps-2*min_steps;k++){
      digitalWrite(pwm_pin,LOW);
      delayMicroseconds(time_gap);
      digitalWrite(pwm_pin,HIGH);
      delayMicroseconds(time_gap);
    }
    //low
    for(int i=0;i<min_steps;i++){
      digitalWrite(pwm_pin,LOW);
      delayMicroseconds(periods[min_steps-1-i]);
      digitalWrite(pwm_pin,HIGH);
      delayMicroseconds(periods[min_steps-1-i]);
    }
  }
  dir=true;
}
void smodeLine(int min_fre,int max_fre,int len,float k){
  for(int i=0;i<len;i++){
    float deno=min_fre+float(max_fre-min_fre)/(1+exp(-k*(i-float(len)/2)/(float(len)/2)));
    //float deno=min_fre+float(max_fre-min_fre)*(1-1/exp(k*i/float(len)));
    periods[i]=int(1000000.0/(2*deno));
  }
}

void stepperj(int fre, float disp,float step_to_pulse){
  if(disp<0){
    dir=!dir;
    disp=-disp;      
  }
  int steps=int(disp/step_to_pulse);
  int time_gap=1000000/(2*fre);
  digitalWrite(dir_pin,dir);//direction 
  for(int i=0;i<steps;i++){
      digitalWrite(pwm_pin,LOW);
      delayMicroseconds(time_gap);
      digitalWrite(pwm_pin,HIGH);
      delayMicroseconds(time_gap);
    }
  dir=true;
}

void moveQie(){
  if(judge(read_pin)){
    digitalWrite(enaqie_pin,LOW);
    //while(analogRead(read_pin)>800){}
    while(judge(read_pin)){}
    //delay(10);
    digitalWrite(enaqie_pin,HIGH);
    
  }
  else if(!judge(read_pin)) {
    digitalWrite(enaqie_pin,LOW);
    //while(analogRead(read_pin)<500){}
    while(!judge(read_pin)){}
    digitalWrite(enaqie_pin,HIGH);
    
  }
}
boolean judge(int pin){
  int a=1;
  while(a==1){
    if(analogRead(pin)>900){
      delay(1);
      if(analogRead(pin)>900){
        a=0;
        return true;
      }
    }
    else if(analogRead(pin)<500){
      delay(1);
      if(analogRead(pin)<500){
        a=0;
        return false;
      }
    }
  }
}
