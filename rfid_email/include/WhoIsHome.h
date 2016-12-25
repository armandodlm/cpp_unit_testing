#ifndef WHO_IS_HOME_H
#define WHO_IS_HOME_H

#include <memory>
#include "RFIDSensor.h"

namespace WhoIsHomeService
{

class WhoIsHome
{
public:
    void NotifyOwner();
private:
    std::unique_ptr<RFIDSensor> m_sensor;
};

}

#endif
