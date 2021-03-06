EESchema Schematic File Version 4
LIBS:kbxBinaryClock.v3-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
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
L kbxBinaryClock.v3-rescue:MAX485E U9
U 1 1 5A0ADC38
P 9000 2400
F 0 "U9" H 8760 2850 50  0000 C CNN
F 1 "MAX485E" H 9030 2850 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 9000 1700 50  0001 C CNN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/81/93/fa/ed/87/47/46/00/CD00003136.pdf/files/CD00003136.pdf/jcr:content/translations/en.CD00003136.pdf" H 9000 2450 50  0001 C CNN
F 4 "ST1480ACDR" H 9000 2400 60  0001 C CNN "Part Number"
	1    9000 2400
	1    0    0    -1  
$EndComp
Text HLabel 3400 1500 1    60   Output ~ 0
OSC32_IN
$Comp
L kbxBinaryClock.v3-rescue:+3.3V #PWR036
U 1 1 5A0B2F0C
P 3800 1500
F 0 "#PWR036" H 3800 1350 50  0001 C CNN
F 1 "+3.3V" H 3800 1640 50  0000 C CNN
F 2 "" H 3800 1500 50  0001 C CNN
F 3 "" H 3800 1500 50  0001 C CNN
	1    3800 1500
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:+BATT #PWR037
U 1 1 5A0B2F97
P 2250 1750
F 0 "#PWR037" H 2250 1600 50  0001 C CNN
F 1 "+BATT" H 2250 1890 50  0000 C CNN
F 2 "" H 2250 1750 50  0001 C CNN
F 3 "" H 2250 1750 50  0001 C CNN
	1    2250 1750
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:GND #PWR038
U 1 1 5A0B2FCB
P 3350 3100
F 0 "#PWR038" H 3350 2850 50  0001 C CNN
F 1 "GND" H 3350 2950 50  0000 C CNN
F 2 "" H 3350 3100 50  0001 C CNN
F 3 "" H 3350 3100 50  0001 C CNN
	1    3350 3100
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C23
U 1 1 5A0B2FFD
P 1900 2100
F 0 "C23" H 1925 2200 50  0000 L CNN
F 1 "100 nF" H 1925 2000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1938 1950 50  0001 C CNN
F 3 "" H 1900 2100 50  0001 C CNN
	1    1900 2100
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:+3.3V #PWR039
U 1 1 5A0B3116
P 1900 1750
F 0 "#PWR039" H 1900 1600 50  0001 C CNN
F 1 "+3.3V" H 1900 1890 50  0000 C CNN
F 2 "" H 1900 1750 50  0001 C CNN
F 3 "" H 1900 1750 50  0001 C CNN
	1    1900 1750
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R39
U 1 1 5A0B3562
P 3600 1950
F 0 "R39" V 3680 1950 50  0000 C CNN
F 1 "10K" V 3600 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3530 1950 50  0001 C CNN
F 3 "" H 3600 1950 50  0001 C CNN
	1    3600 1950
	0    -1   -1   0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R38
U 1 1 5A0B35AA
P 3600 1750
F 0 "R38" V 3680 1750 50  0000 C CNN
F 1 "10K" V 3600 1750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3530 1750 50  0001 C CNN
F 3 "" H 3600 1750 50  0001 C CNN
	1    3600 1750
	0    -1   -1   0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:GND #PWR040
U 1 1 5A0B3A49
P 9000 3200
F 0 "#PWR040" H 9000 2950 50  0001 C CNN
F 1 "GND" H 9000 3050 50  0000 C CNN
F 2 "" H 9000 3200 50  0001 C CNN
F 3 "" H 9000 3200 50  0001 C CNN
	1    9000 3200
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:+3.3V #PWR041
U 1 1 5A0B3ACB
P 9000 1700
F 0 "#PWR041" H 9000 1550 50  0001 C CNN
F 1 "+3.3V" H 9000 1840 50  0000 C CNN
F 2 "" H 9000 1700 50  0001 C CNN
F 3 "" H 9000 1700 50  0001 C CNN
	1    9000 1700
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C26
U 1 1 5A0B3B29
P 9450 1800
F 0 "C26" H 9475 1900 50  0000 L CNN
F 1 "100 nF" H 9475 1700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 9488 1650 50  0001 C CNN
F 3 "" H 9450 1800 50  0001 C CNN
	1    9450 1800
	0    -1   -1   0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:GND #PWR042
U 1 1 5A0B3BC0
P 9700 1900
F 0 "#PWR042" H 9700 1650 50  0001 C CNN
F 1 "GND" H 9700 1750 50  0000 C CNN
F 2 "" H 9700 1900 50  0001 C CNN
F 3 "" H 9700 1900 50  0001 C CNN
	1    9700 1900
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:Conn_01x06 J2
U 1 1 5A0B47CF
P 6150 2500
F 0 "J2" H 6150 2800 50  0000 C CNN
F 1 "Conn_01x06" H 6150 2100 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 6150 2500 50  0001 C CNN
F 3 "" H 6150 2500 50  0001 C CNN
	1    6150 2500
	1    0    0    1   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:GND #PWR043
U 1 1 5A0B4904
P 5850 2800
F 0 "#PWR043" H 5850 2550 50  0001 C CNN
F 1 "GND" H 5850 2650 50  0000 C CNN
F 2 "" H 5850 2800 50  0001 C CNN
F 3 "" H 5850 2800 50  0001 C CNN
	1    5850 2800
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:+3.3V #PWR044
U 1 1 5A0B49D6
P 4900 1800
F 0 "#PWR044" H 4900 1650 50  0001 C CNN
F 1 "+3.3V" H 4900 1940 50  0000 C CNN
F 2 "" H 4900 1800 50  0001 C CNN
F 3 "" H 4900 1800 50  0001 C CNN
	1    4900 1800
	1    0    0    -1  
$EndComp
Text HLabel 1350 2450 0    60   Output ~ 0
RESET
Text HLabel 5600 2300 0    60   Input ~ 0
USART1_TX
Text HLabel 5600 2400 0    60   Output ~ 0
USART1_RX
Text HLabel 8400 2300 0    60   Output ~ 0
USART2_RX
Text HLabel 8400 2400 0    60   Input ~ 0
USART2_MODE
Text HLabel 8400 2600 0    60   Input ~ 0
USART2_TX
$Comp
L kbxBinaryClock.v3-rescue:Conn_02x03_Odd_Even J3
U 1 1 5A0E0EE5
P 10000 2700
F 0 "J3" H 10050 2900 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 10050 2500 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03_Pitch2.54mm" H 10000 2700 50  0001 C CNN
F 3 "" H 10000 2700 50  0001 C CNN
	1    10000 2700
	1    0    0    1   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C25
U 1 1 5A6187BF
P 5800 2050
F 0 "C25" H 5825 2150 50  0000 L CNN
F 1 "100 nF" H 5825 1950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5838 1900 50  0001 C CNN
F 3 "" H 5800 2050 50  0001 C CNN
	1    5800 2050
	-1   0    0    1   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:+3.3V #PWR045
U 1 1 5A6AC87F
P 2950 4100
F 0 "#PWR045" H 2950 3950 50  0001 C CNN
F 1 "+3.3V" H 2950 4240 50  0000 C CNN
F 2 "" H 2950 4100 50  0001 C CNN
F 3 "" H 2950 4100 50  0001 C CNN
	1    2950 4100
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C24
U 1 1 5A6AC90E
P 3600 5350
F 0 "C24" H 3625 5450 50  0000 L CNN
F 1 "100 nF" H 3625 5250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 3638 5200 50  0001 C CNN
F 3 "" H 3600 5350 50  0001 C CNN
	1    3600 5350
	-1   0    0    1   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:GND #PWR046
U 1 1 5A6ACBF8
P 3600 5600
F 0 "#PWR046" H 3600 5350 50  0001 C CNN
F 1 "GND" H 3600 5450 50  0000 C CNN
F 2 "" H 3600 5600 50  0001 C CNN
F 3 "" H 3600 5600 50  0001 C CNN
	1    3600 5600
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:DS3234 U6
U 1 1 5A7DBF61
P 2850 2400
F 0 "U6" H 2600 3100 60  0000 C CNN
F 1 "DS3234" H 3050 3100 60  0000 C CNN
F 2 "Housings_SOIC:SO-20_12.8x7.5mm_Pitch1.27mm" H 2850 2400 60  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/DS3234.pdf" H 2850 2400 60  0001 C CNN
F 4 "Maxim" H 2650 1650 60  0001 C CNN "Manufacturer"
	1    2850 2400
	1    0    0    -1  
$EndComp
Text HLabel 1350 2850 0    60   Input ~ 0
SPI1_SCK
Text HLabel 1350 2650 0    60   Input ~ 0
SPI1_MOSI
Text HLabel 1350 2750 0    60   Output ~ 0
SPI1_MISO
$Comp
L kbxBinaryClock.v3-rescue:GND #PWR047
U 1 1 5A0B3065
P 1900 2250
F 0 "#PWR047" H 1900 2000 50  0001 C CNN
F 1 "GND" H 1900 2100 50  0000 C CNN
F 2 "" H 1900 2250 50  0001 C CNN
F 3 "" H 1900 2250 50  0001 C CNN
	1    1900 2250
	1    0    0    -1  
$EndComp
Text HLabel 1350 2550 0    60   Input ~ 0
SPI1_NSS_RTC
$Comp
L kbxBinaryClock.v3-rescue:LM74 U8
U 1 1 5A7E2809
P 5550 4850
F 0 "U8" H 5650 5150 60  0000 C CNN
F 1 "LM74" H 5700 4550 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 5550 4850 60  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm74.pdf" H 5550 4850 60  0001 C CNN
F 4 "LM74CIMX-3/NOPB" H 5550 4850 60  0001 C CNN "Part Number"
	1    5550 4850
	1    0    0    -1  
$EndComp
Text HLabel 5400 1900 0    60   Output ~ 0
RESET
Wire Wire Line
	9400 2600 9800 2600
Wire Wire Line
	9400 2300 10400 2300
Wire Wire Line
	9600 2300 9600 2700
Wire Wire Line
	9600 2700 9800 2700
Wire Wire Line
	9000 3000 9000 3200
Wire Wire Line
	9800 2800 9600 2800
Wire Wire Line
	9600 2800 9600 3100
Wire Wire Line
	9000 3100 10300 3100
Connection ~ 9000 3100
Wire Wire Line
	9000 1700 9000 1900
Wire Wire Line
	9300 1800 9000 1800
Connection ~ 9000 1800
Wire Wire Line
	9600 1800 9700 1800
Wire Wire Line
	9700 1800 9700 1900
Wire Wire Line
	8600 2400 8600 2500
Wire Wire Line
	5850 2800 5850 2700
Wire Wire Line
	5850 2700 5950 2700
Wire Wire Line
	8400 2300 8600 2300
Wire Wire Line
	8600 2400 8400 2400
Wire Wire Line
	8400 2600 8600 2600
Wire Wire Line
	10300 3100 10300 2800
Connection ~ 9600 3100
Wire Wire Line
	10300 2600 10300 2450
Wire Wire Line
	10300 2450 9800 2450
Wire Wire Line
	9800 2450 9800 2600
Wire Wire Line
	10300 2700 10400 2700
Wire Wire Line
	10400 2700 10400 2300
Connection ~ 9600 2300
Wire Wire Line
	5400 1900 5800 1900
Wire Wire Line
	5800 2200 5950 2200
Wire Wire Line
	5600 2300 5950 2300
Wire Wire Line
	5950 2400 5600 2400
Wire Wire Line
	4900 1800 4900 2500
Wire Wire Line
	4900 2500 5950 2500
Wire Wire Line
	2350 1850 2250 1850
Wire Wire Line
	2250 1850 2250 1750
Wire Wire Line
	2350 2250 1900 2250
Wire Wire Line
	3350 1950 3450 1950
Wire Wire Line
	3400 1850 3350 1850
Wire Wire Line
	3400 1500 3400 1850
Wire Wire Line
	3400 1750 3450 1750
Connection ~ 3400 1750
Wire Wire Line
	2350 2050 2250 2050
Wire Wire Line
	2250 2050 2250 1950
Wire Wire Line
	1900 1950 1900 1750
Wire Wire Line
	3750 1950 3800 1950
Wire Wire Line
	3800 1950 3800 1500
Wire Wire Line
	3750 1750 3800 1750
Connection ~ 3800 1750
Wire Wire Line
	2350 2850 1350 2850
Wire Wire Line
	1350 2750 2350 2750
Wire Wire Line
	2350 2650 1350 2650
Wire Wire Line
	1350 2550 2350 2550
Wire Wire Line
	2350 2950 2350 2850
Wire Wire Line
	3350 2150 3350 3100
Connection ~ 3350 2250
Connection ~ 3350 2350
Connection ~ 3350 2450
Connection ~ 3350 2550
Connection ~ 3350 2650
Connection ~ 3350 2750
Connection ~ 3350 2850
Connection ~ 3350 2950
Wire Wire Line
	3600 4200 3600 5200
Connection ~ 2950 4200
Wire Wire Line
	3350 4700 3600 4700
Connection ~ 3600 4700
Wire Wire Line
	3600 5500 3600 5600
Wire Wire Line
	2850 5500 2850 5550
Wire Wire Line
	2850 5550 6000 5550
Connection ~ 3600 5550
Wire Wire Line
	2950 4100 2950 4200
Text HLabel 1350 4800 0    60   Input ~ 0
SPI1_SCK
Text HLabel 1350 4950 0    60   Input ~ 0
SPI1_MOSI
Text HLabel 1350 5100 0    60   Output ~ 0
SPI1_MISO
Text HLabel 1350 4650 0    60   Input ~ 0
SPI1_NSS_TEMP
Wire Wire Line
	1350 4650 2350 4650
Wire Wire Line
	2350 4800 1350 4800
Wire Wire Line
	1350 4950 2350 4950
Wire Wire Line
	2350 5100 1350 5100
Text HLabel 4900 5000 0    60   Input ~ 0
SPI1_NSS_TEMP
Text HLabel 4900 4850 0    60   Input ~ 0
SPI1_SCK
Wire Wire Line
	4900 4700 5100 4700
Wire Wire Line
	5100 4850 4900 4850
Wire Wire Line
	4900 5000 5100 5000
Text HLabel 4900 4700 0    60   Output ~ 0
SPI1_MISO
Wire Wire Line
	6000 5550 6000 4750
Connection ~ 6000 4850
Connection ~ 6000 4950
Wire Wire Line
	5550 5550 5550 5300
Connection ~ 5550 5550
Wire Wire Line
	5550 4200 5550 4400
Connection ~ 3600 4200
$Comp
L kbxBinaryClock.v3-rescue:DS1722 U7
U 1 1 5A7E240D
P 2850 4950
F 0 "U7" H 3050 5550 60  0000 C CNN
F 1 "DS1722" H 3050 4550 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2850 4950 60  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/DS1722.pdf" H 2850 4950 60  0001 C CNN
	1    2850 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 4200 5550 4200
Wire Wire Line
	2250 1950 1900 1950
Text Notes 2800 6050 0    60   ~ 0
Install DS1722 *OR* LM74 -- DO NOT INSTALL BOTH
$Comp
L kbxBinaryClock.v3-rescue:GS2 SB2
U 1 1 5A9B217E
P 1600 2450
F 0 "SB2" H 1700 2600 50  0000 C CNN
F 1 "GS2" H 1700 2301 50  0000 C CNN
F 2 "Connectors:GS2" V 1674 2450 50  0001 C CNN
F 3 "" H 1600 2450 50  0001 C CNN
	1    1600 2450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1350 2450 1400 2450
Wire Wire Line
	1800 2450 2350 2450
$EndSCHEMATC
