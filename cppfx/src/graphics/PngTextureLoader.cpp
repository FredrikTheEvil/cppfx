#include <cppfx/config.h>
#ifdef CPPFX_USE_LIBPNG
#include <cppfx/graphics/PngTextureLoader.h>
#include <fstream>
#include <cppfx/io/Exceptions.h>

namespace cppfx
{
	namespace graphics
	{

		PngTextureLoader::PngTextureLoader(const ref_ptr<Context>& context) :
			context(context)
		{
		}
		PngTextureLoader::~PngTextureLoader()
		{
		}
		ref_ptr<Texture2D> PngTextureLoader::loadTexture2D(const string& fileName)
		{
			png_structp png_ptr;
			png_infop info_ptr;
			png_byte header[8];
			unsigned int sig_read = 0;
			int color_type, interlace_type;
			FILE *fp = nullptr;
#ifdef CPPFX_HAVE_FOPEN_S
			fopen_s(&fp, fileName.data(), "rb");
#else
			fp = fopen(fileName.data(), "rb");
#endif
			if (fp == nullptr)
				throw io::FileNotFoundException(fileName);

			if ((sig_read = fread(header, 1, 8, fp)) != 8) {
				fclose(fp);
				throw io::EofException("end of file while reading file signature");
			}

			if (png_sig_cmp(header, 0, sig_read))
			{
				fclose(fp);
				throw io::BadFileFormatException("file is not a png file: " + fileName);
			}

			png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
				nullptr, nullptr, nullptr);
			if (png_ptr == nullptr)
			{
				fclose(fp);
				throw RuntimeError("failed to initialize libpng");
			}
			info_ptr = png_create_info_struct(png_ptr);
			if (info_ptr == nullptr) {
				fclose(fp);
				png_destroy_read_struct(&png_ptr, nullptr, nullptr);
				throw OutOfMemoryException("failed to allocate memory for png loading");
			}
			if (setjmp(png_jmpbuf(png_ptr))) {
				png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
				fclose(fp);
				throw RuntimeError("failed to read png file");
			}
			png_init_io(png_ptr, fp);
			png_set_sig_bytes(png_ptr, sig_read);
			png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, nullptr);
			png_uint_32 width, height;
			int bit_depth;
			png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
				&interlace_type, nullptr, nullptr);
			unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
			std::vector<char> data(row_bytes * height);
			png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
			for (int i = 0; i < height; i++)
				memcpy(&data[0] + (row_bytes * i), row_pointers[i], row_bytes);
			png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
			fclose(fp);
			GLenum format = color_type == PNG_COLOR_TYPE_RGB_ALPHA ? GL_RGBA8 : GL_RGB8;
			GLenum dataFormat = color_type == PNG_COLOR_TYPE_RGB_ALPHA ? GL_RGBA : GL_RGB;
			return context->createTexture2D(TextureFormat(format), width, height, TextureDataFormat(dataFormat), TextureDataType::UNSIGNED_BYTE, &data[0]);
		}

	}
}
#endif