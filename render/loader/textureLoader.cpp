#include"textureLoader.h"
#include"../global/config.h"
#include"cache.h"

TextureLoader::TextureLoader()noexcept
{

}

TextureLoader::~TextureLoader()noexcept
{

}

Texture::Ptr TextureLoader::loader(const std::string& path, unsigned char* dataIn , uint32_t widthIn , uint32_t heithtIn )
{
	Texture::Ptr texture = nullptr;
	std::string filePath = path;

	if (filePath.empty())
	{
		filePath = DefaultTexturePath;
	}

	Source::Ptr source = Cache::GetInstance()->GetSource(path);
	if (source) {
		texture = Texture::Create(source->mWidth, source->mHeight);
		texture->mSource = source;
	}
	else
	{
		source = Source::Create();
		source->mNeedsUpdate = false;

		auto& data = source->mData;

		int	picType = 0;
		int	width = 0, height = 0;

		uint32_t dataSize{ 0 };
		unsigned char* bits{ nullptr };

		if (dataIn == nullptr)
		{
			std::fstream file(filePath);
			if (!file.is_open())
			{
				filePath = DefaultTexturePath;
			}
			else
			{
				file.close();
			}
			bits = stbi_load(filePath.c_str(), &width, &height, &picType, toStbImageFormat(TextureFormat::RGBA));

		}
		else
		{
			uint32_t	dataInSize = 0;
			if (!heithtIn)
			{
				dataInSize = widthIn;
			}
			else
			{
				dataInSize = widthIn * height;
			}
			bits = stbi_load_from_memory(dataIn, dataInSize, &width, &height, &picType, toStbImageFormat(TextureFormat::RGBA));
		
		}
		dataSize = width * height * toByteSize(TextureFormat::RGBA);
		
		if (dataSize && bits)
		{
			data.resize(dataSize);
			memcpy(data.data(), bits, dataSize);
		}
		stbi_image_free(bits);

		source->mWidth = width;
		source->mHeight = height;
		texture = Texture::Create(source->mWidth, source->mHeight);
	}
	texture->mSource = source;
	Cache::GetInstance()->CacheSource(filePath, source);
	return texture;
}