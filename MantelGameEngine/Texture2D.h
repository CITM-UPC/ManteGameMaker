#pragma once

#include <string>
#include <memory>

#define CHECKERS_HEIGHT 100
#define CHECKERS_WIDTH 100


class Texture2D
{
private:
	unsigned int _id;

public:
	using Ptr = std::shared_ptr<Texture2D>;
	explicit Texture2D(const std::string& path);

	std::string path;
	int width;
	int height;

	Texture2D(Texture2D&& tex) noexcept;
	Texture2D();
	void bind() const;

	static std::string transformToDDS(const std::string& path);

	~Texture2D();
	

	unsigned int getID() { return _id; };

private:
	Texture2D(const Texture2D&);
	Texture2D operator=(const Texture2D&);
};

