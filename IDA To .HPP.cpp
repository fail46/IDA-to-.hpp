#include <fstream>
#include <iostream>
#include <string>

using std::cout;
using std::string;

int __cdecl main (int ArgCount, char* Args[])
{
	if(ArgCount != 2)
	{
		cout << "Usage: ITD <File>\n";
		return 0;
	}

	std::fstream Input (Args[1], std::ios::in);
	if(Input.is_open() == false)
	{
		cout << "Unable to open input file.\n";
		return 0;
	}

	char* OutputFileName = new char[strlen(Args[1]) + 5];
	sprintf(OutputFileName, "%s.hpp", Args[1]);
	std::fstream Output (OutputFileName, std::ios::out | std::ios::trunc);
	if(Output.is_open() == false)
	{
		cout << "Unable to open output file.\n";
		return 0;
	}

	Output << "#pragma once\n\n// Change this number to what IDA thinks the base address of the module is.\n#define ModuleBase 0x400000\n\n";

	while(Input.good() == true)
	{
		string Line;
		getline(Input, Line);
		if(Line.find("sub_") != Line.npos || Line.find("(") != Line.npos)
		{
			continue;
		}

		string FunctionName (Line.substr(0, Line.find(" ")));
		string Address (Line.substr(Line.find(".text ") + 6, 8));
		Output << "#define " << FunctionName << " 0x" << Address << " - ModuleBase\n";
	}

	cout << "Finished. Output is in: " << OutputFileName << std::endl;
	delete[] OutputFileName;
	Input.close();
	Output.close();
	return 0;
}