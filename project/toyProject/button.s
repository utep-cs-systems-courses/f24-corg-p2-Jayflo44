	.arch msp430g2553
	.p2align 1,0
	.text


	.global GS4down
	.global Gs4up
	.extern P1OUT

GS4down:
	bis #1, &P1OUT 		;green led on
	bis #64, &P1OUT		;red led on
	pop r0
GS4up:
	and #~64, &P1OUT		;turn red led off
	pop r0
