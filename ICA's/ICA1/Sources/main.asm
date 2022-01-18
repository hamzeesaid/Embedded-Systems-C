;************************************************************************
;* HC12 Program:	ICA1           							              *
;* Processor:	MC9S12XDP512											                        *
;* Xtal Speed:	16 MHz													                        *
;* Author:		Hamza Said          											              *
;* Date:		16 sep 2021          									               	*
;*																	                    	              *
;* Details: ASSMEBLY LANGUAGE AND LED'S                     	              *
;*																		                                  *
;************************************************************************

;export symbols
		XDEF 		Entry			   ;export'Entry' symbol
		ABSENTRY 	Entry			;for absolute assembly: app entry point

;include derivative specific macros
		INCLUDE 'derivative.inc'

;********************************************************************
;*		Equates														*
;********************************************************************


;********************************************************************
;*		Variables													*
;********************************************************************
		ORG			RAMStart		;Address $2000


;********************************************************************
;*		Code Section												*
;********************************************************************
		ORG			ROM_4000Start	;Address $4000 (FLASH)
Entry:
		LDS			#RAMEnd+1		;initialize the stack pointer

Main:
      ;Set up
      CLR PT1AD1                    ; Clear Port
      MOVB #%11100000 , DDR1AD1     ; Setup input ELDs/ Output Switches
      MOVB #%00011111, ATD1DIEN1    ; Enable switch inputs
      BCLR PT1AD1, %11100000        ; All LEDs OFF
      
           
Loop:  


;

CheckRed: 
      ;CHECK LEFT PRESS
      BRSET PT1AD1, %00001000, RedOn     
      BRCLR PT1AD1, %00001000, RedOff     

CheckYel: 
      ;CHECK MID PRESS
      BRSET PT1AD1, %00000001, YellowOn   
      BRCLR PT1AD1, %00000001, YellowOff  

CheckGreen: 
      ;CHECK RIGHT PRESS          
      BRSET PT1AD1, %00000010, GreenOn    
      BRCLR PT1AD1, %00000010, GreenOff  
      
CheckAll: 

      ;CHECK UP PRESS 
      BRSET PT1AD1, %00010000, AllOn           
      BRA Loop  
  
  

 RedOn: 
        BSET PT1AD1, %10000000            ;turn RED ON (PAD15)         
        BRA CheckYel 
         
 RedOff:
        BCLR PT1AD1, %10000000            ;turn RED OFF (PAD15)         
        BRA CheckYel         
 
     
 YellowOn:
       BSET PT1AD1, %01000000             ;turn YELLOW ON (PAD14)  
       BRA CheckGreen

 YellowOff: 
       BCLR PT1AD1, %01000000             ;turn YELLOW OFF (PAD14)  
       BRA CheckGreen      
 
        
 GreenOn:
       BSET PT1AD1, %00100000             ;turn GREEN ON (PAD13)  
       BRA CheckAll 
       
 GreenOff:
       BCLR PT1AD1, %00100000             ;turn GREEN OFF (PAD13)  
       BRA CheckAll 
 
 ;    
 AllOn:  ; don't leave until button is released
      BSET PT1AD1,  %11100000            ;turn ON All LEDs                       
      BRCLR PT1AD1, %00010000, AllOff    ;Check for UP switch RELEASE -> S3 (PAD12)
      BRA AllOn
       
 AllOff:
      BCLR PT1AD1,  %11100000            ;turn OFF 3 LEDs                
      BRA Loop             

;********************************************************************
;*		Subroutines													*
;********************************************************************


;********************************************************************
;*		Interrupt Service Routines											*
;********************************************************************


;********************************************************************
;*		Constants													*
;********************************************************************
		ORG			ROM_C000Start	;second block of ROM


;********************************************************************
;*		Look-Up Tables												*
;********************************************************************


;********************************************************************
;*		SCI VT100 Strings											*
;********************************************************************


;********************************************************************
;*		Absolute Library Includes									*
;********************************************************************

		;INCLUDE "Your_Lib.inc"


;********************************************************************
;*		Interrupt Vectors											*
;********************************************************************

		ORG			$FFFE
		DC.W		Entry			;Reset Vector
