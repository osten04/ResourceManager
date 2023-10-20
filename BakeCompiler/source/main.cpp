#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <Windows.h>

using byte = unsigned char;

static std::string bakeDir   ;
static std::string headerPath;

std::vector<byte> readFile( const char* filename )
{
    std::ifstream stream( filename, std::ios::in | std::ios::binary);

    std::vector<byte> contents( ( std::istreambuf_iterator<char>(stream) ), std::istreambuf_iterator<char>() );

    return contents;
}

std::vector<std::string> getFilePaths( std::string _folderPath )
{
    std::vector<std::string> filePaths;

    std::filesystem::directory_iterator dir_itr( _folderPath );

    for (std::filesystem::directory_entry entry : dir_itr)
    {
        if ( entry.is_directory() )
        {
            std::vector< std::string > subDirectory = getFilePaths( entry.path().string() );

            for ( std::string file: subDirectory )
            {
                filePaths.push_back( file );
            }
        }
        else
        {
            filePaths.push_back( entry.path().string() );
        }
    }

    return filePaths;
}

std::string getHeaderString( )
{
    std::string header =
        "// generated code\n"
        "#pragma once\n\n"
        "#include <unordered_map>\n"
        "#include <vector>\n\n"
        "struct sBakedData\n"
        "{\n"
        "\tstd::unordered_map< const char*, std::vector<unsigned char> > data =\n"
        "\t{";

    std::vector<std::string> filePaths = getFilePaths( bakeDir );

    for ( std::string file: filePaths )
    {
        header +=
            "\n\t\t{ \"" +
            file +
            "\", {";

        std::vector< byte > data = readFile( file.c_str() );

        for ( byte b: data )
        {
            header += " " + std::to_string(b) + ",";
        }
        
        header += " } },";
    }

    header += "\n\t};\n};";

    for ( char& character: header )
    {
        if( character == '\\') character = '/';
    }

    return header;
}

int main( int argc, char** argv )
{
#ifdef RELEASE

    HWND handle = GetConsoleWindow();
    ShowWindow( handle, SW_HIDE );

#endif // RALEASE

    if( argc != 3 ) return 0;

    bakeDir    = argv[1];
    headerPath = argv[2];

    std::ofstream ofs( headerPath );

    ofs << getHeaderString();

    ofs.close();

	return 0;
}