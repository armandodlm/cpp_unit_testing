#ifndef RFID_SENSOR_H
#define RFID_SENSOR_H

class RFIDSensor
{
public:
    virtual ~RFIDSensor() { }
    void DetectedRFID() = 0;
    //void RFIDGoingOutOfRange() = 0;
};

#endif
