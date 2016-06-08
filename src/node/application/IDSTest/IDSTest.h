#ifndef _IDSTEST_H_
#define _IDSTEST_H_

#include "VirtualApplication.h"
#include "AgentPacket_m.h"
#include "SensorAgent.h"
#include "DetectionModel.h"
//#include <opencv2/core/core.hpp>
//#include <dlib/svm.h>
//#include <libsvm-3.20/svm.h>

using namespace std;

enum IDSTestTimers {
	REQUEST_SAMPLE		=	1,
	REQUEST_POSITIONS	=	2,
	SEND_PACKET		=	3,
	FIND_PATH		=	4,
	WAIT_FOR_POSITIONS	=	5,
};

class IDSTest: public VirtualApplication {
 private:
	double packet_rate;
	string recipientAddress;
	string nextPlace;
	double startupDelay;
	int detectionType;

	float packet_spacing;
	int dataSN;
	int agentsNum;
	int nodeId;
	SensorAgent *SAgent;
	DetectionModel *model;
	vector<float> info; // in, out, rate, size, modif
	int label;
	float cum;
	int times;

 protected:
	void startup();
	void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
	void handleRadioControlMessage(RadioControlMessage *);
	void timerFiredCallback(int);
	void requestSensorReading(int index);
	void handleSensorReading(SensorReadingMessage *);
};

#endif
