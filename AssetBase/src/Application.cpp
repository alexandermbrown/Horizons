
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include "Resources/AssetSerial.h"
#include "zstr/zstr.hpp"

int main(int argc, char** argv)
{
	if (argc < 3) {
		std::cout << "Error: program requires two arguments:\n - resource definition file path (xml)\n - output file path (lab)" << std::endl;
		return EXIT_FAILURE;
	}

	try {
		AssetBase::AssetSerial serial(argv[1]);

		zstr::ofstream outFile(std::string(argv[2]), std::ios::out | std::ios::trunc | std::ios::binary);

		std::cout << "Compressing " << serial.GetHeaderSerial().fileSize << " bytes to file " << argv[2] << "  ...\n";
		outFile << serial;
		if (outFile.good()) 
			std::cout << "Success!" << std::endl;
		else
			std::cout << "Something may have gone wrong..." << std::endl;
	}
	catch (const char* msg) {
		std::cout << msg;
	}

	return EXIT_SUCCESS;
}
