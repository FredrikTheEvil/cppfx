#pragma once
#ifndef __CPPFX_GRAPHICS_ENUMS_H
#define __CPPFX_GRAPHICS_ENUMS_H

#include <cppfx/config.h>
#include <cppfx/export.h>

#include <type_traits>

#ifdef CPPFX_USE_GLEW
#include <GL/glew.h>
#endif

#ifndef BITMASK
#define BITMASK(EnumType) \
	inline EnumType operator | (EnumType x, EnumType y) { \
		return EnumType(std::underlying_type<EnumType>::type(x) | std::underlying_type<EnumType>::type(y)); \
		} \
	inline bool operator & (EnumType x, EnumType y) { \
		return (std::underlying_type<EnumType>::type(x) | std::underlying_type<EnumType>::type(y)) != 0; \
		}
#endif
namespace cppfx {
	namespace graphics {
		enum ClearFlags : unsigned int {
			COLOR_BUFFER = GL_COLOR_BUFFER_BIT,
			DEPTH_BUFFER = GL_DEPTH_BUFFER_BIT
		};
		BITMASK(ClearFlags)
		enum class MeshTopology : unsigned int {
			TRIANGLES = GL_TRIANGLES
		};
		enum class DepthFunc : unsigned int {
			ALWAYS = GL_ALWAYS,
			NEVER = GL_NEVER,
			LESS = GL_LESS,
			LEQUAL = GL_LEQUAL,
			EQUAL = GL_EQUAL,
			NOTEQUAL = GL_NOTEQUAL,
			GEQUAL = GL_GEQUAL,
			GREATER = GL_GREATER
		};
		enum class ShaderType : unsigned int
		{
			VERTEX_SHADER = GL_VERTEX_SHADER,
			FRAGMENT_SHADER = GL_FRAGMENT_SHADER
		};
		enum class TextureDataType : unsigned int {
			UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
			UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
			UNSIGNED_INT = GL_UNSIGNED_INT,
			FLOAT = GL_FLOAT
		};
		enum class IndexType : unsigned int {
			UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
			UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
			UNSIGNED_INT = GL_UNSIGNED_INT
		};
		enum class VertexElementType : unsigned int {
			UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
			UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
			UNSIGNED_INT = GL_UNSIGNED_INT,
			FLOAT = GL_FLOAT
		};
		enum class VertexAttributeType : unsigned int {
			VERTEX = 0,
			NORMAL = 1,
			TANGENT = 2,
			UV1 = 3,
			UV2 = 4,
			COLOR = 5,
			BONEINDICES = 6,
			BONEWEIGHTS = 7,

			USER_ATTRIBUTE0 = 8,
			USER_ATTRIBUTE1 = 9,
			USER_ATTRIBUTE2 = 10,
			USER_ATTRIBUTE3 = 11,
			USER_ATTRIBUTE4 = 12,
			USER_ATTRIBUTE5 = 13,
			USER_ATTRIBUTE6 = 14,
			USER_ATTRIBUTE7 = 15
		};
		enum class BufferTarget : unsigned int {
			VERTEX_BUFFER = GL_ARRAY_BUFFER,
			INDEX_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
			PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
			PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER
		};
		enum class BufferUsage : unsigned int {
			DYNAMIC_COPY = GL_DYNAMIC_COPY,
			DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
			DYNAMIC_READ = GL_DYNAMIC_READ,

			STATIC_COPY = GL_STATIC_COPY,
			STATIC_DRAW = GL_STATIC_DRAW,
			STATIC_READ = GL_STATIC_READ,

			STREAM_COPY = GL_STREAM_COPY,
			STREAM_DRAW = GL_STREAM_DRAW,
			STREAM_READ = GL_STREAM_READ
		};
		enum class MagFilter : unsigned int {
			LINEAR = GL_LINEAR,
			NEAREST = GL_NEAREST
		};
		enum class MinFilter : unsigned int {
			LINEAR = GL_LINEAR,
			NEAREST = GL_NEAREST,
			NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
			LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST
		};
		enum class TextureFormat : unsigned int {
			RGBA8 = GL_RGBA8,
			RGB8 = GL_RGB8
		};
		enum class TextureDataFormat : unsigned int {
			RGBA = GL_RGBA,
			RGB = GL_RGB
		};
		enum class ShaderProgramParameterType : unsigned int {
			FLOAT = GL_FLOAT,
			VECTOR2 = GL_FLOAT_VEC2,
			VECTOR3 = GL_FLOAT_VEC3,
			VECTOR4 = GL_FLOAT_VEC4,
			IVECTOR2 = GL_INT_VEC2,
			IVECTOR3 = GL_INT_VEC3,
			IVECTOR4 = GL_INT_VEC4,
			MATRIX2 = GL_FLOAT_MAT2,
			MATRIX3 = GL_FLOAT_MAT3,
			MATRIX4 = GL_FLOAT_MAT4,
			TEXTURE2D = GL_SAMPLER_2D
		};
		enum class BlendFunc : unsigned int {
			SRC_ALPHA = GL_SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA
		};
	}
}

#endif