#pragma once
#ifndef __CPPFX_GRAPHICS_CONTEXT_H
#define __CPPFX_GRAPHICS_CONTEXT_H

#include <cppfx/vector.h>
#include <cppfx/ref_ptr.h>

#include <cppfx/graphics/Enums.h>
#include <cppfx/graphics/VertexArray.h>
#include <cppfx/graphics/ShaderProgram.h>
#include <cppfx/graphics/Texture2D.h>
#include <cppfx/graphics/ITextureLoader.h>
#include <cppfx/string.h>
#include <cppfx/platform/Event.h>
#include <set>

#if _MSC_VER && !__INTEL_COMPILER
#	pragma warning(push)
#	pragma warning( disable : 4251 )
#endif

namespace cppfx {
	namespace graphics {
		class CPPFX_API Context : public Object {
		public:
			void setTextureLoader(ITextureLoader* loader);

			bool isCurrent();

			void addEventHandler(platform::EventHandler handler);
			void removeEventHandler(const platform::EventHandler& handler);

			virtual bool processEvents() = 0;
			virtual void makeCurrent() = 0;
			virtual void swapBuffers() = 0;

			int getWidth() const;
			int getHeight() const;

			void clear(ClearFlags clearFlags);
			void setClearColor(const colorf& color);
			const colorf& getClearColor() const;

			void setScissorTestEnabled(bool enabled);
			bool getScissorTestEnabled() const;

			void setScissorRegion(const vector4i& region);
			const vector4i& getScissorRegion() const;

			void setBlendingEnabled(bool enabled);
			bool getBlendingEnabled() const;

			void setDepthTestEnabled(bool enabled);
			void setDepthFunc(DepthFunc depthFunc);

			void setBlendFunc(BlendFunc source, BlendFunc dest);
			void setBlendFuncSeparate(BlendFunc sourceRGB, BlendFunc destRGB, BlendFunc sourceAlpha, BlendFunc destAlpha);
			BlendFunc getBlendFuncSourceRGB() const;
			BlendFunc getBlendFuncDestRGB() const;
			BlendFunc getBlendFuncSourceAlpha() const;
			BlendFunc getBlendFuncDestAlpha() const;

			ref_ptr<Buffer> createBuffer(size_t size, const void* data, BufferUsage usage, BufferTarget hint);
			ref_ptr<VertexArray> createVertexArray(const std::vector<VertexArrayElement>& elements, const std::vector< ref_ptr<Buffer> >& buffers, ref_ptr<Buffer> indexBuffer);
			ref_ptr<ShaderProgram> createShaderProgram(const string& vertexShader, const string& fragmentShader);
			ref_ptr<Texture2D> createTexture2D(TextureFormat format, int width, int height, TextureDataFormat dataFormat, TextureDataType dataType, const void* data);
			ref_ptr<Texture2D> loadTexture2D(const string& fileName);

			void bindTexture(unsigned int textureUnit, ref_ptr<Texture> texture);
			ref_ptr<Texture> getBoundTexture(unsigned int textureUnit);

			void bindVertexArray(ref_ptr<VertexArray> array);
			ref_ptr<VertexArray> getBoundVertexArray();

			void useShaderProgram(ref_ptr<ShaderProgram> shaderProgram);

			void drawElements(MeshTopology mode, size_t offset, size_t numIndices);
			void drawArray(MeshTopology mode, size_t numVertices);

			// Capabilities
			int32_t getNumTextureUnits() const;
			int32_t getMaximumTextureSize() const;
			int32_t getMaximumTextureSize3D() const;
			int32_t getMaximumTextureSizeCube() const;

			int32_t getMajorVersion() const;
			int32_t getMinorVersion() const;
			int32_t getPatchVersion() const;
			int32_t getRevisionVersion() const;

			int32_t getShadingLanguageVersionMajor() const;
			int32_t getShadingLanguageVersionMinor() const;

			bool extensionAvailable(const string& extension) const;
		protected:
			Context();
			~Context();
			void onResize(int width, int height);
			static Context* s_current;
		private:
			Context(const Context&) = delete;
			Context& operator = (const Context&) = delete;
		protected:
			void enumerateCapabilities();

			int width;
			int height;

			colorf clearColor;

			string vendorName;
			string rendererName;

			bool depthTestEnabled;
			DepthFunc depthFunc;
			
			bool scissorTestEnabled;
			vector4i scissorRegion;

			bool blendingEnabled;
			BlendFunc blendFuncSourceRGB;
			BlendFunc blendFuncSourceAlpha;
			BlendFunc blendFuncDestRGB;
			BlendFunc blendFuncDestAlpha;

			std::set<string> capsExtensions;

			int32_t capsNumTexUnits;
			int32_t capsMaxTextureSize;
			int32_t capsMaxTextureSizeCube;
			int32_t capsMaxTextureSize3D;

			int32_t capsGlVersionMajor;
			int32_t capsGlVersionMinor;
			int32_t capsGlVersionPatch;
			int32_t capsGlVersionRevision;

			int32_t capsGlslVersionMajor;
			int32_t capsGlslVersionMinor;

			bool capsVertexArrayObject;

			std::vector< ref_ptr<Texture> > textureUnits;
			ref_ptr<VertexArray> currentVertexArray;

			ref_ptr<ITextureLoader> textureLoader;

			std::vector<platform::EventHandler> eventHandlers;
		};
	} // namespace cppfx::graphics
} // namespace cppfx

#if _MSC_VER && !__INTEL_COMPILER
#	pragma warning(pop)
#endif

#endif