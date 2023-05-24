#include <mcs51/8052.h>
#define TH0_50ms 0x3C//Voi TH va TL co gia tri nhu tren ket hop voi tan so cua AT89S52 la 12Mhz
#define TL0_50ms 0xB0//se tao ra khoang thoi gian giua 2 lan kich hoat ngat la 50ms
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

void delay(unsigned int time)
{
    int i, j;
    for (i = 0; i < 100; i++)
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
        delay(1);
        Beep = !Beep;
    }
    Beep = 0;
}

void Show_Time() // Hàm hiển thị thời gian
{
    int minutes = (time/20) / 60;
    int seconds = (time/20) % 60;
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
            delay(10);
            P1 = 0xFF;
            break;
        case 1: // Bật LEDs hiển thị phút
            Q4 = 1;
            Q3 = 0;
            Q2 = 1;
            Q1 = 1;
            P1 = LED_Mask[minutes % 10];
            delay(10);
            P1 = 0xFF;
            break;
        case 2: // Bật LEDs hiển thị giây
            Q4 = 1;
            Q3 = 1;
            Q2 = 0;
            Q1 = 1;
            P1 = LED_Mask[seconds / 10];
            delay(10);
            P1 = 0xFF;
            break;
        case 3: // Bật LEDs hiển thị giây
            Q4 = 1;
            Q3 = 1;
            Q2 = 1;
            Q1 = 0;
            P1 = LED_Mask[seconds % 10];
            delay(10);
            P1 = 0xFF;
            break;
        }
    }
}

//Timer 0 interrupt handler
void TMR0_Process() __interrupt TF0_VECTOR{
    TF0  = 0;             //Xoa co bao tran
    TR0  = 0;             //Tam dung bo dinh thoi
    TH0  = TH0_50ms;    //Set thanh ghi TH va TL ve gia tri ban dau
    TL0  = TL0_50ms;
    if(time>0){
        time--;//Giam bien timmer di 1
    }
    // if(buzzertimer>0){
    //     buzzertimer--;//Giam bien buzzertimmer di 1
    // }
    // if(buzzertimer<=20*5){//Het thoi gian timmer chuong keu 3 lan
    //     if(buzzertimer%20==0){
    //         P3_6 = !P3_6;
    //     }
    // }
    // TR0  = 1;//Bat lai bo dem thoi gian
    // if(buzzertimer==0){
    //     TR0 = 0;//Ket thuc bo dem thoi gian
    // }
}

// Xử lý ngắt
void ISR1(void) __interrupt(0)
{
    TR0 = 0;
    EA = 0;    //Cam ngat
    time += 20 * 60;
    EA = 1;    //Cho phep ngat
}
void ISR2(void) __interrupt(2)
{
    EA=0;    //Cam ngat
    if(TR0 == 0){
        TR0 = 1;//Bat bo dem thoi gian
    } else{
        TR0 = 0;//Tat bo dem thoi gian
    }
    EA = 1;
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
    }
}