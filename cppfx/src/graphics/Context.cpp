#include "cppfx/graphics/Context.h"
#include "cppfx/graphics/BmpTextureLoader.h"

#ifdef CPPFX_USE_GLEW
#include <GL/glew.h>
#endif
#include <map>
#include <string.h>

namespace cppfx {
	namespace graphics {
		Context* Context::s_current = nullptr;

		Context::Context() :
			clearColor(0.0f, 0.0f, 0.0f, 0.0f),
			depthTestEnabled(false),
			depthFunc(DepthFunc::LEQUAL),
			scissorTestEnabled(false),
			scissorRegion(0, 0, 0, 0),
			blendingEnabled(false),
			blendFuncSourceRGB(BlendFunc::SRC_ALPHA),
			blendFuncSourceAlpha(BlendFunc::SRC_ALPHA),
			blendFuncDestRGB(BlendFunc::ONE_MINUS_SRC_ALPHA),
			blendFuncDestAlpha(BlendFunc::ONE_MINUS_SRC_ALPHA),
			eventHandlers()
		{
		}
		Context::~Context() {
			if (s_current == this)
				s_current = nullptr;
			
		}

		void Context::setTextureLoader(ITextureLoader* loader)
		{
			textureLoader = loader;
		}

		bool Context::isCurrent() {
			return s_current == this;
		}

		void Context::onResize(int width, int height)
		{
			Context::width = width;
			Context::height = height;
			glViewport(0, 0, width, height);
		}

		int Context::getWidth() const
		{
			return width;
		}

		int Context::getHeight() const
		{
			return height;
		}

		void Context::clear(ClearFlags clearFlags)
		{
			glClear(uint32_t(clearFlags));
		}

		void Context::setClearColor(const colorf& color)
		{
			if (clearColor != color) {
				clearColor = color;
				glClearColor(color.r, color.g, color.b, color.a);
			}
		}

		const colorf& Context::getClearColor() const
		{
			return clearColor;
		}

		void Context::setScissorTestEnabled(bool enabled)
		{
			if (scissorTestEnabled != enabled)
			{
				scissorTestEnabled = enabled;
				if (enabled)
					glEnable(GL_SCISSOR_TEST);
				else
					glDisable(GL_SCISSOR_TEST);
			}
		}

		bool Context::getScissorTestEnabled() const
		{
			return scissorTestEnabled;
		}

		void Context::setScissorRegion(const vector4i& region)
		{
			if (scissorRegion != region)
			{
				scissorRegion = region;
				glScissor(region.x, region.y, region.z, region.w);
			}
		}

		const vector4i& Context::getScissorRegion() const
		{
			return scissorRegion;
		}

		void Context::setBlendingEnabled(bool enabled)
		{
			if (enabled != blendingEnabled)
			{
				blendingEnabled = enabled;
				if (enabled)
					glEnable(GL_BLEND);
				else
					glDisable(GL_BLEND);
			}
		}

		bool Context::getBlendingEnabled() const
		{
			return blendingEnabled;
		}

		void Context::setDepthTestEnabled(bool enabled)
		{
			if (depthTestEnabled != enabled)
			{
				depthTestEnabled = enabled;
				glEnable(GL_DEPTH_TEST);
			}
		}

		void Context::setDepthFunc(DepthFunc depthFunc)
		{
			if (Context::depthFunc != depthFunc)
			{
				Context::depthFunc = depthFunc;
				glDepthFunc(GLenum(depthFunc));
			}
		}

		void Context::setBlendFunc(BlendFunc source, BlendFunc dest)
		{
			if (blendFuncSourceRGB == source &&
				blendFuncDestRGB == dest   &&
				blendFuncSourceAlpha == source &&
				blendFuncDestAlpha == dest)
				return; // Dont update OpenGL state if not needed
			glBlendFunc(uint32_t(source), uint32_t(dest));
			blendFuncSourceRGB = source;
			blendFuncDestRGB = dest;
			blendFuncSourceAlpha = source;
			blendFuncDestAlpha = dest;
		}

		void Context::setBlendFuncSeparate(BlendFunc sourceRGB, BlendFunc destRGB, BlendFunc sourceAlpha, BlendFunc destAlpha)
		{
			if (blendFuncSourceRGB == sourceRGB   &&
				blendFuncDestRGB == destRGB     &&
				blendFuncSourceAlpha == sourceAlpha &&
				blendFuncDestAlpha == destAlpha)
				return; // Dont update OpenGL state if not needed

			glBlendFuncSeparate(uint32_t(sourceRGB), uint32_t(destRGB), uint32_t(sourceAlpha), uint32_t(destAlpha));
			blendFuncSourceRGB = sourceRGB;
			blendFuncSourceAlpha = sourceAlpha;
			blendFuncDestRGB = destRGB;
			blendFuncDestAlpha = destAlpha;
		}

		BlendFunc Context::getBlendFuncSourceRGB() const
		{
			return blendFuncSourceRGB;
		}

		BlendFunc Context::getBlendFuncDestRGB() const
		{
			return blendFuncDestRGB;
		}

		BlendFunc Context::getBlendFuncSourceAlpha() const
		{
			return blendFuncSourceAlpha;
		}

		BlendFunc Context::getBlendFuncDestAlpha() const
		{
			return blendFuncDestAlpha;
		}

		ref_ptr<Buffer> Context::createBuffer(size_t size, const void* data, BufferUsage usage, BufferTarget hint)
		{
			ref_ptr<Buffer> buffer(new Buffer(this, size, data, usage, hint));
			return buffer;
		}

		ref_ptr<VertexArray> Context::createVertexArray(const std::vector<VertexArrayElement>& elements, const std::vector<ref_ptr<Buffer>>& buffers, ref_ptr<Buffer> indexBuffer)
		{
			ref_ptr<VertexArray> vao(new VertexArray(this, elements, buffers, indexBuffer));
			return vao;
		}

		ref_ptr<ShaderProgram> Context::createShaderProgram(const string& vertexShader, const string& fragmentShader)
		{
			ref_ptr<ShaderProgram> program(new ShaderProgram(this, vertexShader, fragmentShader));
			return program;
		}

		ref_ptr<Texture2D> Context::createTexture2D(TextureFormat format, int width, int height, TextureDataFormat dataFormat, TextureDataType dataType, const void* data)
		{
			ref_ptr<Texture2D> texture(new Texture2D(this, format, width, height, dataFormat, dataType, data));
			return texture;
		}

		ref_ptr<Texture2D> Context::loadTexture2D(const string& fileName)
		{
			if (textureLoader.valid())
			{
				ref_ptr<Texture2D> tex = textureLoader->loadTexture2D(fileName);
				return tex;
			}
			return nullptr;
		}

		void Context::bindTexture(unsigned textureUnit, ref_ptr<Texture> texture)
		{
			if (textureUnit >= size_t(capsNumTexUnits))
				throw std::runtime_error("texture unit index out of range");
			if (textureUnits[textureUnit] != texture)
			{
				glActiveTexture(GL_TEXTURE0 + textureUnit);
				if (texture.valid())
					texture->bind();
				else
					glBindTexture(GL_TEXTURE_2D, 0);
				textureUnits[textureUnit] = texture;
			}
		}

		ref_ptr<Texture> Context::getBoundTexture(unsigned textureUnit)
		{
			if (textureUnit >= size_t(capsNumTexUnits))
				throw std::runtime_error("texture unit index out of range");
			return textureUnits[textureUnit];
		}

		void Context::bindVertexArray(ref_ptr<VertexArray> array)
		{
			if (currentVertexArray == array)
				return;
			if (currentVertexArray.valid())
				currentVertexArray->Unbind();
			if (array.valid())
				array->Bind();
			currentVertexArray = array;
		}

		ref_ptr<VertexArray> Context::getBoundVertexArray()
		{
			return currentVertexArray;
		}

		void Context::useShaderProgram(ref_ptr<ShaderProgram> shaderProgram)
		{
			if (shaderProgram.valid())
				glUseProgram(shaderProgram->id);
			else
				glUseProgram(0);
		}

		void Context::drawElements(MeshTopology mode, size_t offset, size_t numIndices)
		{
			if (currentVertexArray.valid())
				glDrawElements(GLenum(mode), numIndices, GL_UNSIGNED_INT, reinterpret_cast<const void*>(offset));
		}

		void Context::drawArray(MeshTopology mode, size_t numVertices)
		{
			if (currentVertexArray.valid())
				glDrawArrays(GLenum(mode), 0, numVertices);
		}

		int32_t Context::getNumTextureUnits() const
		{
			return capsNumTexUnits;
		}

		int32_t Context::getMaximumTextureSize() const
		{
			return capsMaxTextureSize;
		}

		int32_t Context::getMaximumTextureSize3D() const
		{
			return capsMaxTextureSize3D;
		}

		int32_t Context::getMaximumTextureSizeCube() const
		{
			return capsMaxTextureSizeCube;
		}

		int32_t Context::getMajorVersion() const
		{
			return capsGlVersionMajor;
		}

		int32_t Context::getMinorVersion() const
		{
			return capsGlVersionMinor;
		}

		int32_t Context::getPatchVersion() const
		{
			return capsGlVersionPatch;
		}

		int32_t Context::getRevisionVersion() const
		{
			return capsGlVersionRevision;
		}

		int32_t Context::getShadingLanguageVersionMajor() const
		{
			return capsGlslVersionMajor;
		}

		int32_t Context::getShadingLanguageVersionMinor() const
		{
			return capsGlslVersionMinor;
		}

		bool Context::extensionAvailable(const string& extension) const
		{
			return capsExtensions.find(extension) != capsExtensions.end();
		}

		void Context::addEventHandler(platform::EventHandler handler)
		{
			if (handler)
				eventHandlers.push_back(handler);
		}

		void Context::removeEventHandler(const platform::EventHandler& handler)
		{
			for (auto itr = eventHandlers.begin(); itr != eventHandlers.end(); ++itr)
			{
				if ((&(*itr)) == &handler)
				{
					eventHandlers.erase(itr);
				}
			}
		}
	} // namespace cppfx::graphics
} // namespace cppfx