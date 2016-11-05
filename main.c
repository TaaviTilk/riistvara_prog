#include	<avr/io.h>
#include	<util/delay.h>
#define	BLINK_DELAY_MS	1000
int	main	(void)

{
    /*	set	pin	5	of	PORTB	for	output*/
    DDRA	|=	_BV(DDA3);
    while(1)	{
        /*	set	pin	5	high	to	turn	led	on	*/
        PORTA	|=	_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    
	    /*	set	pin	5	low	to	turn	led	off	*/
        PORTA	&=	~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}


