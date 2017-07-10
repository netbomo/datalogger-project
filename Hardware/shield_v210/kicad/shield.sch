EESchema Schematic File Version 2
LIBS:shield-rescue
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
LIBS:shield-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
Title "Windlogger Shield"
Date "2017-07-10"
Rev "2.1.0"
Comp "ALEEA"
Comment1 "LONGUET Gilles"
Comment2 "AGPLv3"
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 2475 925  2    60   ~ 0
Anemometer1 : Hall effect or ILS
Text Notes 2475 1925 2    60   ~ 0
Winevane : potentiometer
Text Notes 1975 2650 2    60   ~ 0
9V transformer : AC voltage
$Sheet
S 3900 600  1400 850 
U 560A35D3
F0 "F1 Anemometers" 50
F1 "F1_anemometers.sch" 50
F2 "Wind1" I L 3900 800 60 
F3 "Wind2" I L 3900 1300 60 
F4 "Speed2" O R 5300 1250 60 
F5 "Speed1" O R 5300 800 60 
$EndSheet
$Sheet
S 3900 1650 1375 450 
U 560A35DE
F0 "F2 windvane" 50
F1 "F2_windvane.sch" 50
F2 "Udir" O R 5275 1900 60 
F3 "vane_v+" I L 3900 1775 60 
F4 "vane_sig" I L 3900 1875 60 
F5 "vane_v-" I L 3900 1975 60 
$EndSheet
$Sheet
S 3900 2350 1400 1500
U 560A361A
F0 "F3_Voltage_sensors" 50
F1 "F3_Voltage_sensors.sch" 50
F2 "UAC_hi" I L 3900 2450 60 
F3 "UAC_lo" I L 3900 2600 60 
F4 "Uac" O R 5300 2650 60 
F5 "UDC" O R 5300 3050 60 
F6 "Ubat" I L 3900 2900 60 
F7 "ADC5" O R 5300 3500 60 
F8 "ADC5+" I L 3900 3500 60 
F9 "ADC5-" I L 3900 3600 60 
$EndSheet
$Sheet
S 3900 4225 1400 1450
U 560A3643
F0 "F4_Current_sensors" 50
F1 "F4_Current_sensors.sch" 50
F2 "I1_hi" I L 3900 4375 60 
F3 "I1_lo" I L 3900 4475 60 
F4 "I1" O R 5300 4425 60 
F5 "I2_hi" I L 3900 4725 60 
F6 "I2_lo" I L 3900 4825 60 
F7 "I2" O R 5300 4775 60 
F8 "I3_hi" I L 3900 5075 60 
F9 "I3_lo" I L 3900 5175 60 
F10 "I3" O R 5300 5125 60 
F11 "I4_hi" I L 3900 5425 60 
F12 "I4_lo" I L 3900 5525 60 
F13 "I4" O R 5300 5475 60 
$EndSheet
Text Label 3300 800  0    60   ~ 0
Wind1
Text Label 3350 1300 0    60   ~ 0
Wind2
Text Label 3250 1875 0    60   ~ 0
vane_sig
Text Label 3350 2450 0    60   ~ 0
UAC_hi
Text Label 3350 2600 0    60   ~ 0
UAC_lo
Text Notes 10150 4425 0    60   ~ 0
CLKO/ICP1
Text Notes 10150 4325 0    60   ~ 0
OC1A/PWM
Text Notes 10150 4225 0    60   ~ 0
OC1B/SS/PWM
Text Notes 10150 4125 0    60   ~ 0
OC2A/MOSI/PWM
Text Notes 10150 4025 0    60   ~ 0
MISO
Text Notes 10150 3925 0    60   ~ 0
SCK
Text Notes 10150 3825 0    60   ~ 0
GND
Text Notes 10150 3725 0    60   ~ 0
AREF
Text Notes 8650 4725 0    60   ~ 0
ADC0
Text Notes 8650 4825 0    60   ~ 0
ADC1
Text Notes 8650 4925 0    60   ~ 0
ADC2
Text Notes 8650 5025 0    60   ~ 0
ADC3
Text Notes 8650 5125 0    60   ~ 0
ADC4
Text Notes 8650 5225 0    60   ~ 0
ADC5
Text Notes 10150 5300 0    60   ~ 0
RXD
Text Notes 10150 5200 0    60   ~ 0
TXD
Text Notes 10150 5100 0    60   ~ 0
INT0
Text Notes 10150 5000 0    60   ~ 0
INT1/OC2B
Text Notes 10150 4900 0    60   ~ 0
XCK/T0
Text Notes 10150 4800 0    60   ~ 0
OC0B/T1
Text Notes 10150 4700 0    60   ~ 0
OC0A/AIN0
Text Notes 8650 4150 0    60   ~ 0
GND
Text Notes 8650 3950 0    60   ~ 0
3.3V
Text Notes 8650 4050 0    60   ~ 0
5V
Text Notes 8650 3850 0    60   ~ 0
RESET
Text Notes 10150 4600 0    60   ~ 0
AIN1
Text Label 5675 800  2    60   ~ 0
Speed1
Text Label 5675 1250 2    60   ~ 0
Speed2
Text Label 5600 1900 2    60   ~ 0
Udir
Text Label 5450 2650 0    60   ~ 0
Uac
Text Label 6000 2425 0    60   ~ 0
Udir
Text Label 6000 1825 0    60   ~ 0
Uac
Text Label 6000 2525 0    60   ~ 0
Udc
Text Label 9350 4850 0    60   ~ 0
speed1
Text Label 9350 4750 0    60   ~ 0
speed2
Text Label 8050 4000 0    60   ~ 0
5V
NoConn ~ 8300 4300
Text Label 5600 7400 2    60   ~ 0
RPM
Text Label 9400 5050 0    60   ~ 0
RPM
Text Notes 10150 3625 0    60   ~ 0
SDA
Text Notes 10150 3525 0    60   ~ 0
SCL
NoConn ~ 9800 3500
NoConn ~ 9800 3600
NoConn ~ 9800 3700
NoConn ~ 9800 4200
NoConn ~ 9800 4300
NoConn ~ 9800 4400
Text Notes 2000 7375 0    60   ~ 0
RPM\nSensor
NoConn ~ 9800 5150
NoConn ~ 9800 5250
Text Notes 8650 3750 0    60   ~ 0
IOREF
Text Notes 8650 4250 0    60   ~ 0
GND
Text Notes 8650 4350 0    60   ~ 0
Vin
Text Notes 8650 3650 0    60   ~ 0
Reserved
Text Notes 8450 3250 0    60   ~ 0
Arduino style but with Atmega 1284P
Text Notes 2375 6550 2    60   ~ 0
Temperature DS18B20
Text Label 3250 1975 0    60   ~ 0
vane_v-
Text Label 3250 1775 0    60   ~ 0
vane_v+
NoConn ~ 8300 3600
NoConn ~ 8300 3700
NoConn ~ 8300 3800
Text Notes 2475 1375 2    60   ~ 0
Anemometer2 : Hall effect or ILS
$Comp
L +5V #PWR01
U 1 1 573C32C6
P 3000 7100
F 0 "#PWR01" H 3000 6950 50  0001 C CNN
F 1 "+5V" H 3000 7240 50  0000 C CNN
F 2 "" H 3000 7100 50  0000 C CNN
F 3 "" H 3000 7100 50  0000 C CNN
	1    3000 7100
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR02
U 1 1 573C3921
P 7950 4000
F 0 "#PWR02" H 7950 3850 50  0001 C CNN
F 1 "+5V" H 7950 4140 50  0000 C CNN
F 2 "" H 7950 4000 50  0000 C CNN
F 3 "" H 7950 4000 50  0000 C CNN
	1    7950 4000
	1    0    0    -1  
$EndComp
NoConn ~ 8300 3900
NoConn ~ 9800 4650
Text Notes 8500 1300 2    60   ~ 0
+5V
Text Notes 8500 1550 2    60   ~ 0
GND
$Comp
L +5V #PWR03
U 1 1 57E72DB5
P 9700 1150
F 0 "#PWR03" H 9700 1000 50  0001 C CNN
F 1 "+5V" H 9700 1290 50  0000 C CNN
F 2 "" H 9700 1150 50  0000 C CNN
F 3 "" H 9700 1150 50  0000 C CNN
	1    9700 1150
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 57E733BC
P 9600 1400
F 0 "C2" H 9625 1500 50  0000 L CNN
F 1 "100nF" H 9625 1300 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Rect_L7_W2.5_P5" H 9638 1250 50  0001 C CNN
F 3 "" H 9600 1400 50  0000 C CNN
	1    9600 1400
	1    0    0    -1  
$EndComp
$Comp
L CP C1
U 1 1 57E734D2
P 9350 1400
F 0 "C1" H 9375 1500 50  0000 L CNN
F 1 "10uF" H 9375 1300 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Radial_D5_L6_P2.5" H 9388 1250 50  0001 C CNN
F 3 "" H 9350 1400 50  0000 C CNN
	1    9350 1400
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG04
U 1 1 57E743BD
P 10150 1150
F 0 "#FLG04" H 10150 1245 50  0001 C CNN
F 1 "PWR_FLAG" H 10150 1330 50  0000 C CNN
F 2 "" H 10150 1150 50  0000 C CNN
F 3 "" H 10150 1150 50  0000 C CNN
	1    10150 1150
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG05
U 1 1 57E7446B
P 10150 1550
F 0 "#FLG05" H 10150 1645 50  0001 C CNN
F 1 "PWR_FLAG" H 10150 1730 50  0000 C CNN
F 2 "" H 10150 1550 50  0000 C CNN
F 3 "" H 10150 1550 50  0000 C CNN
	1    10150 1550
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR06
U 1 1 57E7ADEC
P 2975 3975
F 0 "#PWR06" H 2975 3825 50  0001 C CNN
F 1 "+5V" H 2975 4115 50  0000 C CNN
F 2 "" H 2975 3975 50  0000 C CNN
F 3 "" H 2975 3975 50  0000 C CNN
	1    2975 3975
	1    0    0    -1  
$EndComp
Text Notes 525  5150 0    60   ~ 0
The 4 current sensors are design \nto are :\nHTFS 200 or Hass 50-s for DC current\nSCT-013-000 for AC current
Text Notes 2475 2900 2    60   ~ 0
Ubat
Text Label 3600 2900 2    60   ~ 0
Ubat
Text Label 6000 1925 0    60   ~ 0
I1
Text Label 6000 2025 0    60   ~ 0
I2
Text Label 6000 2125 0    60   ~ 0
I3
Text Label 6000 2225 0    60   ~ 0
I4
Text Label 8775 2325 0    60   ~ 0
Ubat
Text Label 8775 2525 0    60   ~ 0
Uac_hi
Text Label 8775 2625 0    60   ~ 0
Uac_lo
Text Notes 8325 2900 0    60   ~ 0
To feed power supply from voltage sensor.\nThe power supply block is on the digital board.
Text Notes 2325 3075 0    60   ~ 0
gnd
Wire Wire Line
	2875 800  3900 800 
Wire Wire Line
	2875 1300 3900 1300
Wire Wire Line
	2800 2450 3900 2450
Wire Wire Line
	2875 2900 3900 2900
Wire Wire Line
	5300 800  5675 800 
Wire Wire Line
	5300 1250 5675 1250
Wire Wire Line
	6000 1925 6375 1925
Wire Wire Line
	6000 2025 6375 2025
Wire Wire Line
	6000 2125 6375 2125
Wire Wire Line
	6000 2225 6375 2225
Wire Wire Line
	6000 2325 6375 2325
Wire Wire Line
	5275 1900 5600 1900
Wire Wire Line
	5300 2650 5625 2650
Wire Wire Line
	2875 7375 3025 7375
Wire Wire Line
	3025 7375 3025 7425
Wire Wire Line
	9800 5050 9400 5050
Wire Wire Line
	9800 4750 9350 4750
Wire Wire Line
	9800 4850 9350 4850
Wire Wire Line
	9800 3800 9550 3800
Wire Wire Line
	9550 3800 9550 4375
Wire Wire Line
	7950 4100 7950 4350
Wire Wire Line
	7950 4100 8300 4100
Wire Wire Line
	2875 1875 3900 1875
Wire Wire Line
	2875 1775 3900 1775
Wire Wire Line
	2875 1975 3900 1975
Wire Wire Line
	2875 900  3075 900 
Wire Wire Line
	3075 900  3075 950 
Wire Wire Line
	2875 1400 3075 1400
Wire Wire Line
	3075 1400 3075 1450
Wire Wire Line
	2875 7275 3500 7275
Wire Wire Line
	9200 1400 9200 1600
Wire Wire Line
	9200 1200 9200 1300
Wire Wire Line
	9200 1300 9000 1300
Wire Wire Line
	9200 1600 10150 1600
Wire Wire Line
	9700 1600 9700 1650
Wire Wire Line
	9700 1200 9700 1150
Wire Wire Line
	9200 1200 10150 1200
Wire Wire Line
	9350 1250 9350 1200
Connection ~ 9350 1200
Wire Wire Line
	9600 1250 9600 1200
Connection ~ 9600 1200
Wire Wire Line
	9600 1550 9600 1600
Connection ~ 9600 1600
Wire Wire Line
	9350 1550 9350 1600
Connection ~ 9350 1600
Wire Wire Line
	7950 4200 8300 4200
Connection ~ 7950 4200
Wire Wire Line
	8300 4000 7950 4000
Wire Wire Line
	10150 1200 10150 1150
Connection ~ 9700 1200
Wire Wire Line
	10150 1600 10150 1550
Connection ~ 9700 1600
Wire Wire Line
	2875 4025 2975 4025
Wire Wire Line
	2975 3975 2975 5625
Wire Wire Line
	2975 4575 2875 4575
Connection ~ 2975 4025
Wire Wire Line
	2975 5125 2875 5125
Connection ~ 2975 4575
Wire Wire Line
	2975 5625 2875 5625
Connection ~ 2975 5125
Wire Wire Line
	2875 4125 3075 4125
Wire Wire Line
	3075 4125 3075 6025
Wire Wire Line
	2875 4675 3075 4675
Connection ~ 3075 4675
Wire Wire Line
	2875 5225 3075 5225
Connection ~ 3075 5225
Wire Wire Line
	2875 5725 3075 5725
Connection ~ 3075 5725
Wire Wire Line
	3500 5075 3500 5325
Wire Wire Line
	3500 5325 2875 5325
Wire Wire Line
	3900 5075 3500 5075
Wire Wire Line
	3900 5175 3600 5175
Wire Wire Line
	3600 5175 3600 5425
Wire Wire Line
	3600 5425 2875 5425
Wire Wire Line
	3900 4825 3725 4825
Wire Wire Line
	3725 4825 3725 4875
Wire Wire Line
	3725 4875 2875 4875
Wire Wire Line
	2875 4775 3675 4775
Wire Wire Line
	3675 4775 3675 4725
Wire Wire Line
	3675 4725 3900 4725
Wire Wire Line
	3900 4475 3575 4475
Wire Wire Line
	3575 4475 3575 4325
Wire Wire Line
	3575 4325 2875 4325
Wire Wire Line
	2875 4225 3650 4225
Wire Wire Line
	3650 4225 3650 4375
Wire Wire Line
	3650 4375 3900 4375
Wire Wire Line
	3900 5425 3675 5425
Wire Wire Line
	3675 5425 3675 5825
Wire Wire Line
	3675 5825 2875 5825
Wire Wire Line
	2875 5925 3750 5925
Wire Wire Line
	3750 5925 3750 5525
Wire Wire Line
	3750 5525 3900 5525
Wire Wire Line
	6375 2425 6000 2425
Wire Wire Line
	6000 2525 6375 2525
Wire Wire Line
	8775 2325 9125 2325
Wire Wire Line
	8775 2425 9125 2425
Wire Wire Line
	8775 2525 9125 2525
Wire Wire Line
	8775 2625 9125 2625
Wire Wire Line
	2975 3150 2975 3000
Wire Wire Line
	2975 3000 2875 3000
Wire Wire Line
	2800 2600 3900 2600
NoConn ~ 9800 5600
NoConn ~ 9800 5900
NoConn ~ 9800 6000
NoConn ~ 9800 6100
NoConn ~ 9800 6200
Text Label 8775 2425 0    60   ~ 0
gnd
Text Label 10000 1600 2    60   ~ 0
gnd
$Comp
L R-RESCUE-Windlogger R1
U 1 1 560B9C15
P 3250 7150
F 0 "R1" V 3330 7150 40  0000 C CNN
F 1 "10k" V 3257 7151 40  0000 C CNN
F 2 "Discret:R4-LARGE_PADS" V 3180 7150 30  0001 C CNN
F 3 "~" H 3250 7150 30  0000 C CNN
F 4 "0.125W" V 3250 7150 60  0001 C CNN "Puissance"
F 5 "1%" V 3250 7150 60  0001 C CNN "Tolérance"
	1    3250 7150
	0    1    1    0   
$EndComp
Wire Wire Line
	3000 7100 3000 7175
Wire Wire Line
	3500 7150 3500 7400
Connection ~ 3500 7275
Wire Wire Line
	3500 7400 5600 7400
Text Notes 8225 925  0    60   ~ 0
alternative power supply to test or use this board alone
$Comp
L GND #PWR07
U 1 1 5803EC79
P 9700 1650
F 0 "#PWR07" H 9700 1400 50  0001 C CNN
F 1 "GND" H 9700 1500 50  0000 C CNN
F 2 "" H 9700 1650 50  0000 C CNN
F 3 "" H 9700 1650 50  0000 C CNN
	1    9700 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 1400 9200 1400
$Comp
L GND #PWR08
U 1 1 58041636
P 7950 4350
F 0 "#PWR08" H 7950 4100 50  0001 C CNN
F 1 "GND" H 7950 4200 50  0000 C CNN
F 2 "" H 7950 4350 50  0000 C CNN
F 3 "" H 7950 4350 50  0000 C CNN
	1    7950 4350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 58041ACE
P 9550 4375
F 0 "#PWR09" H 9550 4125 50  0001 C CNN
F 1 "GND" H 9550 4225 50  0000 C CNN
F 2 "" H 9550 4375 50  0000 C CNN
F 3 "" H 9550 4375 50  0000 C CNN
	1    9550 4375
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 58042172
P 3025 7425
F 0 "#PWR010" H 3025 7175 50  0001 C CNN
F 1 "GND" H 3025 7275 50  0000 C CNN
F 2 "" H 3025 7425 50  0000 C CNN
F 3 "" H 3025 7425 50  0000 C CNN
	1    3025 7425
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 58043951
P 3075 6025
F 0 "#PWR011" H 3075 5775 50  0001 C CNN
F 1 "GND" H 3075 5875 50  0000 C CNN
F 2 "" H 3075 6025 50  0000 C CNN
F 3 "" H 3075 6025 50  0000 C CNN
	1    3075 6025
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 58044479
P 2975 3150
F 0 "#PWR012" H 2975 2900 50  0001 C CNN
F 1 "GND" H 2975 3000 50  0000 C CNN
F 2 "" H 2975 3150 50  0000 C CNN
F 3 "" H 2975 3150 50  0000 C CNN
	1    2975 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 58044D95
P 3075 950
F 0 "#PWR013" H 3075 700 50  0001 C CNN
F 1 "GND" H 3075 800 50  0000 C CNN
F 2 "" H 3075 950 50  0000 C CNN
F 3 "" H 3075 950 50  0000 C CNN
	1    3075 950 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 58044FD5
P 3075 1450
F 0 "#PWR014" H 3075 1200 50  0001 C CNN
F 1 "GND" H 3075 1300 50  0000 C CNN
F 2 "" H 3075 1450 50  0000 C CNN
F 3 "" H 3075 1450 50  0000 C CNN
	1    3075 1450
	1    0    0    -1  
$EndComp
NoConn ~ 7275 6075
NoConn ~ 7275 6200
NoConn ~ 8325 5950
NoConn ~ 8325 6075
Text Notes 7600 5850 0    60   ~ 0
Fixation Holes
Text Notes 8650 5325 0    60   ~ 0
ADC6
Text Notes 8650 5425 0    60   ~ 0
ADC7
NoConn ~ 9800 4950
$Comp
L GND #PWR015
U 1 1 5852ED8A
P 2950 6700
F 0 "#PWR015" H 2950 6450 50  0001 C CNN
F 1 "GND" H 2950 6550 50  0000 C CNN
F 2 "" H 2950 6700 50  0000 C CNN
F 3 "" H 2950 6700 50  0000 C CNN
	1    2950 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 6700 2950 6625
Wire Wire Line
	2950 6625 2875 6625
Wire Wire Line
	2875 6525 3575 6525
Wire Wire Line
	3550 6425 2875 6425
Wire Wire Line
	9325 5700 9800 5700
Wire Wire Line
	9325 5800 9800 5800
Text Label 3575 6525 2    60   ~ 0
1wire_data
Text Label 9325 5800 0    60   ~ 0
1wire_PWR
Text Label 9325 5700 0    60   ~ 0
1wire_data
Text Label 3550 6425 2    60   ~ 0
1wire_PWR
Wire Wire Line
	5525 4775 5300 4775
Wire Wire Line
	5525 4425 5300 4425
Wire Wire Line
	5525 5475 5300 5475
Wire Wire Line
	5525 5125 5300 5125
Text Label 5525 4425 2    60   ~ 0
I1
Text Label 5525 4775 2    60   ~ 0
I2
Text Label 5525 5125 2    60   ~ 0
I3
Text Label 5525 5475 2    60   ~ 0
I4
Wire Notes Line
	2425 3950 2425 6100
Wire Notes Line
	2425 6100 2550 6100
Wire Notes Line
	2550 3950 2425 3950
NoConn ~ 9800 4550
Wire Wire Line
	6000 1825 6375 1825
$Comp
L CONN_01X03 vane1
U 1 1 5855584D
P 2675 1875
F 0 "vane1" H 2575 2075 50  0000 C CNN
F 1 "CONN_01X03" V 2775 1875 50  0000 C CNN
F 2 "w_conn_screw:mstba_2,5%2f3-g-5,08" H 2675 1875 50  0001 C CNN
F 3 "" H 2675 1875 50  0000 C CNN
	1    2675 1875
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X02 Udc1
U 1 1 58556D71
P 2675 2950
F 0 "Udc1" H 2600 3100 50  0000 C CNN
F 1 "CONN_01X02" V 2775 3025 50  0000 C CNN
F 2 "w_conn_screw:mstba_2,5%2f2-g-5,08" H 2675 2950 50  0001 C CNN
F 3 "" H 2675 2950 50  0000 C CNN
	1    2675 2950
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X04 current1
U 1 1 5855938E
P 2675 4175
F 0 "current1" H 2575 4425 50  0000 C CNN
F 1 "CONN_01X04" V 2875 4200 50  0000 C CNN
F 2 "w_conn_screw:mstba_2,5%2f4-g-5,08" H 2675 4175 50  0001 C CNN
F 3 "" H 2675 4175 50  0000 C CNN
	1    2675 4175
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X04 current2
U 1 1 5855A2A9
P 2675 4725
F 0 "current2" H 2575 4975 50  0000 C CNN
F 1 "CONN_01X04" V 2875 4725 50  0000 C CNN
F 2 "w_conn_screw:mstba_2,5%2f4-g-5,08" H 2675 4725 50  0001 C CNN
F 3 "" H 2675 4725 50  0000 C CNN
	1    2675 4725
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X04 current3
U 1 1 5855A3B5
P 2675 5275
F 0 "current3" H 2575 5525 50  0000 C CNN
F 1 "CONN_01X04" V 2875 5250 50  0000 C CNN
F 2 "w_conn_screw:mstba_2,5%2f4-g-5,08" H 2675 5275 50  0001 C CNN
F 3 "" H 2675 5275 50  0000 C CNN
	1    2675 5275
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X04 current4
U 1 1 5855AFF4
P 2675 5775
F 0 "current4" H 2575 6025 50  0000 C CNN
F 1 "CONN_01X04" V 2875 5800 50  0000 C CNN
F 2 "w_conn_screw:mstba_2,5%2f4-g-5,08" H 2675 5775 50  0001 C CNN
F 3 "" H 2675 5775 50  0000 C CNN
	1    2675 5775
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X03 temp1
U 1 1 5855F710
P 2675 6525
F 0 "temp1" H 2575 6725 50  0000 C CNN
F 1 "CONN_01X03" V 2775 6525 50  0000 C CNN
F 2 "w_conn_screw:mstba_2,5%2f3-g-5,08" H 2675 6525 50  0001 C CNN
F 3 "" H 2675 6525 50  0000 C CNN
	1    2675 6525
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X01 h2
U 1 1 585640DC
P 7475 6075
F 0 "h2" H 7175 6075 50  0000 C CNN
F 1 "CONN_01X01" H 7775 6075 50  0000 C CNN
F 2 "Socket_Arduino_Mega:Arduino_1pin" H 7475 6075 50  0001 C CNN
F 3 "" H 7475 6075 50  0000 C CNN
	1    7475 6075
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X01 h4
U 1 1 58564977
P 7475 6200
F 0 "h4" H 7175 6200 50  0000 C CNN
F 1 "CONN_01X01" H 7775 6200 50  0000 C CNN
F 2 "Socket_Arduino_Mega:Arduino_1pin" H 7475 6200 50  0001 C CNN
F 3 "" H 7475 6200 50  0000 C CNN
	1    7475 6200
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X01 h1
U 1 1 58564B74
P 8525 5950
F 0 "h1" H 8225 5950 50  0000 C CNN
F 1 "CONN_01X01" H 8825 5950 50  0000 C CNN
F 2 "Socket_Arduino_Mega:Arduino_1pin" H 8525 5950 50  0001 C CNN
F 3 "" H 8525 5950 50  0000 C CNN
	1    8525 5950
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X01 h3
U 1 1 58564CA1
P 8525 6075
F 0 "h3" H 8225 6075 50  0000 C CNN
F 1 "CONN_01X01" H 8825 6075 50  0000 C CNN
F 2 "Socket_Arduino_Mega:Arduino_1pin" H 8525 6075 50  0001 C CNN
F 3 "" H 8525 6075 50  0000 C CNN
	1    8525 6075
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X08 Analog1
U 1 1 58566141
P 8500 5025
F 0 "Analog1" H 8500 5475 50  0000 C CNN
F 1 "CONN_01X08" V 8600 5025 50  0000 C CNN
F 2 "Socket_Arduino_Mega:Socket_Strip_Arduino_1x08" H 8500 5025 50  0001 C CNN
F 3 "" H 8500 5025 50  0000 C CNN
	1    8500 5025
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X08 Power1
U 1 1 585670D0
P 8500 3950
F 0 "Power1" H 8500 4400 50  0000 C CNN
F 1 "CONN_01X08" V 8600 3950 50  0000 C CNN
F 2 "Socket_Arduino_Mega:Socket_Strip_Arduino_1x08" H 8500 3950 50  0001 C CNN
F 3 "" H 8500 3950 50  0000 C CNN
	1    8500 3950
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X10 PWM1
U 1 1 58567C59
P 10000 3950
F 0 "PWM1" H 10000 4500 50  0000 C CNN
F 1 "CONN_01X10" V 10100 3950 50  0000 C CNN
F 2 "Socket_Arduino_Mega:Socket_Strip_Arduino_1x10" H 10000 3950 50  0001 C CNN
F 3 "" H 10000 3950 50  0000 C CNN
	1    10000 3950
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X08 PWM2
U 1 1 585689E5
P 10000 4900
F 0 "PWM2" H 10150 5300 50  0000 C CNN
F 1 "CONN_01X08" V 10100 4900 50  0000 C CNN
F 2 "Socket_Arduino_Mega:Socket_Strip_Arduino_1x08" H 10000 4900 50  0001 C CNN
F 3 "" H 10000 4900 50  0000 C CNN
	1    10000 4900
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X08 Communication1
U 1 1 58569A7E
P 10000 5850
F 0 "Communication1" H 10000 6300 50  0000 C CNN
F 1 "CONN_01X08" V 10100 5850 50  0000 C CNN
F 2 "Socket_Arduino_Mega:Socket_Strip_Arduino_1x08" H 10000 5850 50  0001 C CNN
F 3 "" H 10000 5850 50  0000 C CNN
	1    10000 5850
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 POWER_SOURCE1
U 1 1 5857136B
P 9325 2475
F 0 "POWER_SOURCE1" H 9325 2725 50  0000 C CNN
F 1 "CONN_01X04" V 9475 2450 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x04" H 9325 2475 50  0001 C CNN
F 3 "" H 9325 2475 50  0000 C CNN
	1    9325 2475
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 5VDC1
U 1 1 58575033
P 8800 1350
F 0 "5VDC1" H 8675 1525 50  0000 C CNN
F 1 "CONN_01X02" V 8900 1350 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x02" H 8800 1350 50  0001 C CNN
F 3 "" H 8800 1350 50  0000 C CNN
	1    8800 1350
	-1   0    0    1   
$EndComp
Wire Notes Line
	11200 3050 6975 3050
Wire Notes Line
	6975 3050 6975 6500
$Comp
L CONN_01X03 RPM1
U 1 1 585985B9
P 2675 7275
F 0 "RPM1" H 2600 7475 50  0000 C CNN
F 1 "CONN_01X03" V 2775 7275 50  0000 C CNN
F 2 "w_conn_screw:mstba_2,5%2f3-g-5,08" H 2675 7275 50  0001 C CNN
F 3 "" H 2675 7275 50  0000 C CNN
	1    2675 7275
	-1   0    0    1   
$EndComp
Wire Wire Line
	3000 7175 2875 7175
Connection ~ 3000 7150
Text Label 5650 3050 2    60   ~ 0
Udc
Wire Wire Line
	5300 3050 5650 3050
$Comp
L CONN_01X02 ADC5
U 1 1 585D89E8
P 2675 3550
F 0 "ADC5" H 2625 3700 50  0000 C CNN
F 1 "CONN_01X02" V 2775 3550 50  0000 C CNN
F 2 "w_conn_screw:mstba_2,5%2f2-g-5,08" H 2675 3550 50  0001 C CNN
F 3 "" H 2675 3550 50  0000 C CNN
	1    2675 3550
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 3500 2875 3500
Wire Wire Line
	3900 3600 2875 3600
Text Label 3075 3500 0    60   ~ 0
ADC5+
Text Label 3075 3600 0    60   ~ 0
ADC5-
$Comp
L BARREL_JACK ADC1
U 1 1 585E60F9
P 2500 2550
F 0 "ADC1" H 2500 2800 50  0000 C CNN
F 1 "BARREL_JACK" H 2500 2350 50  0000 C CNN
F 2 "Connect:BARREL_JACK" H 2500 2550 50  0001 C CNN
F 3 "" H 2500 2550 50  0000 C CNN
	1    2500 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 2550 2800 2650
Connection ~ 2800 2600
$Comp
L CONN_01X03 anemo1
U 1 1 5862AA9C
P 2675 800
F 0 "anemo1" H 2575 1000 50  0000 C CNN
F 1 "CONN_01X03" V 2775 800 50  0000 C CNN
F 2 "w_conn_screw:mstba_2,5%2f3-g-5,08" H 2675 800 50  0001 C CNN
F 3 "" H 2675 800 50  0000 C CNN
	1    2675 800 
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X03 anemo2
U 1 1 5862E3CC
P 2675 1300
F 0 "anemo2" H 2575 1500 50  0000 C CNN
F 1 "CONN_01X03" V 2775 1300 50  0000 C CNN
F 2 "w_conn_screw:mstba_2,5%2f3-g-5,08" H 2675 1300 50  0001 C CNN
F 3 "" H 2675 1300 50  0000 C CNN
	1    2675 1300
	-1   0    0    1   
$EndComp
$Comp
L +5V #PWR016
U 1 1 5862F4AB
P 2950 700
F 0 "#PWR016" H 2950 550 50  0001 C CNN
F 1 "+5V" H 2950 840 50  0000 C CNN
F 2 "" H 2950 700 50  0000 C CNN
F 3 "" H 2950 700 50  0000 C CNN
	1    2950 700 
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR017
U 1 1 5862F5BC
P 2950 1200
F 0 "#PWR017" H 2950 1050 50  0001 C CNN
F 1 "+5V" H 2950 1340 50  0000 C CNN
F 2 "" H 2950 1200 50  0000 C CNN
F 3 "" H 2950 1200 50  0000 C CNN
	1    2950 1200
	0    1    1    0   
$EndComp
Wire Wire Line
	2950 700  2875 700 
Wire Wire Line
	2875 1200 2950 1200
$Comp
L MCP3208 U?
U 1 1 59634CD5
P 6975 2225
F 0 "U?" H 6775 2750 50  0000 R CNN
F 1 "MCP3208" H 6775 2675 50  0000 R CNN
F 2 "" H 7075 2325 50  0001 C CNN
F 3 "" H 7075 2325 50  0001 C CNN
	1    6975 2225
	1    0    0    1   
$EndComp
Text Notes 6075 1300 0    60   ~ 0
Add ADC 12 bit to increase the accuracy
$Comp
L +5V #PWR?
U 1 1 596367C5
P 7025 1525
F 0 "#PWR?" H 7025 1375 50  0001 C CNN
F 1 "+5V" H 7025 1665 50  0000 C CNN
F 2 "" H 7025 1525 50  0000 C CNN
F 3 "" H 7025 1525 50  0000 C CNN
	1    7025 1525
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 596368C9
P 7050 2800
F 0 "#PWR?" H 7050 2550 50  0001 C CNN
F 1 "GND" H 7050 2650 50  0000 C CNN
F 2 "" H 7050 2800 50  0000 C CNN
F 3 "" H 7050 2800 50  0000 C CNN
	1    7050 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6875 2725 6875 2800
Wire Wire Line
	6875 2800 7175 2800
Wire Wire Line
	7175 2800 7175 2725
Connection ~ 7050 2800
Wire Wire Line
	7175 1525 7175 1625
Wire Wire Line
	6875 1525 7175 1525
Wire Wire Line
	6875 1625 6875 1525
Connection ~ 7025 1525
Wire Wire Line
	7575 2025 8000 2025
Wire Wire Line
	8000 2125 7575 2125
Wire Wire Line
	8000 2225 7575 2225
Wire Wire Line
	8000 2325 7575 2325
Text Label 8000 2025 2    60   ~ 0
CS_shield
Text Label 8000 2125 2    60   ~ 0
MOSI
Text Label 8000 2225 2    60   ~ 0
MISO
Text Label 8000 2325 2    60   ~ 0
SCK
Wire Wire Line
	9325 5500 9800 5500
Wire Wire Line
	9325 4100 9800 4100
Wire Wire Line
	9325 4000 9800 4000
Wire Wire Line
	9325 3900 9800 3900
Text Label 9325 5500 0    60   ~ 0
CS_shield
Text Label 9325 4100 0    60   ~ 0
MOSI
Text Label 9325 4000 0    60   ~ 0
MISO
Text Label 9325 3900 0    60   ~ 0
SCK
$EndSCHEMATC
