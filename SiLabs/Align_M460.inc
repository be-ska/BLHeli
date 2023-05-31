;**** **** **** **** ****
;
; BLHeli program for controlling brushless motors in helicopters and multirotors
;
; Copyright 2011, 2012 Steffen Skaug
; This program is distributed under the terms of the GNU General Public License
;
; This file is part of BLHeli.
;
; BLHeli is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; BLHeli is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with BLHeli.  If not, see <http://www.gnu.org/licenses/>.
;
;**** **** **** **** ****
;
; Align M460 hardware definition file
;
;**** **** **** **** ****



;*********************
; Device SiLabs F330
;*********************
$include (c8051f330.inc)

;**** **** **** **** ****
; Uses internal calibrated oscillator set to 24Mhz
;**** **** **** **** ****

;**** **** **** **** ****
; Constant definitions
;**** **** **** **** ****
CSEG AT 1A40h
Eep_ESC_Layout:		DB	"#ALIGN_M460    #"	; ESC layout tag
CSEG AT 1A50h
Eep_ESC_MCU:			DB	"#BLHELI#F330#   "	; Project and MCU tag (16 Bytes)

MCU_48MHZ				EQU	0	; Set to 1 if MCU can run at 48MHz
ONE_S_CAPABLE			EQU	0	; Set to 1 if ESC can operate at 1S
PORT3_EXIST			EQU	0	; Set to 1 if MCU has port3
COMP1_USED			EQU	0	; Set to 1 if MCU has comparator 1 and it is being used
LOCK_BYTE_ADDRESS_16K	EQU	3FFFh	; Address of lock byte if 16k flash size
LOCK_BYTE_ADDRESS_8K	EQU	1DFFh	; Address of lock byte if 8k flash size
HIGH_BEC_VOLTAGE		EQU	0	; Set to 1 or more if high BEC voltage is supported
DAMPED_MODE_ENABLE		EQU	0	; Damped mode disabled
NFETON_DELAY			EQU	15	; Wait delay from pfets off to nfets on
PFETON_DELAY			EQU	15	; Wait delay from nfets off to pfets on
ADC_LIMIT_L			EQU 	85	; Power supply measurement ADC value for which main motor power is limited (low byte)
ADC_LIMIT_H			EQU	0	; Power supply measurement ADC value for which main motor power is limited (2 MSBs)
TEMP_LIMIT			EQU 	109	; Temperature measurement ADC value for which main motor power is limited (low byte, assuming high byte is 1)
TEMP_LIMIT_STEP		EQU	4	; Temperature measurement ADC value increment for which main motor power is further limited
DIP_SWITCH			EQU 1	; DIP switch connected to low side FETs

;**** **** **** **** ****
; ESC specific defaults
;**** **** **** **** ****
DEFAULT_PGM_MAIN_SPOOLUP_TIME		EQU 11	; Main motor spoolup time
DEFAULT_PGM_MAIN_STARTUP_PWR 		EQU 11 	; 1=0.031 2=0.047 3=0.063 4=0.094 5=0.125 6=0.188	7=0.25  8=0.38  9=0.50  10=0.75 11=1.00 12=1.25 13=1.50
DEFAULT_PGM_TAIL_STARTUP_PWR 		EQU 11 	; 1=0.031 2=0.047 3=0.063 4=0.094 5=0.125 6=0.188	7=0.25  8=0.38  9=0.50  10=0.75 11=1.00 12=1.25 13=1.50
DEFAULT_PGM_MULTI_STARTUP_PWR 		EQU 11 	; 1=0.031 2=0.047 3=0.063 4=0.094 5=0.125 6=0.188	7=0.25  8=0.38  9=0.50  10=0.75 11=1.00 12=1.25 13=1.50

;**** **** **** **** ****
; Bootloader definitions
;**** **** **** **** ****
RTX_PORT				EQU 	P0		; Receive/Transmit port
RTX_MDOUT				EQU	P0MDOUT	; Set to 1 for PUSHPULL	
RTX_MDIN				EQU	P0MDIN	; Set to 1 for DIGITAL	
RTX_SKIP				EQU	P0SKIP	; Set to 1 for SKIP		
RTX_PIN					EQU 	3		; RTX pin

SIGNATURE_001			EQU	0f3h		; Device signature
SIGNATURE_002			EQU	030h

;*********************
; PORT 0 definitions *
;*********************  
BnFET		EQU	7	;o low side B - DIP
BpFET		EQU	6	;o high side B
AnFET		EQU	5   ;o low side A - DIP
ApFET 		EQU	4	;o high side A
Rcp_In		EQU	3	;i
LED			EQU 2	;i
LED_w		EQU 1	;o
Vref		EQU	0	;o	

P0_DIGITAL	EQU	NOT((1 SHL BnFET)+(1 SHL AnFET)+(1 SHL Vref)) AND 0FFh
P0_INIT		EQU	NOT((1 SHL BpFET)+(1 SHL ApFET)+(1 SHL LED_w))	AND 0FFh
P0_PUSHPULL	EQU	((1 SHL BpFET)+(1 SHL ApFET)+(1 SHL LED_w)) AND 0FFh
P0_SKIP		EQU	NOT(1 SHL Rcp_In) AND 0FFh ;P0SKIP

;init to see DIP switch
P0_DIGITAL1		EQU 0FFh
P0_INIT1		EQU	NOT((1 SHL BnFET)+(1 SHL BpFET)+(1 SHL AnFET)+(1 SHL ApFET)+(1 SHL LED_w))	AND 0FFh ;P0
P0_PUSHPULL1	EQU	((1 SHL BnFET)+(1 SHL BpFET)+(1 SHL AnFET)+(1 SHL ApFET)+(1 SHL LED_w)) AND 0FFh ;P0MDOUT



MACRO Get_Rcp_Capture_Values
	mov	Temp1, PCA0CPL0			; Get PCA capture values
	mov	Temp2, PCA0CPH0
ENDM
MACRO Read_Rcp_Int
	mov	A, P0
	jnb	Flags3.PGM_RCP_PWM_POL, ($+4)	; Is pwm polarity negative?
	cpl	A						; Yes - invert
ENDM
MACRO Rcp_Int_Enable
	orl	PCA0CPM0, #01h				; Interrupt enabled
ENDM
MACRO Rcp_Int_Disable
	anl	PCA0CPM0, #0FEh			; Interrupt disabled
ENDM
MACRO Rcp_Int_First
	anl	PCA0CPM0, #0CFh			
	jb	Flags3.PGM_RCP_PWM_POL, ($+6)	; Is pwm polarity positive?
	orl	PCA0CPM0, #20h				; Capture rising edge
	jnb	Flags3.PGM_RCP_PWM_POL, ($+6)	; Is pwm polarity negative?
	orl	PCA0CPM0, #10h				; Capture falling edge
ENDM
MACRO Rcp_Int_Second
	anl	PCA0CPM0, #0CFh			
	jb	Flags3.PGM_RCP_PWM_POL, ($+6)	; Is pwm polarity positive?
	orl	PCA0CPM0, #10h				; Capture falling edge
	jnb	Flags3.PGM_RCP_PWM_POL, ($+6)	; Is pwm polarity negative?
	orl	PCA0CPM0, #20h				; Capture rising edge
ENDM
MACRO Rcp_Clear_Int_Flag
	clr 	CCF0						; Clear interrupt flag
ENDM		


;*********************
; PORT 1 definitions *
;*********************
LED_r		EQU	7	;o
CpFET		EQU	6	;i
CnFET		EQU	5	;o
Mux_C		EQU	4	;i
;			EQU 3	;i
Mux_B		EQU	2	;i
Comp_Com	EQU	1	;i
Mux_A		EQU	0	;i

P1_DIGITAL	EQU	NOT((1 SHL Mux_A)+(1 SHL Mux_B)+(1 SHL Comp_Com)+(1 SHL Mux_C)+(1 SHL CnFET)) AND 0FFh
P1_INIT		EQU NOT((1 SHL CpFET)+(1 SHL LED_r)) AND 0FFh	
P1_PUSHPULL	EQU	((1 SHL CpFET)+(1 SHL LED_r)) AND 0FFh	
P1_SKIP		EQU	0

P1_DIGITAL1		EQU	NOT((1 SHL Mux_A)+(1 SHL Mux_B)+(1 SHL Comp_Com)+(1 SHL Mux_C)) AND 0FFh
P1_INIT1		EQU NOT((1 SHL CpFET)+(1 SHL CnFET)+(1 SHL LED_r)) AND 0FFh	
P1_PUSHPULL1	EQU	((1 SHL CpFET)+(1 SHL CnFET)+(1 SHL LED_r)) AND 0FFh	

MACRO AnFET_on
	setb	P0.AnFET
ENDM
MACRO AnFET_off
	clr	P0.AnFET
ENDM
MACRO BnFET_on
	setb	P0.BnFET
ENDM
MACRO BnFET_off
	clr	P0.BnFET
ENDM
MACRO CnFET_on
	setb	P1.CnFET
ENDM
MACRO CnFET_off
	clr	P1.CnFET
ENDM
MACRO All_nFETs_Off
	clr	P0.AnFET
	clr	P0.BnFET
	clr	P1.CnFET
ENDM

MACRO ApFET_on
	setb	P0.ApFET
ENDM
MACRO ApFET_off
	clr	P0.ApFET
ENDM
MACRO BpFET_on
	setb	P0.BpFET
ENDM
MACRO BpFET_off
	clr	P0.BpFET
ENDM
MACRO CpFET_on
	setb	P1.CpFET
ENDM
MACRO CpFET_off
	clr	P1.CpFET
ENDM
MACRO All_pFETs_Off
	clr	P0.ApFET
	clr	P0.BpFET
	clr	P1.CpFET
ENDM

MACRO Brake_FETs_On	
	AnFET_on
	BnFET_on
	CnFET_on
ENDM
MACRO Damping_FET_On
	mov	A, DampingFET
	cjne	A, #(1 SHL CpFET), damp_on_ab
	orl	P1, A
	ajmp	damp_on_exit
damp_on_ab:
	orl	P0, A
damp_on_exit:
ENDM

MACRO Set_Comp_Phase_A
	mov	CPT0MX, #44h	; Set comparator multiplexer to phase A +P1.1-P1.0
ENDM
MACRO Set_Comp_Phase_B
	mov	CPT0MX, #45h	; Set comparator multiplexer to phase B +P1.1-P1.2
ENDM
MACRO Set_Comp_Phase_C
	mov	CPT0MX, #46h	; Set comparator multiplexer to phase C +P1.1-P1.4
ENDM
MACRO Read_Comp_Out
	mov	A, CPT0CN		; Read comparator output
ENDM


;*********************
; PORT 2 definitions *
;*********************
DebugPin		EQU	0	;o

P2_PUSHPULL	EQU	(1 SHL DebugPin)


;**********************
; MCU specific macros *
;**********************
MACRO Interrupt_Table_Definition
CSEG AT 0				; Code segment start
	jmp reset
CSEG AT 0Bh			; Timer0 interrupt	
	jmp	t0_int	
CSEG AT 2Bh			; Timer2 interrupt	
	jmp	t2_int	
CSEG AT 5Bh			; PCA interrupt	
	jmp	pca_int	
CSEG AT 73h			; Timer3 interrupt	
	jmp	t3_int	
ENDM

MACRO Initialize_Xbar	
	mov	XBR1, #41h	; Xbar enabled, CEX0 routed to pin Rcp_In		
ENDM

MACRO Initialize_Adc
	mov	REF0CN, #07h	; Set vdd (3.3V) as reference. Enable temp sensor and bias
	mov	ADC0CF, #58h	; ADC clock 2MHz
	mov	AMX0P, #(8+Mux_B)	; Select positive input (use comparator input B)
	mov	AMX0N, #11h	; Select negative input as ground
	mov	ADC0CN, #80h	; ADC enabled
ENDM

MACRO Set_DIP_A
	mov	AMX0P, #05h	; Set comparator multiplexer to phase A +P1.1-P1.0
ENDM
MACRO Set_DIP_B
	mov	AMX0P, #07h	; Set comparator multiplexer to phase B +P1.1-P1.2
ENDM
MACRO Set_DIP_C
	mov	AMX0P, #0Dh	; Set comparator multiplexer to phase C +P1.1-P1.4
ENDM

MACRO Set_Adc_Ip_Volt
	mov	AMX0P, #(8+Mux_B)	; Select positive input (use comparator input B)
ENDM
MACRO Set_Adc_Ip_Temp
	mov	AMX0P, #10h	; Select temp sensor input
ENDM
MACRO Start_Adc
	mov	ADC0CN, #90h	; ADC start
ENDM
MACRO Read_Adc_Result
	mov	Temp1, ADC0L
	mov	Temp2, ADC0H
ENDM
MACRO Stop_Adc
ENDM
MACRO Set_RPM_Out
ENDM
MACRO Clear_RPM_Out
ENDM
MACRO Set_MCU_Clk_24MHz
ENDM
MACRO Set_MCU_Clk_48MHz
ENDM
; LED control, only LED_0 and LED_1
Set_LED_0 MACRO
	setb P0.LED_w
ENDM
Clear_LED_0 MACRO
	clr	P0.LED_w
ENDM
Set_LED_1 MACRO
	setb P1.LED_r
ENDM
Clear_LED_1 MACRO
	clr	P1.LED_r
ENDM
Set_LED_2 MACRO
ENDM
Clear_LED_2 MACRO
ENDM
Set_LED_3 MACRO
ENDM
Clear_LED_3 MACRO
ENDM