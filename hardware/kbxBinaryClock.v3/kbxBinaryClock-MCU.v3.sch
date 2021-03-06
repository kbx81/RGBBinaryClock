EESchema Schematic File Version 4
LIBS:kbxBinaryClock.v3-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 5
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
L kbxBinaryClock.v3-rescue:+BATT #PWR01
U 1 1 5A040431
P 3150 900
F 0 "#PWR01" H 3150 750 50  0001 C CNN
F 1 "+BATT" H 3150 1040 50  0000 C CNN
F 2 "" H 3150 900 50  0001 C CNN
F 3 "" H 3150 900 50  0001 C CNN
	1    3150 900 
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R19
U 1 1 5A040437
P 10450 6150
F 0 "R19" V 10530 6150 50  0000 C CNN
F 1 "10K" V 10450 6150 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 10380 6150 50  0001 C CNN
F 3 "" H 10450 6150 50  0001 C CNN
	1    10450 6150
	0    1    1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:SW_Push SW1
U 1 1 5A04043E
P 5200 5800
F 0 "SW1" H 5250 5900 50  0000 L CNN
F 1 "SW_Push" H 5200 5740 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_KMR2" H 5200 6000 50  0001 C CNN
F 3 "https://www.ckswitches.com/media/1479/kmr2.pdf" H 5200 6000 50  0001 C CNN
F 4 "KMR221G LFS" H 5200 5800 60  0001 C CNN "Part Number"
	1    5200 5800
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:GND #PWR02
U 1 1 5A04044B
P 2900 1000
F 0 "#PWR02" H 2900 750 50  0001 C CNN
F 1 "GND" H 2900 850 50  0000 C CNN
F 2 "" H 2900 1000 50  0001 C CNN
F 3 "" H 2900 1000 50  0001 C CNN
	1    2900 1000
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R10
U 1 1 5A040468
P 6050 6550
F 0 "R10" V 6130 6550 50  0000 C CNN
F 1 "1K" V 6050 6550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5980 6550 50  0001 C CNN
F 3 "" H 6050 6550 50  0001 C CNN
	1    6050 6550
	0    -1   -1   0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R11
U 1 1 5A04046F
P 6050 6750
F 0 "R11" V 6130 6750 50  0000 C CNN
F 1 "1K" V 6050 6750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5980 6750 50  0001 C CNN
F 3 "" H 6050 6750 50  0001 C CNN
	1    6050 6750
	0    -1   -1   0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R12
U 1 1 5A040476
P 6050 6950
F 0 "R12" V 6130 6950 50  0000 C CNN
F 1 "1K" V 6050 6950 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5980 6950 50  0001 C CNN
F 3 "" H 6050 6950 50  0001 C CNN
	1    6050 6950
	0    -1   -1   0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R13
U 1 1 5A04047D
P 6050 7150
F 0 "R13" V 6130 7150 50  0000 C CNN
F 1 "1K" V 6050 7150 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5980 7150 50  0001 C CNN
F 3 "" H 6050 7150 50  0001 C CNN
	1    6050 7150
	0    -1   -1   0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R14
U 1 1 5A040484
P 6050 7350
F 0 "R14" V 6130 7350 50  0000 C CNN
F 1 "1K" V 6050 7350 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5980 7350 50  0001 C CNN
F 3 "" H 6050 7350 50  0001 C CNN
	1    6050 7350
	0    -1   -1   0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R15
U 1 1 5A04048B
P 6050 7550
F 0 "R15" V 6130 7550 50  0000 C CNN
F 1 "1K" V 6050 7550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5980 7550 50  0001 C CNN
F 3 "" H 6050 7550 50  0001 C CNN
	1    6050 7550
	0    -1   -1   0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C3
U 1 1 5A040492
P 1550 1050
F 0 "C3" H 1575 1150 50  0000 L CNN
F 1 "10 pF" H 1575 950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1588 900 50  0001 C CNN
F 3 "" H 1550 1050 50  0001 C CNN
	1    1550 1050
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C4
U 1 1 5A040499
P 1950 1050
F 0 "C4" H 1975 1150 50  0000 L CNN
F 1 "10 pF" H 1975 950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1988 900 50  0001 C CNN
F 3 "" H 1950 1050 50  0001 C CNN
	1    1950 1050
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C5
U 1 1 5A0404A0
P 2400 1050
F 0 "C5" H 2425 1150 50  0000 L CNN
F 1 "100 nF" H 2425 950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 2438 900 50  0001 C CNN
F 3 "" H 2400 1050 50  0001 C CNN
	1    2400 1050
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:Crystal Y2
U 1 1 5A0404A7
P 1750 1450
F 0 "Y2" H 1750 1600 50  0000 C CNN
F 1 "8 MHz" H 1750 1300 50  0000 C CNN
F 2 "Crystals:Crystal_SMD_5032-4pin_5.0x3.2mm" H 1750 1450 50  0001 C CNN
F 3 "http://www.abracon.com/Resonators/abm3b.pdf" H 1750 1450 50  0001 C CNN
F 4 "ABM3B-8.000MHZ-10-1UT" H 1750 1450 60  0001 C CNN "Part Number"
	1    1750 1450
	-1   0    0    1   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C6
U 1 1 5A0404AE
P 5150 6300
F 0 "C6" H 5175 6400 50  0000 L CNN
F 1 "100 nF" H 5175 6200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5188 6150 50  0001 C CNN
F 3 "" H 5150 6300 50  0001 C CNN
	1    5150 6300
	0    1    -1   0   
$EndComp
Text HLabel 1500 3400 0    60   Input ~ 0
OSC32_IN
Text GLabel 10300 5200 2    60   Output ~ 0
LED0
Text GLabel 1500 4700 0    60   Output ~ 0
LED1
$Comp
L kbxBinaryClock.v3-rescue:LED_Dual_AACC LED26
U 1 1 5A0A13B6
P 2500 6200
F 0 "LED26" H 2500 6425 50  0000 C CNN
F 1 "LED_Dual_AACC" H 2500 5950 50  0000 C CNN
F 2 "LEDs:LED_Cree-PLCC4_3.2x2.8mm_CCW" H 2530 6200 50  0001 C CNN
F 3 "https://www.vcclite.com/product/led-smt-1206-1-72-1v-20ma-redgreen-2k/" H 2530 6200 50  0001 C CNN
F 4 "CMD1522SRUGCTR8" H 2500 6200 60  0001 C CNN "Part Number"
	1    2500 6200
	0    1    1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:2N7002 Q3
U 1 1 5A0A219A
P 3200 6800
F 0 "Q3" V 3250 6550 50  0000 L CNN
F 1 "2N7002" V 3150 6400 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3400 6725 50  0001 L CIN
F 3 "https://www.diodes.com/assets/Datasheets/ds11303.pdf" H 3200 6800 50  0001 L CNN
	1    3200 6800
	0    -1   1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:2N7002 Q2
U 1 1 5A0A2339
P 3100 6300
F 0 "Q2" V 3150 6050 50  0000 L CNN
F 1 "2N7002" V 3050 5900 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3300 6225 50  0001 L CIN
F 3 "https://www.diodes.com/assets/Datasheets/ds11303.pdf" H 3100 6300 50  0001 L CNN
	1    3100 6300
	0    -1   1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R3
U 1 1 5A0A2479
P 2650 7200
F 0 "R3" V 2730 7200 50  0000 C CNN
F 1 "1K" V 2650 7200 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2580 7200 50  0001 C CNN
F 3 "" H 2650 7200 50  0001 C CNN
	1    2650 7200
	0    1    1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R2
U 1 1 5A0A254E
P 2650 7000
F 0 "R2" V 2730 7000 50  0000 C CNN
F 1 "1K" V 2650 7000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2580 7000 50  0001 C CNN
F 3 "" H 2650 7000 50  0001 C CNN
	1    2650 7000
	0    -1   1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:GND #PWR03
U 1 1 5A0A29B0
P 3550 7500
F 0 "#PWR03" H 3550 7250 50  0001 C CNN
F 1 "GND" H 3550 7350 50  0000 C CNN
F 2 "" H 3550 7500 50  0001 C CNN
F 3 "" H 3550 7500 50  0001 C CNN
	1    3550 7500
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R5
U 1 1 5A0A2D6C
P 3850 6050
F 0 "R5" V 3930 6050 50  0000 C CNN
F 1 "1M" V 3850 6050 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3780 6050 50  0001 C CNN
F 3 "" H 3850 6050 50  0001 C CNN
	1    3850 6050
	0    1    1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R6
U 1 1 5A0A2E27
P 3850 6550
F 0 "R6" V 3930 6550 50  0000 C CNN
F 1 "1M" V 3850 6550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3780 6550 50  0001 C CNN
F 3 "" H 3850 6550 50  0001 C CNN
	1    3850 6550
	0    1    1    0   
$EndComp
Text GLabel 3300 5950 1    60   Input ~ 0
LED0
Text GLabel 3450 5950 1    60   Input ~ 0
LED1
$Comp
L kbxBinaryClock.v3-rescue:+3.3V #PWR04
U 1 1 5A0A407B
P 4150 5850
F 0 "#PWR04" H 4150 5700 50  0001 C CNN
F 1 "+3.3V" H 4150 5990 50  0000 C CNN
F 2 "" H 4150 5850 50  0001 C CNN
F 3 "" H 4150 5850 50  0001 C CNN
	1    4150 5850
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:Q_Photo_NPN Q5
U 1 1 5A0AF088
P 5300 1000
F 0 "Q5" V 5250 800 50  0000 L CNN
F 1 "Q_Photo_NPN" V 5500 750 50  0000 L CNN
F 2 "LEDs:LED_D5.0mm" H 5500 1100 50  0001 C CNN
F 3 "https://www.vishay.com/docs/84768/tept5600.pdf" H 5300 1000 50  0001 C CNN
F 4 "TEPT5600" V 5300 1000 60  0001 C CNN "Part Number"
	1    5300 1000
	0    1    1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:GND #PWR05
U 1 1 5A0AF900
P 4350 1450
F 0 "#PWR05" H 4350 1200 50  0001 C CNN
F 1 "GND" H 4350 1300 50  0000 C CNN
F 2 "" H 4350 1450 50  0001 C CNN
F 3 "" H 4350 1450 50  0001 C CNN
	1    4350 1450
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:+3.3V #PWR06
U 1 1 5A0AFA0C
P 6000 1100
F 0 "#PWR06" H 6000 950 50  0001 C CNN
F 1 "+3.3V" H 6000 1240 50  0000 C CNN
F 2 "" H 6000 1100 50  0001 C CNN
F 3 "" H 6000 1100 50  0001 C CNN
	1    6000 1100
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R9
U 1 1 5A0AFAB0
P 4750 1400
F 0 "R9" V 4830 1400 50  0000 C CNN
F 1 "1M" V 4750 1400 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4680 1400 50  0001 C CNN
F 3 "" H 4750 1400 50  0001 C CNN
	1    4750 1400
	0    1    1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C8
U 1 1 5A0AFF80
P 6600 1250
F 0 "C8" H 6625 1350 50  0000 L CNN
F 1 "100 nF" H 6625 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6638 1100 50  0001 C CNN
F 3 "" H 6600 1250 50  0001 C CNN
	1    6600 1250
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C9
U 1 1 5A0B0029
P 6900 1250
F 0 "C9" H 6925 1350 50  0000 L CNN
F 1 "100 nF" H 6925 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6938 1100 50  0001 C CNN
F 3 "" H 6900 1250 50  0001 C CNN
	1    6900 1250
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C10
U 1 1 5A0B00AE
P 7200 1250
F 0 "C10" H 7225 1350 50  0000 L CNN
F 1 "100 nF" H 7225 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7238 1100 50  0001 C CNN
F 3 "" H 7200 1250 50  0001 C CNN
	1    7200 1250
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C11
U 1 1 5A0B0136
P 7500 1250
F 0 "C11" H 7525 1350 50  0000 L CNN
F 1 "1 µF" H 7525 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7538 1100 50  0001 C CNN
F 3 "" H 7500 1250 50  0001 C CNN
	1    7500 1250
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:GND #PWR07
U 1 1 5A0B0638
P 9200 1500
F 0 "#PWR07" H 9200 1250 50  0001 C CNN
F 1 "GND" H 9200 1350 50  0000 C CNN
F 2 "" H 9200 1500 50  0001 C CNN
F 3 "" H 9200 1500 50  0001 C CNN
	1    9200 1500
	1    0    0    -1  
$EndComp
Text GLabel 10300 4500 2    60   Output ~ 0
BEEP
$Comp
L kbxBinaryClock.v3-rescue:Buzzer BZ1
U 1 1 5A0B0FEF
P 1150 6000
F 0 "BZ1" H 1300 6050 50  0000 L CNN
F 1 "Buzzer" H 1300 5950 50  0000 L CNN
F 2 "Buzzers_Beepers:Buzzer_12x9.5RM7.6" V 1125 6100 50  0001 C CNN
F 3 "https://product.tdk.com/en/search/sw_piezo/sw_piezo/em-buzzer/info?part_no=SD1614TT-B1" V 1125 6100 50  0001 C CNN
F 4 "TDK SD1614TT-B1" H 1150 6000 60  0001 C CNN "Part Number"
	1    1150 6000
	-1   0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:2N7002 Q1
U 1 1 5A0B1585
P 1150 6900
F 0 "Q1" H 1350 6975 50  0000 L CNN
F 1 "2N7002" H 1350 6900 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 1350 6825 50  0001 L CIN
F 3 "https://www.diodes.com/assets/Datasheets/ds11303.pdf" H 1150 6900 50  0001 L CNN
	1    1150 6900
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:GND #PWR08
U 1 1 5A0B195D
P 1250 7500
F 0 "#PWR08" H 1250 7250 50  0001 C CNN
F 1 "GND" H 1250 7350 50  0000 C CNN
F 2 "" H 1250 7500 50  0001 C CNN
F 3 "" H 1250 7500 50  0001 C CNN
	1    1250 7500
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R1
U 1 1 5A0B1AC1
P 950 7150
F 0 "R1" V 1030 7150 50  0000 C CNN
F 1 "1M" V 950 7150 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 880 7150 50  0001 C CNN
F 3 "" H 950 7150 50  0001 C CNN
	1    950  7150
	1    0    0    -1  
$EndComp
Text GLabel 850  6900 0    60   Input ~ 0
BEEP
Text HLabel 10300 6300 2    60   Input ~ 0
RESET
$Comp
L kbxBinaryClock.v3-rescue:STM32_SWD_USART P1
U 1 1 5A0B7284
P 9350 6100
F 0 "P1" H 9350 6400 50  0000 C CNN
F 1 "STM32_SWD_USART" H 9350 5800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x05_Pitch1.27mm_SMD" H 9350 4900 60  0001 C CNN
F 3 "https://www.harwin.com/products/M50-3600542R/" H 9350 4900 60  0000 C CNN
F 4 "M50-3600542R" H 9350 6100 60  0001 C CNN "Part Number"
	1    9350 6100
	1    0    0    -1  
$EndComp
Text GLabel 2000 2600 0    60   Input ~ 0
BOOT0
Text GLabel 8650 6400 0    60   Output ~ 0
BOOT0
$Comp
L kbxBinaryClock.v3-rescue:GND #PWR09
U 1 1 5A0BA87B
P 5700 6100
F 0 "#PWR09" H 5700 5850 50  0001 C CNN
F 1 "GND" H 5700 5950 50  0000 C CNN
F 2 "" H 5700 6100 50  0001 C CNN
F 3 "" H 5700 6100 50  0001 C CNN
	1    5700 6100
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:+3.3V #PWR010
U 1 1 5A0BA913
P 10600 5600
F 0 "#PWR010" H 10600 5450 50  0001 C CNN
F 1 "+3.3V" H 10600 5740 50  0000 C CNN
F 2 "" H 10600 5600 50  0001 C CNN
F 3 "" H 10600 5600 50  0001 C CNN
	1    10600 5600
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C12
U 1 1 5A0BB5DF
P 7800 1250
F 0 "C12" H 7825 1350 50  0000 L CNN
F 1 "4.7 µF" H 7825 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7838 1100 50  0001 C CNN
F 3 "" H 7800 1250 50  0001 C CNN
	1    7800 1250
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C7
U 1 1 5A0BB63E
P 6300 1250
F 0 "C7" H 6325 1350 50  0000 L CNN
F 1 "10 nF" H 6325 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6338 1100 50  0001 C CNN
F 3 "" H 6300 1250 50  0001 C CNN
	1    6300 1250
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C13
U 1 1 5A0BB69B
P 8100 1250
F 0 "C13" H 8125 1350 50  0000 L CNN
F 1 "4.7 µF" H 8125 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 8138 1100 50  0001 C CNN
F 3 "" H 8100 1250 50  0001 C CNN
	1    8100 1250
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R16
U 1 1 5A0BCD9F
P 8550 6200
F 0 "R16" V 8630 6200 50  0000 C CNN
F 1 "10K" V 8550 6200 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8480 6200 50  0001 C CNN
F 3 "" H 8550 6200 50  0001 C CNN
	1    8550 6200
	0    1    1    0   
$EndComp
Text HLabel 10300 4100 2    60   Output ~ 0
SPI1_NSS_DISP
Text HLabel 10300 4200 2    60   Output ~ 0
SPI1_SCK
Text HLabel 10300 4300 2    60   Input ~ 0
SPI1_MISO
Text HLabel 10300 4400 2    60   Output ~ 0
SPI1_MOSI
Text HLabel 10300 4600 2    60   Output ~ 0
USART1_TX
Text HLabel 10300 4700 2    60   Input ~ 0
USART1_RX
Text HLabel 1500 5200 0    60   Output ~ 0
BLANK
Text HLabel 1500 4500 0    60   Output ~ 0
CURRENT_LOW
Text HLabel 1500 4600 0    60   Output ~ 0
CURRENT_HIGH
Text HLabel 10300 3900 2    60   Output ~ 0
USART2_TX
Text HLabel 10300 4000 2    60   Input ~ 0
USART2_RX
Text HLabel 10300 3800 2    60   Output ~ 0
USART2_MODE
$Comp
L kbxBinaryClock.v3-rescue:Conn_01x01 TP1
U 1 1 5A0FE878
P 6550 6550
F 0 "TP1" H 6550 6650 50  0000 C CNN
F 1 "TouchPad1" H 6550 6450 50  0000 C CNN
F 2 "TouchPads:TouchPad1" H 6550 6550 50  0001 C CNN
F 3 "" H 6550 6550 50  0001 C CNN
	1    6550 6550
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:Conn_01x01 TP2
U 1 1 5A0FEA07
P 6550 6750
F 0 "TP2" H 6550 6850 50  0000 C CNN
F 1 "TouchPad2" H 6550 6650 50  0000 C CNN
F 2 "TouchPads:TouchPad1" H 6550 6750 50  0001 C CNN
F 3 "" H 6550 6750 50  0001 C CNN
	1    6550 6750
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:Conn_01x01 TP3
U 1 1 5A0FEABC
P 6550 6950
F 0 "TP3" H 6550 7050 50  0000 C CNN
F 1 "TouchPad3" H 6550 6850 50  0000 C CNN
F 2 "TouchPads:TouchPad1" H 6550 6950 50  0001 C CNN
F 3 "" H 6550 6950 50  0001 C CNN
	1    6550 6950
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:Conn_01x01 TP4
U 1 1 5A0FEB72
P 6550 7150
F 0 "TP4" H 6550 7250 50  0000 C CNN
F 1 "TouchPad4" H 6550 7050 50  0000 C CNN
F 2 "TouchPads:TouchPad1" H 6550 7150 50  0001 C CNN
F 3 "" H 6550 7150 50  0001 C CNN
	1    6550 7150
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:Conn_01x01 TP5
U 1 1 5A0FEC2B
P 6550 7350
F 0 "TP5" H 6550 7450 50  0000 C CNN
F 1 "TouchPad5" H 6550 7250 50  0000 C CNN
F 2 "TouchPads:TouchPad1" H 6550 7350 50  0001 C CNN
F 3 "" H 6550 7350 50  0001 C CNN
	1    6550 7350
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:Conn_01x01 TP6
U 1 1 5A0FECEA
P 6550 7550
F 0 "TP6" H 6550 7650 50  0000 C CNN
F 1 "TouchPad6" H 6550 7450 50  0000 C CNN
F 2 "TouchPads:TouchPad1" H 6550 7550 50  0001 C CNN
F 3 "" H 6550 7550 50  0001 C CNN
	1    6550 7550
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R17
U 1 1 5A13A208
P 10450 5850
F 0 "R17" V 10530 5850 50  0000 C CNN
F 1 "10K" V 10450 5850 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 10380 5850 50  0001 C CNN
F 3 "" H 10450 5850 50  0001 C CNN
	1    10450 5850
	0    1    1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R18
U 1 1 5A13A2DD
P 10450 6000
F 0 "R18" V 10530 6000 50  0000 C CNN
F 1 "10K" V 10450 6000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 10380 6000 50  0001 C CNN
F 3 "" H 10450 6000 50  0001 C CNN
	1    10450 6000
	0    1    1    0   
$EndComp
Text GLabel 1500 3900 0    60   BiDi ~ 0
G3_CS
Text GLabel 1500 3700 0    60   BiDi ~ 0
G3_K2
Text GLabel 1500 4400 0    60   BiDi ~ 0
G5_K4
Text GLabel 1500 4000 0    60   BiDi ~ 0
G5_CS
Text GLabel 1500 5100 0    60   BiDi ~ 0
G6_CS
Text GLabel 1500 4900 0    60   BiDi ~ 0
G6_K2
Text GLabel 10300 3700 2    60   Input ~ 0
ADC_IN
Text GLabel 5000 900  1    60   Output ~ 0
ADC_IN
Text GLabel 5800 6550 0    60   BiDi ~ 0
G3_K2
Text GLabel 5800 6750 0    60   BiDi ~ 0
G3_K3
Text GLabel 5800 7150 0    60   BiDi ~ 0
G5_K4
Text GLabel 5800 6950 0    60   BiDi ~ 0
G5_K3
Text GLabel 5800 7350 0    60   BiDi ~ 0
G6_K2
Text GLabel 5800 7550 0    60   BiDi ~ 0
G6_K3
$Comp
L kbxBinaryClock.v3-rescue:C C14
U 1 1 5A1EB557
P 8600 1250
F 0 "C14" H 8625 1350 50  0000 L CNN
F 1 "47 nF" H 8625 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 8638 1100 50  0001 C CNN
F 3 "" H 8600 1250 50  0001 C CNN
	1    8600 1250
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C15
U 1 1 5A1EB643
P 8900 1250
F 0 "C15" H 8925 1350 50  0000 L CNN
F 1 "47 nF" H 8925 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 8938 1100 50  0001 C CNN
F 3 "" H 8900 1250 50  0001 C CNN
	1    8900 1250
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C16
U 1 1 5A1EB6B8
P 9200 1250
F 0 "C16" H 9225 1350 50  0000 L CNN
F 1 "47 nF" H 9225 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 9238 1100 50  0001 C CNN
F 3 "" H 9200 1250 50  0001 C CNN
	1    9200 1250
	1    0    0    -1  
$EndComp
Text GLabel 8600 1000 1    60   BiDi ~ 0
G3_CS
Text GLabel 8900 1000 1    60   BiDi ~ 0
G5_CS
Text GLabel 9200 1000 1    60   BiDi ~ 0
G6_CS
$Comp
L kbxBinaryClock.v3-rescue:Crystal Y1
U 1 1 5A30F44E
P 900 1450
F 0 "Y1" H 900 1600 50  0000 C CNN
F 1 "32.768 KHz" H 900 1300 50  0000 C CNN
F 2 "Crystals:Crystal_SMD_3215-2pin_3.2x1.5mm" H 900 1450 50  0001 C CNN
F 3 "https://www.ecsxtal.com/store/pdf/ECX-.327-CDX-1293.pdf" H 900 1450 50  0001 C CNN
F 4 "ECX-.327-CDX-1293" H 900 1450 60  0001 C CNN "Part Number"
	1    900  1450
	-1   0    0    1   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C1
U 1 1 5A30F87D
P 700 1050
F 0 "C1" H 725 1150 50  0000 L CNN
F 1 "12.5 pF" H 725 950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 738 900 50  0001 C CNN
F 3 "" H 700 1050 50  0001 C CNN
	1    700  1050
	1    0    0    -1  
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:C C2
U 1 1 5A30F96C
P 1100 1050
F 0 "C2" H 1125 1150 50  0000 L CNN
F 1 "12.5 pF" H 1125 950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1138 900 50  0001 C CNN
F 3 "" H 1100 1050 50  0001 C CNN
	1    1100 1050
	1    0    0    -1  
$EndComp
Text GLabel 1500 3800 0    60   BiDi ~ 0
G3_K3
Text GLabel 1500 4300 0    60   BiDi ~ 0
G5_K3
Text GLabel 1500 5000 0    60   BiDi ~ 0
G6_K3
NoConn ~ 2100 3300
NoConn ~ 2100 4200
$Comp
L kbxBinaryClock.v3-rescue:POT RV1
U 1 1 5A6192F6
P 4350 1100
F 0 "RV1" V 4175 1100 50  0000 C CNN
F 1 "100K" V 4250 1100 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Trimmer_Vishay_TS53YJ" H 4350 1100 50  0001 C CNN
F 3 "http://www.bourns.com/docs/Product-Datasheets/TC33.PDF" H 4350 1100 50  0001 C CNN
F 4 "TC33X-2-104E" V 4350 1100 60  0001 C CNN "Part Number"
	1    4350 1100
	0    1    1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R8
U 1 1 5A6AA7C9
P 4750 1100
F 0 "R8" V 4830 1100 50  0000 C CNN
F 1 "10K" V 4750 1100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4680 1100 50  0001 C CNN
F 3 "" H 4750 1100 50  0001 C CNN
	1    4750 1100
	0    1    1    0   
$EndComp
Text GLabel 1500 4800 0    60   Output ~ 0
LED2
$Comp
L kbxBinaryClock.v3-rescue:R R7
U 1 1 5A6D43FC
P 3850 7050
F 0 "R7" V 3930 7050 50  0000 C CNN
F 1 "1M" V 3850 7050 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3780 7050 50  0001 C CNN
F 3 "" H 3850 7050 50  0001 C CNN
	1    3850 7050
	0    1    1    0   
$EndComp
Text GLabel 3600 5950 1    60   Input ~ 0
LED2
$Comp
L kbxBinaryClock.v3-rescue:2N7002 Q4
U 1 1 5A6D6D77
P 3300 7300
F 0 "Q4" V 3350 7050 50  0000 L CNN
F 1 "2N7002" V 3250 6900 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3500 7225 50  0001 L CIN
F 3 "https://www.diodes.com/assets/Datasheets/ds11303.pdf" H 3300 7300 50  0001 L CNN
	1    3300 7300
	0    -1   1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:R R4
U 1 1 5A6D8302
P 2650 7400
F 0 "R4" V 2730 7400 50  0000 C CNN
F 1 "1K" V 2650 7400 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2580 7400 50  0001 C CNN
F 3 "" H 2650 7400 50  0001 C CNN
	1    2650 7400
	0    1    1    0   
$EndComp
$Comp
L kbxBinaryClock.v3-rescue:LED_RGB_COMMON_ANODE LED25
U 1 1 5A6D9EFC
P 1850 6200
F 0 "LED25" H 1850 6475 50  0000 C CNN
F 1 "LED_RGB_COMMON_ANODE" H 1850 5925 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm-4" H 1875 6350 60  0001 C CNN
F 3 "https://www.adafruit.com/product/302" H 1875 6350 60  0001 C CNN
	1    1850 6200
	0    1    1    0   
$EndComp
Connection ~ 4150 6050
Wire Wire Line
	4150 5850 4150 6050
Wire Wire Line
	4700 5800 4900 5800
Wire Wire Line
	5500 6000 5700 6000
Wire Wire Line
	1500 4000 2100 4000
Connection ~ 4350 1400
Wire Wire Line
	4200 1400 4350 1400
Wire Wire Line
	4350 1250 4350 1400
Wire Wire Line
	4500 1100 4600 1100
Wire Wire Line
	5000 1400 4900 1400
Wire Wire Line
	9200 1400 9200 1500
Wire Wire Line
	6300 1400 6600 1400
Connection ~ 5500 6000
Wire Wire Line
	5500 5800 5400 5800
Wire Wire Line
	4900 6300 5000 6300
Connection ~ 4900 5800
Wire Wire Line
	2100 1300 2400 1300
Wire Wire Line
	3150 1300 3150 900 
Connection ~ 2400 1300
Wire Wire Line
	2400 1300 2400 1200
Wire Wire Line
	2100 1800 2100 1300
Wire Wire Line
	1400 1800 2100 1800
Wire Wire Line
	1400 2000 1400 1800
Wire Wire Line
	1100 2000 1400 2000
Wire Wire Line
	1100 2800 1100 2000
Wire Wire Line
	2100 2800 1100 2800
Connection ~ 2400 900 
Wire Wire Line
	2900 900  2900 1000
Connection ~ 1950 900 
Connection ~ 1550 900 
Connection ~ 1100 900 
Wire Wire Line
	700  900  1100 900 
Wire Wire Line
	1300 1900 1300 1700
Wire Wire Line
	1000 1900 1300 1900
Wire Wire Line
	1000 3000 1000 1900
Wire Wire Line
	2100 3000 1000 3000
Wire Wire Line
	1200 1800 1200 1450
Wire Wire Line
	900  1800 1200 1800
Wire Wire Line
	900  3100 900  1800
Wire Wire Line
	2100 3100 900  3100
Wire Wire Line
	800  1700 1100 1700
Wire Wire Line
	2100 2400 2000 2400
Wire Wire Line
	2000 2600 2100 2600
Wire Wire Line
	1950 1200 1950 1450
Wire Wire Line
	1950 1700 1300 1700
Connection ~ 1550 1450
Wire Wire Line
	1200 1450 1550 1450
Connection ~ 1950 1450
Wire Wire Line
	1900 1450 1950 1450
Wire Wire Line
	1550 1450 1550 1200
Connection ~ 1100 1450
Wire Wire Line
	1050 1450 1100 1450
Connection ~ 700  1450
Wire Wire Line
	750  1450 700  1450
Wire Wire Line
	1100 1700 1100 1450
Wire Wire Line
	700  1200 700  1450
Wire Wire Line
	700  3500 2100 3500
Wire Wire Line
	5500 5800 5500 6000
Wire Wire Line
	5500 6300 5300 6300
Wire Wire Line
	4900 5800 4900 6300
Connection ~ 5000 1100
Wire Wire Line
	5000 900  5000 1100
Wire Wire Line
	4900 1100 5000 1100
Wire Wire Line
	5500 1100 5700 1100
Wire Wire Line
	9750 6300 10200 6300
Connection ~ 5700 6000
Connection ~ 8800 6200
Wire Wire Line
	8800 6400 8800 6200
Wire Wire Line
	8650 6400 8800 6400
Connection ~ 8300 6000
Wire Wire Line
	8300 6200 8300 6000
Wire Wire Line
	8400 6200 8300 6200
Wire Wire Line
	8700 6200 8800 6200
Wire Wire Line
	6200 6550 6350 6550
Wire Wire Line
	6200 6750 6350 6750
Wire Wire Line
	6200 6950 6350 6950
Wire Wire Line
	6200 7150 6350 7150
Wire Wire Line
	6200 7350 6350 7350
Wire Wire Line
	6200 7550 6350 7550
Connection ~ 6900 1100
Connection ~ 7200 1100
Connection ~ 6900 1400
Connection ~ 7200 1400
Wire Wire Line
	1250 6100 1250 6450
Wire Wire Line
	1250 7100 1250 7300
Wire Wire Line
	850  6900 950  6900
Connection ~ 6600 1100
Connection ~ 6600 1400
Connection ~ 7500 1100
Connection ~ 7800 1100
Connection ~ 7500 1400
Connection ~ 7800 1400
Wire Wire Line
	950  6900 950  7000
Wire Wire Line
	950  7300 1250 7300
Connection ~ 1250 7300
Connection ~ 6300 1100
Connection ~ 5700 1100
Wire Wire Line
	6000 1100 6000 1900
Connection ~ 6000 1100
Wire Wire Line
	5900 1100 5900 1900
Connection ~ 5900 1100
Wire Wire Line
	5800 1100 5800 1900
Connection ~ 5800 1100
Wire Wire Line
	5700 1900 5700 1100
Connection ~ 8100 1400
Connection ~ 8600 1400
Connection ~ 8900 1400
Connection ~ 9200 1400
Wire Wire Line
	8600 1100 8600 1000
Wire Wire Line
	8900 1100 8900 1000
Wire Wire Line
	9200 1000 9200 1100
Wire Wire Line
	5800 6550 5900 6550
Wire Wire Line
	5900 6750 5800 6750
Wire Wire Line
	5800 6950 5900 6950
Wire Wire Line
	5900 7150 5800 7150
Wire Wire Line
	5800 7350 5900 7350
Wire Wire Line
	5900 7550 5800 7550
Wire Wire Line
	1500 3700 2100 3700
Wire Wire Line
	2100 3800 1500 3800
Wire Wire Line
	1500 3900 2100 3900
Wire Wire Line
	1500 4100 2100 4100
Wire Wire Line
	1500 4300 2100 4300
Wire Wire Line
	2100 4400 1500 4400
Wire Wire Line
	1500 4700 2100 4700
Wire Wire Line
	2100 4800 1500 4800
Wire Wire Line
	1500 4900 2100 4900
Wire Wire Line
	2100 5000 1500 5000
Wire Wire Line
	1500 5100 2100 5100
Wire Wire Line
	9700 3700 10300 3700
Wire Wire Line
	10300 3800 9700 3800
Wire Wire Line
	9700 3900 10300 3900
Wire Wire Line
	10300 4000 9700 4000
Wire Wire Line
	9700 4100 10300 4100
Wire Wire Line
	10300 4200 9700 4200
Wire Wire Line
	9700 4300 10300 4300
Wire Wire Line
	10300 4400 9700 4400
Wire Wire Line
	9700 4500 10300 4500
Wire Wire Line
	9700 4600 10000 4600
Wire Wire Line
	9700 4700 10100 4700
Wire Wire Line
	10300 4800 9700 4800
Wire Wire Line
	9700 4900 10300 4900
Wire Wire Line
	10300 5200 9700 5200
Wire Wire Line
	9800 5900 9750 5900
Wire Wire Line
	9800 5000 9800 5850
Wire Wire Line
	9750 6000 9900 6000
Wire Wire Line
	9900 5100 9900 6000
Wire Wire Line
	10000 6100 9750 6100
Wire Wire Line
	10000 4600 10000 6100
Connection ~ 10000 4600
Wire Wire Line
	10100 6200 9750 6200
Wire Wire Line
	10100 4700 10100 6200
Connection ~ 10100 4700
Wire Wire Line
	10600 5600 10600 5700
Connection ~ 10600 5850
Connection ~ 9900 6000
Wire Wire Line
	10300 5850 9800 5850
Connection ~ 9800 5850
Wire Wire Line
	8950 5900 8950 5700
Wire Wire Line
	8950 5700 10600 5700
Connection ~ 10600 5700
Wire Wire Line
	8950 6000 8950 6100
Wire Wire Line
	6000 6000 6000 5600
Wire Wire Line
	5900 6000 5900 5600
Connection ~ 6000 6000
Wire Wire Line
	5800 6000 5800 5600
Connection ~ 5900 6000
Wire Wire Line
	5700 5600 5700 6000
Connection ~ 5800 6000
Wire Wire Line
	3700 6050 3300 6050
Wire Wire Line
	3100 6050 3100 6100
Wire Wire Line
	3700 6550 3450 6550
Wire Wire Line
	3200 6550 3200 6600
Wire Wire Line
	3700 7050 3600 7050
Wire Wire Line
	3300 7050 3300 7100
Wire Wire Line
	4150 7050 4000 7050
Wire Wire Line
	4000 6550 4150 6550
Connection ~ 4150 6550
Wire Wire Line
	3300 5950 3300 6050
Connection ~ 3300 6050
Wire Wire Line
	3450 5950 3450 6550
Connection ~ 3450 6550
Wire Wire Line
	3600 5950 3600 7050
Connection ~ 3600 7050
Wire Wire Line
	3300 6400 3550 6400
Wire Wire Line
	3550 6400 3550 6900
Wire Wire Line
	3500 7400 3550 7400
Connection ~ 3550 7400
Wire Wire Line
	3400 6900 3550 6900
Connection ~ 3550 6900
Wire Wire Line
	2800 7000 2850 7000
Wire Wire Line
	2850 7000 2850 6400
Wire Wire Line
	2850 6400 2900 6400
Wire Wire Line
	2800 7200 2900 7200
Wire Wire Line
	2900 7200 2900 6900
Wire Wire Line
	2900 6900 3000 6900
Wire Wire Line
	2800 7400 3100 7400
Wire Wire Line
	1250 5900 1400 5900
Wire Wire Line
	1850 5800 1850 5900
Connection ~ 1850 5900
Connection ~ 2400 5900
Wire Wire Line
	2600 6500 2600 6900
Wire Wire Line
	2600 6900 2450 6900
Wire Wire Line
	2450 6900 2450 7000
Wire Wire Line
	2000 7000 2450 7000
Wire Wire Line
	2500 7400 1700 7400
Wire Wire Line
	1700 7400 1700 6450
Wire Wire Line
	2000 6450 2000 7000
Connection ~ 2450 7000
Wire Wire Line
	2500 7200 2400 7200
Wire Wire Line
	1850 7200 1850 6450
Wire Wire Line
	2400 6500 2400 7200
Connection ~ 2400 7200
$Comp
L kbxBinaryClock.v3-rescue:GS3 SB1
U 1 1 5A7D98D2
P 1750 3300
F 0 "SB1" H 1800 3500 50  0000 C CNN
F 1 "GS3" H 1800 3101 50  0000 C CNN
F 2 "Connectors:GS3" V 1838 3226 50  0001 C CNN
F 3 "" H 1750 3300 50  0001 C CNN
	1    1750 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 3300 2000 3300
Wire Wire Line
	2000 3300 2000 3400
Wire Wire Line
	2000 3400 2100 3400
Wire Wire Line
	1600 3400 1500 3400
Wire Wire Line
	1600 3200 800  3200
Wire Wire Line
	800  3200 800  1700
Text HLabel 2000 2400 0    60   Input ~ 0
RESET
Wire Wire Line
	9800 5000 9700 5000
Wire Wire Line
	9700 5100 9900 5100
Text HLabel 4700 5800 0    60   Input ~ 0
RESET
Connection ~ 10600 6000
Wire Wire Line
	10300 6150 10200 6150
Wire Wire Line
	10200 6150 10200 6300
Connection ~ 10200 6300
Wire Wire Line
	4000 6050 4150 6050
Text HLabel 1500 4100 0    60   Input ~ 0
SPI1_MISO_DISP
Wire Wire Line
	2100 4500 1500 4500
Wire Wire Line
	2100 4600 1500 4600
$Comp
L kbxBinaryClock.v3-rescue:STM32F072CBTx U1
U 1 1 5A1E3553
P 5900 3800
F 0 "U1" H 2200 5625 50  0000 L BNN
F 1 "STM32F072CBTx" H 9600 5625 50  0000 R BNN
F 2 "Housings_QFP:LQFP-48_7x7mm_Pitch0.5mm" H 9600 5575 50  0001 R TNN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/cd/46/43/83/22/d3/40/c8/DM00090510.pdf/files/DM00090510.pdf/jcr:content/translations/en.DM00090510.pdf" H 5900 3800 50  0001 C CNN
	1    5900 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 5200 1500 5200
Text HLabel 10300 4800 2    60   Output ~ 0
SPI1_NSS_RTC
Text HLabel 10300 4900 2    60   Output ~ 0
SPI1_NSS_TEMP
$Comp
L kbxBinaryClock.v3-rescue:+V_IN #PWR011
U 1 1 5A7F3DA1
P 1850 5800
F 0 "#PWR011" H 1850 5650 50  0001 C CNN
F 1 "+V_IN" H 1850 5940 50  0000 C CNN
F 2 "" H 1850 5800 50  0001 C CNN
F 3 "" H 1850 5800 50  0001 C CNN
	1    1850 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 1100 4200 1400
$Comp
L kbxBinaryClock.v3-rescue:D D1
U 1 1 5A8915C5
P 1400 6150
F 0 "D1" H 1400 6250 50  0000 C CNN
F 1 "1N4148" H 1400 6050 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 1400 6150 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds30086.pdf" H 1400 6150 50  0001 C CNN
	1    1400 6150
	0    1    1    0   
$EndComp
Wire Wire Line
	1400 6000 1400 5900
Connection ~ 1400 5900
Wire Wire Line
	1400 6300 1400 6450
Wire Wire Line
	1400 6450 1250 6450
Connection ~ 1250 6450
Text Notes 2000 5800 0    60   ~ 0
Install LED25 *OR* LED26\nDO NOT INSTALL BOTH
Wire Wire Line
	4150 6050 4150 6550
Wire Wire Line
	4350 1400 4600 1400
Wire Wire Line
	4350 1400 4350 1450
Wire Wire Line
	5500 6000 5500 6300
Wire Wire Line
	4900 5800 5000 5800
Wire Wire Line
	2400 1300 3150 1300
Wire Wire Line
	2400 900  2900 900 
Wire Wire Line
	1950 900  2400 900 
Wire Wire Line
	1550 900  1950 900 
Wire Wire Line
	1100 900  1550 900 
Wire Wire Line
	1550 1450 1600 1450
Wire Wire Line
	1950 1450 1950 1700
Wire Wire Line
	1100 1450 1100 1200
Wire Wire Line
	700  1450 700  3500
Wire Wire Line
	5000 1100 5000 1400
Wire Wire Line
	5000 1100 5100 1100
Wire Wire Line
	5700 6000 5800 6000
Wire Wire Line
	5700 6000 5700 6100
Wire Wire Line
	8800 6200 8950 6200
Wire Wire Line
	8300 6000 8950 6000
Wire Wire Line
	6900 1100 7200 1100
Wire Wire Line
	7200 1100 7500 1100
Wire Wire Line
	6900 1400 7200 1400
Wire Wire Line
	7200 1400 7500 1400
Wire Wire Line
	6600 1100 6900 1100
Wire Wire Line
	6600 1400 6900 1400
Wire Wire Line
	7500 1100 7800 1100
Wire Wire Line
	7800 1100 8100 1100
Wire Wire Line
	7500 1400 7800 1400
Wire Wire Line
	7800 1400 8100 1400
Wire Wire Line
	1250 7300 1250 7500
Wire Wire Line
	6300 1100 6600 1100
Wire Wire Line
	5700 1100 5800 1100
Wire Wire Line
	6000 1100 6300 1100
Wire Wire Line
	5900 1100 6000 1100
Wire Wire Line
	5800 1100 5900 1100
Wire Wire Line
	8100 1400 8600 1400
Wire Wire Line
	8600 1400 8900 1400
Wire Wire Line
	8900 1400 9200 1400
Wire Wire Line
	10000 4600 10300 4600
Wire Wire Line
	10100 4700 10300 4700
Wire Wire Line
	10600 5850 10600 6000
Wire Wire Line
	9900 6000 10300 6000
Wire Wire Line
	9800 5850 9800 5900
Wire Wire Line
	10600 5700 10600 5850
Wire Wire Line
	6000 6000 8300 6000
Wire Wire Line
	5900 6000 6000 6000
Wire Wire Line
	5800 6000 5900 6000
Wire Wire Line
	4150 6550 4150 7050
Wire Wire Line
	3300 6050 3100 6050
Wire Wire Line
	3450 6550 3200 6550
Wire Wire Line
	3600 7050 3300 7050
Wire Wire Line
	3550 7400 3550 7500
Wire Wire Line
	3550 6900 3550 7400
Wire Wire Line
	1850 5900 2400 5900
Wire Wire Line
	1850 5900 1850 5950
Wire Wire Line
	2400 5900 2600 5900
Wire Wire Line
	2450 7000 2500 7000
Wire Wire Line
	2400 7200 1850 7200
Wire Wire Line
	10600 6000 10600 6150
Wire Wire Line
	10200 6300 10300 6300
Wire Wire Line
	1400 5900 1850 5900
Wire Wire Line
	1250 6450 1250 6700
$EndSCHEMATC
