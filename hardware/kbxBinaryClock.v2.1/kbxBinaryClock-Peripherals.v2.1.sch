EESchema Schematic File Version 4
LIBS:kbxBinaryClock.v2.1-cache
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
L kbxBinaryClock.v2.1-rescue:DS3231 U6
U 1 1 5A0ADBD0
P 2900 2500
F 0 "U6" H 2800 2975 50  0000 R CNN
F 1 "DS3231" H 2800 2900 50  0000 R CNN
F 2 "Housings_SOIC:SOIC-16W_7.5x10.3mm_Pitch1.27mm" H 2950 2100 50  0001 L CNN
F 3 "" H 3170 2750 50  0001 C CNN
	1    2900 2500
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v2.1-rescue:MAX485E U9
U 1 1 5A0ADC38
P 9000 2400
F 0 "U9" H 8760 2850 50  0000 C CNN
F 1 "MAX485E" H 9030 2850 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 9000 1700 50  0001 C CNN
F 3 "" H 9000 2450 50  0001 C CNN
	1    9000 2400
	1    0    0    -1  
$EndComp
Text HLabel 3600 1500 1    60   Output ~ 0
OSC32_IN
$Comp
L power:+3.3V #PWR036
U 1 1 5A0B2F0C
P 2900 1500
F 0 "#PWR036" H 2900 1350 50  0001 C CNN
F 1 "+3.3V" H 2900 1640 50  0000 C CNN
F 2 "" H 2900 1500 50  0001 C CNN
F 3 "" H 2900 1500 50  0001 C CNN
	1    2900 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R40
U 1 1 5A0B2F28
P 3150 1600
F 0 "R40" V 3230 1600 50  0000 C CNN
F 1 "10K" V 3150 1600 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3080 1600 50  0001 C CNN
F 3 "" H 3150 1600 50  0001 C CNN
	1    3150 1600
	0    -1   -1   0   
$EndComp
$Comp
L power:+BATT #PWR037
U 1 1 5A0B2F97
P 3800 1500
F 0 "#PWR037" H 3800 1350 50  0001 C CNN
F 1 "+BATT" H 3800 1640 50  0000 C CNN
F 2 "" H 3800 1500 50  0001 C CNN
F 3 "" H 3800 1500 50  0001 C CNN
	1    3800 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR038
U 1 1 5A0B2FCB
P 2900 3200
F 0 "#PWR038" H 2900 2950 50  0001 C CNN
F 1 "GND" H 2900 3050 50  0000 C CNN
F 2 "" H 2900 3200 50  0001 C CNN
F 3 "" H 2900 3200 50  0001 C CNN
	1    2900 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C23
U 1 1 5A0B2FFD
P 1950 1600
F 0 "C23" H 1975 1700 50  0000 L CNN
F 1 "100 nF" H 1975 1500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1988 1450 50  0001 C CNN
F 3 "" H 1950 1600 50  0001 C CNN
	1    1950 1600
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR039
U 1 1 5A0B3065
P 1700 1600
F 0 "#PWR039" H 1700 1350 50  0001 C CNN
F 1 "GND" H 1700 1450 50  0000 C CNN
F 2 "" H 1700 1600 50  0001 C CNN
F 3 "" H 1700 1600 50  0001 C CNN
	1    1700 1600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R39
U 1 1 5A0B3562
P 2400 1750
F 0 "R39" V 2480 1750 50  0000 C CNN
F 1 "3K3" V 2400 1750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2330 1750 50  0001 C CNN
F 3 "" H 2400 1750 50  0001 C CNN
	1    2400 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R38
U 1 1 5A0B35AA
P 2200 1750
F 0 "R38" V 2280 1750 50  0000 C CNN
F 1 "3K3" V 2200 1750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2130 1750 50  0001 C CNN
F 3 "" H 2200 1750 50  0001 C CNN
	1    2200 1750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR040
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
L power:+3.3V #PWR041
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
L Device:C C26
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
L power:GND #PWR042
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
L kbxBinaryClock.v2.1-rescue:Conn_01x06 J2
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
L power:GND #PWR043
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
L power:+3.3V #PWR044
U 1 1 5A0B49D6
P 4900 1800
F 0 "#PWR044" H 4900 1650 50  0001 C CNN
F 1 "+3.3V" H 4900 1940 50  0000 C CNN
F 2 "" H 4900 1800 50  0001 C CNN
F 3 "" H 4900 1800 50  0001 C CNN
	1    4900 1800
	1    0    0    -1  
$EndComp
Text HLabel 5400 1900 0    60   Output ~ 0
RESET
Text HLabel 1400 2400 0    60   Input ~ 0
I2C1_SCL
Text HLabel 1400 2500 0    60   BiDi ~ 0
I2C1_SDA
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
L kbxBinaryClock.v2.1-rescue:Conn_02x03_Odd_Even J3
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
L Device:C C25
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
L kbxBinaryClock.v2.1-rescue:DS1621 U7
U 1 1 5A6AC3DF
P 2900 5200
F 0 "U7" H 3170 5750 50  0000 C CNN
F 1 "DS1621" H 3260 4650 50  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2900 5200 50  0001 C CNN
F 3 "" H 2900 5200 50  0001 C CNN
	1    2900 5200
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR045
U 1 1 5A6AC87F
P 2900 4250
F 0 "#PWR045" H 2900 4100 50  0001 C CNN
F 1 "+3.3V" H 2900 4390 50  0000 C CNN
F 2 "" H 2900 4250 50  0001 C CNN
F 3 "" H 2900 4250 50  0001 C CNN
	1    2900 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C24
U 1 1 5A6AC90E
P 4150 4650
F 0 "C24" H 4175 4750 50  0000 L CNN
F 1 "100 nF" H 4175 4550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 4188 4500 50  0001 C CNN
F 3 "" H 4150 4650 50  0001 C CNN
	1    4150 4650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR046
U 1 1 5A6ACBF8
P 4150 4850
F 0 "#PWR046" H 4150 4600 50  0001 C CNN
F 1 "GND" H 4150 4700 50  0000 C CNN
F 2 "" H 4150 4850 50  0001 C CNN
F 3 "" H 4150 4850 50  0001 C CNN
	1    4150 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR047
U 1 1 5A6ACE92
P 2900 5900
F 0 "#PWR047" H 2900 5650 50  0001 C CNN
F 1 "GND" H 2900 5750 50  0000 C CNN
F 2 "" H 2900 5900 50  0001 C CNN
F 3 "" H 2900 5900 50  0001 C CNN
	1    2900 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R41
U 1 1 5A800EAE
P 3150 1800
F 0 "R41" V 3230 1800 50  0000 C CNN
F 1 "10K" V 3150 1800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3080 1800 50  0001 C CNN
F 3 "" H 3150 1800 50  0001 C CNN
	1    3150 1800
	0    -1   -1   0   
$EndComp
Text HLabel 1400 2700 0    60   Output ~ 0
RESET
$Comp
L win:GS2 SB2
U 1 1 5A9B3265
P 1900 2700
F 0 "SB2" H 2000 2850 50  0000 C CNN
F 1 "GS2" H 2000 2551 50  0000 C CNN
F 2 "Connectors:GS2" V 1974 2700 50  0001 C CNN
F 3 "" H 1900 2700 50  0001 C CNN
	1    1900 2700
	0    -1   -1   0   
$EndComp
$Comp
L kbxBinaryClock.v2.1-rescue:MCP9804_MSOP U8
U 1 1 5AB87DF3
P 6100 5100
F 0 "U8" H 5850 5550 50  0000 C CNN
F 1 "MCP9804_MSOP" H 6500 5550 50  0000 C CNN
F 2 "Housings_SSOP:MSOP-8_3x3mm_Pitch0.65mm" H 5100 4600 50  0001 C CNN
F 3 "" H 5850 5550 50  0001 C CNN
	1    6100 5100
	1    0    0    -1  
$EndComp
Text HLabel 1400 5100 0    60   Input ~ 0
I2C1_SCL
Text HLabel 1400 4900 0    60   BiDi ~ 0
I2C1_SDA
Text HLabel 5200 4900 0    60   Input ~ 0
I2C1_SCL
Text HLabel 5200 4800 0    60   BiDi ~ 0
I2C1_SDA
Wire Wire Line
	2900 1500 2900 2000
Wire Wire Line
	2100 1600 3000 1600
Connection ~ 2900 1600
Wire Wire Line
	3800 2700 3800 1500
Wire Wire Line
	3400 2700 3800 2700
Wire Wire Line
	2900 3200 2900 3000
Wire Wire Line
	1700 1600 1800 1600
Wire Wire Line
	2400 1900 2400 2400
Wire Wire Line
	2200 1900 2200 2000
Wire Wire Line
	2200 2000 2300 2000
Wire Wire Line
	2300 2000 2300 2500
Connection ~ 2400 1600
Connection ~ 2200 1600
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
	2900 4600 2900 4250
Wire Wire Line
	4150 4800 4150 4850
Wire Wire Line
	2900 5900 2900 5800
Wire Wire Line
	2050 5400 2050 5850
Wire Wire Line
	2050 5850 6100 5850
Connection ~ 2900 5850
Connection ~ 2050 5600
Connection ~ 2050 5500
Wire Wire Line
	3400 2500 3600 2500
Wire Wire Line
	3600 2500 3600 1500
Wire Wire Line
	3300 1600 3600 1600
Connection ~ 3600 1600
Wire Wire Line
	3400 2400 3400 1800
Wire Wire Line
	3400 1800 3300 1800
Wire Wire Line
	3000 1800 2900 1800
Connection ~ 2900 1800
Wire Wire Line
	2100 2700 2400 2700
Wire Wire Line
	2400 2400 1400 2400
Wire Wire Line
	1400 2500 2400 2500
Connection ~ 2300 2500
Wire Wire Line
	1700 2700 1400 2700
Wire Wire Line
	5200 4800 5700 4800
Wire Wire Line
	5700 4900 5200 4900
Wire Wire Line
	2050 4900 1400 4900
Wire Wire Line
	1400 5100 2050 5100
Wire Wire Line
	6100 4600 6100 4400
Wire Wire Line
	6100 4400 2900 4400
Connection ~ 2900 4400
Wire Wire Line
	4150 4500 4150 4400
Connection ~ 4150 4400
Wire Wire Line
	6100 5850 6100 5600
Wire Wire Line
	5700 5200 5700 5850
Connection ~ 5700 5300
Connection ~ 5700 5850
Connection ~ 5700 5400
$EndSCHEMATC
