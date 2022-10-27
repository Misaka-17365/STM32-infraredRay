#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"



struct{
    uint8_t X;
    uint8_t Y;
}item[4];
uint8_t i, j;

uint8_t menu = 1,key_pre = 0, item_number = 1;

uint8_t stage[5][40];

void key()
{
	int i;
	i = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
		if(i != key_pre){
			key_pre = i;
			Delay_ms(20);
			if(key_pre == 0){
				//menu ^= 1;
				
				if(item[0].Y*40 > item[0].X * item[0].X)
				item[0].X++;
				else item[0].Y++;
			}
			
		}
}

int main()
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_9);
    GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	OLED_Init();
	item[0].X = 1;
	item[0].Y = 1;
	
    while(1){


        if(menu == 0){
			OLED_Clear();
            OLED_ShowString(1, 1, "POSITIONING");
            for(i = 0; i < item_number; i++){
                OLED_ShowChar(3+i, 1, '[');
                OLED_ShowChar(3+i, 1+2*6, ']');
                OLED_ShowChar(3+i, 1+5*6, '(');
                OLED_ShowChar(3+i, 1+8*6, ',');
                OLED_ShowChar(3+i, 1+11*6, ')');

            }
            while(menu == 0){
				key();
				for(i = 0; i < item_number; i++){
                OLED_ShowChar(3+i, 1+1*6, '1'+i);
                OLED_ShowChar(3+i, 1+6*6, '0'+item[i].X / 10);
                OLED_ShowChar(3+i, 1+7*6, '0'+item[i].X % 10);

                OLED_ShowChar(3+i, 1+9*6, '0'+item[i].Y / 10);
                OLED_ShowChar(3+i, 1+10*6, '0'+item[i].Y % 10);
				}
            }
        }

        if(menu == 1){
			OLED_Clear();
			OLED_ShowString(1, 1, "TRACING");
			
			OLED_ShowByte(2, 16, 0xc0);
			OLED_ShowByte(2, 59, 0xc0);
			OLED_ShowByte(8, 16, 0x03);
			OLED_ShowByte(8, 59, 0x03);
			for(i = 0; i < 42; i++){
				OLED_ShowByte(2, 17+i, 0x40);
				OLED_ShowByte(8, 17+i, 0x02);
			}
			for(i = 0; i < 5; i++){
				OLED_ShowByte(3+i, 16, 0xff);
				OLED_ShowByte(3+i, 59, 0xff);
			}

			OLED_ShowByte(2, 60, 0x40);
			OLED_ShowByte(2, 61, 0x40);
			OLED_ShowByte(2, 62, 0x40);
			OLED_ShowByte(2, 63, 0x40);
			OLED_ShowByte(2, 64, 0xe0);
			OLED_ShowByte(2, 65, 0x40);
			OLED_ShowChar(2, 68, 'X');

			OLED_ShowByte(8,16, 0xff);
			OLED_ShowByte(8,15, 0x40);
			OLED_ShowByte(8,17, 0x42);
			OLED_ShowChar(8, 7, 'Y');
			OLED_ShowChar(2, 7, 'O');


            while(menu == 1){
				key();
				stage[(item[0].Y - 1) / 8][item[0].X - 1] |= 0x01 << ((item[0].Y - 1) % 8);
				
				for(i = 0; i < 5; i++){
					for(j = 0; j < 40; j++){
						OLED_ShowByte(3+i, 18+j, stage[i][j]);
					}
				}
            }
        }
    }
}

