# PriusDrive Controller Hardware Specifications

## Main Functionality
- One three-phase motor control output
- CAN communication interface
- Current monitoring
    + Drive control circuit current monitoring
    + Motor circuit current monitoring
- Temperature monitoring
    + Drive temperature monitoring
    + Motor temperature monitoring
    + Two coolant loop temperature monitors
- Voltage monitoring
    + DC bus voltage monitoring
    + System supply voltage monitoring (12V supply)
- Dual analogue throttle inputs
- Onboard datalogging - SPI EEPROM/Flash
- Water pump control interface
- Fan control interface
- Emergency stop interface
- Hall effect sensor interface
- GPIO fault output
- Boost converter control

## Design Requirements
- All inputs should be TVS protected
- Controller should default to a safe state if something goes wrong
- Controller should be reasonably noise immune