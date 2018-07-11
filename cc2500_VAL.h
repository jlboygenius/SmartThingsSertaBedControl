
alexabedcontrol.ino  cc2500_REG.h cc2500_VAL.h  


1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
/* Sync word qualifier mode = 30/32 sync word bits detected */
/* CRC autoflush = false */
/* Channel spacing = 199.951172 */
/* Data format = Normal mode */
/* Data rate = 2.39897 */
/* RX filter BW = 203.125000 */
/* Preamble count = 4 */
/* Whitening = false */
/* Address config = No address check */
/* Carrier frequency = 2432.999908 */
/* Device address = 0 */
/* TX power = 0 */
/* Manchester enable = false */
/* CRC enable = true */
/* Deviation = 38.085938 */
/* Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word */
/* Packet length = 255 */
/* Modulation format = 2-FSK */
/* Base frequency = 2432.999908 */
/* Modulated = true */
/* Channel number = 0 */
/* PA table */
#define PA_TABLE {0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,}
/***************************************************************
 *  SmartRF Studio(tm) Export
 *
 *  Radio register settings specifed with C-code
 *  compatible #define statements.
 *
 *  RF device: CC2500
 *
 ***************************************************************/
#define VAL_IOCFG2           0x29
#define VAL_IOCFG1           0x2E
#define VAL_IOCFG0           0x06
#define VAL_FIFOTHR          0x07
#define VAL_SYNC1            0xD3
#define VAL_SYNC0            0x91
#define VAL_PKTLEN           0x03
#define VAL_PKTCTRL1         0x04
#define VAL_PKTCTRL0         0x05
#define VAL_ADDR             0x00
#define VAL_CHANNR           0x03
#define VAL_FSCTRL1          0x08
#define VAL_FSCTRL0          0x00
#define VAL_FREQ2            0x5D
#define VAL_FREQ1            0x93
#define VAL_FREQ0            0xB1
#define VAL_MDMCFG4          0x86
#define VAL_MDMCFG3          0x83
#define VAL_MDMCFG2          0x03
#define VAL_MDMCFG1          0x22
#define VAL_MDMCFG0          0xF8
#define VAL_DEVIATN          0x44
#define VAL_MCSM2            0x07
#define VAL_MCSM1            0x30
#define VAL_MCSM0            0x18
#define VAL_FOCCFG           0x16
#define VAL_BSCFG            0x6C
#define VAL_AGCCTRL2         0x03
Ready.  FishTankController  v0.7.0 
