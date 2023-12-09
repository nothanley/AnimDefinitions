/* Static utility class for reading and writing stream binary */
#include <string>
using namespace std;

namespace BinaryIO {

	std::string ReadString(istream& fs, int size);

	int ReadInt(istream& fs);

	unsigned int ReadUInt(istream& in);

	uint32_t ReadUInt32(istream& in);

	int8_t ReadSInt8(istream& in);

	int16_t ReadSInt16(istream& in);

	int32_t ReadSInt32(istream& in);

	uint64_t ReadUInt64(istream& in);

	unsigned short ReadUShort(istream& in);

	short ReadShort(istream& in);

	uint8_t ReadByte(istream& in);

	float ReadFloat(istream& in);

	bool ReadBool(istream& in);

    void WriteUInt64(ofstream* fs, uint64_t value);

    void WriteUInt32(ofstream* fs, uint32_t value);

    void WriteUInt16(ofstream* fs, uint16_t value);

    void WriteByte(ofstream* fs, uint8_t value);

    void WriteBool(ofstream* fs, bool flag);

    void WriteInt32(ofstream* fs, int32_t value);

    void WriteFloat(ofstream* fs, float value);

    void WriteString(ofstream* fs, std::string string);

    void WriteChars(ofstream* fs, std::string value);

	void WriteSignature(ofstream* fs, std::string value);
}
