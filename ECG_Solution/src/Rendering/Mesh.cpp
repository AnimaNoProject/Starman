#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, _Shader* shader) : _shader(shader), _vertices(vertices), _indices(indices), _textures(textures){
	// Prepare mesh for drawing
	setupMesh();
}

void Mesh::setupMesh() {
	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	glBindVertexArray(VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// Vertex positions --> 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex normals --> 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glBindVertexArray(0);
}

long Mesh::Draw() {
	// First step --> bind appropriate textures for the mesh
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < _textures.size(); i++)
	{
		// Activate proper texture unit before binding
		glActiveTexture(GL_TEXTURE0 + i); 

		string number;
		string name = _textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			// Transfer unsigned int to stream
			number = std::to_string(specularNr++); 
		else if (name == "texture_normal")
			// Transfer unsigned int to stream
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			// Transfer unsigned int to stream
			number = std::to_string(heightNr++); 
		else if (name == "texture_light")
			// Transfer unsigned int to stream
			number = std::to_string(heightNr++);

		// Set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(_shader->getShader(), (name + number).c_str()), i);
		//cout << (name).c_str();

		// Last step --> bind texture
		glBindTexture(GL_TEXTURE_2D, _textures[i].id);
	}

	mat4 accumModel = _transformMatrix * _modelMatrix;	

	_shader->setUniform("model", accumModel);
	_shader->setUniform("normals", mat3(transpose(inverse(accumModel))));

	_shader->setUniform("Ka", _textures[0].Ka);
	_shader->setUniform("Kd", _textures[0].Kd);
	_shader->setUniform("Ks", _textures[0].Ks);
	_shader->setUniform("shyniness", _textures[0].Alpha);

	// Finally draw the mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	// Reset everything after configuration
	glActiveTexture(GL_TEXTURE0);

	return _indices.size()/3;
}

void Mesh::setTransformMatrixForMesh(mat4 transformMatrix)
{
	_transformMatrix = transformMatrix;
}

void Mesh::resetModelMatrixForMesh()
{
	_modelMatrix = mat4(1);
}

void Mesh::transformMesh(mat4 transformation)
{
	_modelMatrix = transformation * _modelMatrix;
}