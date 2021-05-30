#include "mesh.h"

Mesh* loadMesh( const char* vertexFilePath, const char* indexFilePath )
{
	Mesh* pNewMesh = new Mesh();
	
	// load vertex buffer
	FILE* fp = fopen( vertexFilePath, "rb" ); if(fp==NULL){ printf( "Unable to open %s\n", vertexFilePath ); return NULL; }
	fseek( fp, 0L, SEEK_END);
	size_t vertexBufferSize = ftell(fp);
	fseek( fp, 0L, SEEK_SET );
	if(vertexBufferSize%sizeof(vertex)){ printf( "%s is not a valid vertex buffer\n", vertexFilePath ); fclose(fp); return NULL; }
	pNewMesh->vertexList.resize( vertexBufferSize/sizeof(vertex) );
	fread( &pNewMesh->vertexList[0], sizeof(vertex), pNewMesh->vertexList.size(), fp );
	fclose(fp);

	// load index buffer
	fp = fopen( indexFilePath, "rb" ); if(fp==NULL){ printf( "Unable to open %s\n", indexFilePath ); return NULL; }
	fseek( fp, 0L, SEEK_END);
	size_t indexBufferSize = ftell(fp);
	fseek( fp, 0L, SEEK_SET );
	if(vertexBufferSize%sizeof(vertex)){ printf( "%s is not a valid index buffer\n", indexFilePath ); fclose(fp); return NULL; }
	pNewMesh->indexList.resize( indexBufferSize/sizeof(uint) );
	fread( &pNewMesh->indexList[0], sizeof(uint), pNewMesh->indexList.size(), fp );
	fclose(fp);

	return pNewMesh;
}