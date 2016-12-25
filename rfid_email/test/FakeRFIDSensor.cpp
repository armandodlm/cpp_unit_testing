#include "FakeRFIDSensor.h"
#include "RFIDSensor.h"

namespace FakeSensorNamespace
{

// Could this become its own class?

static int LastDetected = -1;

int FakeSensor_LastDetected()
{
    return LastDetected;
}

void FakeSensor_SetLastDetected( int id )
{
    LastDetected = id;
}

}


RFIDSensor::RFIDSensor()
{
    FakeSensorNamespace::FakeSensor_SetLastDetected( -1 );
}

RFIDSensor::~RFIDSensor()
{
}
