/* ========================================
 * Jonathan Fielding
 * My Solar Project code
 * 
 * This file contains all of the address starting positions for specific I2C memory locations
 * It is based on the .xlsx Excel spreadsheet that defines what variables go where
 * ========================================
*/
//#define			WARNING OR REPLY LEVELS			0   // NOT USED YET
#define			I2C_LOAD_RELAY			                1   // only 1 or 0
#define			I2C_EXT_RELAY			                2   // only 1 or 0
//#define			FOR MORE RELAYS IF USED			3
//#define						4
//#define						5
//#define						6
//#define						7
#define			I2C_TIME_MINMAXAVE_MINUTES		8
#define			I2C_TIME_MINMAXAVE_SECONDS		9

#define			I2C_BATT_V_GAIN_LBYTE			10
#define			I2C_BATT_V_GAIN_HBYTE			11
#define			I2C_BATT_V_OFFSET_LBYTE			12
#define			I2C_BATT_V_OFFSET_HBYTE			13
#define			I2C_BATT_I_GAIN_LBYTE			14
#define			I2C_BATT_I_GAIN_HBYTE			15
#define			I2C_BATT_I_OFFSET_LBYTE			16
#define			I2C_BATT_I_OFFSET_HBYTE			17

#define			I2C_LOAD_V_GAIN_LBYTE			18
#define			I2C_LOAD_V_GAIN_HBYTE			19
#define			I2C_LOAD_V_OFFSET_LBYTE			20
#define			I2C_LOAD_V_OFFSET_HBYTE			21
#define			I2C_LOAD_I_GAIN_LBYTE			22
#define			I2C_LOAD_I_GAIN_HBYTE			23
#define			I2C_LOAD_I_OFFSET_LBYTE			24
#define			I2C_LOAD_I_OFFSET_HBYTE			25

#define			I2C_EXT_V_GAIN_LBYTE			26
#define			I2C_EXT_V_GAIN_HBYTE		    27
#define			I2C_EXT_V_OFFSET_LBYTE		    28
#define			I2C_EXT_V_OFFSET_HBYTE		    29
#define			I2C_EXT_I_GAIN_LBYTE			30
#define			I2C_EXT_I_GAIN_HBYTE		    31
#define			I2C_EXT_I_OFFSET_LBYTE		    32
#define			I2C_EXT_I_OFFSET_HBYTE		    33

#define			I2C_EXTRA_V_GAIN_LBYTE			34
#define			I2C_EXTRA_V_GAIN_HBYTE			35
#define			I2C_EXTRA_V_OFFSET_LBYTE		36
#define			I2C_EXTRA_V_OFFSET_HBYTE		37
//#define						38
//#define						39
//#define						40
//#define						41
//#define                       42
//#define                       43
#define			I2C_BMS_I2C_LOAD_V_RECONNECT_LBYTE			    44
#define			I2C_BMS_LOAD_V_RECONNECT_HBYTE			        45
#define			I2C_BMS_LOAD_TIME_ACTIONDELAY_DISC_HBYTE	    46
#define			I2C_BMS_I2C_LOAD_V_DISCMIN_LBYTE			    47
#define			I2C_BMS_LOAD_V_DISCMIN_HBYTE			        48
#define			I2C_BMS_BATT_TIME_EMERG_DELAY_DISC_HBYTE	    49
#define			I2C_BMS_BATT_V_EMERGMIN_RECONNECT_LBYTE	        50
#define			I2C_BMS_BATT_V_EMERGMIN_RECONNECT_HBYTE	        51
#define			I2C_BMS_BATT_V_EMERGMIN_LBYTE			        52
#define			I2C_BMS_BATT_V_EMERGMIN_DISC_HBYTE			    53
#define         I2C_BMS_BATT_V_CRITICAL_WARNING_LBYTE           54
#define			I2C_BMS_BATT_V_CRITICAL_WARNING_HBYTE			55
//#define						56
//#define						57
//#define						58
#define			I2C_RTC_YEAR_LBYTE				59
#define			I2C_RTC_YEAR_HBYTE			    60   //1900-2200 (actual value is 1-65536)
#define			I2C_RTC_MONTH			        61   //1-12
#define			I2C_RTC_DAYOFMONTH			    62   //1-31
#define			I2C_RTC_HOUR			        63   //0-23
#define			I2C_RTC_MINUTE			        64   //0-59
#define			I2C_RTC_SECOND			        65   //0-59
#define			I2C_RTC_CHANGES_FLAG		    66   // 0 or 1
//#define			ALARM FUNCTIONS			67
//#define						68
//#define						69
//#define						70
//#define			SOC FUTURE IMPLEMENTATIONS			71
//#define						72
//#define						73
//#define						74
//#define						75
//#define						76
//#define						77
#define			I2C_SOLPANEL_1_OFFSET_LBYTE    		78
#define			I2C_SOLPANEL_1_OFFSET_HBYTE			79
#define			I2C_SOLPANEL_1_GAIN_LBYTE			80
#define			I2C_SOLPANEL_1_GAIN_HBYTE			81
#define			I2C_SOLPANEL_2_OFFSET_LBYTE			82
#define			I2C_SOLPANEL_2_OFFSET_HBYTE			83
#define			I2C_SOLPANEL_2_GAIN_LBYTE			84
#define			I2C_SOLPANEL_2_GAIN_HBYTE			85
#define			I2C_SOLPANEL_3_OFFSET_LBYTE			86
#define			I2C_SOLPANEL_3_OFFSET_HBYTE			87
#define			I2C_SOLPANEL_3_GAIN_LBYTE			88
#define			I2C_SOLPANEL_3_GAIN_HBYTE			89
#define			I2C_SOLPANEL_4_OFFSET_LBYTE			90
#define			I2C_SOLPANEL_4_OFFSET_HBYTE			91
#define			I2C_SOLPANEL_4_GAIN_LBYTE			92
#define			I2C_SOLPANEL_4_GAIN_HBYTE			93
#define			I2C_SOLPANEL_5_OFFSET_LBYTE			94
#define			I2C_SOLPANEL_5_OFFSET_HBYTE			95
#define			I2C_SOLPANEL_5_GAIN_LBYTE			96
#define			I2C_SOLPANEL_5_GAIN_HBYTE			97
#define			I2C_SOLPANEL_6_OFFSET_LBYTE			98
#define			I2C_SOLPANEL_6_OFFSET_HBYTE			99
#define			I2C_SOLPANEL_6_GAIN_LBYTE			100
#define			I2C_SOLPANEL_6_GAIN_HBYTE			101
#define			I2C_SOLPANEL_7_OFFSET_LBYTE			102
#define			I2C_SOLPANEL_7_OFFSET_HBYTE			103
#define			I2C_SOLPANEL_7_GAIN_LBYTE			104
#define			I2C_SOLPANEL_7_GAIN_HBYTE			105
#define			I2C_SOLPANEL_8_OFFSET_LBYTE			106
#define			I2C_SOLPANEL_8_OFFSET_HBYTE			107
#define			I2C_SOLPANEL_8_GAIN_LBYTE			108
#define			I2C_SOLPANEL_8_GAIN_HBYTE			109

/*
#define						110
#define						111
#define						112
#define						113
#define						114
#define						115
#define						116
#define						117
#define						118
#define						119
#define						120
#define						121
#define						122
#define						123
#define						124
#define						125
#define						126
#define						127
*/
// R/Wr
/************************* R/Wr - Read only boundary ***********************/
// Read only

//#define			FOR BATT TEMPERATURE			128
//#define						129
//#define						130
//#define						131
#define			I2C_RTC_SECONDSLEFT_AVERAGE		    132
#define			I2C_BMS_STATE			            133
#define			I2C_I2C_BATT_SOC					134
#define			I2C_I2C_BATT_USABLE_PERCENTAGE		135

#define			I2C_SOLARPANEL_8_AVE_LBYTE			136
#define			I2C_SOLARPANEL_8_AVE_HBYTE			137
#define			I2C_SOLARPANEL_8_MIN_LBYTE			138
#define			I2C_SOLARPANEL_8_MIN_HBYTE			139
#define			I2C_SOLARPANEL_8_MAX_LBYTE			140
#define			I2C_SOLARPANEL_8_MAX_HBYTE			141
#define			I2C_SOLARPANEL_8_LBYTE			    142
#define			I2C_SOLARPANEL_8_HBYTE			    143

#define			I2C_SOLARPANEL_7_AVE_LBYTE			144
#define			I2C_SOLARPANEL_7_AVE_HBYTE			145
#define			I2C_SOLARPANEL_7_MIN_LBYTE			146
#define			I2C_SOLARPANEL_7_MIN_HBYTE			147
#define			I2C_SOLARPANEL_7_MAX_LBYTE			148
#define			I2C_SOLARPANEL_7_MAX_HBYTE			149
#define			I2C_SOLARPANEL_7_LBYTE			    150
#define			I2C_SOLARPANEL_7_HBYTE			    151

#define			I2C_SOLARPANEL_6_AVE_LBYTE			152
#define			I2C_SOLARPANEL_6_AVE_HBYTE			153
#define			I2C_SOLARPANEL_6_MIN_LBYTE			154
#define			I2C_SOLARPANEL_6_MIN_HBYTE			155
#define			I2C_SOLARPANEL_6_MAX_LBYTE			156
#define			I2C_SOLARPANEL_6_MAX_HBYTE			157
#define			I2C_SOLARPANEL_6_LBYTE			    158
#define			I2C_SOLARPANEL_6_HBYTE			    159

#define			I2C_SOLARPANEL_5_AVE_LBYTE			160
#define			I2C_SOLARPANEL_5_AVE_HBYTE			161
#define			I2C_SOLARPANEL_5_MIN_LBYTE			162
#define			I2C_SOLARPANEL_5_MIN_HBYTE			163
#define			I2C_SOLARPANEL_5_MAX_LBYTE			164
#define			I2C_SOLARPANEL_5_MAX_HBYTE			165
#define			I2C_SOLARPANEL_5_LBYTE			    166
#define			I2C_SOLARPANEL_5_HBYTE			    167

#define			I2C_SOLARPANEL_4_AVE_LBYTE			168
#define			I2C_SOLARPANEL_4_AVE_HBYTE			169
#define			I2C_SOLARPANEL_4_MIN_LBYTE			170
#define			I2C_SOLARPANEL_4_MIN_HBYTE			171
#define			I2C_SOLARPANEL_4_MAX_LBYTE			172
#define			I2C_SOLARPANEL_4_MAX_HBYTE			173
#define			I2C_SOLARPANEL_4_LBYTE			    174
#define			I2C_SOLARPANEL_4_HBYTE			    175

#define			I2C_SOLARPANEL_3_AVE_LBYTE			176
#define			I2C_SOLARPANEL_3_AVE_HBYTE			177
#define			I2C_SOLARPANEL_3_MIN_LBYTE			178
#define			I2C_SOLARPANEL_3_MIN_HBYTE			179
#define			I2C_SOLARPANEL_3_MAX_LBYTE			180
#define			I2C_SOLARPANEL_3_MAX_HBYTE			181
#define			I2C_SOLARPANEL_3_LBYTE			    182
#define			I2C_SOLARPANEL_3_HBYTE			    183

#define			I2C_SOLARPANEL_2_AVE_LBYTE			184
#define			I2C_SOLARPANEL_2_AVE_HBYTE			185
#define			I2C_SOLARPANEL_2_MIN_LBYTE			186
#define			I2C_SOLARPANEL_2_MIN_HBYTE			187
#define			I2C_SOLARPANEL_2_MAX_LBYTE			188
#define			I2C_SOLARPANEL_2_MAX_HBYTE			189
#define			I2C_SOLARPANEL_2_LBYTE			    190
#define			I2C_SOLARPANEL_2_HBYTE			    191

#define			I2C_SOLARPANEL_1_AVE_LBYTE			192
#define			I2C_SOLARPANEL_1_AVE_HBYTE			193
#define			I2C_SOLARPANEL_1_MIN_LBYTE			194
#define			I2C_SOLARPANEL_1_MIN_HBYTE			195
#define			I2C_SOLARPANEL_1_MAX_LBYTE			196
#define			I2C_SOLARPANEL_1_MAX_HBYTE			197
#define			I2C_SOLARPANEL_1_LBYTE			    198
#define			I2C_SOLARPANEL_1_HBYTE			    199

#define			I2C_EXTRA_V_AVE_LBYTE			    200
#define			I2C_EXTRA_V_AVE_HBYTE			    201
#define			I2C_EXTRA_V_MIN_LBYTE			    202
#define			I2C_EXTRA_V_MIN_HBYTE			    203
#define			I2C_EXTRA_V_MAX_LBYTE			    204
#define			I2C_EXTRA_V_MAX_HBYTE			    205
#define			I2C_EXTRA_VOLTAGE_LBYTE			    206
#define			I2C_EXTRA_VOLTAGE_HBYTE			    207

#define			I2C_EXT_I_AVE_LBYTE			        208
#define			I2C_EXT_I_AVE_HBYTE			        209
#define			I2C_EXT_I_MIN_LBYTE			        210
#define			I2C_EXT_I_MIN_HBYTE			        211
#define			I2C_EXT_I_MAX_LBYTE			        212
#define			I2C_EXT_I_MAX_HBYTE			        213
#define			I2C_EXT_CURRENT_LBYTE			    214
#define			I2C_EXT_CURRENT_HBYTE			    215

#define			I2C_EXT_V_AVE_LBYTE			        216
#define			I2C_EXT_V_AVE_HBYTE			        217
#define			I2C_EXT_V_MIN_LBYTE			        218
#define			I2C_EXT_V_MIN_HBYTE			        219
#define			I2C_EXT_V_MAX_LBYTE			        220
#define			I2C_EXT_V_MAX_HBYTE			        221
#define			I2C_EXT_VOLTAGE_LBYTE			    222
#define			I2C_EXT_VOLTAGE_HBYTE			    223

#define			I2C_LOAD_I_AVE_LBYTE			    224
#define			I2C_LOAD_I_AVE_HBYTE			    225
#define			I2C_LOAD_I_MIN_LBYTE			    226
#define			I2C_LOAD_I_MIN_HBYTE			    227
#define			I2C_LOAD_I_MAX_LBYTE			    228
#define			I2C_LOAD_I_MAX_HBYTE			    229
#define			I2C_LOAD_CURRENT_LBYTE		    	230
#define			I2C_LOAD_CURRENT_HBYTE		    	231

#define			I2C_LOAD_V_AVE_LBYTE			    232
#define			I2C_LOAD_V_AVE_HBYTE			    233
#define			I2C_LOAD_V_MIN_LBYTE			    234
#define			I2C_LOAD_V_MIN_HBYTE			    235
#define			I2C_LOAD_V_MAX_LBYTE			    236
#define			I2C_LOAD_V_MAX_HBYTE			    237
#define			I2C_LOAD_VOLTAGE_LBYTE		    	238
#define			I2C_LOAD_VOLTAGE_HBYTE		    	239

#define			I2C_BATT_I_AVE_LBYTE			    240
#define			I2C_BATT_I_AVE_HBYTE			    241
#define			I2C_BATT_I_MIN_LBYTE			    242
#define			I2C_BATT_I_MIN_HBYTE			    243
#define			I2C_BATT_I_MAX_LBYTE			    244
#define			I2C_BATT_I_MAX_HBYTE			    245
#define			I2C_BATT_CURRENT_LBYTE		    	246
#define			I2C_BATT_CURRENT_HBYTE			    247

#define			I2C_BATT_V_AVE_LBYTE			    248
#define			I2C_BATT_V_AVE_HBYTE			    249
#define			I2C_BATT_V_MIN_LBYTE			    250
#define			I2C_BATT_V_MIN_HBYTE			    251
#define			I2C_BATT_V_MAX_LBYTE		    	252
#define			I2C_BATT_V_MAX_HBYTE			    253
#define			I2C_BATT_VOLTAGE_LBYTE			    254
#define			I2C_BATT_VOLTAGE_HBYTE			    255
