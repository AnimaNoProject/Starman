#include "Model.h"
#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "assimp/stb_image.h"

Model::Model(char *path, _Shader* shader) : _shader(shader) {
	loadModel(path);
}

long Model::Draw()
{
	long triangles = 0;
	for (unsigned int i = 0; i < meshes.size(); i++)
		triangles += meshes[i].Draw();
	return triangles;
}

void Model::loadModel(string path)
{
	// Read file using ASSIMP importer
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
	// Print message in case of error
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// Get directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// Process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	// Process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// After all meshes are processed, go ahead and process children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	// All relevant data we need to fill
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	// Iterate through the mesh vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vec3 vector;
		// Get vertex positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// Get vertex normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// Get texture coordinates (UV) in case there are texture coordinates
		if (mesh->mTextureCoords[0])
		{
			vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = vec2(0.0f, 0.0f);
		}
		
		vertices.push_back(vertex);
	}
	// Iterate through the mesh faces and get the corresponding vertex indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Get all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// Final Step --> process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// Last but not least --> load according material textures
	// Diffuse maps
	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// Specular maps
	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// Normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// Height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// Return the complete generated mesh object
	return Mesh(vertices, indices, textures, this->_shader);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
	aiColor3D temp;
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Efficiency test --> check if the texture has already been loaded. If true, continue with next texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				// Set skip=true, if texture has already been loaded from the exact same filepath
				skip = true;
				break;
			}
		}
		if (!skip)
		{   
			// Load texture, if it has not been loaded already
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  
		}
		mat->Get(AI_MATKEY_COLOR_AMBIENT, temp);
		textures[i].Ka = temp.r;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, temp);
		textures[i].Kd = temp.r;
		mat->Get(AI_MATKEY_COLOR_SPECULAR, temp);
		textures[i].Ks = temp.r;
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	// Generate texture
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	// Load texture
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	// If there is a texture, extract the format and bind it
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		// Bind Texture and set texture parameters
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void Model::setTransformMatrix(mat4 transformMatrix)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].setTransformMatrixForMesh(transformMatrix);
}

void Model::resetModelMatrix()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].resetModelMatrixForMesh();
}

void Model::transform(mat4 transformation)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].transformMesh(transformation);
}
