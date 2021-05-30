#include "OBJloader.h"

Obj loadObj(char *fname, char *mtlname ){
	FILE *fp;

	Obj temp;

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<uint> TvertexIndices, TuvIndices, TnormalIndices;

	std::vector<vec3> temp_vertices;
	std::vector<vec2> temp_uvs;
	std::vector<vec3> temp_normals;

	std::vector< vec3 > out_vertices;
	std::vector<vec2> out_uvs;
	std::vector<vec3> out_normals;

	fp = fopen(fname, "r");
	if (!fp){
		printf("can't open %s file \n", fname);
		exit(1);
	}

	while (1){
		char lineHeader[128];
		// read first word
		int fword = fscanf(fp, "%s", lineHeader);
		if (fword == EOF) break;

		if (strcmp(lineHeader, "v") == 0){
			vec3 vertex;
			fscanf(fp, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
			//printf("%f %f %f\n", vertex.x, vertex.y, vertex.z);
		}

		else if (strcmp(lineHeader, "vt") == 0){
			vec2 uv;
			fscanf(fp, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}

		else if (strcmp(lineHeader, "vn") == 0){
			vec3 normal;
			fscanf(fp, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}

		else if (strcmp(lineHeader, "f") == 0){
			std::string vertex1, vertex2, vertex3;
			//unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];

			int matches =
				//fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);

			if (matches != 12){
				printf("File can't be read 'f' options\n");
				exit(1);
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			vertexIndices.push_back(vertexIndex[3]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			uvIndices.push_back(uvIndex[3]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
			normalIndices.push_back(normalIndex[3]);

		}

		else if (strcmp(lineHeader, "mtllib") == 0){
			char mtlN[128];
			FILE *mfp;
			mfp = fopen(mtlname, "r");

			if (!mfp){
				printf("can't open %s file \n", mtlname);
				exit(1);
			}
			fscanf(mfp, "%s", mtlN);

			// Have to implement MTL loader

			fclose(mfp);
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++){

		unsigned int vertexIndex = vertexIndices[i];
		vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
		TvertexIndices.push_back(vertexIndex -1);

		//printf("vertice:%f ", out_vertices[i]);
		//printf("index:%d \n", TvertexIndices[i]);
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++){

		unsigned int uvIndex = uvIndices[i];
		vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
		TuvIndices.push_back(uvIndex -1);
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++){

		unsigned int normalIndex = normalIndices[i];
		vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
		TnormalIndices.push_back(normalIndex -1);
	}
	/*
	zombie ttemp;

	ttemp.vertices = out_vertices;
	ttemp.uvs = out_uvs;
	ttemp.normals = out_normals;

	temp.vertices = ttemp.vertices;
	temp.uvs = ttemp.uvs;
	temp.normals = ttemp.normals;

	for (unsigned int i = 0; i < temp_vertices.size(); i++){
		temp.vertices[i].x = ttemp.vertices[i].x * 100.0f;
		temp.vertices[i].y = ttemp.vertices[i].y * 100.0f;
		temp.vertices[i].z = ttemp.vertices[i].z * 100.0f;
		temp.uvs[i] = ttemp.uvs[i] * 100.0;
		temp.normals[i] = ttemp.normals[i]*100.0;
	}

	*/
	temp.vertices = out_vertices;
	temp.uvs = out_uvs;
	temp.normals = out_normals;

	temp.vertexIndices = TvertexIndices;
	temp.normalIndices = TnormalIndices;
	temp.uvIndices = TuvIndices;

	return temp;

	fclose(fp);
}

