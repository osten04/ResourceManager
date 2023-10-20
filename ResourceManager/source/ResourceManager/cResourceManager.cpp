#include "cResourceManager.h"

#include <fstream>

#if !_DEBUG
#include "sBakedData.h"
#endif // !_DEBUG



RM::cResourceManager::cResourceManager()
{
#if !_DEBUG
	m_baked_data = new sBakedData();
#endif // !_DEBUG
}

RM::cResourceManager::~cResourceManager()
{
	clearResources();

#if !_DEBUG
	delete m_baked_data;
#endif // !_DEBUG
}

RM::byteArray* RM::cResourceManager::loadBinaryFile( const char* _path )
{
	std::ifstream stream( _path, std::ios::in | std::ios::binary );

	if ( stream.fail() )
		return nullptr;
	
	byteArray* contents = new byteArray((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

	return contents;
}

RM::byteArray* RM::cResourceManager::storeMemmory( const char* _path )
{	
	byteArray* data = loadBinaryFile( _path );

	m_stored_data.insert( std::pair( _path, data ) );

	return data;
}

RM::byteArray* RM::cResourceManager::getResource(const char* _path)
{
#if !_DEBUG
	std::unordered_map< const char*, byteArray >::iterator bakedItr = m_baked_data->data.find(_path);

	if ( bakedItr != m_baked_data->data.end())
	{
		return &bakedItr->second;
	}
#endif // !_DEBUG

	std::unordered_map< const char*, byteArray* >::iterator itr = m_stored_data.find(_path);

	if (itr != m_stored_data.end())
	{
		return itr->second;
	}

	return storeMemmory( _path );
}

void RM::cResourceManager::deleteResource( const char* _name )
{
	std::unordered_map< const char*, byteArray* >::iterator itr = m_stored_data.find( _name );;

	if ( itr == m_stored_data.end() ) return;

	delete itr->second;
	m_stored_data.erase(itr);
}

void RM::cResourceManager::clearResources()
{
	for( std::pair< const char*, byteArray*> data: m_stored_data)
	{
		delete data.second;
	}
	m_stored_data.clear();
}

RM::cResourceManager& RM::cResourceManager::getInstance()
{
	static cResourceManager manager;
	return manager;
}