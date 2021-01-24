/*
 * screen.c
 *
 *  Created on: Jan 17, 2021
 *      Author: kaanari
 */
#include <msp430.h>
#include <ssd1306_lib.h>
#include <icon.h>
#include <address.h>
#include <menu_text.h>

#define SEND BIT6
#define A0 BIT0
#define A1 BIT1
#define A2 BIT2
#define ACK BIT7

unsigned char spectrumPixels[3][107] = {0};

const unsigned char TunerPosition_x = 25;
const unsigned char TunerPosition_y = 8;

const unsigned char SpectrumPosition_x = 7;
const unsigned char SpectrumPosition_y = 8;

volatile unsigned char currentPage = 0;

unsigned char operationMode = 0;

void spectrumAxis(){
    drawImage(0,0,128,32,(unsigned char *)&axis,1);

}

unsigned short spectrumMode(){

    operationMode = 1;

    __no_operation();

    adc_config();


    __no_operation();
    spectrumAxis();
    sendCommand(SSD1306_SET_SCROLL | (0));
    __no_operation();
    adc_on();
    spectrum_draw(5);
    adc_off();
    __no_operation();
}

void screenConfig()
{
    ssd1306Init();
    clearScreen();
}

void clearScreen()
{
    fillDisplay (0x00);
}

void printOpenningScreen()
{
    //drawImage(0, 0, 91, 32, (unsigned char *) &logo, 0);

    drawImage(0, 0, 128, 64, (unsigned char *) &logo, 1);

    int i = 0;

        for(i = 0;i<96;i++){
            sendCommand(SSD1306_SET_SCROLL | i % 64);
            __delay_cycles(500000);
        }

     //clearScreen();
     //delay_ms(500);


     drawImage(0, 0, 128, 64, (unsigned char *) &logo_final, 0);
     sendCommand(SSD1306_SET_SCROLL | 0);
     delay_ms(1000);
     clearScreen();


}

void menuSelectTuner(){
    draw6x8Str(0, 0, "M", 1, 0);
    draw6x8Str(0, 1, "E", 1, 0);
    draw6x8Str(0, 2, "N", 1, 0);
    draw6x8Str(0, 3, "U", 1, 0);
    draw6x8Str(41, 0, "            ", 1, 0);
    drawImage(TunerPosition_x, TunerPosition_y,84,16, TUNER_TEXT, 1);

    draw6x8Str(41, 3, "            ", 1, 0);
    draw6x8Str(43, 3, "SPECTRUM", 1, 0);
}

void menuSelectSpectrum(unsigned char page){
    page = page<<2;
    draw6x8Str(0, page, "M", 1, 0);
    draw6x8Str(0, page+1, "E", 1, 0);
    draw6x8Str(0, page+2, "N", 1, 0);
    draw6x8Str(0, page+3, "U", 1, 0);

    draw6x8Str(41, page, "            ", 1, 0);
    if(page){
        drawImage(SpectrumPosition_x, SpectrumPosition_y+32,120,16, SPECTRUM_TEXT, 1);
    }else{
        drawImage(SpectrumPosition_x, SpectrumPosition_y,120,16, SPECTRUM_TEXT, 1);
    }
    draw6x8Str(52, page, "TUNER", 1, 0);
    draw6x8Str(41, page+3, "            ", 1, 0);

}

void menuSelectCredits(unsigned char page){
    page = page<<2;
    draw6x8Str(0, page, "M", 1, 0);
    draw6x8Str(0, page+1, "E", 1, 0);
    draw6x8Str(0, page+2, "N", 1, 0);
    draw6x8Str(0, page+3, "U", 1, 0);

    draw6x8Str(41, page, "            ", 1, 0);
    if(page){
        drawImage(SpectrumPosition_x, SpectrumPosition_y+32,120,16, SPECTRUM_TEXT, 1);
    }else{
        drawImage(SpectrumPosition_x, SpectrumPosition_y,120,16, SPECTRUM_TEXT, 1);
    }
    draw6x8Str(52, page, "TUNER", 1, 0);
    draw6x8Str(41, page+3, "            ", 1, 0);

}

void menu()
{

    menuSelectTuner();
    __no_operation();
    menuSelectSpectrum(1);
    __no_operation();

    volatile unsigned char readValue = 0;

    while(1)
    {
    //__delay_cycles(5000000);                             // Delay between transmissions
     delay_ms(200);
     P1OUT ^= BIT0;
     P1OUT |= ACK;
     while(!SEND);
     readValue = (P2IN & (A0|A1|A2));
     //readValue = readValue>>2;
     P1OUT &= ~ACK;

     if(readValue){
         if(readValue == 7){
             WDTCTL = 0x00;
         }
         else if(readValue == 5){
             if(currentPage == 0){

             }else{

                 spectrumMode();
                 fillDisplay (0x00);
                 menuSelectSpectrum(1);
                 sendCommand(SSD1306_SET_SCROLL | (32));
                 delay_ms(10);
                 menuSelectTuner();



                 __no_operation();

                 currentPage = 1;
                 continue;

             }

         }
         else if(readValue == 2){
             changePage(1);
             currentPage = 1;
         }else if(readValue == 1){
             changePage(0);
             currentPage = 0;
         }
     }



    }


    //delay_ms(1500);

}


char changePage(unsigned char page){
    int i = 0;
    if(page == 1){

        if(currentPage == 1){
            for(i = 32;i<40;i++){
                sendCommand(SSD1306_SET_SCROLL | (i+1));
                __delay_cycles(200000);

            }
            for(i = 39;i>30;i--){
                sendCommand(SSD1306_SET_SCROLL | (i+1));
                __delay_cycles(100000);

            }
            sendCommand(SSD1306_SET_SCROLL | (32));
            return 0;
        }

        for(;i<32;i++){
            sendCommand(SSD1306_SET_SCROLL | (i+1));
            __delay_cycles(100000);
        }
    }else{
        if(currentPage == 0){
            for(i = 62;i>54;i--){
                sendCommand(SSD1306_SET_SCROLL | ((i+1)%64));
                __delay_cycles(200000);

            }
            for(i = 55;i<63;i++){
                sendCommand(SSD1306_SET_SCROLL | (i+1));
                __delay_cycles(100000);

            }
            sendCommand(SSD1306_SET_SCROLL | (0));
            return 0;
        }

        for(i=32;i>=0;i--){
            sendCommand(SSD1306_SET_SCROLL | (i));
            __delay_cycles(100000);
        }
    }
}



void spectrumScreen(char fr[],unsigned char max_mag){

    __no_operation();
    //spectrumAxis();

       unsigned char linePixels = 0;

       unsigned char temp = 0;
       unsigned int i = 0;
       for(;i<107;i++){
           linePixels = 0;
           temp = fr[i];

           if(temp == 0){
               spectrumPixels[0][i] = 0x00;
               spectrumPixels[1][i] = 0x00;
               spectrumPixels[2][i] = 0x00;
               continue;

           }

           temp = temp>>2;

           if(temp == 0){
               spectrumPixels[0][i] = 0x00;
               spectrumPixels[1][i] = 0x00;
               spectrumPixels[2][i] = 0x00;
               continue;
           }


           if(temp > 23)
               temp += 9;
           else if(temp > 15){
               temp += 7;
           }else if(temp > 7){
               temp += 5;
           }else{
               temp += 3;
           }

           temp = temp>>1;



           if(temp<9){

               for(;temp>0;temp--){
                   linePixels = linePixels >> 1;
                   linePixels = linePixels | 0x80;
               }
               spectrumPixels[0][i] = 0;
               spectrumPixels[1][i] = 0;
               spectrumPixels[2][i] = linePixels;

           }else if(temp<17){
               temp = temp-8;
               for(;temp>0;temp--){
                   linePixels = linePixels >> 1;
                   linePixels = linePixels | 0x80;
               }
               spectrumPixels[0][i] = 0;
               spectrumPixels[1][i] = linePixels;
               spectrumPixels[2][i] = 0xFF;

           }else{

               temp = temp-16;
               for(;temp>0;temp--){
                   linePixels = linePixels >> 1;
                   linePixels = linePixels | 0x80;
               }
               spectrumPixels[0][i] = linePixels;
               spectrumPixels[1][i] = 0xFF;
               spectrumPixels[2][i] = 0xFF;

           }


       }

       drawImage(5, 0, 107, 24, (unsigned char *)&spectrumPixels, 1);




    __no_operation();



}

