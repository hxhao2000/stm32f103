/***********************************************************************************************************************************
    *	@file  	  w25qxx.c
	*   @date     2020-08-17		
	*	@author   魔女开发板   老周
    *   @brief    代码分享     Q群文件夹：262901124  自由下载
    *   @taobao   淘宝硬件     ttps://demoboard.taobao.com/
    ********************************************************************************************************************************
    ** 1.8寸彩色显示屏模块引脚说明(ST7735)
    ** GND         电源地
    ** VCC         3.3V。不要接5V, 模块不带LDO，5V很易烧屏
    ** SCL/SCK     SPI的SCK引脚  
    ** SDA/DIN     SPI的MOSI引脚 
    ** RES/RST     复位引脚，最好和单片机的复位引脚相接
    ** DC /RS      命令、数据选择引脚
    ** CS /CE      SPI的CS片选引脚
    ** BL /LED     控制背光LED， 高电平亮。引脚已串1K电阻，且已10K上拉。
    **  
    ** 测试硬件    魔女开发板_STM32F103RCT6 + 1.8寸彩屏模块(ST7735)
************************************************************************************************************************************/
#include "lcd_st7735.h"
#include "stm32f10x_spi.h"
#include "font.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
/*****************************************************************************
 ** 液晶控制宏定义, 主要为了简化文件内操作
 *****************************************************************************/
// LCD_CS  
#define	LCD_CS_LOW  	LCD_CS_GPIO->BSRR=LCD_CS_PIN <<16
#define LCD_CS_HIGH     LCD_CS_GPIO->BSRR=LCD_CS_PIN  
// 
#define	LCD_BL_LOW  	LCD_BL_GPIO->BSRR=LCD_BL_PIN  <<16 
#define	LCD_BL_HIGH  	LCD_BL_GPIO->BSRR=LCD_BL_PIN 

#define	LCD_RS_LOW  	LCD_DC_GPIO->BSRR=LCD_DC_PIN <<16
#define	LCD_RS_HIGH  	LCD_DC_GPIO->BSRR=LCD_DC_PIN 
//LCD_RST
#define	LCD_RES_LOW  	LCD_RES_GPIO->BSRR=LCD_RES_PIN <<16
#define	LCD_RES_HIGH  	LCD_RES_GPIO->BSRR=LCD_RES_PIN 
 	


/*****************************************************************************
 ** 本地变量
 *****************************************************************************/
struct _LCD              // 结构体：用于保存信息、状态
{     
    u8  InitOK;
	u16 width;			 // LCD 宽度
	u16 height;			 // LCD 高度
    
    u32 bColor;          // 背景色
}xLCD;

/*****************************************************************************
 ** 本地函数
 ****************************************************************************/
void gpioInit(void);                                         // 引脚初始化
void spiInit(void);                                          // SPI通信协议初始化
void sendOrder(u16 order);                                   // 向液晶屏发送：寄存器地址值(指令), 8位
void sendByte (u8  Data);                                    // 向液晶屏发送：数据，8位
void sendShort(u16 Data);                                    // 向液晶屏发送：数据，16位
void setCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd);  // 设置显示区域


// 引脚初始化
void gpioInit(void)
{
//#if 1
//    // 使用魔女开发板团队编写的初始化函数，可直接替代下方标准库
//    GPIOSet (LCD_SCK_GPIO , LCD_SCK_PIN , GPIO_Mode_AF ,  GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
//    GPIOSet (LCD_MOSI_GPIO, LCD_MOSI_PIN, GPIO_Mode_AF ,  GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
//    GPIOSet (LCD_RES_GPIO , LCD_RES_PIN , GPIO_Mode_OUT , GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
//    GPIOSet (LCD_DC_GPIO ,  LCD_DC_PIN ,  GPIO_Mode_OUT , GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
//		GPIOSet (LCD_CS_GPIO ,  LCD_CS_PIN ,  GPIO_Mode_OUT , GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
//		GPIOSet (LCD_BL_GPIO ,  LCD_BL_PIN ,  GPIO_Mode_OUT , GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );	
//#else
	// 使用标准库
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );// PORT时钟使能 
    // SCK
	GPIO_InitStructure.GPIO_Pin   = LCD_SCK_PIN  ;	  
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;           // 复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_SCK_GPIO, &GPIO_InitStructure);
    // MOSI
	GPIO_InitStructure.GPIO_Pin   = LCD_MOSI_PIN ;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; 
    GPIO_Init(LCD_MOSI_GPIO , &GPIO_InitStructure);
    // RES
	GPIO_InitStructure.GPIO_Pin   = LCD_RES_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
    GPIO_Init(LCD_RES_GPIO , &GPIO_InitStructure);
    // DC
	GPIO_InitStructure.GPIO_Pin   = LCD_DC_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
    GPIO_Init(LCD_DC_GPIO , &GPIO_InitStructure);
    // CS
	GPIO_InitStructure.GPIO_Pin   = LCD_CS_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
    GPIO_Init(LCD_CS_GPIO , &GPIO_InitStructure);
	// BL
	GPIO_InitStructure.GPIO_Pin   = LCD_BL_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
    GPIO_Init(LCD_BL_GPIO , &GPIO_InitStructure);
//#endif
	LCD_CS_HIGH ;   // 先上拉，防止电平浮动
	LCD_BL_HIGH ;   // 打开屏幕显示	
}

// SPI通信协议初始化
// 重要：因为SPI总线可能挂载多个要求不同通信参数的设备，最好在每次通信前，调用一次配置函数 
void spiInit(void)
{
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	
 

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
	//SPI2_ReadWriteByte(0xff);//启动传输		 
 

}   

// SPI发送传送1字节
u8 spiSendByte(u8 data)
{
    static u16 retry;
    
	retry=0;	
    while((LCD_SPI_PORT -> SR & 2) == 0) {  // 等待发送区为空	
		retry++;
		if(retry>500) return 0;
	}
    LCD_SPI_PORT ->DR = data;
	
	retry =0;    
    while((LCD_SPI_PORT -> SR & 1) == 0){   // 等待接收完数据	
		retry++;
		if(retry>250) return 0;
	}
    return LCD_SPI_PORT -> DR ;       
}		   



/*****************************************************************************
*函  数：LCD_Init
*功  能：初始化lcd
*参  数：为了简化理解、便于移植，参数(屏幕像素、显示方向)在h文件里进行修改
*返回值：无 
*备  注：
*****************************************************************************/
void LCD_Init(void)  
{ 	 
    xLCD.InitOK = 0; 
    xLCD.bColor  = BLACK;
    
	// 显示方向像素匹配
    if( (LCD_DIR ==1) || (LCD_DIR ==3))
    {
        xLCD.width  = LCD_WIDTH ;       // 屏宽度像素，超过此值驱动芯片会自动换行，注意：如果屏幕右边有花屏，就加大这个值
        xLCD.height = LCD_HIGH ;        // 屏高度像素， 注意：如果屏幕下面有花屏，就加大这个值
    }
    else
    {
        xLCD.width  = LCD_HIGH;
        xLCD.height = LCD_WIDTH;
    }      
	
	// 初始化GPIO
	gpioInit();
	// 初始化SPI外设、协议
    spiInit();
	
	// 屏幕复位	
	LCD_RES_LOW ;	       // LCD_RST=0	  //SPI接口复位
	Delay_ms(20);          // delay 20 ms 
    LCD_RES_HIGH  ;        // LCD_RST=1		
	Delay_ms(20);

	sendOrder(0x11);       // 退出睡眠模式
	Delay_ms(120);         // Delay 120ms
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	sendOrder(0xB1);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
    
	sendOrder(0xB2);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
    
	sendOrder(0xB3);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
	sendOrder(0xB4); // Dot inversion
	sendByte(0x03);
    
	sendOrder(0xC0); // ST7735R Power Sequence
	sendByte(0x28);
	sendByte(0x08);
	sendByte(0x04);
    
	sendOrder(0xC1);
	sendByte(0XC0);
    
	sendOrder(0xC2);
	sendByte(0x0D);
	sendByte(0x00);
	sendOrder(0xC3);
    
	sendByte(0x8D);
	sendByte(0x2A);
    
	sendOrder(0xC4);
	sendByte(0x8D);
	sendByte(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	sendOrder(0xC5); //VCOM
	sendByte(0x1A);
	sendOrder(0x36); //MX, MY, RGB mode
	if(LCD_DIR == 1)  sendByte(0xC0);  // 重要：显示方向控制，C0/00/A0/60,  C8/08/A8/68
	if(LCD_DIR == 2)  sendByte(0x00);      
    if(LCD_DIR == 3)  sendByte(0xA0);
    if(LCD_DIR == 4)  sendByte(0x60);
	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	sendOrder(0xE0);
	sendByte(0x04);
	sendByte(0x22);
	sendByte(0x07);
	sendByte(0x0A);
	sendByte(0x2E);
	sendByte(0x30);
	sendByte(0x25);
	sendByte(0x2A);
	sendByte(0x28);
	sendByte(0x26);
	sendByte(0x2E);
	sendByte(0x3A);
	sendByte(0x00);
	sendByte(0x01);
	sendByte(0x03);
	sendByte(0x13);
    
	sendOrder(0xE1);
	sendByte(0x04);
	sendByte(0x16);
	sendByte(0x06);
	sendByte(0x0D);
	sendByte(0x2D);
	sendByte(0x26);
	sendByte(0x23);
	sendByte(0x27);
	sendByte(0x27);
	sendByte(0x25);
	sendByte(0x2D);
	sendByte(0x3B);
	sendByte(0x00);
	sendByte(0x01);
	sendByte(0x04);
	sendByte(0x13);
    
    // 后期复制增加的，不明白
    sendOrder(0x2a);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x7f);
    // 后期复制增加的，不明白
	sendOrder(0x2b);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x9f);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
	sendOrder(0x3A); // 65k mode
	sendByte(0x05);
	
    sendOrder(0x29); // Display on

	LCD_Fill(1, 1, xLCD.width , xLCD.height , BLACK );
    xLCD.InitOK = 1; 	
}  
// 向LCD发送寄存器地址(指令)
void sendOrder(u16 order)
{ 
	LCD_CS_LOW;            // SPI设备片选拉低，开始通信
    LCD_RS_LOW;            // RS高: 数据值， RS低: 寄存器地址值
	spiSendByte(order);    // 发送 寄存器地址值
	LCD_CS_HIGH;           // SPI设备片选拉高，结束通信 
}
// 向LCD发送数值
void sendByte(u8 data) 
{
 	LCD_CS_LOW;            // SPI设备片选拉低，开始通信
	LCD_RS_HIGH;	       // RS高: 数据值， RS低: 寄存器地址值
	spiSendByte(data);     // 发送1字节
	LCD_CS_HIGH;           // SPI设备片选拉高，结束通信 			 
}	
// 向LCD发送2个字节
void sendShort(u16 data)
{
 	LCD_CS_LOW;            // SPI设备片选拉低，开始通信
	LCD_RS_HIGH;	       // RS高: 数据， RS低: 指令
	spiSendByte(data>>8);  // 发送1字节
	spiSendByte(data);     // 发送1字节
	LCD_CS_HIGH;           // SPI设备片选拉高，结束通信 	
}
				   
//写寄存器: 寄存器地址，数据
void sendRegisterValue(u8 reg, u16 val)
{
	sendOrder(reg);
	sendShort(val);
}	 
	
/*****************************************************************
 * 函  数：LCD_Display
 * 功  能：显示屏开关
 * 参  数：0:熄，非0：开
 * 返回值：无
 *
 * 备  注： 魔女开发板团队编写   淘宝 https://demoboard.taobao.com
 * 分  享： 不断更新于Q群文件夹：262901124  最后修改_2020年09月01日
******************************************************************/
void LCD_Display(u8 sw)
{
	if(sw==0)
	    LCD_BL_LOW ;
	else
		LCD_BL_HIGH ;	
}
	
/*****************************************************************
 * 函  数：setCursor
 * 功  能：设置显示区域，在此区域写点数据自动换行
 * 参  数：xStart横坐标起点, yStart纵坐标起点
 *         xEnd横坐标线束点，yEnd纵坐标线束点
 * 返回值：无
 *
 * 备  注： 魔女开发板团队编写   淘宝 https://demoboard.taobao.com
 * 分  享： 不断更新于Q群文件夹：262901124  最后修改_2020年09月01日
******************************************************************/
void setCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
{		
	sendOrder(0x2A);
	sendByte (0x00);   // 起始位置x高位，因为1.8寸屏是128*160, 不大于255, 直接写0省事
	sendByte (xStart); // 起始位置x低位，值传递时自动舍弃了高8位，也省得运算了
	sendByte (0x00);   // 起始位置y高位
	sendByte (xEnd);   // 起始位置x位位

	sendOrder(0x2B);
	sendByte (0x00);
	sendByte (yStart);
	sendByte (0x00);
	sendByte (yEnd);
	
	sendOrder(0x2c);  // 发送写数据命令
}

/*****************************************************************
 * 函  数：drawPoint
 * 功  能：画一个点
 * 参  数：x坐标，y坐标, 16位颜色值
 * 返回值：无
 * 
 * 备  注： 魔女开发板团队编写   淘宝 https://demoboard.taobao.com
 * 分  享： 不断更新于Q群文件夹：262901124  最后修改_2020年09月01日
******************************************************************/
void drawPoint(u16 x, u16 y, u16 color)
{
    setCursor(x, y, x, y);      //设置光标位置 
    sendShort(color);    
}

/******************************************************************
 * 函数名： LCD_Fill
 * 功  能： 在指定区域内填充单个颜色
 *          屏幕坐标从1开始；屏幕左右和下方实际上有多余行列
 * 参  数： u16 xStart, yStart  左上角起始坐标
 *          u16 xEnd,   yEnd    右下角起始坐标
 *          u32 Color           填充的颜色
 *
 * 备  注： 魔女开发板团队编写   淘宝 https://demoboard.taobao.com
 * 分  享： 不断更新于Q群文件夹：262901124  最后修改_2020年09月01日
 *****************************************************************/
void LCD_Fill(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd, u16 color)
{     
	spiInit();                                      // 防止SPI参数被其它设备修改了
	u32 pixel = (xEnd-xStart+1) * (yEnd-yStart+1);	// 填充的像素数量
								   
	setCursor(xStart, yStart, xEnd, yEnd);      	// 设定填充范围
    while(pixel-->0)                                // 发送颜色值
       sendShort(color);                            
}  







/******************************************************************
 * 函数名： drawAscii
 * 功  能： 在指定位置显示一个字符	
 * 参  数： u16 x,y     起始坐标
 *          u8  num     要显示的字符:" "--->"~"
 *          u8  size    字体大小 12/16/24/32
 *          u32 fColor  字体颜色
 *          u32 bColor  背景颜色
 * 备  注： 参考原子哥和野火大神的代码后修改  资料存放Q群：262901124 
 *****************************************************************/
void drawAscii(u16 x,u16 y,u8 num,u8 size,u32 fColor, u32 bColor)
{  			
	spiInit();                                        // 防止SPI参数被其它设备修改了
	
    if(xLCD.InitOK ==0) return;
    
    u8 temp;
	u8 t,t1;
	u16 y0=y;
    
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		   // 得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';                                       // 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)         temp=asc2_1206[num][t];   // 调用1206字体
		else if(size==16)    temp=asc2_1608[num][t];   // 调用1608字体
		else if(size==24)    temp=asc2_2412[num][t];   // 调用2412字体
		else if(size==32)    temp=asc2_3216[num][t];   // 调用3216字体
		else return;								   // 没有的字库		
		for(t1=0; t1<8; t1++)
		{			    
            if(temp&0x80)   drawPoint (x, y, fColor);  // 字体 画点 
            else            drawPoint (x, y, bColor);  // 背景 画点
            temp<<=1;
			y++;
			if(y>=xLCD.height)    return;		       // 超出屏幕高度(底)
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=xLCD.width) return;              // 超出屏幕宽度(宽)
				break;
			}
		}  	 
	}  	    	 	  
} 







/******************************************************************************
 * 函  数： LCD_String
 * 功  能： 在屏幕上显示字符串，支持英文、中文
            重要提示：注意字库的格式、位置， 本函数字库在W25Q128中
 *         
 * 参  数： 
 * 返回值：　
 * 备  注： 魔女开发板团队  资料存放Q群：262901124        最后修改_2020年05月1８日
 ******************************************************************************/  
void LCD_String(u16 x, u16 y, char* pFont, u8 size, u32 fColor, u32 bColor)
{
    if(xLCD .InitOK ==0 ) return;
    
    u16 xStart = x;
    
    // 字体大小控制
    if( size!=12 && size!=16 && size!=24 && size!=32 )
        size=24;    
    
    while(*pFont != 0)
    {
        // 位置控制        
        if(x>(xLCD.width-size))       // 如果这一行不够位置，就下一行
        {
            x=xStart;
            y=y+size;
        }
        if(y>(xLCD.height - size))    // 如果到了屏幕底部，就返回，不再输出
            return;        
        
		// 判断文字是ASCII还是汉字
        if(*pFont < 127)              // ASCII字符
        {
            drawAscii (x, y, *pFont, size, fColor, bColor);            
            pFont++;
            x+=size/2;            
        }
    }      
}





