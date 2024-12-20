### Test Case 1: Initial Boot with No Wi-Fi Configuration
1. **Precondition**: Ensure no Wi-Fi credentials are saved in NVS.
2. **Steps**:
   - Power on the device.
   - Observe the device attempting to connect to Wi-Fi.
3. **Expected Result**:
   - The device should prompt the user to enter Wi-Fi credentials.
   - The UI should display a message indicating that Wi-Fi configuration is required.

### Test Case 2: Successful Wi-Fi Connection at Boot
1. **Precondition**: Ensure valid Wi-Fi credentials are saved in NVS.
2. **Steps**:
   - Power on the device.
   - Observe the device connecting to Wi-Fi.
3. **Expected Result**:
   - The device should successfully connect to the Wi-Fi network.
   - The UI should display a message indicating successful connection.
   - The device should proceed to synchronize system time and prayer times.

### Test Case 3: Wi-Fi Connection Failure at Boot
1. **Precondition**: Ensure invalid Wi-Fi credentials are saved in NVS.
2. **Steps**:
   - Power on the device.
   - Observe the device attempting to connect to Wi-Fi.
3. **Expected Result**:
   - The device should fail to connect to the Wi-Fi network.
   - The UI should display a message indicating connection failure.
   - The device should prompt the user to re-enter Wi-Fi credentials.

### Test Case 4: Wi-Fi Reconnection After Disconnection
1. **Precondition**: Ensure valid Wi-Fi credentials are saved in NVS and the device is connected to Wi-Fi.
2. **Steps**:
   - Disconnect the Wi-Fi network (e.g., turn off the router).
   - Wait for the device to detect the disconnection.
   - Reconnect the Wi-Fi network (e.g., turn on the router).
3. **Expected Result**:
   - The device should detect the Wi-Fi disconnection and attempt to reconnect.
   - The UI should display a message indicating reconnection attempts.
   - The device should successfully reconnect to the Wi-Fi network.

### Test Case 5: Wi-Fi Connection Failure During Reconnect
1. **Precondition**: Ensure valid Wi-Fi credentials are saved in NVS and the device is connected to Wi-Fi.
2. **Steps**:
   - Disconnect the Wi-Fi network (e.g., turn off the router).
   - Wait for the device to detect the disconnection.
   - Change the Wi-Fi password on the router.
   - Reconnect the Wi-Fi network (e.g., turn on the router).
3. **Expected Result**:
   - The device should detect the Wi-Fi disconnection and attempt to reconnect.
   - The UI should display a message indicating reconnection attempts.
   - The device should fail to reconnect due to the incorrect password.
   - The UI should prompt the user to re-enter Wi-Fi credentials.

### Test Case 6: System Time Synchronization
1. **Precondition**: Ensure the device is connected to Wi-Fi.
2. **Steps**:
   - Observe the device synchronizing system time with NTP servers.
3. **Expected Result**:
   - The device should successfully synchronize system time.
   - The UI should display the current system time.
   - The device should proceed to synchronize prayer times.

### Test Case 7: Prayer Times Synchronization
1. **Precondition**: Ensure the device is connected to Wi-Fi and system time is synchronized.
2. **Steps**:
   - Observe the device synchronizing prayer times with the server.
3. **Expected Result**:
   - The device should successfully retrieve and display prayer times.
   - The UI should display the prayer times for Fajr, Sunrise, Dhuhr, Asr, Maghrib, and Isha.

### Test Case 8: Prayer Times Update Notification
1. **Precondition**: Ensure the device is connected to Wi-Fi and prayer times are synchronized.
2. **Steps**:
   - Wait for the next prayer time to approach.
   - Observe the device updating the prayer times.
3. **Expected Result**:
   - The device should notify the user of the upcoming prayer time.
   - The UI should highlight the next prayer time and display the remaining time.

### Test Case 9: Manual Wi-Fi Configuration Change
1. **Precondition**: Ensure the device is connected to Wi-Fi.
2. **Steps**:
   - Access the Wi-Fi settings via the UI.
   - Change the Wi-Fi credentials to a new network.
   - Save the new credentials.
3. **Expected Result**:
   - The device should disconnect from the current Wi-Fi network.
   - The device should attempt to connect to the new Wi-Fi network.
   - The UI should display a message indicating successful connection to the new network.

### Test Case 10: System Time and Prayer Times Update After Reconnect
1. **Precondition**: Ensure the device is connected to Wi-Fi and prayer times are synchronized.
2. **Steps**:
   - Disconnect the Wi-Fi network (e.g., turn off the router).
   - Wait for the device to detect the disconnection.
   - Reconnect the Wi-Fi network (e.g., turn on the router).
3. **Expected Result**:
   - The device should successfully reconnect to the Wi-Fi network.
   - The device should re-synchronize system time and prayer times.
   - The UI should display the updated system time and prayer times.

These test cases cover various scenarios related to Wi-Fi connectivity, system time synchronization, and prayer times synchronization. They ensure that the device handles different situations gracefully and provides appropriate feedback to the user via the UI.