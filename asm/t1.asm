; a simple playfield
; source for DASM, 6502 instruction set, importing standard headers
    processor 6502
    include vcs.h

; start at $F000
    org $F000

Start ; standard prelimanaries...
    sei  ; disable interrupts (are there any?)
    cld  ; clear BCD mode.

; initialize stack
    ldx  #$FF
    txs

; reset RAM and TIA registers (X is set to $FF, we spare $00, which is VSYNC)
    lda #0
ClearMem 
    sta 0,X
    dex
    bne ClearMem

Frame
; start with a sequence of 3 lines of VSYNC
    lda  #2
    sta  VSYNC  ; write 2 to VSYNC to start VSYNC sequence
    sta  WSYNC  ; off until next line by strobing WSYNC register
    sta  WSYNC  ; second line
    sta  WSYNC  ; third line

; set up a timer to tick out after 37 lines @ 76 cycles = 2812
; we'll use a PIA timer ticking down every 64 cycles
; and there'll be some cycles used for setting it and some inaccuracy
; so we'll set it to a value of 43 (decimal)

    lda #43
    sta TIM64T

; and end vertical sync time
    lda #0
    sta VSYNC  ; write 0 to VSYNC

; business logic
; just set X to zero (used for a bit of effect)
    ldx #0
; not much to do, so wait for the timer to tick down

WaitForTimer
    lda INTIM
    bne WaitForTimer


; setup number of scan-lines for count down in Y
    ldy #191

; and end VBLANK by writing 0 to the VBLANK register at the next line
    sta WSYNC
    sta VBLANK  ; enable image (AC is still 0)

ScanLoop
    sta WSYNC   ; wait for end of previous line
    inx         ; fancy effect: increment X twice
    inx 
    stx COLUBK  ; and store it as the background-color
    dey         ; count down scan-lines...
    bne ScanLoop

OverScan
    ldx #0
    lda #2      ; prepare to set VBLANK
    sta WSYNC   ; wait for end of line
    stx COLUBK  ; reset background to black (AC is still 0)
    sta VBLANK  ; disable the video signal

; now bide some time (30 lines)
    ldy #30
OverScanWait
    sta WSYNC
    dey
    bne OverScanWait

    jmp Frame


; finally, set start and interrupt vectors

    org $FFFC
    .word Start
    .word Start
