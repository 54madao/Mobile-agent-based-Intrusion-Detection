package node.application.IDSTest;

simple IDSTest like node.application.iApplication {
parameters:

	// para from iApplication
 	string applicationID = default ("IDSTest");
	bool collectTraceInfo = default (false);
	int priority = default (1);
	int packetHeaderOverhead = default (0);		// in bytes
	int constantDataPayload = default (100);	// in bytes
	
	bool isSink			= default (false);
	
	// self defined
	string nextRecipient = default ("0");	// Destination for packets produced in this node. 
											// This parameter can be used to create an
											// application-level static routing. This way we can
											// have a multi-hop throughput test 
	string nextPlace = default("0");  

	double packet_rate = default (0);	// packets per second, by default we transmit no packets
	double startupDelay = default (0);	// delay in seconds before the app stars producing packets
	int agentsNum = default (1);
	int detectionType = default (0);
	double latencyHistogramMax = default (200);
	int latencyHistogramBuckets = default (10);

 gates:
 	output toCommunicationModule;
	output toSensorDeviceManager;
	input fromCommunicationModule;
	input fromSensorDeviceManager;
	input fromResourceManager;
}

