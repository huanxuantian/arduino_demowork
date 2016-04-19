# arduino_demowork
arduino_demowork with arduino 1.6.8 leonardo&mega2560 base board


board ref:

	leonardo ver3b
---
	mega2560 
environment:

	arduino1.x(1.6.8 in case)
extend library:

	LiquidCrystal_I2C
		LCD model buildin or thrid party
---
	MsTimer2
		using avr timer2(16bit) to hanlde task(Interrupt)
---
	TimerOne
		using avr timer1(32bit) to handle task(Interrupt)
---
	U8glib
		using for oled displayer 
---
	PJON(not use in project)
		onewrite system library
---
	ajson(not include now)
		json parser on avr arch,for highly string data in http 
shared library:
	Ethernet(build in)
		handle standed ethernet sheild broad with spi interface
  
sheild board:
  
	w5100 ethernet sheild(pin ref to /Arduino_Leonardo-REV3b/
	arduino_uno_ethernet_pins (1).png)
	spi(soft/hard) ref:
		SCK=D13
		MISO=D12
		MOSI=D11
		SS=D10(ethernet)/SS=D4(spicardreader)
---
	DFROBOT LCD Keypad Shield(pin ref to /manual/
	[www.manuallib.com]_DFROBOT LCD Keypad Shield Users Manual 用户手册.pdf )
	LCD1602 ref(4 line mode):
		LCD_D4=D4
		LCD_D5=D5
		LCD_D6=D6
		LCD_D7=D7
		LCD_RS=D8
		LCD_EN=D9
		LCD_BL=D10
	ADC key ref(5 key mode):
		ADC=A0（ref limit(0-1024):{50,200,400,600,800}）	
---  
	oled model board(self build pin ref to /manual/0.96寸OLED使用手册V1.1/)
	with stick input（x，y）massage and button 0;
	board designed will ref to pcb and sch in next time
	softspi ref:
		SCK=D3
		MOSI=D5
		CS=D7
		A0=D6
	stick ref:
		x:[0v,A0,5v]
		y:[0v,A1,5v]
		center:D2
	other ref:
		DS18B20=D8
		IRemote=D9
	
	
  
project in case:
  
	LCDTIME-NET
	library ref:
		LiquidCrystal_I2C
		MsTimer2
		TimerOne
		Ethernet(build in)
---
	LCDTIME-WEB
	library ref:
		LiquidCrystal_I2C
		MsTimer2
		TimerOne
		Ethernet(build in)
---
	oled-time
	library ref:
		U8glib
		MsTimer2
		TimerOne
		Ethernet(build in)
---
	oled-time-web
	library ref:
		U8glib
		MsTimer2
		TimerOne
		Ethernet(build in)
---
	snake_arduino
	library ref:
		U8glib
		MsTimer2
		Ethernet(build in)
	