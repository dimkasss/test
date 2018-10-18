#include <iostream>

#include "exported.h"

#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"

#include "boost/filesystem.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

// recursive scan input folders
std::vector<std::string> getAllFiles(char *rootFolder)
{
	std::vector<std::string> filesList;

	// root folder
	std::string root(rootFolder);
	std::cout << "ROOT FOLDER : " << root << std::endl;

	// check root folder
	if (!boost::filesystem::is_directory(boost::filesystem::path(std::string(root))))
	{
		throw std::runtime_error("Error: Root directory is not dir.");
	}

	// recursivly scan root folder on images content

	if (!root.empty())
	{
		boost::filesystem::path pathR(root);
		boost::filesystem::recursive_directory_iterator end;

		for (boost::filesystem::recursive_directory_iterator i(pathR); i != end; ++i)
		{
			const boost::filesystem::path cp = (*i);

			if (!boost::filesystem::is_directory(cp))
			{
				filesList.push_back(cp.string());
			}
		}
	}

	return filesList;
}

// output result file
int saveResultFile(std::string pathToSave, std::vector<std::vector<cv::Rect>> detectionResult, std::vector<std::string> resultFileNames)
{
	boost::property_tree::ptree pt;

	for (int i = 0; i < detectionResult.size(); i++)
	{
		std::stringstream si;
		si << i;

		boost::property_tree::ptree child_fileName;
		child_fileName.put("pathOut", resultFileNames[i]);


		for (int j = 0; j < detectionResult[i].size(); j++)
		{
			boost::property_tree::ptree child_rectangle;
			child_rectangle.put("x", detectionResult[i][j].x);
			child_rectangle.put("y", detectionResult[i][j].y);
			child_rectangle.put("width", detectionResult[i][j].width);
			child_rectangle.put("height", detectionResult[i][j].height);

			//boost::property_tree::ptree child_face;
			//child_face.put_child("face1", child_rectangle);

			std::stringstream sj;
			sj << j;

			child_fileName.push_back(std::make_pair("face" + sj.str(), child_rectangle));

		}

		pt.add_child("file" + si.str(), child_fileName);

	}

	//boost::property_tree::write_json(std::cout, pt);
	boost::filesystem::path saveFile(pathToSave);
	saveFile = saveFile / "result.json";
	boost::property_tree::write_json(saveFile.string(), pt);

	return 0;
}



// main function
extern "C" {
	EXPORTED int detectFace(char *rootFolder)
	{
		// get files to process
		std::vector<std::string> filesList;

		try
		{
			filesList = getAllFiles(rootFolder);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			return 1;
		}


		// load classifier
		std::string classifierFile = "haarcascade_frontalface_alt2.xml";
		//std::string classifierFile = "lbpcascade_frontalface.xml";

		boost::filesystem::path cwd(boost::filesystem::current_path());
		boost::filesystem::path pathClassifier = cwd / classifierFile;

		cv::CascadeClassifier classifier;
		try
		{
			classifier = cv::CascadeClassifier(pathClassifier.string());
			if (classifier.empty())
			{
				throw std::runtime_error("Error: Classifier is not loaded.");
			}
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			return 2;
		}


		// process files list
		std::vector<std::vector<cv::Rect>> detectionResult;
		std::vector<std::string> resultFileNames;

		for (int i = 0; i < filesList.size(); i++)
		{
			// read image
			cv::Mat img = cv::imread(filesList[i]);
			if (img.empty())
			{
				std::cout << "FILE : " << filesList[i] << std::endl;
				std::cout << "FORMAT IS NOT SUPPORTED" << std::endl;
				continue;
			}

			// convert to 8U
			cv::Mat img8U;
			try
			{
				cv::cvtColor(img, img8U, cv::COLOR_BGR2GRAY);
			}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
				continue;
			}


			// face detection
			std::vector<cv::Rect> faces;
			classifier.detectMultiScale(img8U, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));


			// save result image

			// path to save
			boost::filesystem::path pathSave(filesList[i]);
			std::string parentPath = pathSave.parent_path().string();
			std::string fileName = pathSave.stem().string();
			std::string fileExt = pathSave.extension().string();

			pathSave = boost::filesystem::path(parentPath);
			pathSave = pathSave / (fileName + "_PROC" + ".jpg");

			for (int j = 0; j < faces.size(); j++)
			{
				cv::rectangle(img, faces[j], cv::Scalar(0, 255, 0), 2, 8, 0);

				// blur face ROI
				cv::Mat faceROI = img(faces[j]);
				GaussianBlur(faceROI, faceROI, cv::Size(51, 51), 0, 0);

			}

			// resize
			cv::resize(img, img, cv::Size(img.cols / 2, img.rows / 2), 0., 0., cv::INTER_CUBIC);

			cv::imwrite(pathSave.string(), img);

			// result vector coordinates of faces and filenames
			detectionResult.push_back(faces);
			resultFileNames.push_back(pathSave.string());

			// print results
			std::cout << "FILE : " << filesList[i] << std::endl;
			std::cout << "FACES DETECTED : " << faces.size() << std::endl;

		}


		// save results
		saveResultFile(std::string(rootFolder), detectionResult, resultFileNames);



		return 0;
	};
}
