# Accident-alert-system
## Components Needed:

- Arduino Uno
- GPS module (NEO 6M)
- Accelerometer (gy-61)
- GSM module (SIM900A)
- Buzzer(5V)
- Push button

## Circuit Diagram:
![circuit-diagram](https://github.com/user-attachments/assets/9edbe41e-5ace-4f4e-93cf-6d5907c8cf75)


## Steps:

1. Set up the hardware
   - Connect components according to the circuit diagram above
  
2. Install required libraries
   - Open Arduino IDE
   - Go to Sketch > Include Library > Add .ZIP Library
   - Select TinyGPSPlus.zip (It is included in this repository. Just download it)

3. Upload the code
   - Copy the provided code into Arduino IDE
   - Modify the emergency contact number in the `SendMessage()` function
   - Upload the code to your Arduino board

4. Test the system
   - Power on the device
   - Simulate an accident by shaking the accelerometer
   - Verify that the buzzer sounds and an SMS alert is sent

## How it works:

- The system continuously monitors accelerometer data
- If a sudden change in acceleration is detected:
  - The buzzer sounds for 10 seconds (can be cancelled by pressing the button)
  - GPS coordinates are obtained
  - An SMS alert with Google Maps link is sent to the predefined number

## Customization:

Adjust `sensitivity` and `devibrate` variables to fine-tune crash detection

Note: This code includes some unused functions (like `RecieveMessage()`) that can be removed or utilized for future enhancements.
