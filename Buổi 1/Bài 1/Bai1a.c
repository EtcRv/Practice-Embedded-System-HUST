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

void delay(u16 i)
{
    while (i--)
        ;
}

void main()
{
    int fl = 1; // Biến xác định chiều sáng của đèn. 1 thì đèn sáng từ trái qua phải, 0 thì đèn sáng từ phải qua trái.
    int i = 0;
    while (1)
    {
        switch (i)
        {
        case 0: // Tắt tất cả đèn
            offAllLed();
            delay(50000);
            fl = 1;
            i += 1;
            break;
        case 1: // Sáng cặp đèn liền nhau và tắt đèn đã sáng trước đó
            led1 = 0;
            led2 = 0;
            led3 = 1;
            delay(50000); //~450ms
            if (fl)
                i += 1;
            else
                i -= 1;
            break;
        case 2: // Sáng cặp đèn liền nhau và tắt đèn đã sáng trước đó
            led1 = 1;
            led2 = 0;
            led3 = 0;
            led4 = 1;
            delay(50000); //~450ms
            if (fl)
                i += 1;
            else
                i -= 1;
            break;
        case 3: // Sáng cặp đèn liền nhau và tắt đèn đã sáng trước đó
            led2 = 1;
            led3 = 0;
            led4 = 0;
            led5 = 1;
            delay(50000); //~450ms
            if (fl)
                i += 1;
            else
                i -= 1;
            break;
        case 4: // Sáng cặp đèn liền nhau và tắt đèn đã sáng trước đó
            led3 = 1;
            led4 = 0;
            led5 = 0;
            led6 = 1;
            delay(50000); //~450ms
            if (fl)
                i += 1;
            else
                i -= 1;
            break;
        case 5: // Sáng cặp đèn liền nhau và tắt đèn đã sáng trước đó
            led4 = 1;
            led5 = 0;
            led6 = 0;
            led7 = 1;
            delay(50000); //~450ms
            if (fl)
                i += 1;
            else
                i -= 1;
            break;
        case 6: // Sáng cặp đèn liền nhau và tắt đèn đã sáng trước đó
            led5 = 1;
            led6 = 0;
            led7 = 0;
            led8 = 1;
            delay(50000); //~450ms
            if (fl)
                i += 1;
            else
                i -= 1;
            break;
        case 7: // Sáng cặp đèn liền nhau và tắt đèn đã sáng trước đó
            led6 = 1;
            led7 = 0;
            led8 = 0;
            delay(50000); //~450ms
            if (fl)
                i += 1;
            else
                i -= 1;
            break;
        case 8: // Tắt tất cả đèn
            offAllLed();
            delay(50000);
            fl = 0;
            i -= 1;
            break;
        }
    }
}
