#include <reg51.h>
#include <mcs51/8052.h>

#define led1 P1_0
#define led2 P1_1
#define led3 P1_2
#define led4 P1_3
#define led5 P1_4
#define led6 P1_5
#define led7 P1_6
#define led8 P1_7

unsigned int flag = 1;

void delay_ms(unsigned int time);

void option1();
void option2();
void option3();

// Xử lý ngắt
void ISR1(void) __interrupt(0)
{
    if (flag == 3)
        flag = 1;
    else
        flag++;
}
void ISR2(void) __interrupt(2)
{
    if (flag == 1)
        flag = 3;
    else
        flag--;
}

// Bật đèn thứ i
void turnSingleLedOn(int i)
{
    switch (i)
    {
    case 0:
        led1 = 0;
        break;
    case 1:
        led2 = 0;
        break;
    case 2:
        led3 = 0;
        break;
    case 3:
        led4 = 0;
        break;
    case 4:
        led5 = 0;
        break;
    case 5:
        led6 = 0;
        break;
    case 6:
        led7 = 0;
        break;
    case 7:
        led8 = 0;
        break;
    }
}

// Tắt đèn thứ i
void turnSingleLedOff(int i)
{
    switch (i)
    {
    case 0:
        led1 = 1;
        break;
    case 1:
        led2 = 1;
        break;
    case 2:
        led3 = 1;
        break;
    case 3:
        led4 = 1;
        break;
    case 4:
        led5 = 1;
        break;
    case 5:
        led6 = 1;
        break;
    case 6:
        led7 = 1;
        break;
    case 7:
        led8 = 1;
        break;
    }
}

// tắt tất cả đèn
void offAllLed()
{
    led1 = 1;
    led2 = 1;
    led3 = 1;
    led4 = 1;
    led5 = 1;
    led6 = 1;
    led7 = 1;
    led8 = 1;
}

void main()
{
    EX0 = 1;
    EX1 = 1;
    IT0 = 1;
    IT1 = 1;
    EA = 1;

    while (1)
    {
        if (flag == 1)
            option1();
        if (flag == 2)
            option2();
        if (flag == 3)
            option3();
    }
}

void delay_ms(unsigned int time)
{
    int i, j;
    for (i = 0; i < 1000; i++)
    {
        for (j = 0; j < time; j++)
            ;
    }
}

void option1()
{
    offAllLed();
    delay_ms(10);
    // chay den tu trai sang phai
    for (int i = 0; i < 7; i++)
    {
        if (flag != 1)
            return;
        turnSingleLedOn(i);
        turnSingleLedOn(i + 1);
        delay_ms(10);
        turnSingleLedOff(i);
        turnSingleLedOff(i + 1);
    }

    // chay den tu phai sang trai
    for (int i = 0; i < 7; i++)
    {
        if (flag != 1)
            return;
        turnSingleLedOn(7 - i);
        turnSingleLedOn(6 - i);
        delay_ms(10);
        turnSingleLedOff(7 - i);
        turnSingleLedOff(6 - i);
    }
}

void option2()
{
    offAllLed();
    delay_ms(10);
    int numberOfLedOff = 8;
    for (int i = 0; i < 8; i++)
    {
        if (flag != 2)
            return;
        // Chạy vòng lặp để sáng từng đèn đang off
        for (int j = 0; j < numberOfLedOff; j++)
        {
            if (flag != 2)
                return;
            turnSingleLedOn(j);
            delay_ms(10);
            turnSingleLedOff(j);
            delay_ms(10);
        }
        turnSingleLedOn(numberOfLedOff - 1); // Đèn cuối cùng sáng trong các đèn off sẽ được sáng lên
        delay_ms(10);
        numberOfLedOff -= 1;
    }
    // Tắt các đèn từ cuối về đầu
    int numberOfLedOn = 8;
    for (int i = numberOfLedOn - 1; i >= 0; i--)
    {
        if (flag != 2)
            return;
        turnSingleLedOff(i);
        delay_ms(10);
    }
}

void option3()
{
    offAllLed();
    delay_ms(10);
    int numberOfLed = 8;
    for (int i = 0; i <= 8; i++)
    {
        if (flag != 3)
            return;
        if (i == 8)
        {
            delay_ms(10);
        }
        turnSingleLedOn(i);                   // Bật đèn i
        turnSingleLedOn(numberOfLed - 1 - i); // Bật đèn đối xứng với đèn i
        delay_ms(10);
        turnSingleLedOff(i);                   // Tắt đèn i
        turnSingleLedOff(numberOfLed - 1 - i); // Tắt đèn đối xứng với đèn i
        delay_ms(10);
    }
}
