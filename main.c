#include <main.h>
#include <stdbool.h> 

#define LCD_RS_PIN       PIN_E0
#define LCD_RW_PIN       PIN_E1
#define LCD_ENABLE_PIN   PIN_E2
#define LCD_DATA4        PIN_D4
#define LCD_DATA5        PIN_D5
#define LCD_DATA6        PIN_D6
#define LCD_DATA7        PIN_D7 
#include <lcd.c>

#define TRIGGER_1   PIN_C0
#define ECHO_1      input(PIN_C1)
#define TRIGGER_2   PIN_C2
#define ECHO_2      input(PIN_C3)
#define TRIGGER_3   PIN_C4
#define ECHO_3      input(PIN_C5)
#define TRIGGER_4   PIN_C6
#define ECHO_4      input(PIN_C7)

#define BUTTON_B0    input(PIN_B0)  
#define BUTTON_B6    input(PIN_B6)  
#define BUTTON_B7    input(PIN_B7)  

float threshold = 50;
bool lcd_mode = false;

#define buzzer       PIN_D2

float distance[4], time[4]; 

float measure_distance(int sensor)
{
    switch(sensor) {
        case 1:
            output_high(TRIGGER_1);  
            break;
        case 2:
            output_high(TRIGGER_2);   // B?t Trigger c?a c?m bi?n 2
            break;
        case 3:
            output_high(TRIGGER_3);   // B?t Trigger c?a c?m bi?n 3
            break;
        case 4:
            output_high(TRIGGER_4);   // B?t Trigger c?a c?m bi?n 4
            break;
    }
    delay_ms(50);
    switch(sensor) 
    {
        case 1:
            output_low(TRIGGER_1);   
            break;
        case 2:
            output_low(TRIGGER_2);   
            break;
        case 3:
            output_low(TRIGGER_3);    
            break;
        case 4:
            output_low(TRIGGER_4);    
    }

    switch(sensor) 
    {
        case 1:
            while(ECHO_1 == 0) {}
            break;
        case 2:
            while(ECHO_2 == 0) {}
            break;
        case 3:
            while(ECHO_3 == 0) {}
            break;
        case 4:
            while(ECHO_4 == 0) {}
            break;
    }

    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
    set_timer1(0);

    switch(sensor) 
    {
        case 1:
            while(ECHO_1 == 1) {}
            break;
        case 2:
            while(ECHO_2 == 1) {}
            break;
        case 3:
            while(ECHO_3 == 1) {}
            break;
        case 4:
            while(ECHO_4 == 1) {}
            break;
    }

    setup_timer_1(T1_DISABLED);

    time[sensor - 1] = get_timer1() * 1.6f;  
    distance[sensor - 1] = (float)time[sensor - 1] / 58.0f; 

    return distance[sensor - 1];
}

void distance_display()
{
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "D:%.0fcm   ", distance[0]);
    lcd_gotoxy(1, 2);
    printf(lcd_putc, "T:%.0fcm   ", distance[1]);
    lcd_gotoxy(9, 1);
    printf(lcd_putc, "N:%.0fcm   ", distance[2]);
    lcd_gotoxy(9, 2);
    printf(lcd_putc, "B:%.0fcm   ", distance[3]);
}

void threshold_display()
{
    lcd_gotoxy(3, 1);
    printf(lcd_putc, "Set Threshold");

    lcd_gotoxy(1, 2);
    printf(lcd_putc, "Threshold: %f", threshold);
}

void set_threshold()
{
    if (BUTTON_B6 == 0) 
    {
        while(BUTTON_B6 == 0)   {}
        threshold += 10;
    }
    if (BUTTON_B7 == 0)  
    {
        while(BUTTON_B7 == 0)   {}
        threshold -= 10;
        if (threshold < 0) threshold = 0;  
    }
}

void choose_mode()
{
    if (BUTTON_B0 == 0)  
    {
        while(BUTTON_B0 == 0)   
        {}
        lcd_mode = !lcd_mode;
        lcd_putc("\f");
    }
}

void main()
{
    lcd_init();  
    lcd_putc("\f");
    lcd_gotoxy(1, 1);
    lcd_putc("HELLO");
    delay_ms(1000);
    while(TRUE)
    {
      choose_mode();
      measure_distance(1);
      measure_distance(2);
      measure_distance(3);
      measure_distance(4);

      bool alarm_triggered = false;
      for (int i = 0; i < 4; i++) 
      {
            if (distance[i] > threshold) 
            {
                alarm_triggered = true; 
                break; 
            }
        }

        if (alarm_triggered) 
        {
            output_high(buzzer); 
        } 
        else 
        {
            output_low(buzzer);  
        }

     
      if(lcd_mode == false)
      {
           distance_display();      
      }
      else
      {
           threshold_display();
           set_threshold();
      }

        delay_ms(100);  
    }
}

