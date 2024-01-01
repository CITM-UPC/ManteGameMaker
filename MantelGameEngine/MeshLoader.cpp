#include "MeshLoader.h"

#include <GL/glew.h>
#include <span>

#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <fstream>

#include <filesystem>


namespace fs = std::filesystem;

using namespace std;

struct aiMeshExt : aiMesh {
    auto verts() const { return span((vec3f*)mVertices, mNumVertices); }
    auto texCoords() const { return span((vec3f*)mTextureCoords[0], mNumVertices); }
    auto faces() const { return span(mFaces, mNumFaces); }
};

struct aiSceneExt : aiScene {
    auto materials() const { return span(mMaterials, mNumMaterials); }
    auto meshes() const { return span((aiMeshExt**)mMeshes, mNumMeshes); }
};


std::vector<std::string> MeshLoader::loadFromFile(const std::string& path)
{
    const auto scene_ptr = aiImportFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_ForceGenNormals);
    const aiSceneExt& scene = *(aiSceneExt*)scene_ptr;

    fs::path pathPath(path.c_str());
    std::vector<std::string> pathList;
   

    //intraduce texture
    vector<string> texturePath;
    for (const auto& material : scene.materials()) {

        aiString aiPath;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath);
        string currentTexture;
        currentTexture = aiPath.C_Str();
        
        size_t lastChar = currentTexture.find_last_of('\\');
        currentTexture = currentTexture.substr(lastChar + 1);
        lastChar = currentTexture.find_last_of('.');
        currentTexture = currentTexture.substr(0, lastChar);
        currentTexture = "Library/Materials/" + currentTexture + ".dds";

        texturePath.push_back(currentTexture);
    }
   
    //load meshes
    int i = 0;
    for (const auto& mesh_ptr : scene.meshes()) {
        string fileName = pathPath.filename().string();
        size_t lastDot = fileName.find_last_of('.');
        fileName = fileName.substr(0, lastDot);
        fileName = fileName + std::to_string(i) + ".sht";
        fs::path customPath = fs::path("Library/Meshes/") / fs::path(fileName); //to library

        //std::filesystem::create_directories(customPath);
    

        scene_ptr->mName.C_Str();

        ofstream oFile(customPath.string(), ios::binary);

        const auto& mesh = *mesh_ptr;

        vector<VertexV3T2> vertex_data;
        for (size_t i = 0; i < mesh.verts().size(); ++i) {

            VertexV3T2 v;
            v.vertex = {mesh.verts()[i].x, mesh.verts()[i].y, mesh.verts()[i].z};
            if (mesh.HasTextureCoords(i))
                v.texCoords = { mesh.texCoords()[i].x, mesh.texCoords()[i].y };
            else
                v.texCoords = { 0, 0 };
            vertex_data.push_back(v);
        }

        vector<unsigned int> index_data;
        for (const auto& face : mesh.faces()) {
            index_data.push_back(face.mIndices[0]);
            index_data.push_back(face.mIndices[1]);
            index_data.push_back(face.mIndices[2]);

        }

        MeshLoader mesh_sptr = MeshLoader();

        mesh_sptr.texture = texturePath[mesh.mMaterialIndex];
        mesh_sptr.vertex_data = vertex_data;
        mesh_sptr.index_data = index_data;
        //mesh_sptr.transform = transformMatrix;

        for (size_t i = 0; i < mesh.mNumVertices; i++) {
            aiVector3D normal = mesh.mNormals[i];
            vec3f glmNormal(normal.x, normal.y, normal.z);
            mesh_sptr.meshNorms.push_back(glmNormal);
        }

        for (size_t i = 0; i < mesh.mNumVertices; i++) {
            aiVector3D vert = mesh.mVertices[i];
            vec3f glmNormal(vert.x, vert.y, vert.z);
            mesh_sptr.meshVerts.push_back(glmNormal);
        }

        for (size_t i = 0; i < mesh.mNumFaces; i++) {
            aiFace face = mesh.mFaces[i];

            vec3f v0(mesh.mVertices[face.mIndices[0]].x, mesh.mVertices[face.mIndices[0]].y, mesh.mVertices[face.mIndices[0]].z);
            vec3f v1(mesh.mVertices[face.mIndices[1]].x, mesh.mVertices[face.mIndices[1]].y, mesh.mVertices[face.mIndices[1]].z);
            vec3f v2(mesh.mVertices[face.mIndices[2]].x, mesh.mVertices[face.mIndices[2]].y, mesh.mVertices[face.mIndices[2]].z);

            vec3f faceNormal = glm::cross(v1 - v0, v2 - v0);
            faceNormal = glm::normalize(faceNormal);
            mesh_sptr.meshFaceNorms.push_back(faceNormal);

            vec3f faceCenter = (v0 + v1 + v2) / 3.0f;
            mesh_sptr.meshFaceCenters.push_back(faceCenter);
        }
        mesh_sptr.numFaces = mesh.mNumFaces;
         oFile << mesh_sptr;

        //AddLog
         oFile.close();
         pathList.push_back(customPath.generic_string());
         ++i;
    }

    aiReleaseImport(scene_ptr);
    

    return pathList;
}

std::ostream& MeshLoader::serialize(std::ostream& os) const
{
    size_t vLength = vertex_data.size();
    os.write((char*)&vLength, sizeof(vLength));
    os.write((char*)vertex_data.data(), vertex_data.size() * sizeof(VertexV3T2));

    vLength = index_data.size();
    os.write((char*)&vLength, sizeof(vLength));
    os.write((char*)index_data.data(), index_data.size() * sizeof(unsigned int));

    vLength = meshVerts.size();
    os.write((char*)&vLength, sizeof(vLength));
    os.write((char*)meshVerts.data(), meshVerts.size() * sizeof(vec3f));

    vLength = meshNorms.size();
    os.write((char*)&vLength, sizeof(vLength));
    os.write((char*)meshNorms.data(), meshNorms.size() * sizeof(vec3f));

    vLength = meshFaceCenters.size();
    os.write((char*)&vLength, sizeof(vLength));
    os.write((char*)meshFaceCenters.data(), meshFaceCenters.size() * sizeof(vec3f));

    vLength = meshFaceNorms.size();
    os.write((char*)&vLength, sizeof(vLength));
    os.write((char*)meshFaceNorms.data(), meshFaceNorms.size() * sizeof(vec3f));

    os.write((char*)&numFaces, sizeof(unsigned int));

    vLength = texture.size();
    os.write((char*)&vLength, sizeof(vLength));
    os.write(&texture[0], vLength);

    return os;
}

std::istream& MeshLoader::deserialize(std::istream& is)
{
    size_t vLength = 0;
    is.read((char*)&vLength, sizeof(vLength));
    vertex_data.resize(vLength);
    is.read((char*)vertex_data.data(), vLength * sizeof(VertexV3T2));

    is.read((char*)&vLength, sizeof(vLength));
    index_data.resize(vLength);
    is.read((char*)index_data.data(), vLength * sizeof(unsigned int));

    is.read((char*)&vLength, sizeof(vLength));
    meshVerts.resize(vLength);
    is.read((char*)meshVerts.data(), vLength * sizeof(vec3f));

    is.read((char*)&vLength, sizeof(vLength));
    meshNorms.resize(vLength);
    is.read((char*)meshNorms.data(), vLength * sizeof(vec3f));

    is.read((char*)&vLength, sizeof(vLength));
    meshFaceCenters.resize(vLength);
    is.read((char*)meshFaceCenters.data(), vLength * sizeof(vec3f));

    is.read((char*)&vLength, sizeof(vLength));
    meshFaceNorms.resize(vLength);
    is.read((char*)meshFaceNorms.data(), vLength * sizeof(vec3f));

    is.read((char*)&numFaces, sizeof(unsigned int));

    is.read((char*)&vLength, sizeof(vLength));
    texture.resize(vLength);
    is.read(&texture[0], vLength);

    return is;
}