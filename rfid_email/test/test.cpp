#include <iostream>
#include <gtest/gtest.h>

#include "WhoIsHome.h"
#include "FakeRFIDSensor.h"

using namespace FakeSensorNamespace;
using namespace WhoIsHomeService;

static const int NOTHING_REGISTERD = -1; 

class WhoIsHomeTestSuite : public ::testing::Test
{
public:
    void SetUp();
    void TearDown();

    std::unique_ptr<WhoIsHome> m_notification_service;
};


void WhoIsHomeTestSuite::SetUp()
{
    m_notification_service.reset( new WhoIsHome() );
}       

void WhoIsHomeTestSuite::TearDown()
{
}       


TEST_F( WhoIsHomeTestSuite, FreshStartMeansNoEmailsSent )
{
    EXPECT_EQ( NOTHING_REGISTERD, FakeSensor_LastDetected() );
}


TEST_F( WhoIsHomeTestSuite, RFIDDetectedBySensorArbitraryUser )
{

}
