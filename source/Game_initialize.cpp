#include "Game_initialize.h"

void Game_initialize::Read_Jsonfile(const std::string& path, rapidjson::Document& doc)
{
	FILE* file_pointer = nullptr;
	//open json file for reading in binary mode
#ifdef _WIN32
	fopen_s(&file_pointer, path.c_str(), "rb");
#else
	file_pointer = fopen(path.c_str(), "rb");
#endif

	char buffer[65536];
	//fileread stream used to read from the file
	rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
	doc.ParseStream(stream);
	std::fclose(file_pointer);

	if (doc.HasParseError()) {
		rapidjson::ParseErrorCode errorCode = doc.GetParseError();
		std::cout << "error parsing json at [" << path << "]" << std::endl;
		exit(0);
	}

}

std::string Game_initialize::obtain_word_after_phrase(const std::string& input, const std::string& phrase)
{
	//find the position in input string
	size_t pos = input.find(phrase);

	//find the starting position of the next word (after space)
	pos += phrase.length();

	while (pos < input.size() && std::isspace(input[pos]))
	{
		++pos;
	}

	//if we at the end of the string
	if (pos == input.size()) return "";

	//find the end position of the word
	size_t endPos = pos;
	while (endPos < input.size() && !std::isspace(input[endPos]))
	{
		++endPos;
	}

	return input.substr(pos, endPos - pos);
}
