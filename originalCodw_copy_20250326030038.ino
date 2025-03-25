volatile unsigned long duration = 0;
volatile unsigned long start = 0;
volatile unsigned long end = 0;
volatile unsigned long j = 0;

int delayFuncMS(long setTime) {
    volatile int i = 0;
    for (i = 0; i < setTime; i++);
}

void USART_Init() {
    UBRR0H = 0;
    UBRR0L = 103;  
    UCSR0B = (1 << TXEN0); 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  
}

void USART_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void USART_Print(const char* str) {
    while (*str) {
        USART_Transmit(*str++);
    }
}

void USART_PrintDouble(double value) {
    char buffer[10];
    dtostrf(value, 6, 2, buffer); 
    USART_Print(buffer);
}

void triggerUltrasonic() {
        PORTB &= ~0x02; 
        delayFuncMS(80);  
        PORTB |= 0x02; 
        delayFuncMS(320);  
        PORTB &= ~0x02; 
}

long measureEchoTime() {
    j=0;
    do {
        j++;
    } while (!(PINB & (1 << PB2))); 

    start = j;

    do {
        j++; 
    } while (PINB & (1 << PB2)); 

    end = j; 

    duration = end - start;
    j=0;

    return duration;
    
}

int main() {
    USART_Init();

    DDRB |= 0x02;  
    DDRB &= ~0x04; 

    while (1) {

        triggerUltrasonic(); 
        unsigned long duration = measureEchoTime(); 
        double distance = (duration * 0.0343) / 2;  

          USART_Print("Distance: ");
          USART_PrintDouble(distance);
          USART_Print(" cm\n");
          USART_Print("Duration: ");
          USART_PrintDouble(duration);
          USART_Print(" us\n");
        if(distance>5){
          USART_Print("HIGH\n");
        }else if(distance<5 && distance >3){
          USART_Print("MEDIUM\n");
        }else{
          USART_Print("LOW\n");
        }

        delayFuncMS(4000); 
}
return 1;
}
