#include "main.h"



size_t getFileSize(FILE *FileHandle)
{
	long CurrentOffset = ftell(FileHandle);

	fseek(FileHandle, 0L, SEEK_END);

	size_t FileSize = ftell(FileHandle);

	fseek(FileHandle, CurrentOffset, SEEK_SET);

	return FileSize;
}


void readFile(const wchar_t *filePath, char **fileBuffer, size_t *fSize)
{
	FILE *fHandle = _wfopen(filePath, L"rb");

	if (fHandle) {
		int err = fseek(fHandle, 0, SEEK_SET);

		if (!err) {
			*fSize = getFileSize(fHandle);

			if (fSize > 0) {
				*fileBuffer = new char[*fSize]();
				size_t LengthRead = fread(*fileBuffer, sizeof(char), *fSize, fHandle);
				
				err = ferror(fHandle);
				if (err) printf("Error at fread()\n");
			}
			else printf("Error at fileSize");
		}
		else printf("Error at fseek()\n");

		fclose(fHandle);
	}
}