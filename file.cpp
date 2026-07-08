#include "main.h"



size_t getFileSize(FILE *FileHandle)
{
	long CurrentOffset = ftell(FileHandle);

	fseek(FileHandle, 0L, SEEK_END);

	size_t FileSize = ftell(FileHandle);

	fseek(FileHandle, CurrentOffset, SEEK_SET);

	return FileSize;
}

std::string readFile(const std::filesystem::path& file_path) 
{
    std::ifstream file_stream(file_path);
    
    if (!file_stream.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_path.string());
    }

    std::ostringstream buffer;
    buffer << file_stream.rdbuf(); // Read the file buffer directly into the string stream
    return buffer.str();
}
