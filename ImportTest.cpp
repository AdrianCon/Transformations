#include <iostream>
#include <vector>
#include <string>
#include <glm/vec3.hpp> // glm::vec3
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

int main(int argc, char const *argv[]){

 	vector< glm::vec3 > out_vertices;
    vector< glm::vec3 > out_uvs;
    vector< glm::vec3 > out_normals;

	vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	vector< glm::vec3 > temp_vertices;
	vector< glm::vec3 > temp_uvs;
	vector< glm::vec3 > temp_normals;

	FILE * file = fopen("Test.obj", "r");

	if( file == NULL ){
	    printf("Impossible to open the file !\n");
	    return 0;
	}

	while( 1 ){

	    char lineHeader[128];
	    // read the first word of the line
	    int res = fscanf(file, "%s", lineHeader);
	    if (res == EOF)
	        break; // EOF = End Of File. Quit the loop.
	    
	    // else : parse lineHeader
	    if ( strcmp( lineHeader, "v" ) == 0 ){
	    glm::vec3 vertex;
	    fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
	    temp_vertices.push_back(vertex);

	    }else if ( strcmp( lineHeader, "vt" ) == 0 ){
	    glm::vec3 uv;
	    fscanf(file, "%f %f\n", &uv.x, &uv.y );
	    temp_uvs.push_back(uv);

	    }else if ( strcmp( lineHeader, "vn" ) == 0 ){
	    glm::vec3 normal;
	    fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
	    temp_normals.push_back(normal);
	    }else if ( strcmp( lineHeader, "f" ) == 0 ){
	    string vertex1, vertex2, vertex3;
	    
	    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	    
	    int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
	    
	    if (matches != 9){
	        printf("File can't be read by our simple parser : ( Try exporting with other options\n");
	        return 0;
	    }

	   vertexIndices.push_back(vertexIndex[0]);
	    vertexIndices.push_back(vertexIndex[1]);
	    vertexIndices.push_back(vertexIndex[2]);
	   uvIndices.push_back(uvIndex[0]);
	    uvIndices    .push_back(uvIndex[1]);
	    uvIndices    .push_back(uvIndex[2]);
	    normalIndices.push_back(normalIndex[0]);
	    normalIndices.push_back(normalIndex[1]);
	    normalIndices.push_back(normalIndex[2]);

		}
	}

	 // For each vertex of each triangle
    for( unsigned int i=0; i<vertexIndices.size(); i++ ){
    	unsigned int vertexIndexAux = vertexIndices[i];
    	glm::vec3 vertex = temp_vertices[ vertexIndexAux-1 ];
    	out_vertices.push_back(vertex);

    	
    }

	cout << sizeof(glm::vec3)  << endl;	
	/*vector< glm::vec3 > vertices;
	vector< glm::vec2 > uvs;
	vector< glm::vec3 > normals; // Won't be used at the moment*/
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);
	return 0;
}