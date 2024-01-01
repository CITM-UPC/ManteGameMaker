#pragma once

#include "Types.h"
#include <vector>
#include <array>
#include <string>

#include <ostream>
#include <istream>


struct MeshLoader
{
	struct VertexV3T2 {
		struct { float x, y, z; } vertex;
		struct { float s, t; } texCoords;
	};

	std::vector<VertexV3T2> vertex_data;
	std::vector<unsigned int> index_data;
	unsigned int numFaces;

	std::vector<vec3f> meshVerts;
	std::vector<vec3f> meshNorms;
	std::vector<vec3f> meshFaceCenters;
	std::vector<vec3f> meshFaceNorms;

	std::string texture;

	using Ptr = std::shared_ptr<MeshLoader>;

	static std::vector<std::string> loadFromFile(const std::string& path);

	std::ostream& serialize(std::ostream& os) const;
	std::istream& deserialize(std::istream& is);

};

inline std::ostream& operator<<(std::ostream& os, const MeshLoader& mesh) {
	return mesh.serialize(os);
}

inline std::istream& operator>>(std::istream& is, MeshLoader& mesh) {
	return mesh.deserialize(is);
}
