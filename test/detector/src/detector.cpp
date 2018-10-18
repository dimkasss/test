// std
#include <iostream>

// boost
#include <boost/filesystem.hpp>
#include <boost/dll.hpp>

// Define EXPORTED for any platform
#ifdef _WIN32
#define DLL_NAME "face.dll"
#else
# define EXPORTED
#define DLL_NAME "face.so"
#endif

// IMPORTED FUNCTIONS
typedef int(*detectFace)(char *path);

int main(int argc, char* argv[])
{
	std::cout << "Face Detector" << std::endl;

	if (argc < 2)
	{
		std::cout << "ERROR: The root images folder is not specified." << std::endl;
		return 1;
	}


	// get cwd
	boost::filesystem::path cwd(boost::filesystem::current_path());
	std::cout << "CWD : " << cwd << std::endl;

	// load Dll
	std::string dllName = DLL_NAME;
	//std::string dllName = "face.so";
	boost::filesystem::path pathDll = cwd / dllName;
	boost::dll::shared_library myDll;

	try
	{
		myDll = boost::dll::shared_library(pathDll.string());
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}


	// load function
	detectFace detectFunction = NULL;
	try
	{
		detectFunction = myDll.get<int(char *path)>("detectFace");
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 2;
	}

	// call dll function
	int err = (detectFunction)(argv[1]);

	// unload fll
	myDll.unload();

	return 0;
}
