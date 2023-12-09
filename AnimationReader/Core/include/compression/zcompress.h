/* Performs zlib compress/decopmression given a compressed file stream */
#include <fstream>
#pragma comment(lib, "Ws2_32.lib")
#pragma once

using namespace std;
typedef unsigned char byte;

class ADef
{
	enum {
		DEFS = 0x7665725F
	};

public:
	std::string filePath;
	std::filebuf* fileBuffer = new std::filebuf();
	uint32_t fileSize;

	ADef(const char* FilePath);
	void openFile(const char* path);
    void processFile();
    void writeToFile(const std::string& outputPath, const byte* data, size_t size, bool generateHeader);
	static byte* DecompressMemoryStream(std::ifstream* fs, uint32_t* decompressed_size);
    static byte* decompressZLIB(const byte* data, size_t compressedSize, size_t decompressedSize);
	uint32_t getFileSize(std::ifstream& fileStream);
	byte* compressZLIB(const byte* data, size_t dataSize, size_t& compressedSize);

};
