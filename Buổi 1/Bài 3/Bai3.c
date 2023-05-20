#include <reg51.h>
#include <mcs51/8052.h>
#define B_set P3_2
#define B_start P3_3

#define Q4 P2_0
#define Q3 P2_1
#define Q2 P2_2
#define Q1 P2_3
#define Beep P3_6

typedef unsigned int u16;
typedef unsigned char u8;
u8 i;
u8 LED_Mask[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

unsigned int flag;

// Biến đếm thời gian
unsigned int time;

void delay_ms(unsigned int time)
{
    int i, j;
    for (i = 0; i < 1000; i++)
    {
        for (j = 0; j < time; j++)
            ;
    }
}

void turnOnBuzzer()
{
    unsigned int i;
    // 3 times
    for (i = 0; i < 6; i++)
    {
        delay_ms(1000);
        Beep = !Beep;
    }
    Beep = 0;
}

void Show_Time() // Hàm hiển thị thời gian
{
    int minutes = time / 60;
    int seconds = time % 60;
    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0: // Bật LEDs hiển thị phút
            Q4 = 0;
            Q3 = 1;
            Q2 = 1;
            Q1 = 1;
            P1 = LED_Mask[minutes / 10];
            // delay_ms(1);
            // P1 = 0xFF;
            break;
        case 1: // Bật LEDs hiển thị phút
            Q4 = 1;
            Q3 = 0;
            Q2 = 1;
            Q1 = 1;
            P1 = LED_Mask[minutes % 10];
            // delay_ms(1);
            // P1 = 0xFF;
            break;
        case 2: // Bật LEDs hiển thị giây
            Q4 = 1;
            Q3 = 1;
            Q2 = 0;
            Q1 = 1;
            P1 = LED_Mask[seconds / 10];
            // delay_ms(1);
            // P1 = 0xFF;
            break;
        case 3: // Bật LEDs hiển thị giây
            Q4 = 1;
            Q3 = 1;
            Q2 = 1;
            Q1 = 0;
            P1 = LED_Mask[seconds % 10];
            // delay_ms(1);
            // P1 = 0xFF;
            break;
        }
    }
}

// Xử lý ngắt
void ISR1(void) __interrupt(0)
{
    flag = 1;
}
void ISR2(void) __interrupt(2)
{
    flag = 2;
}

void main()
{
    time = 0;
    Beep = 0;

    EX0 = 1;
    EX1 = 1;
    IT0 = 1;
    IT1 = 1;
    EA = 1;

    while (1)
    {
        Show_Time();

        if (flag == 1)
        {
            time += 60;
            flag = 0;
        }
        else if (flag == 2)
        {
            if (time == 0)
            {
                turnOnBuzzer();
                flag = 0;
            }
            else
            {
                delay_ms(1000);
                time -= 1;
            }
        }
    }
}