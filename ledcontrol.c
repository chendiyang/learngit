#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXTIMINGS  85
#define DHTPIN 
 
char  LED_1=25; /* GPIO.25对应树莓派37号引脚 */
char  LED_2=24; /* GPIO.24对应树莓派35号引脚 */
char  LED_3=23; /* GPIO.23对应树莓派33号引脚 */
char  LED_4=28; /* GPIO.28对应树莓派38号引脚 */
char  LED_State[6]={ 0, 0, 0, 0, 0, 0 };  /*灯的状态*/
int dht11_dat[5] = { 0, 0, 0, 0, 0 };     /*温度传感器的数据*/
/*输入四个参数控制的灯的状态,1代表开灯,0代表关灯*/
void control_led_Open(int a,int b,int c,int d)
{
   digitalWrite(LED_1,a);
   LED_State[0]=a;
   digitalWrite(LED_2,b);
   LED_State[1]=b;
   digitalWrite(LED_3,c);
   LED_State[2]=c;
   digitalWrite(LED_4,d);
   LED_State[3]=d;
}

/*oid control_led_Off()

{

   digitalWrite(LED_1,0);
   LED_State[0]=0;
   digitalWrite(LED_2,0);
   LED_State[1]=0;
   digitalWrite(LED_3,0);
   LED_State[2]=0;
   digitalWrite(LED_4,0);
   LED_State[3]=0;
}*/

void read_dht_led_data()
{   
    uint8_t counter     = 0;    /*计数器*/
    uint8_t laststate   = HIGH; /*指定GPIO针脚为高电平*/
   
    uint8_t j       = 0, i;
    float   f; /* 华氏温度 */
    /*初始化的dht11传感器的数据*/
   dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;


   /* 将引脚DHTPIN设置为低电平,并延迟18毫秒 */
    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    /* 然后将引脚DHTPIN设置为高电平,并延迟40毫秒 */
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    /* 将引脚设置为输出状态 */
    pinMode( DHTPIN, INPUT );

    /* 检查传来数据是否正确,若正确则读取数据 */
    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHTPIN );

        if ( counter == 255 )
            break;

        /* 忽略前三次的过渡数据 */
        if ( (i >= 4) && (i % 2 == 0) )
        {
            /* 将每一位转化成存储字节 */
            dht11_dat[j / 8] <<= 1;
            if ( counter > 16 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    /*
     * 读取40位,dht11一共有5个数据,每个数据有8位+ 并验证一下数据是否正确
     * 如果数据正确,将dht11获取的数据输出,(同时输出灯的状态)
     */
    if ( (j >= 40) &&
         (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
    {
        f = dht11_dat[2] * 9. / 5. + 32;
        printf( "Temperature = %d.%d *C (%.1f *F),The State for led: %4d,%4d,%4d,%4d,%4d,%4d\n",
             dht11_dat[2], dht11_dat[3], f, LED_State[0],LED_State[1],LED_State[2],LED_State[3],LED_State[4],LED_State[5]);
    }else  {
        printf( "Data not good, skip\n" );
    }
}

int main( void )
{
    int a[4]={0,0,0,0};
    int i;
    if ( wiringPiSetup() < 0 )
        return -1;
    /*修改灯的状态为输出状态*/
    pinMode(LED_1,OUTPUT);
    pinMode(LED_2,OUTPUT);
    pinMode(LED_3,OUTPUT);
    pinMode(LED_4,OUTPUT);
    /*传入4个参数分别控制4个灯的状态*/
    for(i=0;i<4;i++)
       scanf("%d",&a[i]);
   control_led_Open(a[0],a[1],a[2],a[3]);
    while ( 1 )
    {   
        /*读取dht11数据函数*/ 
        read_dht11_dat();
        delay( 1000 ); 
    }

    return(0);
}

