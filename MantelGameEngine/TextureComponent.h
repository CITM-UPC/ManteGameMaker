#pragma once
#include "Component.h"
#include "Texture2D.h"

class TextureComponent : public Component {
public:

	TextureComponent(GameObject* owner);
	~TextureComponent();

	void setTexture(Texture2D::Ptr texture);

	Texture2D::Ptr getTexture();

protected:

private:
	Texture2D::Ptr _texture = nullptr;
};