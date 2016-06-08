#ifndef SENSORAGENT_H
#define SENSORAGENT_H

#include "IAgent.h"
#include <sstream>
// include headers that implement a archive in simple text format
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
using namespace std;

class SensorAgent : public IAgent{
friend class boost::serialization::access;
public:
	//IAgent(int agentId, int nextPlace):agentId(agentId), agentType(0),nextPlace(nextPlace){};
	SensorAgent(void){};
	SensorAgent(int agentId):IAgent(agentId, 0){};
	SensorAgent(SensorAgent & sa):IAgent(sa.getId(), sa.getAgentType(), sa.getCurPlace(), sa.getSamples(), sa.getResults(), sa.getDetectionModel(), sa.getLabels()){};
	virtual string run();

	virtual ~SensorAgent(void){};
	

private:
  template<class Archive>  
  void serialize(Archive& ar, const unsigned int version){  
  // serialize base class information  
  	ar & boost::serialization::base_object<IAgent>(*this);  
  }
};


/*class SensorAgent{
friend class boost::serialization::access;
public:
	SensorAgent(void){};
	SensorAgent(int agentId):agentId(agentId), agentType(0){};
	virtual ~SensorAgent(void){};
	string run();
	void setId(int agentId){this->agentId = agentId;};
	int getId(){return this->agentId;};
	void setCurPlace(string curPlace){this->curPlace = curPlace;};
	string getCurPlace(){return curPlace;};
	//int getNextPlace(){return this->nextPlace};
	
	template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
  	ar & agentId;
    ar & curPlace;
    ar & agentType;
  }
private:
	int agentId;
	string curPlace;
	//int nextPlace;
	int agentType; // 0 -> SA; 1 -> CA
};*/

#endif
