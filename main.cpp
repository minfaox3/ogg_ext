#include <windows.h>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <string>

enum class LANGUAGE : int {
	ENGLISH,
	JAPANESE,
	MANDARIN
};

enum class STRING_DESC : int {
	LANGUAGE_NAME,
	ERROR_ARG_NOT_FOUND,
	LOG_READ_FILE,
	ERROR_CANT_OPEN_FILE,
	ERROR_CANT_WRITE_FILE,
	LOG_END
};

std::wstring STRING_PACK[3][6] = {
	{
		L"ENGLISH",
		L"Give the file path as an argument.",
		L"File to read: ",
		L"Failed to open file.",
		L"Failed to write file/directory.",
		L"Complete."
	},{
		L"日本語",
		L"ファイルパスを引数として渡してください。",
		L"読み込みファイル：",
		L"ファイルの読み込みに失敗しました。",
		L"ファイル/ディレクトリの書き込みに失敗しました。",
		L"完了"
	},{
		L"普通话",
		L"请提供文件路径作为参数。",
		L"读取文件：",
		L"读取失败了：",
		L"作成失败了：",
		L"完了"
	}
};

int wmain(int argc, wchar_t* argv[]) {
	//start initialization
	std::ios_base::sync_with_stdio();
	std::wcout.imbue(std::locale(""));
	LANGUAGE user_lang = LANGUAGE::ENGLISH;
	switch (GetUserDefaultUILanguage())
	{
	case 1041:
		user_lang = LANGUAGE::JAPANESE;
		break;
	case 4:
	case 30724:
	case 2052:
	case 4100:
		user_lang = LANGUAGE::MANDARIN;
		break;
	default:
		break;
	}

	std::wstring* sp = STRING_PACK[static_cast<int>(user_lang)];

	if (argc < 2) {
		std::wcout << sp[static_cast<int>(STRING_DESC::ERROR_ARG_NOT_FOUND)] << std::endl;
		system("pause");
		return 0;
	}

	std::wstring file_path = argv[1];
	
	std::wcout << sp[static_cast<int>(STRING_DESC::LOG_READ_FILE)] << file_path << std::endl;

	//start directory creation
	
	std::wstring dir_name = file_path.substr(file_path.find_last_of(L"\\") + 1);
	dir_name = dir_name.erase(dir_name.find_last_of(L"."));

	struct _stat buffer;
	if (_wstat(dir_name.c_str(), &buffer)!=0 && _wmkdir(dir_name.c_str()) != 0) {
		std::wcout << sp[static_cast<int>(STRING_DESC::ERROR_CANT_OPEN_FILE)] << std::endl;
		system("pause");
		return 0;
	}

	//start i/o file

	std::ifstream ifs(argv[1], std::ios::in | std::ios::binary|std::ifstream::ate);

	std::streampos end_ps = ifs.tellg();

	ifs.seekg(0, std::ifstream::beg);

	if (!ifs) {
		std::wcout << sp[static_cast<int>(STRING_DESC::ERROR_CANT_OPEN_FILE)] << std::endl;
		system("pause");
		return 0;
	}

	int sf_cnt = 1;
	const char oggs[62] = {
		0x4f, 0x67, 0x67, 0x53, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x4f, 0x67, 0x67, 0x53
	};
	int oggs_index = 0;
	int oggs_cnt = 0;
	std::streampos last_ps;
	std::streampos ps;
	char data;

	while (!ifs.eof()) {
		ifs.read(&data, sizeof(char));
		if (0x00 == oggs[oggs_index] || oggs[oggs_index] == data) {
			if ((oggs_cnt == 0 || oggs_cnt == 1) && 0 == oggs_index) {
				ps = ifs.tellg();
			}
			oggs_index++;
		} else {
			oggs_index = 0;
		}

		if (62 == oggs_index) {
			oggs_cnt++;

			if (1 == oggs_cnt) {
				last_ps = ps;
			} else if (2 == oggs_cnt) {
				std::streampos tmp_ps = ifs.tellg();

				char tmp_cp;

				ifs.seekg(last_ps - std::streamoff(1));

				std::wstring sound_file_name = dir_name;
				sound_file_name += L"_";
				sound_file_name += std::to_wstring(sf_cnt);
				sound_file_name += L".ogg";

				std::wcout << sound_file_name << std::endl;
				std::wcout << "SIZE=" << ps - last_ps << "B" << std::endl;

				std::ofstream ofs((dir_name + L"\\") + sound_file_name, std::ios::trunc | std::ios::binary);
				while (ifs.tellg() < (ps - std::streamoff(1)) ) {
					ifs.read(&tmp_cp, sizeof(tmp_cp));
					ofs.write(&tmp_cp, sizeof(tmp_cp));
				}
				if (ofs.fail()) {
					std::wcout << sp[static_cast<int>(STRING_DESC::ERROR_CANT_WRITE_FILE)] << std::endl;
				}
				ofs.close();

				ifs.seekg(tmp_ps);

				sf_cnt++;
				last_ps = ps;
				oggs_cnt = 1;
			}

			oggs_index = 0;
		}
	}

	char tmp_cp;
	ifs.clear();
	ifs.seekg(last_ps - std::streamoff(1));

	std::wstring sound_file_name = file_path.substr(file_path.find_last_of(L"\\") + 1);
	sound_file_name.erase(sound_file_name.find_last_of(L"."));
	sound_file_name += L"_";
	sound_file_name += std::to_wstring(sf_cnt);
	sound_file_name += L".ogg";

	std::wcout << sound_file_name << std::endl;
	std::wcout << "SIZE=" << end_ps - last_ps << "B" << std::endl;

	std::ofstream ofs((dir_name + L"\\") + sound_file_name, std::ios::trunc | std::ios::binary);
	while (true) {
		ifs.read(&tmp_cp, sizeof(tmp_cp));
		if (ifs.eof())break;
		ofs.write(&tmp_cp, sizeof(tmp_cp));
	}
	if (ofs.fail()) {
		std::wcout << sp[static_cast<int>(STRING_DESC::ERROR_CANT_WRITE_FILE)] << std::endl;
	}
	ofs.close();

	ifs.close();
	
	std::wcout << sp[static_cast<int>(STRING_DESC::LOG_END)] << std::endl;

	system("pause");
}