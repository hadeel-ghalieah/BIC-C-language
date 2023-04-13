#define lcd_rs_pin pin_c0
#define lcd_rw_pin pin_c6
#define lcd_enable_pin pin_c1
#define lcd_data4 pin_c2
#define lcd_data5 pin_c3
#define lcd_data6 pin_c4
#define lcd_data7 pin_c5
#include <16f877a.h>
#device ADC=10
#use delay (clock=80000000)
#include<lcd.c>
#include<SHT75.c>
 int16 i;
 int dc=1, h=1;
 #int_ext 
void ext_isr(){

   clear_interrupt(INT_EXT);
   if(dc==0) output_low(pin_a4);
   else output_high(pin_a4);
   
}

int press(){
            output_high(pin_b3);
            output_high(pin_b2);
            output_low(pin_b1);
            
      if(!input_state(pin_b4))  return 1;
      if(!input_state(pin_b5)) return 4;
      if(!input_state(pin_b6))  return 7;
      if(!input_state(pin_b7))  return 77;
  
            output_high(pin_b3);
            output_low(pin_b2);
            output_high(pin_b1);
      if(!input_state(pin_b4))  return 2;
      if(!input_state(pin_b5))  return 5;
      if(!input_state(pin_b6))  return 8;
      if(!input_state(pin_b7))  return 0;
  
            output_low(pin_b3);
            output_high(pin_b2);
            output_high(pin_b1);
      if(!input_state(pin_b4))  return 3;
      if(!input_state(pin_b5))  return 6;
      if(!input_state(pin_b6))  return 9;
      if(!input_state(pin_b7))  return 99;
   
   }


void adcr(){         i=read_adc();
                     lcd_putc('\f');
                     lcd_gotoxy(1,1);
                
                     i=i/2; 
                     if(i>100) {i-=3;printf(lcd_putc, "Room1:%3lu %cC", i,223);}
                     else if(i>70){i-=2;printf(lcd_putc, "Room1:%2lu %cC", i,223);}
                     else if(i>25){ i-=1;printf(lcd_putc, "Room1:%2lu %cC", i,223);}
                     else printf(lcd_putc, "Room1:%2lu %cC", i,223);
                     
                     if(i<25){lcd_gotoxy(1,2);
                              lcd_putc("it's cold");
                              output_high(pin_d0);
                              output_low(pin_c7);
                              output_low(pin_c6);
                              output_low(pin_d1);  dc=1;};
                     if(i>=25 && i<=40){lcd_gotoxy(1,2);
                                        lcd_putc("it's normal");
                                        output_high(pin_c6);
                                        output_low(pin_c7);
                                        output_low(pin_d0);
                                         output_low(pin_d1);   dc=1;};
                     if(i>40){lcd_gotoxy(1,2);
                              lcd_putc("it's hot");
                              output_high(pin_c7);
                              output_low(pin_c6);
                              output_low(pin_d0);
                               output_low(pin_d1);   dc=0;};
                     }

void main(){
      float r,t;
     float op;
      int x;
  ext_int_edge(H_TO_L);
  clear_interrupt(INT_EXT); 
  enable_interrupts(INT_EXT);  
  enable_interrupts(GLOBAL);
 
      set_tris_b(0xF1);
      set_tris_a(0x0f);
      set_tris_e(0x04);  
      output_a(0xf0);
      output_e(0x00);
      set_tris_c(0x00);
      set_tris_d(0x00);
      output_d(0x00);
      port_b_pullups(true);
      lcd_init();
      sht_init();
  
  setup_adc(ADC_CLOCK_DIV_32);
  setup_adc_ports(AN0);
  
  lcd_gotoxy(1,1);
  lcd_putc("Room1: ");

     lcd_gotoxy(10,1);
  lcd_putc("Room2: ");

  lcd_gotoxy(1,2);
  lcd_putc("Room3: ");

  lcd_gotoxy(10,2);
  lcd_putc("Room4: ");
while(TRUE)
       {
       
      
       
           x= press();
           if(x==1){ set_adc_channel(0);
                     adcr();
                     output_high(pin_d2); 
                     output_low(pin_d3); 
                     output_low(pin_d4); 
                     output_low(pin_d5);
                    }
           
            if(x==2){
                     set_adc_channel(1);
                     adcr();
                     
                     output_low(pin_d2); 
                     output_high(pin_d3); 
                     output_low(pin_d4); 
                     output_low(pin_d5);}
            
             if(x==3){set_adc_channel(2);
                     
                     adcr();
                     output_high(pin_d2); 
                     output_high(pin_d3); 
                     output_low(pin_d4); 
                     output_low(pin_d5);}
             
              if(x==4){set_adc_channel(3);
                     adcr();
                     output_low(pin_d2); 
                     output_low(pin_d3); 
                     output_high(pin_d4); 
                     output_low(pin_d5);}
              
               if(x==5){}
               
                if(x==6){
                         sht_rd(r,t);
                         lcd_putc('\f');
                         lcd_gotoxy(1,1);
                         if(t<40) h=0;
                         // printf(lcd_putc, "Temp : %3.1f %cC   ", r, 223);
                          printf(lcd_putc, "RH   : %3.1f %%   ", t);
                       if(t<40){lcd_gotoxy(1,2);
                              lcd_putc("it's low");
                              output_high(pin_d1);
                              output_low(pin_d0);
                              output_low(pin_c7);
                              output_low(pin_c6);}                             
                              delay_ms(500); }
                
                  if(x==7){ set_adc_channel(7);
                            op=read_adc();
                           op=op*17.5/10;
                           lcd_putc('\f');
                            lcd_gotoxy(1,1);
                           if (op<60){op-=5;lcd_putc("Dark");}
                           else lcd_putc("Sunny");
                          
                            lcd_gotoxy(1,2);
                            printf(lcd_putc, "opacity :%3.1f  %%",op);}
                 
                  if(x==8){ }
                  if(x==9){}
                  if(x==77){  if(h==0)output_high(pin_e0);
                                      else output_low(pin_e0);}
                  if(x==0){}
                  if(x==99){    }
                      
  }
}



