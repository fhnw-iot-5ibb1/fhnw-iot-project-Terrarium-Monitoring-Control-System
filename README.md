# IoT Engineering
## Terrarium - Monitoring and Control System

A temperatur, humidity and reaction monitoring system with configurable temperature, humidity and alerts.
Control the values with the Terrarium-Box and with the included [WebApplication](https://fhnw-iot-5ibb1.github.io/fhnw-iot-project-nadia-benjamin/TerrariumWebApp.html) which can be used on Mobile and Desktop.


## Introduction
This project is part of the [IoT Engineering](../../../fhnw-iot) course.

* 2-person teams, building an IoT system.
* 32 hours of work per person, 1 prototype.
* 10' presentation of the project at Demo Day.
* Slides, source code and setup steps on GitHub.
* Both team members are able to explain the project.

### Team members

* [Benjamin Brodwolf](https://github.com/BenjaminBrodwolf) 
* [Nadia Kramer](https://github.com/nadiakramer)

## Deliverables

### Source code

[Arduino/ESP8266_DTH11_rgbLED_Device.ino](Arduino/ESP8266_DTH11_rgbLED_Device/ESP8266_DTH11_rgbLED_Device.ino)

[Arduino/ESP8266_Ultrasonic_redLED_Device.ino](Arduino/ESP8266_Ultrasonic_redLED_Device/ESP8266_Ultrasonic_redLED_Device.ino)

##### Setup software
* Set Wi-Fi credentials

    ```
    const char *ssid = "MY_SSID";
    const char *password = "MY_PASSWORD";
    ```
    
 * Set Firebase credentials

    ```
  TODO
    ```
      
    
... (adapt as required)

1) Embedded code / microcontroller firmware.
2) Glue Code used on the gateway or "in the cloud".
3) App or Web UI code, or IoT platform setup steps.

### Presentation
4-slide presentation, PDF format, committed to (this) project repo.

[MY_TEAM_PROJECT_PRESENTATION.pdf](MY_TEAM_PROJECT_PRESENTATION.pdf)

1) Use-case of your project.
2) Reference model of your project.
3) Single slide interface documentation.
4) Issues you faced, how you solved them.

### Live demo
Working end-to-end prototype, "device to cloud", part of your 10' presentation.

[https://TerrariumWebApp.html](https://fhnw-iot-5ibb1.github.io/fhnw-iot-project-nadia-benjamin/TerrariumWebApp.html)

1) Sensor input on a IoT device triggers an event.
2) The event or measurement shows up online, in an app or Web client.
3) The event triggers actuator output on the same or on a separate IoT device.

## Submission deadline
Commit and push to (this) project repo before Demo Day, _13.01.2020, 00:00_.
