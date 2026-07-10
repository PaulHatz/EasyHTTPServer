#include "main.h"
#include "FileType.h"

//Use file name to get extension. Will return 0 if file has no extension or given path ends with a directory.
const char *getFileExtensionWithName(const char *fileName)
{
	size_t off = strlen(fileName) - 1;

	while (off >= 0) {
		switch (fileName[off]) {
		case '.':
			return &fileName[off];
		case '/':
			return nullptr;
		default: break;
		}

		if (off == 0) break;

		off--;
	}

	return nullptr;
}



std::vector<std::string_view> SplitHeaders(std::string_view headers)
{
    std::vector<std::string_view> lines;

    while (!headers.empty())
    {
        size_t pos = headers.find("\r\n");

        if (pos == std::string_view::npos)
        {
            lines.push_back(headers);
            break;
        }

        lines.push_back(headers.substr(0, pos));
        headers.remove_prefix(pos + 2);
    }

    return lines;
}

//Send header of response.
size_t HTTP_SendHeader(SOCKET sock, int response_code, const char *ContentType, size_t ContentLength)
{
	if (strlen(ContentType) > 50) return 0; //Buffer overflow protection

	char *responseHeader = new char[256]();

	size_t sz = sprintf(responseHeader,
		"HTTP/1.1 %ld OK\r\n"
		"Server: EasyHTTPServer/1.0.0\r\n"
		"Accept-Ranges: bytes\r\n"
		"Connection: close\r\n"
		"Content-Type: %s\r\n"
		"Content-Length: %lu\r\n"
		"\r\n",
		response_code,
		ContentType,
		ContentLength
	);

	send(sock, responseHeader, sz, 0);

	delete[] responseHeader;

	return sz;
}

void HTTP_ProcessRequest(SOCKET sock, char * buffer, size_t sz)
{
	auto headers = SplitHeaders(std::string_view(buffer, sz));
	auto request = std::string_view(headers.at(0));

	auto firstSpace = request.find(' ');
	if (firstSpace == std::string_view::npos)
    	return;

	auto secondSpace = request.find(' ', firstSpace + 1);
	if (secondSpace == std::string_view::npos)
    	return;

	auto method = request.substr(0, firstSpace);

	auto path =
		request.substr(firstSpace + 1,
					secondSpace - firstSpace - 1);

	auto version =
		request.substr(secondSpace + 1);

	auto req_path = std::string(path);

	while (!req_path.empty() && (req_path.front() == '/' || req_path.front() == '\\'))
	{
		req_path.erase(req_path.begin());
	}

	fs::path root = fs::canonical("html");
	fs::path requested_file = fs::weakly_canonical(root / req_path);



	if (IsWithinRoot(root, requested_file) && fs::exists(requested_file))
	{
		if (fs::is_directory(requested_file)) {
  			requested_file /= "index.html";
		}

		auto extension = getFileExtensionWithName(requested_file.c_str());
		auto file_contents = readFile(requested_file);
		auto fileSize =file_contents.length();
		if(fileSize > 0) {
			auto mime_type = GetMimeType(extension);

			HTTP_SendHeader(sock, 200, mime_type, fileSize + 2);
			
			char end[] = { 0x0D, 0x0A };
			send(sock, file_contents.c_str(), fileSize, 0);
			send(sock, end, 2, 0);
		} 
	}
	else {
		HTTP_SendHeader(sock, 404, "text/html", 0);
		std::cout << "Error 404: " << (requested_file) << std::endl;
	} 
}

void HTTP_SocketThread(int sock)
{
	char *buffer = new char[512];

	ssize_t sz = recv(sock, buffer, 512, 0);

	if (sz > 0 && sz <= 512)
		HTTP_ProcessRequest(
			sock, 
			buffer, 
			sz
		);

	std::this_thread::sleep_for(1ms);

	closesocket(sock);

	delete[] buffer;

	//The thread will terminate
}
