#include "zcompress.h"
#include "zlib/zlib.h"
#include "../../src/BinaryIO.h"
#include <iostream>
#include <vector>
#pragma once
#pragma comment(lib, "Ws2_32.lib")

using namespace std;
using namespace BinaryIO;



ADef::ADef(const char* FilePath) {
		openFile(FilePath);
	}

void 
ADef::openFile(const char* path) {

	//verifies if file is readable
	ifstream inFile(path);

	if (inFile.good())
		fileBuffer->open(path, ios::in | ios::binary);
	else
		throw("Cannot Open File.");

	//store filepath to object
	filePath = path;
	printf("Opening File: %s\n", path);

	// store size
	fileSize = getFileSize(inFile);

	// evaluate
	processFile();
	inFile.close();
}

void 
ADef::processFile() {
	std::iostream fs(fileBuffer);

	uint32_t fileMagic = BinaryIO::ReadUInt32(fs);
	bool isCompressedAdef = (fileMagic == DEFS);

	if (isCompressedAdef) {
		uint32_t compressed_size;
		uint32_t decompressed_size;

		// seek to metadata
		fs.seekp(0xC);
		compressed_size = ReadUInt32(fs);

		fs.seekp(0x14);
		decompressed_size = ReadUInt32(fs);

		// Read compressed bytes
		std::vector<byte> compressedBuffer(compressed_size);
		fs.read(reinterpret_cast<char*>(compressedBuffer.data()), compressed_size);

		// Decompress the zlib buffer
		byte* decompressedBuffer;
		decompressedBuffer = decompressZLIB(compressedBuffer.data(),
			compressed_size, decompressed_size);

		// Write buffer to an output file
		std::string outputPath = this->filePath;// ".decompressed";
		writeToFile(outputPath, decompressedBuffer, decompressed_size, false);

		//Cleanup memory
		delete[] decompressedBuffer;
//		std::cout << "\n\nDecompressed file: " << this->filePath << "\n";
//		system("pause");
	}
	else {
		uint32_t decompressed_size;
		size_t compressed_size;

		// seek to beginning
		fs.seekp(0x0);
		decompressed_size = this->fileSize;

		// Read file stream
		std::vector<byte> decompressedBuffer(decompressed_size);
		fs.read(reinterpret_cast<char*>(decompressedBuffer.data()), decompressed_size);


		// Compress buffer
		byte* compressed_data = compressZLIB(decompressedBuffer.data(),
			decompressed_size, compressed_size);

		// Write buffer to an output file
		std::string outputPath = this->filePath;// +".compressed";
		writeToFile(outputPath, compressed_data, compressed_size, true);

		//Cleanup memory
		delete[] compressed_data;
//		std::cout << "\n\nCompressed file: " << this->filePath << "\n";
//		system("pause");
	}



}

uint32_t 
ADef::getFileSize(std::ifstream& fileStream)
{
	std::streampos currentPosition = fileStream.tellg();
	fileStream.seekg(0, std::ios::end);
	std::streampos fileSize = fileStream.tellg();
	fileStream.seekg(currentPosition, std::ios::beg);
	return uint32_t(fileSize);
}

void 
ADef::writeToFile(const std::string& outputPath, const byte* data, size_t size, bool generateHeader)
{
	std::ofstream outputFile(outputPath, std::ios::binary);

	if (generateHeader) {
		uint32_t zlibMagic = 0x7665725F;
		uint32_t version = 0x6;
		uint32_t csizeMagic = 0x73697A63;
		uint32_t usizeMagic = 0x73697A75;
		uint32_t compressedSize = size;
		uint32_t decompressedSize = this->fileSize;

		outputFile.write((char*)&zlibMagic, 4);
		outputFile.write((char*)&version, 4);
		outputFile.write((char*)&csizeMagic, 4);
		outputFile.write((char*)&compressedSize, 4);
		outputFile.write((char*)&usizeMagic, 4);
		outputFile.write((char*)&decompressedSize, 4);
	}

	outputFile.write(reinterpret_cast<const char*>(data), size);
	outputFile.close();
}

byte* 
ADef::compressZLIB(const byte* data, size_t dataSize, size_t& compressedSize)
{
//	std::cout << "Compressing...\n";
	const size_t bufferSize = compressBound(dataSize);
	byte* compressedData = new byte[bufferSize];

	z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = static_cast<uInt>(dataSize);
	stream.next_in = const_cast<byte*>(data);
	stream.avail_out = static_cast<uInt>(bufferSize);
	stream.next_out = compressedData;

	int result = deflateInit(&stream, Z_BEST_COMPRESSION);
	if (result != Z_OK) {
		delete[] compressedData;
		throw std::runtime_error("Error initializing z_stream for compression");
	}

	result = deflate(&stream, Z_FINISH);
	if (result != Z_STREAM_END) {
		deflateEnd(&stream);
		delete[] compressedData;
		throw std::runtime_error("Error compressing data");
	}

	compressedSize = stream.total_out;

	result = deflateEnd(&stream);
	if (result != Z_OK) {
		delete[] compressedData;
		throw std::runtime_error("Error ending compression");
	}

	return compressedData;
}

byte* 
ADef::decompressZLIB(const byte* data, size_t compressedSize, size_t decompressedSize) {
//	std::cout << "Decompressing...\n";
	byte* uncompressedData = new byte[decompressedSize];
	z_stream stream;

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;

	stream.avail_in = compressedSize;
	stream.next_in = const_cast<byte*>(data);
	stream.avail_out = decompressedSize;
	stream.next_out = uncompressedData;

	int result = inflateInit(&stream);
	if (result != Z_OK)
		throw std::runtime_error("Error initializing z_stream");

	result = inflate(&stream, Z_FINISH);
	if (result != Z_STREAM_END)
		throw std::runtime_error("Error inflating");

	inflateEnd(&stream);
	return uncompressedData;
}

byte* 
ADef::DecompressMemoryStream(std::ifstream* fs, uint32_t* decompressed_size) {
	uint32_t compressed_size;
	uint32_t fileMagic = BinaryIO::ReadUInt32(*fs);
	bool isCompressedAdef = (fileMagic == DEFS);
	if (!isCompressedAdef) { return nullptr; }

	fs->seekg(0xC);
	compressed_size = ReadUInt32(*fs);
	fs->seekg(0x14);
	*decompressed_size = ReadUInt32(*fs);

	// Read compressed bytes
	std::vector<byte> compressedBuffer(compressed_size);
	fs->read(reinterpret_cast<char*>(compressedBuffer.data()), compressed_size);

	// Decompress the zlib buffer
	byte* decompressedBuffer;
	decompressedBuffer = decompressZLIB(compressedBuffer.data(),
		compressed_size, *decompressed_size);
	return decompressedBuffer;
}




