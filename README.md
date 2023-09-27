# Mouse-Motion-Encoder
Using a mouse's optical sensor as a XY motion encoder for physical robots

The sensors inside optical mouses are very accurate xy motion detection devices , they use what can be seen as a camera to calculate the motion seen between pixel captures.
For this project, since the idea is to use the sensor in robots, we are using microcontrollers to interface with the mouse via PS2, which is still an interface supported by most mice, even though the connection is through usb. Through the ps2, we get clock and Data+ connections, where all information is passed. To communicate the library made by Bob Grant (getis) is being used to get X and Y values stored by the sensor.

