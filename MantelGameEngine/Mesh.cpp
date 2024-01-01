#include "Mesh.h"

#include <GL/glew.h>

#include <span>

#include <vector>
#include <array>

#include <iostream>
#include <fstream>

#include <filesystem>
namespace fs = std::filesystem;

using namespace std;


std::vector<Mesh::Ptr> Mesh::loadFromFile(const std::vector<std::string>& path) {



    //load meshes
    vector<Mesh::Ptr> mesh_ptrs;

    for (int i = 0; i < path.size(); ++i) {
        MeshLoader forLoading;

        ifstream iFile(path[i], ios::binary);

        iFile >> forLoading;

        auto mesh_sptr = make_shared<Mesh>(Formats::F_V3T2, forLoading.vertex_data.data(), forLoading.vertex_data.size(), forLoading.numFaces,
            forLoading.index_data.data(), forLoading.index_data.size());

        mesh_sptr->path = path[i];
        mesh_sptr->meshVerts = forLoading.meshVerts;
        mesh_sptr->meshNorms = forLoading.meshNorms;
        mesh_sptr->meshFaceCenters = forLoading.meshFaceCenters;
        mesh_sptr->meshFaceNorms = forLoading.meshFaceNorms;

        mesh_sptr->loadTextureToMesh(forLoading.texture);


        iFile.close();
        mesh_ptrs.push_back(mesh_sptr);
    }


    return mesh_ptrs;
}

Mesh::Ptr Mesh::loadFromFile(const std::string& path) {

    //load meshes

        MeshLoader forLoading;

        ifstream iFile(path, ios::binary);

        iFile >> forLoading;

        auto mesh_sptr = make_shared<Mesh>(Formats::F_V3T2, forLoading.vertex_data.data(), forLoading.vertex_data.size(), 
            forLoading.numFaces, forLoading.index_data.data(), forLoading.index_data.size());

        mesh_sptr->path = path;
        mesh_sptr->meshVerts = forLoading.meshVerts;
        mesh_sptr->meshNorms = forLoading.meshNorms;
        mesh_sptr->meshFaceCenters = forLoading.meshFaceCenters;
        mesh_sptr->meshFaceNorms = forLoading.meshFaceNorms;

        mesh_sptr->loadTextureToMesh(forLoading.texture);


        iFile.close();
    


    return mesh_sptr;
}


void Mesh::loadTextureToMesh(const std::string& path) 
{
    auto texture_ptr = make_shared<Texture2D>(path);
    texture = texture_ptr;
}


Mesh::Mesh(Formats format, const void* vertex_data, unsigned int numVerts, unsigned int numFaces, const unsigned int* index_data, unsigned int numIndexs) :
    _format(format),
    _numVerts(numVerts),
    _numIndexs(numIndexs),
    _numFaces(numFaces)
{

    glGenBuffers(1, &_vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);

    switch (_format) {
    case Formats::F_V3:
        glBufferData(GL_ARRAY_BUFFER, sizeof(V3) * numVerts, vertex_data, GL_STATIC_DRAW);
        for (const auto& v : span((V3*)vertex_data, numVerts)) {
            aabb.min = glm::min(aabb.min, vec3(v.v));
            aabb.max = glm::max(aabb.max, vec3(v.v));
        }
        break;
    case Formats::F_V3C4:
        glBufferData(GL_ARRAY_BUFFER, sizeof(V3C4) * numVerts, vertex_data, GL_STATIC_DRAW);
        for (const auto& v : span((V3C4*)vertex_data, numVerts)) {
            aabb.min = glm::min(aabb.min, vec3(v.v));
            aabb.max = glm::max(aabb.max, vec3(v.v));
        }
        break;
    case Formats::F_V3T2:
        glBufferData(GL_ARRAY_BUFFER, sizeof(V3T2) * numVerts, vertex_data, GL_STATIC_DRAW);
        for (const auto& v : span((V3T2*)vertex_data, numVerts)) {
            aabb.min = glm::min(aabb.min, vec3(v.v));
            aabb.max = glm::max(aabb.max, vec3(v.v));
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
    _numFaces(b._numFaces),
    texture(b.texture)
{
    meshVerts = b.meshVerts;
    meshNorms = b.meshNorms;
    meshFaceCenters = b.meshFaceCenters;
    meshFaceNorms = b.meshFaceNorms;


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
        if (texture.get() && drawChecker==false)
        {
            texture->bind();
        }
        else {
            checkboard.get()->bind();
        }
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

    if (drawNormalsVerts && !meshVerts.empty() && !meshNorms.empty()) {
        glLineWidth(normalLineWidth);
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);

        for (int i = 0; i < _numVerts; i++) {
            glVertex3f(meshVerts[i].x, meshVerts[i].y, meshVerts[i].z);
            glVertex3f(meshVerts[i].x + meshNorms[i].x * normalLineLength,
                meshVerts[i].y + meshNorms[i].y * normalLineLength,
                meshVerts[i].z + meshNorms[i].z * normalLineLength);
        }

        glColor3f(1.0f, 1.0f, 0.0f);
        glEnd();
    }

    if (drawNormalsFaces && !meshFaceCenters.empty() && !meshFaceNorms.empty()) {
        glLineWidth(normalLineWidth);
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);

        for (int i = 0; i < _numFaces; i++) {
            glm::vec3 endPoint = meshFaceCenters[i] + normalLineLength * meshFaceNorms[i];
            glVertex3f(meshFaceCenters[i].x, meshFaceCenters[i].y, meshFaceCenters[i].z);
            glVertex3f(endPoint.x, endPoint.y, endPoint.z);
        }

        glColor3f(0.0f, 1.0f, 1.0f);
        glEnd();
    }


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}


Mesh::~Mesh() {
    if (_vertex_buffer_id) glDeleteBuffers(1, &_vertex_buffer_id);
    if (_indexs_buffer_id) glDeleteBuffers(1, &_indexs_buffer_id);
}

const unsigned int Mesh::getFacesNum()
{
    return _numFaces;
}

const unsigned int Mesh::getVertsNum()
{
    return _numVerts;
}
