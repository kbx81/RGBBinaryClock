EESchema Schematic File Version 4
LIBS:kbxBinaryClock.v4-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L kbxBinaryClock.v4-rescue:MAX485E U6
U 1 1 5A0ADC38
P 6500 4600
AR Path="/5A0ADC38" Ref="U6"  Part="1" 
AR Path="/5A0ADB1D/5A0ADC38" Ref="U6"  Part="1" 
F 0 "U6" H 6260 5050 50  0000 C CNN
F 1 "MAX485E" H 6530 5050 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 6500 3900 50  0001 C CNN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/81/93/fa/ed/87/47/46/00/CD00003136.pdf/files/CD00003136.pdf/jcr:content/translations/en.CD00003136.pdf" H 6500 4650 50  0001 C CNN
F 4 "ST1480ACDR" H 6500 4600 60  0001 C CNN "Part Number"
	1    6500 4600
	1    0    0    -1  
$EndComp
Text HLabel 3500 1150 1    60   Output ~ 0
OSC32_IN
$Comp
L power:+3.3V #PWR028
U 1 1 5A0B2F0C
P 3900 1150
F 0 "#PWR028" H 3900 1000 50  0001 C CNN
F 1 "+3.3V" H 3900 1290 50  0000 C CNN
F 2 "" H 3900 1150 50  0001 C CNN
F 3 "" H 3900 1150 50  0001 C CNN
	1    3900 1150
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR029
U 1 1 5A0B2F97
P 2350 1400
F 0 "#PWR029" H 2350 1250 50  0001 C CNN
F 1 "+BATT" H 2350 1540 50  0000 C CNN
F 2 "" H 2350 1400 50  0001 C CNN
F 3 "" H 2350 1400 50  0001 C CNN
	1    2350 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR030
U 1 1 5A0B2FCB
P 3450 2750
F 0 "#PWR030" H 3450 2500 50  0001 C CNN
F 1 "GND" H 3450 2600 50  0000 C CNN
F 2 "" H 3450 2750 50  0001 C CNN
F 3 "" H 3450 2750 50  0001 C CNN
	1    3450 2750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C19
U 1 1 5A0B2FFD
P 2000 1750
F 0 "C19" H 2025 1850 50  0000 L CNN
F 1 "100 nF" H 2025 1650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 2038 1600 50  0001 C CNN
F 3 "" H 2000 1750 50  0001 C CNN
	1    2000 1750
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR031
U 1 1 5A0B3116
P 2000 1400
F 0 "#PWR031" H 2000 1250 50  0001 C CNN
F 1 "+3.3V" H 2000 1540 50  0000 C CNN
F 2 "" H 2000 1400 50  0001 C CNN
F 3 "" H 2000 1400 50  0001 C CNN
	1    2000 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R24
U 1 1 5A0B3562
P 3700 1600
F 0 "R24" V 3780 1600 50  0000 C CNN
F 1 "10K" V 3700 1600 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3630 1600 50  0001 C CNN
F 3 "" H 3700 1600 50  0001 C CNN
	1    3700 1600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R23
U 1 1 5A0B35AA
P 3700 1400
F 0 "R23" V 3780 1400 50  0000 C CNN
F 1 "10K" V 3700 1400 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3630 1400 50  0001 C CNN
F 3 "" H 3700 1400 50  0001 C CNN
	1    3700 1400
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR032
U 1 1 5A0B3A49
P 6500 5400
F 0 "#PWR032" H 6500 5150 50  0001 C CNN
F 1 "GND" H 6500 5250 50  0000 C CNN
F 2 "" H 6500 5400 50  0001 C CNN
F 3 "" H 6500 5400 50  0001 C CNN
	1    6500 5400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR033
U 1 1 5A0B3ACB
P 6500 3900
F 0 "#PWR033" H 6500 3750 50  0001 C CNN
F 1 "+3.3V" H 6500 4040 50  0000 C CNN
F 2 "" H 6500 3900 50  0001 C CNN
F 3 "" H 6500 3900 50  0001 C CNN
	1    6500 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C20
U 1 1 5A0B3B29
P 6950 4000
F 0 "C20" H 6975 4100 50  0000 L CNN
F 1 "100 nF" H 6975 3900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6988 3850 50  0001 C CNN
F 3 "" H 6950 4000 50  0001 C CNN
	1    6950 4000
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR034
U 1 1 5A0B3BC0
P 7200 4100
F 0 "#PWR034" H 7200 3850 50  0001 C CNN
F 1 "GND" H 7200 3950 50  0000 C CNN
F 2 "" H 7200 4100 50  0001 C CNN
F 3 "" H 7200 4100 50  0001 C CNN
	1    7200 4100
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v4-rescue:Conn_01x06 J4
U 1 1 5A0B47CF
P 10150 4800
AR Path="/5A0B47CF" Ref="J4"  Part="1" 
AR Path="/5A0ADB1D/5A0B47CF" Ref="J4"  Part="1" 
F 0 "J4" H 10150 5100 50  0000 C CNN
F 1 "Conn_01x06" H 10150 4400 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 10150 4800 50  0001 C CNN
F 3 "" H 10150 4800 50  0001 C CNN
	1    10150 4800
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR035
U 1 1 5A0B4904
P 9850 5100
F 0 "#PWR035" H 9850 4850 50  0001 C CNN
F 1 "GND" H 9850 4950 50  0000 C CNN
F 2 "" H 9850 5100 50  0001 C CNN
F 3 "" H 9850 5100 50  0001 C CNN
	1    9850 5100
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR036
U 1 1 5A0B49D6
P 8900 4100
F 0 "#PWR036" H 8900 3950 50  0001 C CNN
F 1 "+3.3V" H 8900 4240 50  0000 C CNN
F 2 "" H 8900 4100 50  0001 C CNN
F 3 "" H 8900 4100 50  0001 C CNN
	1    8900 4100
	1    0    0    -1  
$EndComp
Text HLabel 1450 2100 0    60   Output ~ 0
RESET
Text HLabel 9600 4600 0    60   Input ~ 0
USART1_TX
Text HLabel 9600 4700 0    60   Output ~ 0
USART1_RX
Text HLabel 5900 4500 0    60   Output ~ 0
USART2_RX
Text HLabel 5900 4600 0    60   Input ~ 0
USART2_MODE
Text HLabel 5900 4800 0    60   Input ~ 0
USART2_TX
$Comp
L kbxBinaryClock.v4-rescue:Conn_02x03_Odd_Even J3
U 1 1 5A0E0EE5
P 7500 4900
AR Path="/5A0E0EE5" Ref="J3"  Part="1" 
AR Path="/5A0ADB1D/5A0E0EE5" Ref="J3"  Part="1" 
F 0 "J3" H 7550 5100 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 7550 4700 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03_Pitch2.54mm" H 7500 4900 50  0001 C CNN
F 3 "" H 7500 4900 50  0001 C CNN
	1    7500 4900
	1    0    0    1   
$EndComp
$Comp
L Device:C C22
U 1 1 5A6187BF
P 9800 4350
F 0 "C22" H 9825 4450 50  0000 L CNN
F 1 "100 nF" H 9825 4250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 9838 4200 50  0001 C CNN
F 3 "" H 9800 4350 50  0001 C CNN
	1    9800 4350
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR037
U 1 1 5A6AC87F
P 6700 1300
F 0 "#PWR037" H 6700 1150 50  0001 C CNN
F 1 "+3.3V" H 6700 1440 50  0000 C CNN
F 2 "" H 6700 1300 50  0001 C CNN
F 3 "" H 6700 1300 50  0001 C CNN
	1    6700 1300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C21
U 1 1 5A6AC90E
P 7350 2550
F 0 "C21" H 7375 2650 50  0000 L CNN
F 1 "100 nF" H 7375 2450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7388 2400 50  0001 C CNN
F 3 "" H 7350 2550 50  0001 C CNN
	1    7350 2550
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR038
U 1 1 5A6ACBF8
P 7350 2800
F 0 "#PWR038" H 7350 2550 50  0001 C CNN
F 1 "GND" H 7350 2650 50  0000 C CNN
F 2 "" H 7350 2800 50  0001 C CNN
F 3 "" H 7350 2800 50  0001 C CNN
	1    7350 2800
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v4-rescue:DS3234 U5
U 1 1 5A7DBF61
P 2950 2050
AR Path="/5A7DBF61" Ref="U5"  Part="1" 
AR Path="/5A0ADB1D/5A7DBF61" Ref="U5"  Part="1" 
F 0 "U5" H 2700 2750 60  0000 C CNN
F 1 "DS3234" H 3150 2750 60  0000 C CNN
F 2 "Housings_SOIC:SO-20_12.8x7.5mm_Pitch1.27mm" H 2950 2050 60  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/DS3234.pdf" H 2950 2050 60  0001 C CNN
F 4 "Maxim" H 2750 1300 60  0001 C CNN "Manufacturer"
	1    2950 2050
	1    0    0    -1  
$EndComp
Text HLabel 1450 2500 0    60   Input ~ 0
SPI1_SCK
Text HLabel 1450 2300 0    60   Input ~ 0
SPI1_MOSI
Text HLabel 1450 2400 0    60   Output ~ 0
SPI1_MISO
$Comp
L power:GND #PWR039
U 1 1 5A0B3065
P 2000 1900
F 0 "#PWR039" H 2000 1650 50  0001 C CNN
F 1 "GND" H 2000 1750 50  0000 C CNN
F 2 "" H 2000 1900 50  0001 C CNN
F 3 "" H 2000 1900 50  0001 C CNN
	1    2000 1900
	1    0    0    -1  
$EndComp
Text HLabel 1450 2200 0    60   Input ~ 0
SPI1_NSS_RTC
$Comp
L kbxBinaryClock.v4-rescue:LM74 U8
U 1 1 5A7E2809
P 9300 2050
AR Path="/5A7E2809" Ref="U8"  Part="1" 
AR Path="/5A0ADB1D/5A7E2809" Ref="U8"  Part="1" 
F 0 "U8" H 9400 2350 60  0000 C CNN
F 1 "LM74" H 9450 1750 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 9300 2050 60  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm74.pdf" H 9300 2050 60  0001 C CNN
F 4 "LM74CIMX-3/NOPB" H 9300 2050 60  0001 C CNN "Part Number"
	1    9300 2050
	1    0    0    -1  
$EndComp
Text HLabel 9400 4200 0    60   Output ~ 0
RESET
Wire Wire Line
	6900 4800 7300 4800
Wire Wire Line
	6900 4500 7100 4500
Wire Wire Line
	7100 4350 7100 4500
Wire Wire Line
	7100 4900 7300 4900
Wire Wire Line
	6500 5200 6500 5300
Wire Wire Line
	7300 5000 7100 5000
Wire Wire Line
	7100 5000 7100 5300
Connection ~ 6500 5300
Wire Wire Line
	6500 3900 6500 4000
Wire Wire Line
	6800 4000 6500 4000
Connection ~ 6500 4000
Wire Wire Line
	7100 4000 7200 4000
Wire Wire Line
	7200 4000 7200 4100
Wire Wire Line
	6100 4600 6100 4700
Wire Wire Line
	9850 5100 9850 5000
Wire Wire Line
	9850 5000 9950 5000
Wire Wire Line
	5900 4500 6100 4500
Wire Wire Line
	6100 4600 5900 4600
Wire Wire Line
	5900 4800 6100 4800
Wire Wire Line
	7800 5300 7800 5000
Connection ~ 7100 5300
Wire Wire Line
	7300 4650 7800 4650
Wire Wire Line
	7300 4800 7300 4650
Wire Wire Line
	7800 4900 7900 4900
Wire Wire Line
	7900 4900 7900 4500
Connection ~ 7100 4500
Wire Wire Line
	9400 4200 9800 4200
Wire Wire Line
	9800 4500 9950 4500
Wire Wire Line
	9600 4600 9950 4600
Wire Wire Line
	9950 4700 9600 4700
Wire Wire Line
	8900 4100 8900 4800
Wire Wire Line
	8900 4800 9950 4800
Wire Wire Line
	2450 1500 2350 1500
Wire Wire Line
	2350 1500 2350 1400
Wire Wire Line
	2450 1900 2000 1900
Wire Wire Line
	3450 1600 3550 1600
Wire Wire Line
	3500 1500 3450 1500
Wire Wire Line
	3500 1150 3500 1400
Wire Wire Line
	3500 1400 3550 1400
Connection ~ 3500 1400
Wire Wire Line
	2450 1700 2350 1700
Wire Wire Line
	2350 1700 2350 1600
Wire Wire Line
	2000 1600 2000 1400
Wire Wire Line
	3850 1600 3900 1600
Wire Wire Line
	3900 1600 3900 1400
Wire Wire Line
	3850 1400 3900 1400
Connection ~ 3900 1400
Wire Wire Line
	2450 2500 1450 2500
Wire Wire Line
	1450 2400 2450 2400
Wire Wire Line
	2450 2300 1450 2300
Wire Wire Line
	1450 2200 2450 2200
Wire Wire Line
	2450 2600 2450 2500
Wire Wire Line
	3450 1800 3450 1900
Connection ~ 3450 1900
Connection ~ 3450 2000
Connection ~ 3450 2100
Connection ~ 3450 2200
Connection ~ 3450 2300
Connection ~ 3450 2400
Connection ~ 3450 2500
Connection ~ 3450 2600
Wire Wire Line
	7350 1400 7350 1900
Connection ~ 6700 1400
Wire Wire Line
	7100 1900 7350 1900
Connection ~ 7350 1900
Wire Wire Line
	7350 2700 7350 2750
Wire Wire Line
	6600 2700 6600 2750
Wire Wire Line
	6600 2750 7350 2750
Connection ~ 7350 2750
Wire Wire Line
	6700 1300 6700 1400
Text HLabel 5100 2000 0    60   Input ~ 0
SPI1_SCK
Text HLabel 5100 2150 0    60   Input ~ 0
SPI1_MOSI
Text HLabel 5100 2300 0    60   Output ~ 0
SPI1_MISO
Text HLabel 5100 1850 0    60   Input ~ 0
SPI1_NSS_TEMP
Wire Wire Line
	5100 1850 6100 1850
Wire Wire Line
	6100 2000 5100 2000
Wire Wire Line
	5100 2150 6100 2150
Wire Wire Line
	6100 2300 5100 2300
Text HLabel 8650 2200 0    60   Input ~ 0
SPI1_NSS_TEMP
Text HLabel 8650 2050 0    60   Input ~ 0
SPI1_SCK
Wire Wire Line
	8650 1900 8850 1900
Wire Wire Line
	8850 2050 8650 2050
Wire Wire Line
	8650 2200 8850 2200
Text HLabel 8650 1900 0    60   Output ~ 0
SPI1_MISO
Wire Wire Line
	9750 2750 9750 2150
Connection ~ 9750 2050
Connection ~ 9750 2150
Wire Wire Line
	9300 2750 9300 2500
Connection ~ 9300 2750
Wire Wire Line
	9300 1400 9300 1600
Connection ~ 7350 1400
$Comp
L kbxBinaryClock.v4-rescue:DS1722 U7
U 1 1 5A7E240D
P 6600 2150
AR Path="/5A7E240D" Ref="U7"  Part="1" 
AR Path="/5A0ADB1D/5A7E240D" Ref="U7"  Part="1" 
F 0 "U7" H 6800 2750 60  0000 C CNN
F 1 "DS1722" H 6800 1750 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 6600 2150 60  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/DS1722.pdf" H 6600 2150 60  0001 C CNN
	1    6600 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 1400 6700 1400
Wire Wire Line
	2350 1600 2000 1600
Text Notes 6550 3200 0    60   ~ 0
Install DS1722 *OR* LM74 -- DO NOT INSTALL BOTH
$Comp
L win:GS2 SB4
U 1 1 5A9B217E
P 1700 2100
F 0 "SB4" H 1800 2250 50  0000 C CNN
F 1 "GS2" H 1800 1951 50  0000 C CNN
F 2 "Connectors:GS2" V 1774 2100 50  0001 C CNN
F 3 "" H 1700 2100 50  0001 C CNN
	1    1700 2100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1450 2100 1500 2100
Wire Wire Line
	1900 2100 2450 2100
$Comp
L Device:LED_Dual_AACC LED26
U 1 1 5B2225C7
P 2850 4350
F 0 "LED26" H 2850 4575 50  0000 C CNN
F 1 "LED_Dual_AACC" H 2850 4100 50  0000 C CNN
F 2 "LEDs:LED_Cree-PLCC4_3.2x2.8mm_CCW" H 2880 4350 50  0001 C CNN
F 3 "https://www.vcclite.com/product/led-smt-1206-1-72-1v-20ma-redgreen-2k/" H 2880 4350 50  0001 C CNN
F 4 "CMD1522SRUGCTR8" H 2850 4350 60  0001 C CNN "Part Number"
	1    2850 4350
	0    1    1    0   
$EndComp
$Comp
L kbxBinaryClock.v4-rescue:2N7002 Q4
U 1 1 5B2225CE
P 3550 4950
AR Path="/5B2225CE" Ref="Q4"  Part="1" 
AR Path="/5A0ADB1D/5B2225CE" Ref="Q4"  Part="1" 
F 0 "Q4" V 3600 4700 50  0000 L CNN
F 1 "2N7002" V 3500 4550 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3750 4875 50  0001 L CIN
F 3 "https://www.diodes.com/assets/Datasheets/ds11303.pdf" H 3550 4950 50  0001 L CNN
	1    3550 4950
	0    -1   1    0   
$EndComp
$Comp
L kbxBinaryClock.v4-rescue:2N7002 Q3
U 1 1 5B2225D5
P 3450 4450
AR Path="/5B2225D5" Ref="Q3"  Part="1" 
AR Path="/5A0ADB1D/5B2225D5" Ref="Q3"  Part="1" 
F 0 "Q3" V 3500 4200 50  0000 L CNN
F 1 "2N7002" V 3400 4050 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3650 4375 50  0001 L CIN
F 3 "https://www.diodes.com/assets/Datasheets/ds11303.pdf" H 3450 4450 50  0001 L CNN
	1    3450 4450
	0    -1   1    0   
$EndComp
$Comp
L Device:R R21
U 1 1 5B2225DC
P 3000 5350
F 0 "R21" V 3080 5350 50  0000 C CNN
F 1 "1K" V 3000 5350 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2930 5350 50  0001 C CNN
F 3 "" H 3000 5350 50  0001 C CNN
	1    3000 5350
	0    1    1    0   
$EndComp
$Comp
L Device:R R20
U 1 1 5B2225E3
P 3000 5150
F 0 "R20" V 3080 5150 50  0000 C CNN
F 1 "1K" V 3000 5150 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2930 5150 50  0001 C CNN
F 3 "" H 3000 5150 50  0001 C CNN
	1    3000 5150
	0    -1   1    0   
$EndComp
$Comp
L power:GND #PWR040
U 1 1 5B2225EA
P 3900 5650
F 0 "#PWR040" H 3900 5400 50  0001 C CNN
F 1 "GND" H 3900 5500 50  0000 C CNN
F 2 "" H 3900 5650 50  0001 C CNN
F 3 "" H 3900 5650 50  0001 C CNN
	1    3900 5650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R25
U 1 1 5B2225F0
P 4200 4200
F 0 "R25" V 4280 4200 50  0000 C CNN
F 1 "1M" V 4200 4200 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4130 4200 50  0001 C CNN
F 3 "" H 4200 4200 50  0001 C CNN
	1    4200 4200
	0    1    1    0   
$EndComp
$Comp
L Device:R R26
U 1 1 5B2225F7
P 4200 4700
F 0 "R26" V 4280 4700 50  0000 C CNN
F 1 "1M" V 4200 4700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4130 4700 50  0001 C CNN
F 3 "" H 4200 4700 50  0001 C CNN
	1    4200 4700
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR041
U 1 1 5B222600
P 4500 4000
F 0 "#PWR041" H 4500 3850 50  0001 C CNN
F 1 "+3.3V" H 4500 4140 50  0000 C CNN
F 2 "" H 4500 4000 50  0001 C CNN
F 3 "" H 4500 4000 50  0001 C CNN
	1    4500 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:Buzzer BZ1
U 1 1 5B222607
P 1500 4150
F 0 "BZ1" H 1650 4200 50  0000 L CNN
F 1 "Buzzer" H 1650 4100 50  0000 L CNN
F 2 "Buzzers_Beepers:Buzzer_12x9.5RM7.6" V 1475 4250 50  0001 C CNN
F 3 "https://product.tdk.com/en/search/sw_piezo/sw_piezo/em-buzzer/info?part_no=SD1614TT-B1" V 1475 4250 50  0001 C CNN
F 4 "TDK SD1614TT-B1" H 1500 4150 60  0001 C CNN "Part Number"
	1    1500 4150
	-1   0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v4-rescue:2N7002 Q2
U 1 1 5B22260E
P 1500 5050
AR Path="/5B22260E" Ref="Q2"  Part="1" 
AR Path="/5A0ADB1D/5B22260E" Ref="Q2"  Part="1" 
F 0 "Q2" H 1700 5125 50  0000 L CNN
F 1 "2N7002" H 1700 5050 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 1700 4975 50  0001 L CIN
F 3 "https://www.diodes.com/assets/Datasheets/ds11303.pdf" H 1500 5050 50  0001 L CNN
	1    1500 5050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR042
U 1 1 5B222615
P 1600 5650
F 0 "#PWR042" H 1600 5400 50  0001 C CNN
F 1 "GND" H 1600 5500 50  0000 C CNN
F 2 "" H 1600 5650 50  0001 C CNN
F 3 "" H 1600 5650 50  0001 C CNN
	1    1600 5650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R19
U 1 1 5B22261B
P 1300 5300
F 0 "R19" V 1380 5300 50  0000 C CNN
F 1 "1M" V 1300 5300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 1230 5300 50  0001 C CNN
F 3 "" H 1300 5300 50  0001 C CNN
	1    1300 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R27
U 1 1 5B222623
P 4200 5200
F 0 "R27" V 4280 5200 50  0000 C CNN
F 1 "1M" V 4200 5200 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4130 5200 50  0001 C CNN
F 3 "" H 4200 5200 50  0001 C CNN
	1    4200 5200
	0    1    1    0   
$EndComp
$Comp
L kbxBinaryClock.v4-rescue:2N7002 Q5
U 1 1 5B22262B
P 3650 5450
AR Path="/5B22262B" Ref="Q5"  Part="1" 
AR Path="/5A0ADB1D/5B22262B" Ref="Q5"  Part="1" 
F 0 "Q5" V 3700 5200 50  0000 L CNN
F 1 "2N7002" V 3600 5050 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3850 5375 50  0001 L CIN
F 3 "https://www.diodes.com/assets/Datasheets/ds11303.pdf" H 3650 5450 50  0001 L CNN
	1    3650 5450
	0    -1   1    0   
$EndComp
$Comp
L Device:R R22
U 1 1 5B222632
P 3000 5550
F 0 "R22" V 3080 5550 50  0000 C CNN
F 1 "1K" V 3000 5550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2930 5550 50  0001 C CNN
F 3 "" H 3000 5550 50  0001 C CNN
	1    3000 5550
	0    1    1    0   
$EndComp
$Comp
L win:LED_RGB_COMMON_ANODE LED25
U 1 1 5B222639
P 2200 4350
F 0 "LED25" H 2200 4625 50  0000 C CNN
F 1 "LED_RGB_COMMON_ANODE" H 2200 4075 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm-4" H 2225 4500 60  0001 C CNN
F 3 "https://www.adafruit.com/product/302" H 2225 4500 60  0001 C CNN
	1    2200 4350
	0    1    1    0   
$EndComp
Connection ~ 4500 4200
Wire Wire Line
	4500 4000 4500 4200
Wire Wire Line
	1600 4250 1600 4600
Wire Wire Line
	1600 5250 1600 5450
Wire Wire Line
	1200 5050 1300 5050
Wire Wire Line
	1300 5050 1300 5150
Wire Wire Line
	1300 5450 1600 5450
Connection ~ 1600 5450
Wire Wire Line
	4050 4200 3650 4200
Wire Wire Line
	3450 4200 3450 4250
Wire Wire Line
	4050 4700 3800 4700
Wire Wire Line
	3550 4700 3550 4750
Wire Wire Line
	4050 5200 3950 5200
Wire Wire Line
	3650 5200 3650 5250
Wire Wire Line
	4500 5200 4350 5200
Wire Wire Line
	4350 4700 4500 4700
Connection ~ 4500 4700
Wire Wire Line
	3650 3950 3650 4200
Connection ~ 3650 4200
Wire Wire Line
	3800 3950 3800 4700
Connection ~ 3800 4700
Wire Wire Line
	3950 3950 3950 5200
Connection ~ 3950 5200
Wire Wire Line
	3650 4550 3900 4550
Wire Wire Line
	3900 4550 3900 5050
Wire Wire Line
	3850 5550 3900 5550
Connection ~ 3900 5550
Wire Wire Line
	3750 5050 3900 5050
Connection ~ 3900 5050
Wire Wire Line
	3150 5150 3200 5150
Wire Wire Line
	3200 5150 3200 4550
Wire Wire Line
	3200 4550 3250 4550
Wire Wire Line
	3150 5350 3250 5350
Wire Wire Line
	3250 5350 3250 5050
Wire Wire Line
	3250 5050 3350 5050
Wire Wire Line
	3150 5550 3450 5550
Wire Wire Line
	1600 4050 1750 4050
Wire Wire Line
	2200 3950 2200 4050
Connection ~ 2200 4050
Connection ~ 2750 4050
Wire Wire Line
	2950 4650 2950 5050
Wire Wire Line
	2950 5050 2800 5050
Wire Wire Line
	2800 5050 2800 5150
Wire Wire Line
	2350 5150 2800 5150
Wire Wire Line
	2850 5550 2050 5550
Wire Wire Line
	2050 5550 2050 4600
Wire Wire Line
	2350 4600 2350 5150
Connection ~ 2800 5150
Wire Wire Line
	2850 5350 2750 5350
Wire Wire Line
	2200 5350 2200 4600
Wire Wire Line
	2750 4650 2750 5350
Connection ~ 2750 5350
Wire Wire Line
	4350 4200 4500 4200
$Comp
L kbxBinaryClock.v4-rescue:+V_IN #PWR043
U 1 1 5B222675
P 2200 3950
AR Path="/5B222675" Ref="#PWR043"  Part="1" 
AR Path="/5A0ADB1D/5B222675" Ref="#PWR043"  Part="1" 
F 0 "#PWR043" H 2200 3800 50  0001 C CNN
F 1 "+V_IN" H 2200 4090 50  0000 C CNN
F 2 "" H 2200 3950 50  0001 C CNN
F 3 "" H 2200 3950 50  0001 C CNN
	1    2200 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:D D2
U 1 1 5B22267B
P 1750 4300
F 0 "D2" H 1750 4400 50  0000 C CNN
F 1 "1N4148" H 1750 4200 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 1750 4300 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds30086.pdf" H 1750 4300 50  0001 C CNN
	1    1750 4300
	0    1    1    0   
$EndComp
Wire Wire Line
	1750 4150 1750 4050
Connection ~ 1750 4050
Wire Wire Line
	1750 4450 1750 4600
Wire Wire Line
	1750 4600 1600 4600
Connection ~ 1600 4600
Text Notes 2350 3950 0    60   ~ 0
Install LED25 *OR* LED26\nDO NOT INSTALL BOTH
Text HLabel 1200 5050 0    60   Input ~ 0
BEEP
Text HLabel 3650 3950 1    60   Input ~ 0
LED0
Text HLabel 3800 3950 1    60   Input ~ 0
LED1
Text HLabel 3950 3950 1    60   Input ~ 0
LED2
$Comp
L Device:R R29
U 1 1 5B308577
P 7700 4350
F 0 "R29" V 7780 4350 50  0000 C CNN
F 1 "120" V 7700 4350 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7630 4350 50  0001 C CNN
F 3 "" H 7700 4350 50  0001 C CNN
	1    7700 4350
	0    1    1    0   
$EndComp
$Comp
L Device:R R28
U 1 1 5B309286
P 6800 5300
F 0 "R28" V 6880 5300 50  0000 C CNN
F 1 "0" V 6800 5300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6730 5300 50  0001 C CNN
F 3 "" H 6800 5300 50  0001 C CNN
	1    6800 5300
	0    1    1    0   
$EndComp
Wire Wire Line
	6950 5300 7100 5300
Wire Wire Line
	6650 5300 6500 5300
$Comp
L win:GS2 SB5
U 1 1 5B309DA5
P 7300 4350
F 0 "SB5" V 7200 4350 50  0000 C CNN
F 1 "GS2" H 7400 4201 50  0000 C CNN
F 2 "Connectors:GS2" V 7374 4350 50  0001 C CNN
F 3 "" H 7300 4350 50  0001 C CNN
	1    7300 4350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7800 4800 7800 4650
Wire Wire Line
	7850 4650 7850 4350
Connection ~ 7800 4650
Wire Wire Line
	7500 4350 7550 4350
Wire Wire Line
	6500 5300 6500 5400
Wire Wire Line
	6500 4000 6500 4100
Wire Wire Line
	7100 5300 7800 5300
Wire Wire Line
	7100 4500 7900 4500
Wire Wire Line
	7100 4500 7100 4900
Wire Wire Line
	3500 1400 3500 1500
Wire Wire Line
	3900 1400 3900 1150
Wire Wire Line
	3450 1900 3450 2000
Wire Wire Line
	3450 2000 3450 2100
Wire Wire Line
	3450 2100 3450 2200
Wire Wire Line
	3450 2200 3450 2300
Wire Wire Line
	3450 2300 3450 2400
Wire Wire Line
	3450 2400 3450 2500
Wire Wire Line
	3450 2500 3450 2600
Wire Wire Line
	3450 2600 3450 2750
Wire Wire Line
	6700 1400 7350 1400
Wire Wire Line
	7350 1900 7350 2400
Wire Wire Line
	7350 2750 7350 2800
Wire Wire Line
	7350 2750 9300 2750
Wire Wire Line
	9750 2050 9750 1950
Wire Wire Line
	9750 2150 9750 2050
Wire Wire Line
	9300 2750 9750 2750
Wire Wire Line
	7350 1400 9300 1400
Wire Wire Line
	4500 4200 4500 4700
Wire Wire Line
	1600 5450 1600 5650
Wire Wire Line
	4500 4700 4500 5200
Wire Wire Line
	3650 4200 3450 4200
Wire Wire Line
	3800 4700 3550 4700
Wire Wire Line
	3950 5200 3650 5200
Wire Wire Line
	3900 5550 3900 5650
Wire Wire Line
	3900 5050 3900 5550
Wire Wire Line
	2200 4050 2750 4050
Wire Wire Line
	2200 4050 2200 4100
Wire Wire Line
	2750 4050 2950 4050
Wire Wire Line
	2800 5150 2850 5150
Wire Wire Line
	2750 5350 2200 5350
Wire Wire Line
	1750 4050 2200 4050
Wire Wire Line
	1600 4600 1600 4850
Wire Wire Line
	7800 4650 7850 4650
$EndSCHEMATC
