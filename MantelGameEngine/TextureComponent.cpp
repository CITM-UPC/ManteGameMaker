#include "TextureComponent.h"

TextureComponent::TextureComponent(GameObject* owner) : Component(owner)
{
	componentType = TEXTURE;
}

TextureComponent::~TextureComponent()
{
}

void TextureComponent::setTexture(Texture2D::Ptr texture)
{
		_texture = texture;
}

Texture2D::Ptr TextureComponent::getTexture()
{
	return _texture;
}
