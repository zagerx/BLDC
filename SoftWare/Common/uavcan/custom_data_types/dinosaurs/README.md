# Dinosaurs

This namespace contains the application-specific regulated DSDL namespace for PA-AMR, ACRO,
This is the core piece of the [DS-015 UAVCAN Drone Standard](https://github.com/Dronecode/SIG-UAVCAN-Drone).

This namespace contains the following nested namespaces:
actuator bootstrap indicator physics power sensor

## How to use GlobalHealth and NodeManage protocol?

### GlobalHealth
The GlobalHealth protocol is used to publish health of the node.
Each node need publish this message.
It should be noted that error_code field, this field is from a specific equipment.
For example, the node is a motor driver, so error_code field is from actuator/wheel_motor/ErroeCode file.

### NodeManage
The NodeManage protocol is used to enable or disable broadcast or request function of other node.
Each node need subscription this message, and enable or disable the above function.
