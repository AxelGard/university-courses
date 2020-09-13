;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 
;; Mall fÃ¶r lab1 i TSEA28 Datorteknik Y
;;
;; 190123 K.Palmkvist
;;

	;; Ange att koden Ã¤r fÃ¶r thumb mode
	.thumb
	.text
	.align 2




	;; Ange att labbkoden startar hÃ¤r efter initiering
	.global	main

	;; Strings
	;namn3  .string ”hej”,10,13

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 	Placera programmet hÃ¤r

main:				; Start av programmet
	bl inituart
	bl initGPIOB
	bl initGPIOF

	bl setCode
activateLock:
	bl activatealarm
clearBuffer:
	bl clearinput
getinput:
	bl getkey
	mov r3, #0x9
	cmp r3, r4; if r4 < 10 (0xA)
	bpl addkeytobuffer

	mov r3, #0xE; if r4 < 15 (0xF)
	cmp r3, r4
	bpl clearBuffer

	bl checkcode ; F key pressed
	cmp r4, #0x1 ; if correct code
	beq correctCode
	mov r5, #0xF
	bl printstring ; Wrong code
	b clearBuffer
correctCode:
	bl deactivatealarm
getInputAgain:
	bl getkey
	cmp r4, #0xA
	beq aPressed
	b getInputAgain
aPressed:
	b activateLock
addkeytobuffer:
	bl addkey
	b getinput


endloop: b endloop

	;bl setCode


	;b	main		; Denna rad bÃ¶r tas bort (bara ett exempel)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; SUBROUTINES
;

FELKOD .string  "Felaktig kod",10,13

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Inargument: Pekare till str ̈angen i r4
;             L ̈angd p ̊a str ̈angen i r5
printstring:
	; F ̈orberedelseuppgift: Skriv denna subrutin!
	PUSH { LR }
	mov r6, #0x0
	adr r7, FELKOD
stringLoop:
	ldrb r0, [r7]
	bl printchar
	add r7,r7,#0x1
	add r6,r6,#0x1
	cmp r6, r5
	bne stringLoop
	POP { LR }
	bx  lr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Inargument: Inga
; Utargument: Inga
;
; Funktion: Sätter en hårdkodad kod

setCode:
	mov r0,#(0x20001013 & 0xffff)
	movt r0,#(0x20001013 >> 16)
	mov r1,#0x01
	strb r1,[r0]

	mov r0,#(0x20001012 & 0xffff)
	movt r0,#(0x20001012 >> 16)
	mov r1,#0x02
	strb r1,[r0]

	mov r0,#(0x20001011 & 0xffff)
	movt r0,#(0x20001011 >> 16)
	mov r1,#0x03
	strb r1,[r0]

	mov r0,#(0x20001010 & 0xffff)
	movt r0,#(0x20001010 >> 16)
	mov r1,#0x04
	strb r1,[r0]

	bx lr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Inargument: Inga
; Utargument: Inga
;
; Funktion: Tänder grön lysdiod (bit 3 = 1, bit 2 = 0, bit 1 = 0)
deactivatealarm:
	; Förberedelseuppgift: Skriv denna subrutin!
	mov r0, #0x8
	mov r1,#(GPIOF_GPIODATA & 0xffff)
	movt r1,#(GPIOF_GPIODATA >> 16)
	strb r0, [r1]
	bx  lr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Inargument: Inga
; Utargument: Inga
;
; Funktion: T ̈ander r ̈od lysdiod (bit 3 = 0, bit 2 = 0, bit 1 = 1)
activatealarm:
	; F ̈orberedelseuppgift: Skriv denna subrutin!
	mov r0, #0x2
	mov r1,#(GPIOF_GPIODATA & 0xffff)
	movt r1,#(GPIOF_GPIODATA >> 16)
	strb r0, [r1]
	bx  lr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Inargument: Inga
; Utargument: Tryckt knappt returneras i r4
getkey:
	; F ̈orberedelseuppgift: Skriv denna subrutin!
	mov r1,#(GPIOB_GPIODATA & 0xffff)
	movt r1,#(GPIOB_GPIODATA >> 16)
	mov r7, #(0x009AA200 & 0xffff) ; 0x009AA200 is about the number of
	movt r7, #(0x009AA200 >> 16) ; times it loops in 5 sec
	mov r6, #0x0
waitForStrobe1:
	add r6, r6, #0x1
	cmp r6, r7
	beq fakeAPressed

	ldrb r0,[r1]
	ands r2,r0,#0x10 ; Check if bit 4 = 1
	beq waitForStrobe1 ; if not loop
	mov r4, r0
	and r4,r4,#0xF ; Keep the last 4 bits (bit 0-3)
waitForStrobe0:
	ldrb r0,[r1]
	ands r2,r0,#0x10 ; Check if bit 4 = 1
	bne waitForStrobe0 ; if: loop
	bx  lr
fakeAPressed:
	mov r4, #0xA
	bx lr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Inargument: Vald tangent i r4
; Utargument: Inga
;
; Funktion: Flyttar inneh ̊allet p ̊a 0x20001000-0x20001002 fram ̊at en byte
; till 0x20001001-0x20001003. Lagrar sedan inneh ̊allet i r4 p ̊a
; adress 0x20001000.
addkey:
	; F ̈orberedelseuppgift: Skriv denna subrutin!
	mov r0,#(0x20001000 & 0xffff)
	movt r0,#(0x20001000 >> 16)

	mov r1,#(0x20001001 & 0xffff)
	movt r1,#(0x20001001 >> 16)

	mov r2,#(0x20001002 & 0xffff)
	movt r2,#(0x20001002 >> 16)

	mov r3,#(0x20001003 & 0xffff)
	movt r3,#(0x20001003 >> 16)

	ldrb r5,[r2] ; Move all values to next position in memory
	strb r5,[r3]
	ldrb r5,[r1]
	strb r5,[r2]
	ldrb r5,[r0]
	strb r5,[r1]

	strb r4,[r0] ; store key in r4 at 0x20001000

	bx  lr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Inargument: Inga
; Utargument: Ing
;
; Funktion: S ̈atter inneh ̊allet p ̊a 0x20001000-0x20001003 till 0xFF
clearinput:
	; F ̈orberedelseuppgift: Skriv denna subrutin!
	mov r0,#(0x20001000 & 0xffff)
	movt r0,#(0x20001000 >> 16)
	mov r1,#0xFF
	strb r1,[r0]

	mov r0,#(0x20001001 & 0xffff)
	movt r0,#(0x20001001 >> 16)
	mov r1,#0xFF
	strb r1,[r0]

	mov r0,#(0x20001002 & 0xffff)
	movt r0,#(0x20001002 >> 16)
	mov r1,#0xFF
	strb r1,[r0]

	mov r0,#(0x20001003 & 0xffff)
	movt r0,#(0x20001003 >> 16)
	mov r1,#0xFF
	strb r1,[r0]

	bx  lr
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Inargument: Inga
; Utargument: Returnerar 1 i r4 om koden var korrekt, annars 0 i r4
checkcode:
	; F ̈orberedelseuppgift: Skriv denna subrutin!
	mov r0,#(0x20001000 & 0xffff)
	movt r0,#(0x20001000 >> 16)
	mov r1,#(0x20001010 & 0xffff)
	movt r1,#(0x20001010 >> 16)

	ldr r2, [r0]
	ldr r3, [r1]
	cmp r2, r3
	bne wrongCode

	mov r4, #0x1 ; Correct code
	bx lr

wrongCode:
	mov r4, #0x0
	bx  lr


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;,
;;;
;;; Allt hÃ¤r efter ska inte Ã¤ndras
;;;
;;; Rutiner fÃ¶r initiering
;;; Se labmanual fÃ¶r vilka namn som ska anvÃ¤ndas
;;;
	
	.align 4

;; 	Initiering av seriekommunikation
;;	FÃ¶rstÃ¶r r0, r1
	
inituart:
	mov r1,#(RCGCUART & 0xffff)		; Koppla in serieport
	movt r1,#(RCGCUART >> 16)
	mov r0,#0x01
	str r0,[r1]

	mov r1,#(RCGCGPIO & 0xffff)
	movt r1,#(RCGCGPIO >> 16)
	ldr r0,[r1]
	orr r0,r0,#0x01
	str r0,[r1]		; Koppla in GPIO port A

	nop			; vÃ¤nta lite
	nop
	nop

	mov r1,#(GPIOA_GPIOAFSEL & 0xffff)
	movt r1,#(GPIOA_GPIOAFSEL >> 16)
	mov r0,#0x03
	str r0,[r1]		; pinnar PA0 och PA1 som serieport

	mov r1,#(GPIOA_GPIODEN & 0xffff)
	movt r1,#(GPIOA_GPIODEN >> 16)
	mov r0,#0x03
	str r0,[r1]		; Digital I/O pÃ¥ PA0 och PA1

	mov r1,#(UART0_UARTIBRD & 0xffff)
	movt r1,#(UART0_UARTIBRD >> 16)
	mov r0,#0x08
	str r0,[r1]		; SÃ¤tt hastighet till 115200 baud
	mov r1,#(UART0_UARTFBRD & 0xffff)
	movt r1,#(UART0_UARTFBRD >> 16)
	mov r0,#44
	str r0,[r1]		; Andra vÃ¤rdet fÃ¶r att fÃ¥ 115200 baud

	mov r1,#(UART0_UARTLCRH & 0xffff)
	movt r1,#(UART0_UARTLCRH >> 16)
	mov r0,#0x60
	str r0,[r1]		; 8 bit, 1 stop bit, ingen paritet, ingen FIFO
	
	mov r1,#(UART0_UARTCTL & 0xffff)
	movt r1,#(UART0_UARTCTL >> 16)
	mov r0,#0x0301
	str r0,[r1]		; BÃ¶rja anvÃ¤nda serieport

	bx  lr

; Definitioner fÃ¶r registeradresser (32-bitars konstanter)
GPIOHBCTL	.equ	0x400FE06C
RCGCUART	.equ	0x400FE618
RCGCGPIO	.equ	0x400fe608
UART0_UARTIBRD	.equ	0x4000c024
UART0_UARTFBRD	.equ	0x4000c028
UART0_UARTLCRH	.equ	0x4000c02c
UART0_UARTCTL	.equ	0x4000c030
UART0_UARTFR	.equ	0x4000c018
UART0_UARTDR	.equ	0x4000c000
GPIOA_GPIOAFSEL	.equ	0x40004420
GPIOA_GPIODEN	.equ	0x4000451c
GPIOB_GPIODATA	.equ	0x400053fc
GPIOB_GPIODIR	.equ	0x40005400
GPIOB_GPIOAFSEL	.equ	0x40005420
GPIOB_GPIOPUR	.equ	0x40005510
GPIOB_GPIODEN	.equ	0x4000551c
GPIOB_GPIOAMSEL	.equ	0x40005528
GPIOB_GPIOPCTL	.equ	0x4000552c
GPIOF_GPIODATA	.equ	0x4002507c
GPIOF_GPIODIR	.equ	0x40025400
GPIOF_GPIOAFSEL	.equ	0x40025420
GPIOF_GPIODEN	.equ	0x4002551c
GPIOF_GPIOLOCK	.equ	0x40025520
GPIOKEY		.equ	0x4c4f434b
GPIOF_GPIOPUR	.equ	0x40025510
GPIOF_GPIOCR	.equ	0x40025524
GPIOF_GPIOAMSEL	.equ	0x40025528
GPIOF_GPIOPCTL	.equ	0x4002552c

;; Initiering av port F
;; FÃ¶rstÃ¶r r0, r1, r2
initGPIOF:
	mov r1,#(RCGCGPIO & 0xffff)
	movt r1,#(RCGCGPIO >> 16)
	ldr r0,[r1]
	orr r0,r0,#0x20		; Koppla in GPIO port F
	str r0,[r1]
	nop 			; VÃ¤nta lite
	nop
	nop

	mov r1,#(GPIOHBCTL & 0xffff)	; AnvÃ¤nd apb fÃ¶r GPIO
	movt r1,#(GPIOHBCTL >> 16)
	ldr r0,[r1]
	mvn r2,#0x2f		; bit 5-0 = 0, Ã¶vriga = 1
	and r0,r0,r2
	str r0,[r1]

	mov r1,#(GPIOF_GPIOLOCK & 0xffff)
	movt r1,#(GPIOF_GPIOLOCK >> 16)
	mov r0,#(GPIOKEY & 0xffff)
	movt r0,#(GPIOKEY >> 16)
	str r0,[r1]		; LÃ¥s upp port F konfigurationsregister

	mov r1,#(GPIOF_GPIOCR & 0xffff)
	movt r1,#(GPIOF_GPIOCR >> 16)
	mov r0,#0x1f		; tillÃ¥t konfigurering av alla bitar i porten
	str r0,[r1]

	mov r1,#(GPIOF_GPIOAMSEL & 0xffff)
	movt r1,#(GPIOF_GPIOAMSEL >> 16)
	mov r0,#0x00		; Koppla bort analog funktion
	str r0,[r1]

	mov r1,#(GPIOF_GPIOPCTL & 0xffff)
	movt r1,#(GPIOF_GPIOPCTL >> 16)
	mov r0,#0x00		; anvÃ¤nd port F som GPIO
	str r0,[r1]

	mov r1,#(GPIOF_GPIODIR & 0xffff)
	movt r1,#(GPIOF_GPIODIR >> 16)
	mov r0,#0x0e		; styr LED (3 bits), andra bitar Ã¤r ingÃ¥ngar
	str r0,[r1]

	mov r1,#(GPIOF_GPIOAFSEL & 0xffff)
	movt r1,#(GPIOF_GPIOAFSEL >> 16)
	mov r0,#0		; alla portens bitar Ã¤r GPIO
	str r0,[r1]

	mov r1,#(GPIOF_GPIOPUR & 0xffff)
	movt r1,#(GPIOF_GPIOPUR >> 16)
	mov r0,#0x11		; svag pull-up fÃ¶r tryckknapparna
	str r0,[r1]

	mov r1,#(GPIOF_GPIODEN & 0xffff)
	movt r1,#(GPIOF_GPIODEN >> 16)
	mov r0,#0xff		; alla pinnar som digital I/O
	str r0,[r1]

	bx lr


;; Initiering av port B
;; FÃ¶rstÃ¶r r0, r1
initGPIOB:
	mov r1,#(RCGCGPIO & 0xffff)
	movt r1,#(RCGCGPIO >> 16)
	ldr r0,[r1]
	orr r0,r0,#0x02		; koppla in GPIO port B
	str r0,[r1]
	nop			; vÃ¤nta lite
	nop
	nop

	mov r1,#(GPIOB_GPIODIR & 0xffff)
	movt r1,#(GPIOB_GPIODIR >> 16)
	mov r0,#0x0		; alla bitar Ã¤r ingÃ¥ngar
	str r0,[r1]

	mov r1,#(GPIOB_GPIOAFSEL & 0xffff)
	movt r1,#(GPIOB_GPIOAFSEL >> 16)
	mov r0,#0		; alla portens bitar Ã¤r GPIO
	str r0,[r1]

	mov r1,#(GPIOB_GPIOAMSEL & 0xffff)
	movt r1,#(GPIOB_GPIOAMSEL >> 16)
	mov r0,#0x00		; anvÃ¤nd inte analoga funktioner
	str r0,[r1]

	mov r1,#(GPIOB_GPIOPCTL & 0xffff)
	movt r1,#(GPIOB_GPIOPCTL >> 16)
	mov r0,#0x00		; anvÃ¤nd inga specialfunktioner pÃ¥ port B
	str r0,[r1]

	mov r1,#(GPIOB_GPIOPUR & 0xffff)
	movt r1,#(GPIOB_GPIOPUR >> 16)
	mov r0,#0x00		; ingen pullup pÃ¥ port B
	str r0,[r1]

	mov r1,#(GPIOB_GPIODEN & 0xffff)
	movt r1,#(GPIOB_GPIODEN >> 16)
	mov r0,#0xff		; alla pinnar Ã¤r digital I/O
	str r0,[r1]

	bx lr


;; Utskrift av ett tecken pÃ¥ serieport
;; r0 innehÃ¥ller tecken att skriva ut (1 byte)
;; returnerar fÃ¶rst nÃ¤r tecken skickats
;; fÃ¶rstÃ¶r r0, r1 och r2
printchar:
	mov r1,#(UART0_UARTFR & 0xffff)	; peka pÃ¥ serieportens statusregister
	movt r1,#(UART0_UARTFR >> 16)
loop1:
	ldr r2,[r1]			; hÃ¤mta statusflaggor
	ands r2,r2,#0x20		; kan ytterligare tecken skickas?
	bne loop1			; nej, fÃ¶rsÃ¶k igen
	mov r1,#(UART0_UARTDR & 0xffff)	; ja, peka pÃ¥ serieportens dataregister
	movt r1,#(UART0_UARTDR >> 16)
	str r0,[r1]			; skicka tecken
	bx lr




