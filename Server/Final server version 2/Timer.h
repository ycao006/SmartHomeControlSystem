
void Timer_init(){
//--------Timer init----------------------------------------
    noInterrupts();           // disable all interrupts
    TCCR5A = 0;
    TCCR5B = 0;
  
    TCNT5 = 0;            // preload timer 65536-16MHz/256/2Hz
    TCCR5B |= (1 << CS12);    // 256 prescaler 
    TIMSK5 |= (1 << TOIE5);   // enable timer overflow interrupt
    interrupts();             // enable all interrupts
//----------------------------------------------------------- 
}

void Watchdog_init(){
//--------watchdog init-----------------------------------
    wdt_disable();
    // I often do serial i/o at startup to allow the user to make config changes of
    // various constants. This is often using fgets which will wait for user input.
    // any such 'slow' activity needs to be completed before enabling the watchdog timer.

    // the following forces a pause before enabling WDT. This gives the IDE a chance to
    // call the bootloader in case something dumb happens during development and the WDT
    // resets the MCU too quickly. Once the code is solid, remove this.
    delay(2000);
    // enable the watchdog timer. There are a finite number of timeouts allowed (see wdt.h).
    // Notes I have seen say it is unwise to go below 250ms as you may get the WDT stuck in a
    // loop rebooting.
    // The timeouts I'm most likely to use are:
    // WDTO_1S
    // WDTO_2S
    // WDTO_4S
    // WDTO_8S
    wdt_enable(WDTO_8S);
}

