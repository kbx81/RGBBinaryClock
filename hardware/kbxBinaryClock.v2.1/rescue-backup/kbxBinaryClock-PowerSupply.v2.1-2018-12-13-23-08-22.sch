EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:stm32
LIBS:win
LIBS:mechanical
LIBS:switches
LIBS:maxim
LIBS:sensors
LIBS:kbxBinaryClock.v2.1-cache
EELAYER 25 0
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
L Jack-DC J1
U 1 1 5A06C211
P 2900 4000
F 0 "J1" H 2900 4210 50  0000 C CNN
F 1 "Jack-DC" H 2900 3825 50  0000 C CNN
F 2 "kbxCustoms:BARREL_JACK" H 2950 3960 50  0001 C CNN
F 3 "" H 2950 3960 50  0001 C CNN
	1    2900 4000
	1    0    0    -1  
$EndComp
$Comp
L C C22
U 1 1 5A06C24E
P 6100 4050
F 0 "C22" H 6125 4150 50  0000 L CNN
F 1 "10 µF" H 6125 3950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6138 3900 50  0001 C CNN
F 3 "" H 6100 4050 50  0001 C CNN
	1    6100 4050
	1    0    0    -1  
$EndComp
$Comp
L C C21
U 1 1 5A06C283
P 5100 4050
F 0 "C21" H 5125 4150 50  0000 L CNN
F 1 "100 nF" H 5125 3950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5138 3900 50  0001 C CNN
F 3 "" H 5100 4050 50  0001 C CNN
	1    5100 4050
	1    0    0    -1  
$EndComp
$Comp
L Battery_Cell BT1
U 1 1 5A06C2CC
P 8200 4000
F 0 "BT1" H 8300 4100 50  0000 L CNN
F 1 "CR2032" H 8300 4000 50  0000 L CNN
F 2 "Battery_Holders:Keystone_103_1x20mm-CoinCell" V 8200 4060 50  0001 C CNN
F 3 "" V 8200 4060 50  0001 C CNN
	1    8200 4000
	1    0    0    -1  
$EndComp
$Comp
L CP C20
U 1 1 5A0AB81C
P 4200 4050
F 0 "C20" H 4225 4150 50  0000 L CNN
F 1 "100 µF" H 4225 3950 50  0000 L CNN
F 2 "Capacitors_SMD:CP_Elec_8x10" H 4238 3900 50  0001 C CNN
F 3 "" H 4200 4050 50  0001 C CNN
	1    4200 4050
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR027
U 1 1 5A0AB8E2
P 8200 3600
F 0 "#PWR027" H 8200 3450 50  0001 C CNN
F 1 "+BATT" H 8200 3740 50  0000 C CNN
F 2 "" H 8200 3600 50  0001 C CNN
F 3 "" H 8200 3600 50  0001 C CNN
	1    8200 3600
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR028
U 1 1 5A0AB93A
P 6600 3600
F 0 "#PWR028" H 6600 3450 50  0001 C CNN
F 1 "+3.3V" H 6600 3740 50  0000 C CNN
F 2 "" H 6600 3600 50  0001 C CNN
F 3 "" H 6600 3600 50  0001 C CNN
	1    6600 3600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR029
U 1 1 5A0ABA04
P 4200 4400
F 0 "#PWR029" H 4200 4150 50  0001 C CNN
F 1 "GND" H 4200 4250 50  0000 C CNN
F 2 "" H 4200 4400 50  0001 C CNN
F 3 "" H 4200 4400 50  0001 C CNN
	1    4200 4400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR030
U 1 1 5A0ABA30
P 5100 4400
F 0 "#PWR030" H 5100 4150 50  0001 C CNN
F 1 "GND" H 5100 4250 50  0000 C CNN
F 2 "" H 5100 4400 50  0001 C CNN
F 3 "" H 5100 4400 50  0001 C CNN
	1    5100 4400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR031
U 1 1 5A0ABA5C
P 5600 4400
F 0 "#PWR031" H 5600 4150 50  0001 C CNN
F 1 "GND" H 5600 4250 50  0000 C CNN
F 2 "" H 5600 4400 50  0001 C CNN
F 3 "" H 5600 4400 50  0001 C CNN
	1    5600 4400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR032
U 1 1 5A0ABA88
P 6100 4400
F 0 "#PWR032" H 6100 4150 50  0001 C CNN
F 1 "GND" H 6100 4250 50  0000 C CNN
F 2 "" H 6100 4400 50  0001 C CNN
F 3 "" H 6100 4400 50  0001 C CNN
	1    6100 4400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 5A0ABAB4
P 8200 4400
F 0 "#PWR033" H 8200 4150 50  0001 C CNN
F 1 "GND" H 8200 4250 50  0000 C CNN
F 2 "" H 8200 4400 50  0001 C CNN
F 3 "" H 8200 4400 50  0001 C CNN
	1    8200 4400
	1    0    0    -1  
$EndComp
$Comp
L +9V #PWR034
U 1 1 5A0AC145
P 4200 3600
F 0 "#PWR034" H 4200 3450 50  0001 C CNN
F 1 "+9V" H 4200 3740 50  0000 C CNN
F 2 "" H 4200 3600 50  0001 C CNN
F 3 "" H 4200 3600 50  0001 C CNN
	1    4200 3600
	1    0    0    -1  
$EndComp
$Comp
L Polyfuse F1
U 1 1 5A0B6BAC
P 3450 3900
F 0 "F1" V 3350 3900 50  0000 C CNN
F 1 "~2A" V 3550 3900 50  0000 C CNN
F 2 "Fuse_Holders_and_Fuses:Fuse_SMD2920" H 3500 3700 50  0001 L CNN
F 3 "" H 3450 3900 50  0001 C CNN
	1    3450 3900
	0    1    1    0   
$EndComp
$Comp
L DIODE D2
U 1 1 5A0B6C19
P 3900 3900
F 0 "D2" H 3900 4000 40  0000 C CNN
F 1 "S3A" H 3900 3800 40  0000 C CNN
F 2 "Diodes_SMD:D_SMC" H 3900 3900 60  0001 C CNN
F 3 "" H 3900 3900 60  0000 C CNN
	1    3900 3900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR035
U 1 1 5A0B6F52
P 3300 4400
F 0 "#PWR035" H 3300 4150 50  0001 C CNN
F 1 "GND" H 3300 4250 50  0000 C CNN
F 2 "" H 3300 4400 50  0001 C CNN
F 3 "" H 3300 4400 50  0001 C CNN
	1    3300 4400
	1    0    0    -1  
$EndComp
$Comp
L LD1117S33TR_SOT223 U5
U 1 1 5A06C0AF
P 5600 3900
F 0 "U5" H 5450 4025 50  0000 C CNN
F 1 "LD1117S33TR" H 5600 4025 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:TO-252-2" H 5600 4100 50  0001 C CNN
F 3 "" H 5700 3650 50  0001 C CNN
	1    5600 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 4100 3300 4400
Wire Wire Line
	3200 4100 3300 4100
Wire Wire Line
	3600 3900 3700 3900
Wire Wire Line
	3200 3900 3300 3900
Wire Wire Line
	8200 3800 8200 3600
Wire Wire Line
	8200 4100 8200 4400
Wire Wire Line
	6100 4400 6100 4200
Wire Wire Line
	5600 4200 5600 4400
Wire Wire Line
	5100 4400 5100 4200
Wire Wire Line
	4200 4200 4200 4400
Connection ~ 6100 3900
Wire Wire Line
	6600 3900 6600 3600
Wire Wire Line
	5900 3900 6600 3900
Connection ~ 5100 3900
Connection ~ 4200 3900
Wire Wire Line
	4200 3900 4200 3600
Wire Wire Line
	4100 3900 5300 3900
Wire Wire Line
	3200 4000 3200 4100
$EndSCHEMATC
