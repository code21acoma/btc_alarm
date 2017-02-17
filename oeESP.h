const int RED = 15;
const int GREEN = 13;//13
const int BLUE = 12;//12
const int PHOTO = A0;
const int BUTTON = 4;

void ledRGB(int del){
  analogWrite(RED, 1023);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  delay(del);

  analogWrite(RED, 0);
  analogWrite(GREEN, 1023);
  analogWrite(BLUE, 0);
  delay(del);

  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 1023);
  delay(del);

  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE,0);
  
}  

void ledRed()
{
   analogWrite(RED, 1023);
   analogWrite(GREEN,0);
   analogWrite(BLUE,0);  
  }

void ledGre()
{
   analogWrite(RED, 0);
   analogWrite(GREEN,1023);
   analogWrite(BLUE,0);  
  }

void ledBlu()
{
   analogWrite(RED, 0);
   analogWrite(GREEN,0);
   analogWrite(BLUE,1023);  
  }

void blinkRed(int lon, int loff){
   analogWrite(RED, 1023);
   analogWrite(GREEN, 0);
   analogWrite(BLUE, 0);
   delay(lon);
   analogWrite(RED, 0);
   analogWrite(GREEN, 0);
   analogWrite(BLUE, 0);    
   delay(loff);    
  }

void blinkGre(int lon, int loff){
   analogWrite(RED, 0);
   analogWrite(GREEN,1023);
   analogWrite(BLUE, 0);
   delay(lon);
   analogWrite(RED, 0);
   analogWrite(GREEN, 0);
   analogWrite(BLUE, 0);    
   delay(loff);    
  }

void blinkBlu(int lon, int loff){
   analogWrite(RED, 0);
   analogWrite(GREEN,0);
   analogWrite(BLUE,1023);
   delay(lon);
   analogWrite(RED, 0);
   analogWrite(GREEN, 0);
   analogWrite(BLUE, 0);    
   delay(loff);    
  }
