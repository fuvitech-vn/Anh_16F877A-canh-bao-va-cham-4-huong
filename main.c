#include <main.h>

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

float distance[4], time[4]; 

// H�m �o kho?ng c�ch t? c?m bi?n si�u �m
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
            output_low(TRIGGER_1);    // T?t Trigger c?a c?m bi?n 1
            break;
        case 2:
            output_low(TRIGGER_2);    // T?t Trigger c?a c?m bi?n 2
            break;
        case 3:
            output_low(TRIGGER_3);    // T?t Trigger c?a c?m bi?n 3
            break;
        case 4:
            output_low(TRIGGER_4);    // T?t Trigger c?a c?m bi?n 4
            break;
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

// H�m hi?n th? kho?ng c�ch l�n LCD
void distance_display()
{
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "D:%.0fcm ", distance[0]);
    lcd_gotoxy(1, 2);
    printf(lcd_putc, "T:%.0fcm ", distance[1]);
    lcd_gotoxy(9, 1);
    printf(lcd_putc, "N:%.0fcm   ", distance[2]);
    lcd_gotoxy(9, 2);
    printf(lcd_putc, "B:%.0fcm   ", distance[3]);
}

void main()
{
    lcd_init();  // Kh?i t?o LCD
    lcd_putc("\f");
    lcd_gotoxy(1, 1);
    lcd_putc("HELLO");
    delay_ms(1000);
    while(TRUE)
    {
        // �o kho?ng c�ch t? 4 c?m bi?n
        measure_distance(1);
        measure_distance(2);
        measure_distance(3);
        measure_distance(4);

        // Hi?n th? k?t qu? l�n LCD
        distance_display();

        delay_ms(100);  // Th?i gian ch? gi?a c�c l?n �o
    }
}

