#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <ctime>
#include "i2c.hpp"
#include "psoccomm.hpp"
#include "uart.hpp"
#include "cccomm.hpp"

using namespace std;

int main (int argc, char* argv[])
{
	//Menu selection
	char menu_selection;

	//TEST SECTION
	CCComm chargeController;
	PSOCComm psoc;

	while(1)
	{	

		/* THIS CODE IS USED FOR DEBUGGING THE CHARGE CONTROLLER PROTOCOL */

		printf("\nWhat info do you want (Enter the number of your selection): \n\n");
		printf("PSOC:\n");
		printf("1. Battery Voltage\n");
		printf("2. Battery Current\n");
		printf("3. Load Voltage\n");
		printf("4. Load Current\n");
		printf("5. External Voltage\n");
		printf("6. External Current\n");
		printf("7. Extra Voltage\n");
		printf("Charge Controller:\n");
		printf("8. PV Input Voltage\n");
		printf("9. Battery Voltage\n");
		printf("a. Charging Current\n");
		printf("b. Charging Power\n");
		printf("c. Max Output Power\n");
		printf("d. Nominal Battery Voltage\n");
		printf("e. Nominal Charging Current\n");

		cin >> menu_selection;

		/* Transmit the inquiry */
		int tx_count = -1;
		float val = 0;

		switch (menu_selection)
		{
			case '1':
				val = psoc.getBatteryVoltage();
				printf("Battery Voltage: %f\n", val);
				break;
			case '2':
				val = psoc.getBatteryCurrent();
				printf("Battery Current: %f\n", val);
				break;
			case '3':
				val = psoc.getLoadVoltage();
				printf("Load Voltage: %f\n", val);
				break;
			case '4':
				val = psoc.getLoadCurrent();
				printf("Load Current: %f\n", val);
				break;
			case '5':
				val = psoc.getExternalVoltage();
				printf("External Voltage: %f\n", val);
				break;
			case '6':
				val = psoc.getExternalCurrent();
				printf("External Current: %f\n", val);
				break;
			case '7':
				val = psoc.getExtraVoltage();
				printf("Extra Voltage: %f\n", val);
				break;
			case '8':
				val = chargeController.getPVInputVoltage();
				printf("PV Input Voltage: %f\n", val);
				break;
			case '9':
				val = chargeController.getBattVoltage();
				printf("Battery Voltage: %f\n", val);
				break;
			case 'a':
				val = chargeController.getChargingCurrent();
				printf("Charging Current: %f\n", val);
				break;
			case 'b':
				val = chargeController.getChargingPower();
				printf("Charging Power: %f\n", val);
				break;
			case 'c':
				val = chargeController.getMaxOutputPower();
				printf("Max Output Power: %f\n", val);
				break;
			case 'd':
				val = chargeController.getNominalBattVoltage();
				printf("Nominal Battery Voltage: %f\n", val);
				break;
			case 'e':
				val = chargeController.getNominalChargingCurrent();
				printf("Nominal Charging Current: %f\n", val);
				break;
			default:
				printf("Error: Incorrect entry!\n");
		}		
	}
	return 1;
}
