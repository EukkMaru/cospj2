int main(int argc, char *argv[])
{
    DataReceiver *dr;                                         // Pointer to a DataReceiver object
    DataSet *ds;                                              // Pointer to a DataSet object
    HouseData *house;                                         // Pointer to a HouseData object
    TemperatureData *tdata;                                   // Pointer to a TemperatureData object
    HumidityData *hdata;                                      // Pointer to a HumidityData object
    PowerData *pdata;                                         // Pointer to a PowerData object
    int num, tmp;                                             // Integer variables for various uses
    time_t curr, ts;                                          // Time variables
    double max_temp, avg_temp, min_temp;                      // Variables for temperature data
    double max_humid, avg_humid, min_humid;                   // Variables for humidity data
    double power, sum_power, avg_power, max_power, min_power; // Variables for power data
    unsigned char buf[BUFLEN];                                // Buffer for storing data
    unsigned char *p;                                         // Pointer for buffer manipulation

    curr = 1609459200;         // Set current time (example value)
    dr = new DataReceiver();   // Instantiate a DataReceiver object
    ds = dr->getDataSet(curr); // Get the DataSet for the current time

    // 1. Write a statement to get the timestamp value to 'ts' and print out the value (please refer to dataset.h)
    ts = ds->getTimestamp();             // Get the timestamp from the dataset
    cout << "timestamp: " << ts << endl; // Print the timestamp

    // 2. Write a statement to get the number of house data that contains the private information and the power value to 'num' (dataset.h)
    num = ds->getNumHouseData();                // Get the number of house data entries
    cout << "# of house data: " << num << endl; // Print the number of house data entries

    // 3. Write a statement to get the first house data to 'house' (please refer to dataset.h)
    house = ds->getHouseData(0); // Get the first house data

    // Write a statement to get the 10th house data to 'house' (dataset.h)
    house = ds->getHouseData(9); // Get the 10th house data

    // Get the power data to 'pdata' (house_data.h)
    pdata = house->getPowerData(); // Get the power data from the house data

    // Get the daily power value to 'power' and print out the value (power_data.h)
    power = pdata->getValue();          // Get the power value
    cout << "Power: " << power << endl; // Print the power value

    // Explicitly cast the type from double to int and assign it to 'tmp', and print out the value
    tmp = (int)pdata->getValue();              // Cast the power value to int
    cout << "Power (casted): " << tmp << endl; // Print the casted power value

    // Compute the value averaged over all the power data by using 'sum_power' and 'num',
    // assign the average value to 'avg_power', and print out the value
    sum_power = 0; // Initialize sum of power
    for (int i = 0; i < num; i++)
    {
        house = ds->getHouseData(i);    // Get each house data
        pdata = house->getPowerData();  // Get the power data from each house
        sum_power += pdata->getValue(); // Accumulate the power values
    }
    avg_power = sum_power / num;                  // Calculate the average power
    cout << "Power (avg): " << avg_power << endl; // Print the average power

    // Find the maximum value among all the power data
    max_power = -1; // Initialize maximum power to a low value
    for (int i = 0; i < num; i++)
    {
        house = ds->getHouseData(i);   // Get each house data
        pdata = house->getPowerData(); // Get the power data from each house
        power = pdata->getValue();     // Get the power value

        if (power > max_power) // Update maximum power if the current power is greater
            max_power = power;
    }
    cout << "Power (max): " << max_power << endl; // Print the maximum power

    // Find the minimum value among all the power data
    min_power = 10000; // Initialize minimum power to a high value
    for (int i = 0; i < num; i++)
    {
        house = ds->getHouseData(i);   // Get each house data
        pdata = house->getPowerData(); // Get the power data from each house
        power = pdata->getValue();     // Get the power value

        if (power < min_power) // Update minimum power if the current power is lower
            min_power = power;
    }
    cout << "Power (min): " << min_power << endl; // Print the minimum power

    // 4. Write a statement to get the temperature data to 'tdata' (dataset.h)
    tdata = ds->getTemperatureData(); // Get the temperature data

    // Get the maximum value of the daily temperature (temperature_data.h)
    max_temp = tdata->getMax();                        // Get the maximum temperature value
    cout << "Temperature (max): " << max_temp << endl; // Print the maximum temperature

    // Get the average value of the daily temperature (temperature_data.h)
    avg_temp = tdata->getValue();                      // Get the average temperature value
    cout << "Temperature (avg): " << avg_temp << endl; // Print the average temperature

    // Get the minimum value of the daily temperature (temperature_data.h)
    min_temp = tdata->getMin();                        // Get the minimum temperature value
    cout << "Temperature (min): " << min_temp << endl; // Print the minimum temperature

    // Explicitly cast the type of the maximum value from double to int, assign the resultant value to 'tmp', and print it out
    tmp = (int)tdata->getMax();                           // Cast the maximum temperature value to int
    cout << "Temperature (max, casted): " << tmp << endl; // Print the casted maximum temperature value

    // 5. Write a statement to get the humidity data to 'hdata' (dataset.h)
    hdata = ds->getHumidityData(); // Get the humidity data

    // Get the maximum value of the daily humidity (humidity_data.h)
    max_humid = hdata->getMax();                     // Get the maximum humidity value
    cout << "Humidity (max): " << max_humid << endl; // Print the maximum humidity

    // Get the average value of the daily humidity (humidity_data.h)
    avg_humid = hdata->getValue();                   // Get the average humidity value
    cout << "Humidity (avg): " << avg_humid << endl; // Print the average humidity

    // Get the minimum value of the daily humidity (humidity_data.h)
    min_humid = hdata->getMin();                     // Get the minimum humidity value
    cout << "Humidity (min): " << min_humid << endl; // Print the minimum humidity

    // Explicitly cast the type of the minimum value from double to int, assign the resultant value to 'tmp', and print it out
    tmp = (int)hdata->getMin();                        // Cast the minimum humidity value to int
    cout << "Humidity (min, casted): " << tmp << endl; // Print the casted minimum humidity value

    // 6. Initialize the buffer 'buf' with zeros (its length is defined as BUFLEN) (use the memset() function, please google it!)
    memset(buf, 0, BUFLEN); // Initialize buffer with zeros

    // 7. Write statements to save the values into 'buf' using 'p' as follows:
    // # of house data (2 bytes) || maximum power (integer) (4 bytes) || maximum temperature (integer) (2 bytes)
    // Print out the buffer
    // Please use the macros defined in edge/byte_op.h
    p = buf;                              // Set pointer to the beginning of the buffer
    VAR_TO_MEM_2BYTES_BIG_ENDIAN(num, p); // Save the number of house data as 2 bytes in big-endian order
    tmp = (int)max_power;                 // Cast the maximum power value to int
    VAR_TO_MEM_4BYTES_BIG_ENDIAN(tmp, p); // Save the maximum power as 4 bytes in big-endian order
    tmp = (int)max_temp;                  // Cast the maximum temperature value to int
    VAR_TO_MEM_2BYTES_BIG_ENDIAN(tmp, p); // Save the maximum temperature as 2 bytes in big-endian order

    tmp = p - buf;       // Calculate the size of the data written to the buffer
    PRINT_MEM(buf, tmp); // Print the buffer contents

    return 0; // Return 0 to indicate successful completion
}
