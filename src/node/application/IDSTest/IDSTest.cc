#include "IDSTest.h"
#include <fstream>
#include <chrono>
#include <time.h>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
using namespace std;
using namespace cv;
using namespace cv::ml;

Define_Module(IDSTest);

void IDSTest::startup()
{
//	packet_rate = par("packet_rate");
//	startupDelay = par("startupDelay");
//	packet_spacing = packet_rate > 0 ? 1 / float (packet_rate) : -1;
//	dataSN = 0;
//	if (packet_spacing > 0 && recipientAddress.compare(SELF_NETWORK_ADDRESS) != 0)
//		setTimer(SEND_PACKET, packet_spacing + startupDelay);
//	else
//		trace() << "Not sending packets";
//	declareOutput("Packets received per node");

	//recipientAddress = par("nextRecipient").stringValue();
	detectionType = par("detectionType");

	// Set up training data
//  int labels[4] = {1, -1, -1, -1};
//  float trainingData[4][2] = { {501, 10}, {255, 10}, {501, 255}, {10, 501} };
//  Mat trainingDataMat(4, 2, CV_32FC1, trainingData);
//  Mat labelsMat(4, 1, CV_32SC1, labels);
//   
//	// train model
//	model = new DetectionModel();
//	model->setFuncId(IDS_ANN);
//	model->train(trainingDataMat, ROW_SAMPLE, labelsMat);
//	
	times = 0;
	if (isSink){			
		// map all sensor node in the network
		

		cum = 0;
		
		//read training data
		int labels[4500];
		float trainingData[4500][6];
		int nor = 0;
		int abnor = 0;
		
		ifstream in("trainData.txt");
    ifstream in1("labelData.txt");
		if(in.is_open() && in1.is_open()){
			int i = 0;
			while(in.good() && !in.eof() && in1.good() && !in1.eof() && i < 4500) {
				for(int j = 0; j < 6; j++){
					in >> trainingData[i][j];
				}
				in1 >> labels[i];
				if(labels[i] == 1){
					nor++;
				}
				else{
					abnor++;
				}
				i++;
			}
			in.close();
			in1.close();
		}
		trace() << "normal data:" << nor;
		trace() << "attack: " << abnor;
//		for(int m = 0; m < 4500; m++){
//			for(int n = 0; n < 6; n++){
//				trace() << trainingData[m][n] << " ";
//			}
//			trace() << labels[m] << "\n";
//		}
		
		Mat labelsMat(4500, 1, CV_32SC1, labels);
		Mat trainingDataMat(4500, 6, CV_32FC1, trainingData);
//		trace() << trainingDataMat;
//		trace() << labelsMat;
		
 		chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();           
		model = new DetectionModel();
		model->setFuncId(IDS_ANN);
		model->train(trainingDataMat, ROW_SAMPLE, labelsMat);
		end = chrono::system_clock::now();
		chrono::duration<double> elapsed_seconds = end-start;
		trace() << "time used: " << elapsed_seconds.count();
		// create agents
		SAgent = new SensorAgent(0);
		SAgent->setCurPlace(SELF_NETWORK_ADDRESS);
		
		if(detectionType == 0){
			trace() << "Compute in Hub";
		}
		else{
			trace() << "Compute on sensors";
			SAgent->setDetectionModel(*model);
			//SAgent.setModel(model)
		}
		
		// send agents to other nodes
		setTimer(SEND_PACKET, 10);
	}
	else{
		//setTimer (REQUEST_SAMPLE, sampleInterval);
//		SAgent = new SensorAgent(0);
//		SAgent->setCurPlace(SELF_NETWORK_ADDRESS);
	}
}

void IDSTest::fromNetworkLayer(ApplicationPacket * genericPacket,
		const char *source, double rssi, double lqi){
	//int sequenceNumber = rcvPacket->getSequenceNumber();

	/*if (recipientAddress.compare(SELF_NETWORK_ADDRESS) == 0) {
		trace() << "Received packet #" << sequenceNumber << " from node " << source;
		collectOutput("Packets received per node", atoi(source));
		//option 1 : test here
	// Packet has to be forwarded to the next hop recipient
	} else {
		//option 2: test here
		ApplicationPacket* fwdPacket = rcvPacket->dup();
		// Reset the size of the packet, otherwise the app overhead will keep adding on
		fwdPacket->setByteLength(0);
		toNetworkLayer(fwdPacket, recipientAddress.c_str());
	}*/
	AppNetInfoExchange_type info = genericPacket->getAppNetInfoExchange();
	string src = info.source.c_str();
	string des = info.destination.c_str();
	trace() << "source: " << src;
	trace() << "destination: " << des;
	if(des.compare(SELF_NETWORK_ADDRESS) == 0){
	
		SensorAgent SA;
		AgentPacket		*rcvPacket = check_and_cast<AgentPacket *>(genericPacket);

		//istringstream is(content);  
    //boost::archive::binary_iarchive ia(is);  
    //ia >> SA;
    	
    char buffer[40000];
    for(int i = 0; i < 40000; i++){
    	buffer[i] = rcvPacket->getContent(i);
    } 
    boost::iostreams::basic_array_source<char> sr(buffer, 40000);  
		boost::iostreams::stream< boost::iostreams::basic_array_source<char> > source(sr);
		boost::archive::binary_iarchive ia(source);
		ia >> SA;
		SA.setCurPlace(SELF_NETWORK_ADDRESS);
		trace() << "Place: " << SA.run();
		
		vector<vector<float> > data = SA.getSamples();
		vector<float> labels = SA.getLabels();
		srand((int)time(0) + atoi(SELF_NETWORK_ADDRESS) + times);
		for(int i = 0; i < 200; i++){
			vector<float> tmp;
			int id = atoi(SELF_NETWORK_ADDRESS);
			int in, out, label;
			while((in = rand()%12) == id);
			while((out = rand()%12) == id);
			int rate = (rand() % 10) + 1;
			int size = (rand() % 6) + 1;
			int modif = rand() % 2;
			if(id != 0 && (in == 0 || in == id - 1) && (out == 0 || out == id + 1) && size < 3 && rate > 0 && rate < 6 && modif == 0) {
        label = 1;
      }
      else if(id == 0 && in > 0 && in < 6 && out > 0 && out < 6 && rate > 0 && rate < 6 && size < 4 && modif == 0){
        label = 1;
      }
      else{
        label = -1;
      }
      tmp.push_back(id);
      tmp.push_back(in);
      tmp.push_back(out);
      tmp.push_back(rate);
      tmp.push_back(size);
      tmp.push_back(modif);
      data.push_back(tmp);
      labels.push_back(label);
		}
		times++;
	  trace() << "This is time: " << times;
		
		
		if(isSink){
			trace() << "agent back";
			
			if(detectionType == 0){
				chrono::time_point<chrono::system_clock> start, end;
				trace() << "Compute in Hub";
				//Mat sampleMat = (Mat_<float>(1,2) << 203,402);
				SA.setSamples(data);
				SA.setLabels(labels);
				vector<vector<float> > testData = SA.getSamples();
				vector<float> testLabels = SA.getLabels();
				trace() << "data size: " << testData.size();
				trace() << "label size: " << testLabels.size();
//				for(auto &x : testData){
//					Mat sampleMat;
//					Mat(x).reshape(1, 1).convertTo(sampleMat, CV_32F);
//					trace() << sampleMat;
//					trace() <<"Predict: " << model->predict(sampleMat);
//				}
				int count = 0;
				float precision;
				start = chrono::system_clock::now(); 
				for(int i = 0; i < testData.size(); i++){
					Mat sampleMat;
					Mat(testData[i]).reshape(1, 1).convertTo(sampleMat, CV_32F);
					trace() << sampleMat;
					double response = model->predict(sampleMat);
					if(response == testLabels[i]){
						count++;
					}
					trace() << "Label:                           " << testLabels[i];
					trace() <<"Predict: " << response;
				}
				end = chrono::system_clock::now();
				chrono::duration<double> elapsed_seconds = end-start;
				trace() << "time used: " << elapsed_seconds.count();
				precision = (float)count / testData.size();
				trace() << "Precision: " << precision;
				cum += precision;
				trace() << "Cum: " << cum;
				if(times < 1){
					vector<vector<float> > newData;
					vector<float> newLabels;
					SA.setSamples(newData);
					SA.setLabels(newLabels);
					SAgent = new SensorAgent(SA);
					setTimer(SEND_PACKET, 10);
				}
			}
			else{
				trace() << "Compute on sensors";
				vector<float> results = SA.getResults();
				for(auto &x :results){
					trace() << "Predict: " << x;
				}
			}
		}
		else{
			trace() << "agent get";
			if(detectionType == 0){
				trace() << "Compute in Hub";
				SA.setSamples(data);
				SA.setLabels(labels);
			}
			else{
				trace() << "Compute on sensors";
				float sampleData[2] = {203, 402};
				float sampleData1[2] = {111, 222};
				vector<vector<float> > data;
				vector<float> tmp(sampleData, sampleData + 2);
				vector<float> tmp1(sampleData1, sampleData1 + 2);
				data.push_back(tmp);
				data.push_back(tmp1);
				SA.setSamples(data);
				
				vector<float> results;
				for(auto &x : data){
					Mat sampleMat;
					Mat(x).reshape(1, 1).convertTo(sampleMat, CV_32F);
					trace() << sampleMat;
					float ret = model->predict(sampleMat);
					trace() <<"Predict: " << ret;
					results.push_back(ret);
				}
				
				SA.setResults(results);
			}
			
			SAgent = new SensorAgent(SA);
			setTimer(SEND_PACKET, 10);
		} 
	}
	else{
		trace() << "error node";
	}
}

void IDSTest::timerFiredCallback(int index)
{
	switch (index) {
		case SEND_PACKET:{
			//trace() << "Sending packet #" << dataSN;
			//toNetworkLayer(createGenericDataPacket(0, dataSN), par("nextRecipient"));
			//dataSN++;
			//setTimer(SEND_PACKET, packet_spacing);
			AgentPacket *pkt = new AgentPacket("pkt", APPLICATION_PACKET);
			 
    	//ostringstream os;  
   	 	//boost::archive::binary_oarchive oa(os);  
    	//oa << *SAgent;  
    	
    	//string content = os.str();
    	//string::iterator it = content.begin();
			//trace() << "Send size: "<< content.size();
			//trace() << "Send: " << content;
			//AgentInfo tmp;
			//tmp.srcNode = SELF_NETWORK_ADDRESS;
			//tmp.content = content; 
			//tmp.desNode = par("nextPlace"); 
			//strcpy(tmp.content, c);
			//pkt->setExtraData(tmp);
			//pkt->setContent(content.c_str());
			//string c = (pkt->getContent()).buffer();
			//trace() << c;
			//pkt->setByteLength (content.size());
			
			char buffer[40000];
			boost::iostreams::basic_array_sink<char> si(buffer, 40000);  
			boost::iostreams::stream< boost::iostreams::basic_array_sink<char> > sink(si);
			boost::archive::binary_oarchive oa(sink);
			oa << *SAgent;
			pkt->setContentArraySize(40000);
			for(int i = 0; i < 40000; i++){
				pkt->setContent(i, buffer[i]);
			}
			
			//pkt->setContent(buffer);
			pkt->setByteLength (40000);
			
			toNetworkLayer(pkt, par("nextPlace").stringValue());
			break;
		}
		case REQUEST_SAMPLE: {
			//setTimer ( REQUEST_SAMPLE, sampleInterval );
			//requestSensorReading();
			break;
		}
		case WAIT_FOR_POSITIONS: {
			break;
		}
	}
}

// This method processes a received carrier sense interupt. Used only for demo purposes
// in some simulations. Feel free to comment out the trace command.
void IDSTest::handleRadioControlMessage(RadioControlMessage *radioMsg)
{
	switch (radioMsg->getRadioControlMessageKind()) {
		case CARRIER_SENSE_INTERRUPT:
			trace() << "CS Interrupt received! current RSSI value is: " << radioModule->readRSSI();
                        break;
	}
}

void IDSTest::requestSensorReading(int index){

}

void IDSTest::handleSensorReading(SensorReadingMessage * sensorMsg)
{
	/*string sensType(sensorMsg->getSensorType());
	double sensValue = sensorMsg->getSensedValue();

	if (isSink) {
		trace() << "Sink recieved SENSOR_READING (while it shouldnt) "
		    << sensValue << " (int)" << (int)sensValue;
		return;
	}

	if (sensValue < reportTreshold) {
		trace() << "Sensed value " << sensValue << " is less than the treshold ("
			<< reportTreshold << "), discarding";
		return;
	}

	currentSampleAccumulated += sampleSize;
	if (currentSampleAccumulated < maxSampleAccumulated) {
		trace() << "Accumulated " << currentSampleAccumulated << "/" << maxSampleAccumulated << " bytes of samples";
		return;
	}

	trace() << "Sending report packet, sequence number " << currSampleSN;
	ApplicationPacket *newPkt = createGenericDataPacket((double)self, currSampleSN, currentSampleAccumulated);
	newPkt->setName(REPORT_PACKET_NAME);
	toNetworkLayer(newPkt, reportDestination.c_str());
	currentSampleAccumulated = 0;
	currSampleSN++;*/
}
