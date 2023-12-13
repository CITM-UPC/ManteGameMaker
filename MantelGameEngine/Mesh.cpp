#include "Mesh.h"
#include <GL/glew.h>

#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <span>

#include <vector>
#include <array>

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

void Mesh::RecalculateBoundingBox() {
    // Inicializar los límites con valores extremos para poder encontrar los mínimos y máximos
    //boundingBoxMin = glm::vec3(std::numeric_limits<float>::max());
    //boundingBoxMax = glm::vec3(std::numeric_limits<float>::lowest());

    //// Iterar sobre cada vértice y ajustar los límites
    //for (const auto& vert : ) {
    //    boundingBoxMin.x = std::min(boundingBoxMin.x, vert.x);
    //    boundingBoxMin.y = std::min(boundingBoxMin.y, vert.y);
    //    boundingBoxMin.z = std::min(boundingBoxMin.z, vert.z);

    //    boundingBoxMax.x = std::max(boundingBoxMax.x, vert.x);
    //    boundingBoxMax.y = std::max(boundingBoxMax.y, vert.y);
    //    boundingBoxMax.z = std::max(boundingBoxMax.z, vert.z);
    //}
}


std::string Mesh::getTexturePathFromFbxPath(const std::string& path)
{
    const auto scene_ptr = aiImportFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
    const aiSceneExt& scene = *(aiSceneExt*)scene_ptr;

    //load textures
    vector<Texture2D::Ptr> texture_ptrs;
    for (const auto& material : scene.materials()) {
        aiString aiPath;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath);
        fs::path texPath = fs::path(path).parent_path() / fs::path(aiPath.C_Str()).filename();
        return texPath.string();
    }
    return "pathNotFound";
}

std::vector<Mesh::Ptr> Mesh::loadFromFile(const std::string& path) {

    const auto scene_ptr = aiImportFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
    const aiSceneExt& scene = *(aiSceneExt*)scene_ptr;

    //load textures
    vector<Texture2D::Ptr> texture_ptrs;
    for (const auto& material : scene.materials()){
        aiString aiPath;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath);
        fs::path texPath = fs::path(path).parent_path() / fs::path(aiPath.C_Str()).filename();
        auto texture_ptr = make_shared<Texture2D>(texPath.string());
        texture_ptrs.push_back(texture_ptr);
    }
    
    //load meshes
    vector<Mesh::Ptr> mesh_ptrs;
    for (const auto& mesh_ptr : scene.meshes()) {

        const auto& mesh = *mesh_ptr;
        
        vector<V3T2> vertex_data;
        for (size_t i = 0; i < mesh.verts().size(); ++i) {
            V3T2 v = { mesh.verts()[i], vec2f(mesh.texCoords()[i].x, mesh.texCoords()[i].y) };
            vertex_data.push_back(v);
        }

        vector<unsigned int> index_data;
        for (const auto& face : mesh.faces()) {
            index_data.push_back(face.mIndices[0]);
            index_data.push_back(face.mIndices[1]);
            index_data.push_back(face.mIndices[2]);
        }

        auto mesh_sptr = make_shared<Mesh>(Formats::F_V3T2, vertex_data.data(), vertex_data.size(), index_data.data(), index_data.size());
        mesh_sptr->texture = texture_ptrs[mesh.mMaterialIndex];

        // calculing bounds to do aabb
        glm::vec3 minBounds(std::numeric_limits<float>::max());
        glm::vec3 maxBounds(std::numeric_limits<float>::lowest());

        for (const auto& vert : mesh.verts()) {
            minBounds.x = std::min(minBounds.x, vert.x);
            minBounds.y = std::min(minBounds.y, vert.y);
            minBounds.z = std::min(minBounds.z, vert.z);

            maxBounds.x = std::max(maxBounds.x, vert.x);
            maxBounds.y = std::max(maxBounds.y, vert.y);
            maxBounds.z = std::max(maxBounds.z, vert.z);
        }
        mesh_sptr->boundingBoxMin = minBounds;
        mesh_sptr->boundingBoxMax = maxBounds;

        mesh_ptrs.push_back(mesh_sptr);
    }

    aiReleaseImport(scene_ptr);

    return mesh_ptrs;
}

Mesh::Mesh(Formats format, const void* vertex_data, unsigned int numVerts, const unsigned int* index_data, unsigned int numIndexs) : 
    _format(format),
    _numVerts(numVerts),
    _numIndexs(numIndexs)
{
    glGenBuffers(1, &_vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);

    switch (_format) {
    case Formats::F_V3:
        glBufferData(GL_ARRAY_BUFFER, sizeof(V3) * numVerts, vertex_data, GL_STATIC_DRAW);
        for (const auto& v : span((V3*)vertex_data, numVerts)) {
            //aabb.min = glm::min(aabb.min, vec3(v.v));
            //aabb.max = glm::max(aabb.max, vec3(v.v));
        }
        break;
    case Formats::F_V3C4:
        glBufferData(GL_ARRAY_BUFFER, sizeof(V3C4) * numVerts, vertex_data, GL_STATIC_DRAW);
        for (const auto& v : span((V3C4*)vertex_data, numVerts)) {
            //aabb.min = glm::min(aabb.min, vec3(v.v));
            //aabb.max = glm::max(aabb.max, vec3(v.v));
        }
        break;
    case Formats::F_V3T2:
        glBufferData(GL_ARRAY_BUFFER, sizeof(V3T2) * numVerts, vertex_data, GL_STATIC_DRAW);
        for (const auto& v : span((V3T2*)vertex_data, numVerts)) {
            //aabb.min = glm::min(aabb.min, vec3(v.v));
            //aabb.max = glm::max(aabb.max, vec3(v.v));
        }
        break;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (index_data) {
        glGenBuffers(1, &_indexs_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexs_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndexs, index_data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else {
        _indexs_buffer_id = 0;
    }
}


Mesh::Mesh(Mesh&& b) noexcept :
    _format(b._format),
    _vertex_buffer_id(b._vertex_buffer_id),
    _numVerts(b._numVerts),
    _indexs_buffer_id(b._indexs_buffer_id),
    _numIndexs(b._numIndexs),
    texture(b.texture)
{
    b._vertex_buffer_id = 0;
    b._indexs_buffer_id = 0;

}

void Mesh::draw() {

    glColor4ub(255, 255, 255, 255);

    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);
    glEnableClientState(GL_VERTEX_ARRAY);

    switch (_format) {
    case Formats::F_V3:
        glVertexPointer(3, GL_FLOAT, 0, nullptr);
        break;
    case Formats::F_V3C4:
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(V3C4), nullptr);
        glColorPointer(4, GL_FLOAT, sizeof(V3C4), (void*)sizeof(V3));
        break;
    case Formats::F_V3T2:
        glEnable(GL_TEXTURE_2D);
        if (texture.get()) texture->bind();
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(V3T2), nullptr);
        glTexCoordPointer(2, GL_FLOAT, sizeof(V3T2), (void*)sizeof(V3));
        break;
    }

    if (_indexs_buffer_id) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexs_buffer_id);
        glDrawElements(GL_TRIANGLES, _numIndexs, GL_UNSIGNED_INT, nullptr);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, _numVerts);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

Mesh::~Mesh() {
    if(_vertex_buffer_id) glDeleteBuffers(1, &_vertex_buffer_id);
    if(_indexs_buffer_id) glDeleteBuffers(1, &_indexs_buffer_id);
}