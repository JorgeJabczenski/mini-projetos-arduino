#include "LedControl.h"

#define DIN 13
#define CS  12
#define CLK 11

#define DELAY   10

LedControl lc = LedControl(DIN, CLK, CS, 1);

struct pos_s{
    int x;
    int y;
};
typedef struct pos_s pos_t;

pos_t dir  = {1,-1}, bola = {3, 3};
int raquete[] = {B11100000,B01110000,B00111000,B00011100, B00001110, B00000111,B00000111};
int counter = 0, morto = 0;
int delay_bola = 25;

void setup() {
    lc.shutdown(0, false);
    lc.setIntensity(0, 8);
    lc.clearDisplay(0);

    randomSeed(analogRead(0));

    Serial.begin(9600);
}

void loop() {
    lc.setRow(0,7,raquete[analogRead(A5)/170]);
    lc.setLed(0,bola.y, bola.x, 1);
    
    if (morto){
            bola.x =  3; bola.y = 0;
            dir.y  = -1;
            morto = 0;
            delay_bola = 25;
            delay(1000);
    }
    
    if(!(counter % delay_bola)){
        morto = 0;
        lc.setLed(0,bola.y, bola.x, 0);
        if (bola.x == 7 || bola.x == 0)
            dir.x *= -1;
        if (bola.y == 0)
            dir.y *= -1;

        if (bola.y == 6){
            int bateu = (B10000000 >> bola.x);
            if (raquete[analogRead(A5)/170] & bateu){
                if (delay_bola > 5){
                    delay_bola--;
                    Serial.println(delay_bola);
                }
                dir.y *= -1;
                int direcao = random(1000);
                if (direcao > 700){
                    dir.x *= -1;
                }

            } else {
                morto = 1;
            }
        }

        bola.y = bola.y + dir.y;
        bola.x = bola.x + dir.x;
    }
    counter = (counter + 1) % 10000;
    delay(DELAY);
}
