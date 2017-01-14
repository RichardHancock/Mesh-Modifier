#include <iostream>
#include <assimp/Exporter.hpp>

void processCommandLine(int argc, char **argv);
int main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	processCommandLine(argc, argv);

	Assimp::Exporter* exporter = new Assimp::Exporter();

	size_t count = exporter->GetExportFormatCount();
	for (size_t i = 0; i < count; i++)
	{
		const aiExportFormatDesc* format = exporter->GetExportFormatDescription(i);
		std::cout << "ID: " << format->id << " Ext: ." << format->fileExtension <<
			" Desc: " << format->description << std::endl;
	}



	system("pause");
	delete exporter;
	return 0;
}

void processCommandLine(int argc, char **argv)
{
	std::cout << "These were the passed in args:" << std::endl;
	for (int i = 1; i < argc; i++)
	{
		std::cout << argv[i] << std::endl;
	}
	std::cout << std::endl;
}