#include <mcs51/8052.h>
#define led1 P1_0
#define led2 P1_1
#define led3 P1_2
#define led4 P1_3
#define led5 P1_4
#define led6 P1_5
#define led7 P1_6
#define led8 P1_7

typedef unsigned int u16;
typedef unsigned char u8;

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

void delay(u16 i)
{
    while (i--)
        ;
}

void main()
{
    int numberOfLedOn = 8;
    int numberOfLedOff = 8;
    while (1)
    {
        numberOfLedOff = 8;
        for (int i = 0; i < 8; i++)
        {
            // Chạy vòng lặp để sáng từng đèn đang off
            for (int j = 0; j < numberOfLedOff; j++)
            {
                turnSingleLedOn(j);
                delay(5000);
                turnSingleLedOff(j);
                delay(5000);
            }
            turnSingleLedOn(numberOfLedOff - 1); // Đèn cuối cùng sáng trong các đèn off sẽ được sáng lên
            delay(5000);
            numberOfLedOff -= 1;
        }
        // Tắt các đèn từ cuối về đầu
        numberOfLedOn = 8;
        for (int i = numberOfLedOn - 1; i >= 0; i--)
        {
            turnSingleLedOff(i);
            delay(5000);
        }
    }
}
