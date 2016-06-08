#include "DetectionModel.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

void DetectionModel::train(Mat const& samples, int layout, Mat const& labels){
	switch(funcId){
		case IDS_SVM: {
			// Train the SVM
    	//! [init]
    	Ptr<SVM> svm = SVM::create();
    	//SVM *svm = SVM::create();
    	svm->setType(SVM::C_SVC);
    	svm->setKernel(SVM::RBF);
    	//svm->setNu(0.5);
    	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, 1e-6));
    	//! [init]
    	//! [train]
    	//convertTo CV_32S if C_SVC or NU_SVC
    	svm->train(samples, layout, labels);
    	//! [train]
    	//trainer = svm.staticCast<void>();
    	trainer = svm;
			break;
		}
		case IDS_KMEANS: {
			break;
		}
		case IDS_KNN: {
    	Ptr<KNearest> knn = KNearest::create();
    	knn->setIsClassifier(true);
			knn->setAlgorithmType(KNearest::Types::BRUTE_FORCE);
			knn->setDefaultK(67); // square root of rows
    	knn->train(samples, layout, labels);
    	//trainer = knn.staticCast<void>();
    	trainer = knn;
			break;
		}
		case IDS_ANN: {
//			Mat ann_labels;
//			if(labels.type() != CV_32F && labels.type() != CV_64F){
//					
//				labels.convertTo(ann_labels, CV_32F);
//			}
//			else{
//				labels.copyTo(ann_labels);
//			}
			
			Mat ann_labels = Mat::zeros( labels.rows, 2, CV_32FC1 );
			for( int i = 0; i < labels.rows; i++ ){
			  int mark = labels.at<int>(i) == 1 ? 0 : 1;
        ann_labels.at<float>(i, mark) = 1.f;
    	}
			
    	Ptr<ANN_MLP> ann = ANN_MLP::create();
    	
    	int nInputs = samples.cols;
    	int nOutputs = ann_labels.cols;
    	int nInter = (samples.rows / 5) < 2 ? 2 : (samples.rows / 5);
    	int layers[3] = {nInputs, nInter, nOutputs};
    	Mat layersMat(1, 3, CV_32SC1, layers);
    	
    	ann->setLayerSizes(layersMat);
    	//setActivationFunction 1,1
    	ann->setActivationFunction(ANN_MLP::SIGMOID_SYM, 1, 1);
    	ann->setTrainMethod(ANN_MLP::BACKPROP, 0.001); //0.001
    	ann->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, 0.01));
    	
    	ann->train(samples, layout, ann_labels);
    	//trainer = ann.staticCast<void>();
    	trainer = ann;
			break;
		}
		case IDS_NBC: {
    	Ptr<NormalBayesClassifier> nbc = NormalBayesClassifier::create();
    	
    	nbc->train(samples, layout, labels);
    	//trainer = nbc.staticCast<void>();
    	trainer = nbc;
			break;
		}
		case IDS_DT: {
			Ptr<DTrees> dtree = DTrees::create();
    	dtree->setMaxDepth(10);
    	dtree->setMinSampleCount(4);
    	dtree->setUseSurrogates(false);
    	dtree->setCVFolds(0); // the number of cross-validation folds
    	dtree->setUse1SERule(false);
    	dtree->setTruncatePrunedTree(false);
    	dtree->train(samples, layout, labels);
			trainer = dtree;
			break;
		}
	}
}

double DetectionModel::predict(Mat const& sample){
//	switch(funcId){
//		case IDS_SVM: {
//			Ptr<SVM> svm = trainer.staticCast<SVM>();
//			return svm->predict(sample);
//			break;
//		}
//		case IDS_KMEANS: {
//			break;
//		}
//		case IDS_KNN: {
//			Ptr<KNearest> knn = trainer.staticCast<KNearest>();
//			return knn->predict(sample);
//			break;
//		}
//		case IDS_ANN: {
//			Ptr<ANN_MLP> ann = trainer.staticCast<ANN_MLP>();
//			return ann->predict(sample);
//			break;
//		}
//		case IDS_NBC: {
//			Ptr<NormalBayesClassifier> nbc = trainer.staticCast<NormalBayesClassifier>();
//			return nbc->predict(sample);
//			break;
//		}
//		case IDS_DT: {
//			Ptr<DTrees> dt = trainer.staticCast<DTrees>();
//			return dt->predict(sample);
//			break;
//		}
//	}
	if(funcId == IDS_ANN){
		return trainer->predict(sample) == 0 ? 1 : -1;
	}
	else{
		return (double) trainer->predict(sample);
	}
	
}
