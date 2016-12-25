#ifndef RFID_SENSOR_H
#define RFID_SENSOR_H

class RFIDSensor
{
public:
    RFIDSensor();
    ~RFIDSensor();
    void DetectedRFID();
    void RFIDGoingOutOfRange();
};

#endif
