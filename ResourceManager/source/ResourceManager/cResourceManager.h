#pragma once

#include <vector>
#include <unordered_map>

#if !_DEBUG
class sBakedData;
#endif // !_DEBUG

namespace RM
{ 
	typedef unsigned char byte;
	typedef std::vector<byte> byteArray;

	class cResourceManager
	{
	public:
		cResourceManager();
		~cResourceManager();

		static cResourceManager& getInstance();

		byteArray* getResource( const char* _path );

		void deleteResource( const char* _name );

		void clearResources();

	private:
		byteArray* loadBinaryFile( const char* _path);

		byteArray* storeMemmory(const char* _path);

		std::unordered_map< const char*, byteArray* > m_stored_data;

#if !_DEBUG
		sBakedData* m_baked_data;
#endif // !_DEBUG
	};
}