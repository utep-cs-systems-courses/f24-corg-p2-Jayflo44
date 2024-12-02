	.arch msp430g2553
	.p2align 1,0
	.text
	.global ledAlter
	.extern P1OUT

ledAlter:
	cmp #0, r12
	jz off
	bis #64, &P1OUT 	;or
	and #~1, &P1OUT
	pop r0
off:	and #~64, &P1OUT
	bis #1, &P1OUT
	pop r0
