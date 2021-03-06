#include <cppfx/graphics/BmpTextureLoader.h>
#include <fstream>
#include <cppfx/io/Exceptions.h>

namespace cppfx {
	namespace graphics {
		enum class BitmapFileCompression : unsigned int {
			NONE = 0,
			RLE8 = 1,
			RLE4 = 2
		};
		struct BitmapFileInfo {
			char signature[2];
			unsigned int fileSize;
			unsigned short reserved0;
			unsigned short reserved1;
			unsigned int pixelOffset;
			unsigned int infoHeaderSize;
			unsigned int width;
			unsigned int height;
			unsigned short numPlanes;
			unsigned short numBitsPerPixel;
			BitmapFileCompression compression;
			unsigned int pixelDataSize;
			unsigned int pixelsPerMeterX;
			unsigned int pixelsPerMeterY;
			unsigned int numColors;
			unsigned int numImportantColors;

			void parse(std::istream& stream, const string& fileName) {
				stream.read(signature, 2);
				if (!stream.good())
				{
					if (stream.eof())
						throw io::EofException("end of file while reading file signature");
					throw io::IoException("failed to read file signature from stream");
				}
				if (signature[0] != 'B' || signature[1] != 'M')
					throw io::BadFileFormatException("file is not a bmp file: " + fileName);
				stream.read(reinterpret_cast<char*>(&fileSize), 4);
				stream.read(reinterpret_cast<char*>(&reserved0), 2);
				stream.read(reinterpret_cast<char*>(&reserved1), 2);
				stream.read(reinterpret_cast<char*>(&pixelOffset), 4);
				stream.read(reinterpret_cast<char*>(&infoHeaderSize), 4);
				stream.read(reinterpret_cast<char*>(&width), 4);
				stream.read(reinterpret_cast<char*>(&height), 4);
				stream.read(reinterpret_cast<char*>(&numPlanes), 2);
				stream.read(reinterpret_cast<char*>(&numBitsPerPixel), 2);
				stream.read(reinterpret_cast<char*>(&compression), 4);
				stream.read(reinterpret_cast<char*>(&pixelDataSize), 4);
				stream.read(reinterpret_cast<char*>(&pixelsPerMeterX), 4);
				stream.read(reinterpret_cast<char*>(&pixelsPerMeterY), 4);
				stream.read(reinterpret_cast<char*>(&numColors), 4);
				stream.read(reinterpret_cast<char*>(&numImportantColors), 4);

				if (numBitsPerPixel != 16 && numBitsPerPixel != 24 && numBitsPerPixel != 32)
					throw RuntimeError("unsupported pixel format");
			}
		};

		BmpTextureLoader::BmpTextureLoader(const ref_ptr<Context>& context) :
			context(context)
		{
		}
		
		void bmpCopyPixel(unsigned char* bmpPixelPtr, unsigned char* bufferPixelPtr, unsigned int numBitsPerPixel) {
			if (numBitsPerPixel == 16) {
				unsigned int b = *bmpPixelPtr & 0x1F;
				unsigned int g = (*bmpPixelPtr >> 5) & 0x3F;
				unsigned int r = (*bmpPixelPtr >> 11) & 0x1F;
				bufferPixelPtr[0] = r;
				bufferPixelPtr[1] = g;
				bufferPixelPtr[2] = b;
			}
			if (numBitsPerPixel == 24) {
				bufferPixelPtr[0] = bmpPixelPtr[2];
				bufferPixelPtr[1] = bmpPixelPtr[1];
				bufferPixelPtr[2] = bmpPixelPtr[0];
			}
			else if (numBitsPerPixel == 32) {
				bufferPixelPtr[2] = bmpPixelPtr[3];
				bufferPixelPtr[1] = bmpPixelPtr[2];
				bufferPixelPtr[0] = bmpPixelPtr[1];
				bufferPixelPtr[3] = bmpPixelPtr[0];
			}
		}

		ref_ptr<Texture2D> BmpTextureLoader::loadTexture2D(const string& fileName) {
			string filePath = fileName;
			for (auto i = filePath.begin(); i != filePath.end(); ++i)
			{
				if (*i == '\\')
					*i = '/';
			}
#ifdef WIN32
			for (auto i = filePath.begin(); i != filePath.end(); ++i)
			{
				if (*i == '/')
					*i = '\\';
			}
#endif
			std::ifstream ifs(filePath, std::ios_base::binary | std::ios_base::in);
			if (!ifs.good())
				throw io::FileNotFoundException(fileName);
			BitmapFileInfo fileInfo;
			fileInfo.parse(ifs, fileName);
			if (!ifs.good())
			{
				if (ifs.eof())
					throw io::EofException("end of file while reading bitmap header");
				throw io::IoException("failed to read bitmap header from stream");
			}
			
			unsigned int rowSize = (((fileInfo.numBitsPerPixel * fileInfo.width) + 31) / 32) * 4;
			unsigned int dataSize = rowSize * fileInfo.height;

			unsigned int bufferPixelSize = fileInfo.numBitsPerPixel / 8;
			if (fileInfo.numBitsPerPixel == 16)
				bufferPixelSize = 24;
			unsigned int bufferSize = fileInfo.width * fileInfo.height * bufferPixelSize;

			std::vector<char> imageData(dataSize);
			std::vector<char> buffer(bufferSize);
			ifs.read(const_cast<char*>(imageData.data()), dataSize);
			if (!ifs.good())
			{
				if (ifs.eof())
					throw io::EofException("end of file while reading bitmap data");
				throw io::IoException("failed to read bitmap data from stream");
			}

			if (fileInfo.compression != BitmapFileCompression::NONE)
				throw RuntimeError("bmp rle compression is not supported");

			for (unsigned int i = 0; i < fileInfo.height; i++) {
				auto y = fileInfo.height - i - 1;
				auto rowPtr = &imageData[rowSize*y];
				for (unsigned int x = 0; x < fileInfo.width; x++) {
					auto pixelPtr = &rowPtr[(fileInfo.numBitsPerPixel / 8)*x];
					auto bufferPixelPtr = &buffer[((i * fileInfo.height) + x) * bufferPixelSize];

					bmpCopyPixel(reinterpret_cast<unsigned char*>(pixelPtr), reinterpret_cast<unsigned char*>(bufferPixelPtr), fileInfo.numBitsPerPixel);
				}
			}

			TextureFormat format = bufferPixelSize == 3 ? TextureFormat::RGB8 :
				TextureFormat::RGBA8;
			TextureDataFormat dataFormat = bufferPixelSize == 3 ? TextureDataFormat::RGB :
				TextureDataFormat::RGBA;

			ref_ptr<Texture2D> texture = context->createTexture2D(format, fileInfo.width, fileInfo.height, dataFormat, TextureDataType::UNSIGNED_BYTE, buffer.data());
			return texture;
		}
	}
}