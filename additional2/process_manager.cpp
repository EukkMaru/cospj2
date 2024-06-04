// TODO: You should implement this function if you want to change the result of the aggregation
uint8_t *ProcessManager::processData(DataSet *ds, int *dlen)
{
    uint8_t *ret, *p;                               // Pointers for result data and current position
    int num, len;                                   // Variables for number of house data and length
    HouseData *house;                               // Pointer to HouseData object
    Info *info;                                     // Pointer to Info object (not used in current implementation)
    TemperatureData *tdata;                         // Pointer to TemperatureData object
    HumidityData *hdata;                            // Pointer to HumidityData object
    PowerData *pdata;                               // Pointer to PowerData object
    char buf[BUFLEN];                               // Buffer for temporary data
    ret = (uint8_t *)malloc(BUFLEN);                // Allocate memory for the result
    int tmp, min_humid, min_temp, min_power, month; // Variables for temporary data and minimum values
    time_t ts;                                      // Variable for timestamp
    struct tm *tm;                                  // Structure for time information

    tdata = ds->getTemperatureData(); // Get temperature data from dataset
    hdata = ds->getHumidityData();    // Get humidity data from dataset
    num = ds->getNumHouseData();      // Get number of house data entries

    // Example) I will give the minimum daily temperature (1 byte), the minimum daily humidity (1 byte),
    // the minimum power data (2 bytes), the month value (1 byte) to the network manager

    // Example) getting the minimum daily temperature
    min_temp = (int)tdata->getMin(); // Get the minimum temperature and cast to int

    // Example) getting the minimum daily humidity
    min_humid = (int)hdata->getMin(); // Get the minimum humidity and cast to int

    // Example) getting the minimum power value
    min_power = 10000; // Initialize minimum power to a high value
    for (int i = 0; i < num; i++)
    {
        house = ds->getHouseData(i);   // Get each house data entry
        pdata = house->getPowerData(); // Get the power data from each house
        tmp = (int)pdata->getValue();  // Get the power value and cast to int

        if (tmp < min_power) // Update minimum power if current power is lower
            min_power = tmp;
    }

    // Example) getting the month value from the timestamp
    ts = ds->getTimestamp(); // Get the timestamp from dataset
    tm = localtime(&ts);     // Convert timestamp to local time
    month = tm->tm_mon + 1;  // Get the month value (tm_mon is 0-11, so add 1)

    // Example) initializing the memory to send to the network manager
    memset(ret, 0, BUFLEN); // Initialize the result memory with zeros
    *dlen = 0;              // Initialize the length of data to 0
    p = ret;                // Set the pointer to the beginning of the result memory

    // Example) saving the values in the memory
    VAR_TO_MEM_1BYTE_BIG_ENDIAN(min_temp, p);   // Save the minimum temperature (1 byte)
    *dlen += 1;                                 // Increment the data length by 1
    VAR_TO_MEM_1BYTE_BIG_ENDIAN(min_humid, p);  // Save the minimum humidity (1 byte)
    *dlen += 1;                                 // Increment the data length by 1
    VAR_TO_MEM_2BYTES_BIG_ENDIAN(min_power, p); // Save the minimum power (2 bytes)
    *dlen += 2;                                 // Increment the data length by 2
    VAR_TO_MEM_1BYTE_BIG_ENDIAN(month, p);      // Save the month value (1 byte)
    *dlen += 1;                                 // Increment the data length by 1

    return ret; // Return the result memory
}
