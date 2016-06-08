#ifndef IAGENT_H
#define IAGENT_H

#include "DetectionModel.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>
using namespace std;

class IAgent{
friend class boost::serialization::access;
public:
	IAgent(void){};
	IAgent(int agentId, int agentType):agentId(agentId), agentType(agentType){};
	IAgent(int agentId, int agentType, string curPlace, vector<vector<float> > detectionSamples, vector<float> results, DetectionModel dm, vector<float> labels):agentId(agentId), agentType(agentType), curPlace(curPlace), detectionSamples(detectionSamples), results(results), dm(dm), labels(labels){};
	virtual ~IAgent(void){};
	virtual string run() = 0;

	void setId(int agentId){this->agentId = agentId;};
	int getId() const {return this->agentId;};
	void setCurPlace(string curPlace){this->curPlace = curPlace;};
	string getCurPlace() const {return curPlace;};
	void setAgentType(int type){agentType = type;};
	int getAgentType() const {return agentType;};
	void setDetectionModel(DetectionModel dm){this->dm = dm;};
	DetectionModel getDetectionModel() const {return dm;};
	void setSamples(vector<vector<float> > ds){detectionSamples = ds;};
	vector<vector<float> > getSamples() const {return detectionSamples;};
	void setResults(vector<float> results){this->results = results;};
	vector<float> getResults() const {return results;};
	void setLabels(vector<float> labels){this->labels = labels;};
	vector<float> getLabels() const {return labels;};
	//int getNextPlace(){return this->nextPlace};
	
	template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
  	ar & agentId;
    ar & curPlace;
    ar & agentType;
    ar & dm;
    ar & detectionSamples;
    ar & results;
    ar & labels;
  }
private:
	int agentId;
	string curPlace;
	//int nextPlace;
	int agentType; // 0 -> SA; 1 -> CA
	vector<vector<float> > detectionSamples;
	vector<float> results;
	vector<float> labels;
	DetectionModel dm;
};

#endif
