#include "pch.h"

#include <stdlib.h>
#include "Resources/AssetSerial.h"
#include "zstr/zstr.hpp"

int main(int argc, char** argv)
{
	std::cout << "AssetBase v0.1.0\n" << std::string(32, '-') << '\n';

	if (argc < 3) {
		std::cout << "Error: program requires two arguments:\n 1) resource definition file path (.xml)\n 2) output file path (.lab)" << std::endl;
		return EXIT_FAILURE;
	}

	try {
		AssetBase::AssetSerial serial(argv[1]);

		zstr::ofstream outFile(std::string(argv[2]), std::ios::out | std::ios::trunc | std::ios::binary);

		std::cout << std::string(32, '-') << "\nCompressing " << serial.GetHeaderSerial().fileSize << " bytes to file " << argv[2] << "  ...\n";
		outFile << serial;
		if (outFile.good()) 
			std::cout << "Finished!" << std::endl;
		else
			std::cout << "Something may have gone wrong..." << std::endl;
	}
	catch (const char* msg) {
		std::cout << msg;
	}

	return EXIT_SUCCESS;
}
