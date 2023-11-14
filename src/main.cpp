
#include <XViewer.h>

// Create XViewer object.
XViewer xv;

// Create an IPAddress object to assign to the webserver.
IPAddress ip(192, 168, 1, 2);

// Creating wifi variables.
char ssid[] = "ssid";
char pass[] = "password"; // Minimum 8 characters

// Creating users variables.
char user1[] = "User_01";
char pass1[] = "Pass_01";
char newPass1[] = "NewPass1";

char user2[] = "User_02";
char pass2[] = "Pass_02";

char user3[] = "User_03";
char pass3[] = "Pass_03";

// Creating productions variables.
char pCode1[] = "Prod_code_01";
char pName1[] = "Prod_name_01";
char pCustomer1[] = "Prod_customer_01";
char pNewCustomer1[] = "Prod_NewCustomer_01";

char pCode2[] = "Prod_code_02";
char pName2[] = "Prod_name_02";
char pNewName2[] = "Prod_NewName_02";
char pCustomer2[] = "Prod_customer_02";

// Creating alarm variables.
char aCode1[] = "Alarm_code_01";
char aName1[] = "Alarm_name_01";

char aCode2[] = "Alarm_code_02";
char aName2[] = "Alarm_name_02";
char aNewName2[] = "Alarm_NewName_02";

char aCode3[] = "Alarm_code_03";
char aName3[] = "Alarm_name_03";

// Creating sensors variables.
uint8_t sCode1 = 1;
char sName1[] = "Sens_name_01";
char sType1[] = "Sens_type_01";

uint8_t sCode2 = 2;
char sName2[] = "Sens_name_02";
char sNewName2[] = "Sens_NewName_02";
char sType2[] = "Sens_type_02";

uint8_t sCode3 = 3;
char sName3[] = "Sens_name_03";
char sType3[] = "Sens_type_03";

uint8_t sCode4 = 4;
char sName4[] = "Sens_name_04";
char sType4[] = "Sens_type_04";
char sNewType4[] = "Sens_NewType_04";

uint8_t sCode5 = 5;
char sName5[] = "Sens_name_05";
char sType5[] = "Sens_type_05";

void setup()
{
    // Configure serial port for debugging and wait for two seconds.
    Serial.begin(115200);
    thread_sleep_for(2000);

    // Configure the server's IP address and the SSID and password for the WiFi network.
    xv.config(ip, ssid, pass);

    // Initialize the library
    xv.init();

    // Enable debug for the user section
    xv.setUserDbg();

    // Add users by specifying their permission level.
    xv.addUser(user1, pass1, true);
    xv.addUser(user2, pass2, false);
    xv.addUser(user3, pass3, false);

    // Update user1's password to newPass1.
    xv.updateUserPass(user1, newPass1);

    // Set user2 as working user
    xv.setWorkingUser(user2);

    // Printing all user just added.
    xv.printUsers();

    // Disable debug for the user section
    xv.setUserDbg(false);

    // Enable debug for the production section
    xv.setProdDbg();

    // Add productions.
    xv.addProd(pCode1, pName1, pCustomer1);
    xv.addProd(pCode2, pName2, pCustomer2);

    // Updates the 'total ok' counter of production 2 to 30.
    xv.updateProdCounter(pCode2, T_OK, 30);

    // Updates the 'partial ok' counter of production 2 to 15.
    xv.updateProdCounter(pCode2, P_OK, 15);

    // Increases the 'total nok' counter of production 1 by one.
    xv.updateProdCounter(pCode1, T_NOK);

    // Update production customer 1 with new customer.
    xv.updateProdCustomer(pCode1, pNewCustomer1);

    // Update production name 2 with new name.
    xv.updateProdName(pCode2, pNewName2);

    // Set production 2 as currently in use.
    xv.setCurrentProd(pCode2);

    // Printing all production just added.
    xv.printProds();

    // Disable debug for the production section
    xv.setProdDbg(false);

    // Enable debug for the alarm section
    xv.setAlarmDbg();

    // Add alarms.
    xv.addAlarm(aCode1, aName1);
    xv.addAlarm(aCode2, aName2);
    xv.addAlarm(aCode3, aName3);

    // Update the name of Alarm 2 with the new name.
    xv.updateAlarmName(aCode2, aNewName2);

    // Update alarm counter 1 to 10.
    xv.updateAlarmCounter(aCode1, 10);

    // Increases the alarm counter 2 by 1
    xv.updateAlarmCounter(aCode2);

    // Printing all alarms just added.
    xv.printAlarms();

    // Disable debug for the alarm section
    xv.setAlarmDbg(false);

    // Enable debug for the sensor section
    xv.setSensDbg();

    // Add sensor.
    xv.addSensor(sCode1, sName1, sType1);
    xv.addSensor(sCode2, sName2, sType2);
    xv.addSensor(sCode3, sName3, sType3);
    xv.addSensor(sCode4, sName4, sType4);
    xv.addSensor(sCode5, sName5, sType5);

    // Update sensor name 2.
    xv.updateSensName(sCode2, sNewName2);

    // Update sensor type 4.
    xv.updateSensType(sCode4, sNewType4);

    // Updates the status of sensors 2, 4, and 5 to true.
    xv.updateSensState(sCode2, true);
    xv.updateSensState(sCode4, true);
    xv.updateSensState(sCode5, true);

    // Printing all sensors just added.
    xv.printSensors();

    // Disable debug for the sensor section
    xv.setSensDbg(false);

    // Debugging enabled for loop sections.
    xv.setSensDbg();
    xv.setProdDbg();
    xv.setAlarmDbg();
}

uint8_t x = 0;
bool sensSate = false;
processPhaseEnum currentPhase = PROCESS_IDLE;

void loop()
{
    // Call 'handleClient' in a loop to listen for requests from the application.
    xv.handleClient();

    // Just an example for using loop methods.
    if (x < 20 * 10) // 50ms * 20 = 1sec -> false every 10sec
    {
        x++;
    }
    else
    {
        x = 0;

        // Randomise the status of sensors 3 and 5.
        sensSate = !sensSate;
        xv.updateSensState(sCode3, sensSate);
        xv.updateSensState(sCode5, sensSate);

        // Increasing the T_OK and P_OK counts of Production 2.
        xv.updateProdCounter(pCode2, T_OK);
        xv.updateProdCounter(pCode2, P_OK);

        // Change the process step to the next one every 10 sec. Display alarm 3 in case of 'PROCESS_ERROR'.
        if (currentPhase == PROCESS_IDLE)
            currentPhase = PROCESS_ONGOING;
        else if (currentPhase == PROCESS_ONGOING)
            currentPhase = PROCESS_OK;
        else if (currentPhase == PROCESS_OK)
            currentPhase = PROCESS_ERROR;
        else if (currentPhase == PROCESS_ERROR)
            currentPhase = PROCESS_IDLE;
        xv.setProcessStatus(currentPhase, aCode3);

        // if the process phase is set as PROCESS_ERROR, I increase the corresponding alarm counter
        if (currentPhase == PROCESS_ERROR)
            xv.updateAlarmCounter(aCode3);
    }

    thread_sleep_for(50);
}
