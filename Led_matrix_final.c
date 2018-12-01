sfr unsigned int volatile CONFIG     =     0x2007;

// Software SPI configuration
sbit SoftSpi_SDI at RA2_bit;
sbit SoftSpi_SDO at RB4_bit;
sbit SoftSpi_CLK at RB5_bit;
sbit SoftSpi_SDI_Direction at TRISA2_bit;
sbit SoftSpi_SDO_Direction at TRISB4_bit;
sbit SoftSpi_CLK_Direction at TRISB5_bit;
// max7219 LOAD/CS(-) pin configuration
sbit Chip_Select at RB3_bit;
sbit Chip_Select_Direction at TRISB3_bit;
sbit Chip_Select1 at RA0_bit;
sbit Chip_Select1_Direction at TRISA0_bit;

char message[50],msg[35];
unsigned char i,StringLength,k;

unsigned char temp;

short  l, scroll, shift_step=1,row,speed;
char index;

const char UART_txt1[] = "-8X16 LED Matrix Display -\r\n";
const char UART_txt2[] = "Zelite li unos poruke? D/N\r\n";
const char UART_txt3[] = "Zavrsite poruku sa slovom f\r\n";
const char UART_txt4[] = "Cekanje na poruku(MAX. 50 char.)\r\n";
const char UART_txt5[] = "Poruka primljena\r\n";

unsigned short Buffer[7][2] = {
                              {0,0},
                              {0,0},
                              {0,0},
                              {0,0},
                              {0,0},
                              {0,0},
                              {0,0},
                             };


const unsigned short CharData[][7] ={
{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
{0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000000,  0b00000100},
{0b00001010, 0b00001010, 0b00001010, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
{0b00000000, 0b00001010, 0b00011111, 0b00001010, 0b00011111, 0b00001010,  0b00011100},
{0b00000111, 0b00001100, 0b00010100, 0b00001100, 0b00000110, 0b00000101, 0b00011100},
{0b00011001, 0b00011010, 0b00000010, 0b00000100, 0b00000100, 0b00001000, 0b00001011},
{0b00000110, 0b00001010, 0b00010010, 0b00010100, 0b00001001, 0b00010110, 0b00001001},
{0b00000100, 0b00000100, 0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
{0b00000010, 0b00000100, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00000100},
{0b00001000, 0b00000100, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00001000},
{0b00010101, 0b00001110, 0b00011111, 0b00001110, 0b00010101, 0b00000000, 0b00000000},
{0b00000000, 0b00000000, 0b00000100, 0b00000100, 0b00011111, 0b00000100, 0b00000100},
{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000110, 0b00001000},
{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001110, 0b00000000, 0b00000000},
{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000100},
{0b00000001, 0b00000010, 0b00000010, 0b00000100, 0b00000100, 0b00001000, 0b00010000},
{0b00001110, 0b00010001, 0b00010011, 0b00010001, 0b00010101, 0b00010001, 0b00001110},
{0b00000100, 0b00001100, 0b00010100, 0b00000100, 0b00000100, 0b00000100, 0b00011111},
{0b00001110, 0b00010001, 0b00010001, 0b00000010, 0b00000100, 0b00001000, 0b00011111},
{0x1C,0x22,0x02,0x0C,0x02,0x22,0x1C},
{0b00010000, 0b00010000, 0b00010100, 0b00010100, 0b00011111, 0b00000100, 0b00000100},
{0b00011111, 0b00010000, 0b00010000, 0b00011110, 0b00000001, 0b00000001, 0b00011110},
{0b00000111, 0b00001000, 0b00010000, 0b00011110, 0b00010001, 0b00010001, 0b00001110},
{0b00011111, 0b00000001, 0b00000001, 0b00000001, 0b00000010, 0b00000100, 0b00010000},
{0b00001110, 0b00010001, 0b00010001, 0b00001110, 0b00010001, 0b00010001, 0b00001110},
{0x1C,0x22,0x22,0x1E,0x02,0x22,0x1C},
{0b00000000, 0b00000100, 0b00000100, 0b00000000, 0b00000000, 0b00000100, 0b00000000},
{0b00000000, 0b00000100, 0b00000100, 0b00000000, 0b00000000, 0b00000100, 0b00001000},
{0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00001000, 0b00000100, 0b00000001},
{0b00000000, 0b00000000, 0b00000000, 0b00011110, 0b00000000, 0b00011110, 0b00000000},
{0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000010, 0b00000100, 0b00010000},
{0b00001110, 0b00010001, 0b00010001, 0b00000010, 0b00000100, 0b00000100, 0b00000100},
{0b00001110, 0b00010001, 0b00010001, 0b00010101, 0b00010101, 0b00010001, 0b00011110},
{0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00011111, 0b00010001, 0b00010001},
{0b00011110, 0b00010001, 0b00010001, 0b00011110, 0b00010001, 0b00010001, 0b00011110},
{0b00000111, 0b00001000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00000111},
{0b00011100, 0b00010010, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00011100},
{0b00011111, 0b00010000, 0b00010000, 0b00011110, 0b00010000, 0b00010000, 0b00011111},
{0b00011111, 0b00010000, 0b00010000, 0b00011110, 0b00010000, 0b00010000, 0b00010000},
{0b00001110, 0b00010001, 0b00010000, 0b00010000, 0b00010111, 0b00010001, 0b00001110},
{0b00010001, 0b00010001, 0b00010001, 0b00011111, 0b00010001, 0b00010001, 0b00010001},
{0b00011111, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00011111},
{0x1E,0x04,0x04,0x04,0x04,0x24,0x18},
{0b00010001, 0b00010010, 0b00010100, 0b00011000, 0b00010100, 0b00010010, 0b00010001},
{0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00011111},
{0b00010001, 0b00011011, 0b00011111, 0b00010101, 0b00010001, 0b00010001, 0b00010001},
{0b00010001, 0b00011001, 0b00011001, 0b00010101, 0b00010101, 0b00010011, 0b00010001},
{0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00001110},
{0b00011110, 0b00010001, 0b00010001, 0b00011110, 0b00010000, 0b00010000, 0b00010000},
{0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010101, 0b00001111},
{0b00011110, 0b00010001, 0b00010001, 0b00011110, 0b00010100, 0b00010010, 0b00010001},
{0x0E,0x11,0x10,0x0E,0x01,0x11,0x0E},
{0b00011111, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100},
{0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00001110},
{0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00000100},
{0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010101, 0b00001010},
{0b00010001, 0b00010001, 0b00001010, 0b00000100, 0b00000100, 0b00001010, 0b00010001},
{0b00010001, 0b00010001, 0b00001010, 0b00000100, 0b00000100, 0b00000100, 0b00000100},
{0b00011111, 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00011111},
{0x14,0x08,0x1C,0x22,0x20,0x22,0x1C},
{0x04,0x08,0x1C,0x22,0x20,0x22,0x1C},
{0x78,0x44,0x44,0xe4,0x44,0x44,0x78},
{0x14,0x08,0x1e,0x20,0x3c,0x02,0x3c},
{0x14,0x08,0x3E,0x04,0x08,0x10,0x3e},
};


unsigned short Find_StrLength()
{
  return strlen(message);

}
char * CopyConst2Ram(char * dest, const char * src)
{
  char * d ;
  d = dest;
  for(;*dest++ = *src++;)
    ;
  return d;
}


void ListenSerial()
{
  unsigned char RxByte;

  asm CLRWDT;


  if (UART1_Data_Ready())
   {
    RxByte = UART1_Read();
    if(RxByte == 'D')
    {
      UART1_Write_Text(CopyConst2Ram(msg,Uart_txt3));
      UART1_Write_Text(CopyConst2Ram(msg,Uart_txt4));
      //CONFIG=0x216A;
      while (!UART1_Data_Ready());

      
      UART1_Read_Text(message, "f", 250);    // reads text until 'f' is found
      
       asm CLRWDT;
      StringLength = Find_StrLength();

      UART1_Write_Text(CopyConst2Ram(msg,Uart_txt5));
      UART1_Write_Text(CopyConst2Ram(msg,Uart_txt1));
      UART1_Write_Text(CopyConst2Ram(msg,Uart_txt2));

    }
   }
}
// max7219 configuration
void max7219_init1()
{

 Chip_Select = 0;       // SELECT MAX
 Soft_SPI_Write(0x09);      //  Disable decoding
 Soft_SPI_Write(0x00);
 Chip_Select = 1;       // DESELECT MAX

 Chip_Select = 0;
Soft_SPI_Write(0x0A);     //  Max segment luminosity intensity
 Soft_SPI_Write(0x0F);
 Chip_Select = 1;

 Chip_Select = 0;
 Soft_SPI_Write(0x0B);
 Soft_SPI_Write(0x07);      // Display refresh
 Chip_Select = 1;


 Chip_Select = 0;
 Soft_SPI_Write(0x0C);
 Soft_SPI_Write(0x01);      // Turn on the display
 Chip_Select = 1;

 Chip_Select = 0;
 Soft_SPI_Write(0x00);
 Soft_SPI_Write(0xFF);      // No test
 Chip_Select = 1;
}

void max7219_init2()
{

 Chip_Select1 = 0;
 Soft_SPI_Write(0x09);
 Soft_SPI_Write(0x00);
 Chip_Select1 = 1;

 Chip_Select1 = 0;
Soft_SPI_Write(0x0A);
 Soft_SPI_Write(0x0F);
 Chip_Select1 = 1;

 Chip_Select1 = 0;
 Soft_SPI_Write(0x0B);
 Soft_SPI_Write(0x07);
 Chip_Select1 = 1;

 Chip_Select1 = 0;
 Soft_SPI_Write(0x0C);
 Soft_SPI_Write(0x01);
 Chip_Select1 = 1;

 Chip_Select1 = 0;
 Soft_SPI_Write(0x00);
 Soft_SPI_Write(0xFF);
 Chip_Select1 = 1;
}
//Send data function for fist max
 void Write_Byte_First(unsigned short Row, unsigned short Value)
{
     Chip_Select = 0;
     Soft_SPI_Write(Row);
     Soft_SPI_Write(Value);
     Chip_Select = 1;
 }
////Send data function for second max
 void Write_Byte_Second(unsigned short Row, unsigned short Value)
{
      Chip_Select1 = 0;
      Soft_SPI_Write(Row);
      Soft_SPI_Write(Value);
      Chip_Select1 = 1;

}
//Clear matrix function
void Clear_Matrix(void)
{
 unsigned short x;

 for(x=1;x<9;x++)
 {
  Write_Byte_First(x,0x00);
  Write_Byte_Second(x,0x00);
  }
}




void Find_Char()
{
     if (message[k]>100)
      {
         switch (message[k])
          {
             case 200: temp = CharData[59][row]; break;
             case 198: temp = CharData[60][row]; break;
             case 208: temp = CharData[61][row]; break;
             case 138: temp = CharData[62][row]; break;
             case 142: temp = CharData[63][row]; break;
          }
      }
      else
          {
              index = message[k];
              temp = CharData[index-32][row];
          }
           asm CLRWDT;
}







void main()
{       unsigned char RxByte;
        TRISA=0;
        //UART pins
        TRISB.b1=1;
        TRISB.b2=0;

        //Status led
        TRISA.b3=0;
        PORTA.b3=0;


        OPTION_REG=0b00101110;

        //MAXX tris pins
        Chip_Select_Direction = 0;
        Chip_Select1_Direction = 0;



        UART1_Init(9600);
        delay_ms(100);

        Soft_SPI_init();
        delay_ms(100);

        max7219_init1();
        max7219_init2();
        asm CLRWDT;


        Clear_Matrix();


         StringLength = Find_StrLength();



        //Test displays
        for (i=1;i<8;i++)
        {
        Write_Byte_First(i,0xff);
        Write_Byte_Second(i,0xff);
        delay_ms(100);
         }

         Clear_Matrix();

       UART1_Write_Text(CopyConst2Ram(msg,Uart_txt1));
       UART1_Write_Text(CopyConst2Ram(msg,Uart_txt2));
       asm CLRWDT;

        while(1)
        {
              ListenSerial();

             for (k=0; k<StringLength; k++)
             {


                 PORTA.b3=~PORTA.b3;

              for (scroll=0; scroll<(8/shift_step); scroll++)
               {
                   for (row=0; row<7; row++)
                   {
                         Find_Char();
                      Buffer[row][0] = (Buffer[row][0] << Shift_Step)| (temp >> ((5-shift_step)-scroll*shift_step));
                      Buffer[row][1] = (Buffer[row][1] << Shift_Step) | (Buffer[row][0] >> (6-Shift_Step));

                   }
                      speed=5;
                       asm CLRWDT;
                      for(l=0; l<speed;l++)
                {
                     for (i=0; i<7; i++)
                         {
                              Write_Byte_First(i+1,Buffer[i][0]);
                              Write_Byte_Second(i+1,Buffer[i][1]);
                         }
                }



               }
              }
        }

 }