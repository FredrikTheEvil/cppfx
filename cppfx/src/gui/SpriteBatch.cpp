#include <cppfx/gui/SpriteBatch.h>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.inl>

namespace cppfx
{
	namespace gui
	{
		SpriteBatchDrawCall::SpriteBatchDrawCall() :
			texture(nullptr),
			shader(nullptr),
			offset(0)
		{
		}

		SpriteBatchDrawCall::SpriteBatchDrawCall(const SpriteBatchDrawCall& dc) :
			texture(dc.texture),
			shader(dc.shader),
			offset(dc.offset)
		{
		}

		SpriteBatchDrawCall& SpriteBatchDrawCall::operator=(const SpriteBatchDrawCall& dc)
		{
			texture = dc.texture;
			shader = dc.shader;
			offset = dc.offset;
			return *this;
		}

		SpriteBatch::SpriteBatch(ref_ptr<graphics::Context> context, ref_ptr<GuiShader> guiShader) :
			screenSize(640, 480),
			context(context),
			capacity(65000),
			vertices(),
			indices(),
			index(0),
			shader(guiShader)
		{
			vertices.reserve(capacity);
			indices.reserve(capacity);
			static const std::vector<graphics::VertexArrayElement> elements = {
				{ 2, graphics::VertexElementType::FLOAT, graphics::VertexAttributeType::VERTEX, 0, false },
				{ 2, graphics::VertexElementType::FLOAT, graphics::VertexAttributeType::UV1, 0, false },
				{ 4, graphics::VertexElementType::FLOAT, graphics::VertexAttributeType::COLOR, 0, false }
			};
			vertexBuffer = context->createBuffer(vertices.capacity()*sizeof(SpriteVertex), vertices.data(), graphics::BufferUsage::DYNAMIC_DRAW, graphics::BufferTarget::VERTEX_BUFFER);
			indexBuffer = context->createBuffer(indices.capacity() * sizeof(unsigned int), indices.data(), graphics::BufferUsage::DYNAMIC_DRAW, graphics::BufferTarget::INDEX_BUFFER);
			std::vector< ref_ptr<graphics::Buffer> > vertexBuffers = { vertexBuffer };
			vertexArray = context->createVertexArray(elements, vertexBuffers, indexBuffer);
		}

		void SpriteBatch::clear()
		{
			index = 0;
			vertices.clear();
			verticesDirty = true;
			indices.clear();
			indicesDirty = true;
			drawCalls.clear();
		}

		void SpriteBatch::updateVertices(size_t offset, const SpriteBatchSprite& sprite)
		{
			if (offset >= vertices.size())
				throw std::runtime_error("sprite vertex out of range");
			vertices[offset + 0] = sprite.topLeft;
			vertices[offset + 1] = sprite.bottomLeft;
			vertices[offset + 2] = sprite.topRight;
			vertices[offset + 3] = sprite.bottomRight;
			verticesDirty = true;
		}

		void SpriteBatch::setScreenSize(int width, int height)
		{
			screenSize = vector2f(float(width), float(height));
		}

		void SpriteBatch::push(const SpriteBatchSprite& sprite, const ref_ptr<graphics::Texture2D>& texture)
		{
			if (drawCalls.size() == 0)
			{
				SpriteBatchDrawCall drawCall;
				drawCall.texture = texture;
				drawCall.offset = indices.size();
				drawCalls.push_back(drawCall);
			}
			else
			{
				SpriteBatchDrawCall& drawCall = drawCalls[drawCalls.size() - 1];
				if (drawCall.texture != texture || drawCall.shader.valid())
				{
					SpriteBatchDrawCall newDrawCall;
					newDrawCall.texture = texture;
					newDrawCall.shader = nullptr;
					newDrawCall.offset = indices.size();
					drawCalls.push_back(newDrawCall);
				}
			}

			int i = vertices.size();
			vertices.push_back(sprite.topLeft);
			vertices.push_back(sprite.bottomLeft);
			vertices.push_back(sprite.topRight);
			vertices.push_back(sprite.bottomRight);
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
			indices.push_back(i + 1);
			indices.push_back(i + 3);
			indices.push_back(i + 2);
			verticesDirty = true;
			indicesDirty = true;
		}

		void SpriteBatch::push(const SpriteBatchSprite& sprite, const ref_ptr<graphics::Texture2D>& texture, const ref_ptr<GuiShader>& shader)
		{
			if (drawCalls.size() == 0)
			{
				SpriteBatchDrawCall drawCall;
				drawCall.texture = texture;
				drawCall.offset = indices.size();
				drawCalls.push_back(drawCall);
			}
			else
			{
				SpriteBatchDrawCall& drawCall = drawCalls[drawCalls.size() - 1];
				if (drawCall.texture != texture || drawCall.shader != shader)
				{
					SpriteBatchDrawCall newDrawCall;
					newDrawCall.texture = texture;
					newDrawCall.shader = shader;
					newDrawCall.offset = indices.size();
					drawCalls.push_back(newDrawCall);
				}
			}

			int i = vertices.size();
			vertices.push_back(sprite.topLeft);
			vertices.push_back(sprite.bottomLeft);
			vertices.push_back(sprite.topRight);
			vertices.push_back(sprite.bottomRight);
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
			indices.push_back(i + 1);
			indices.push_back(i + 3);
			indices.push_back(i + 2);
			verticesDirty = true;
			indicesDirty = true;
		}

		void SpriteBatch::draw()
		{
			matrix4f projMatrix = glm::ortho(0.0f, screenSize.x, screenSize.y, 0.0f, 0.0f, 10000.0f);
			GuiShader* curShaderPtr = nullptr;

			int numDrawCalls = drawCalls.size();
			if (numDrawCalls > 0) {
				if (verticesDirty) {
					vertexBuffer->setSubData(0, vertices.size()*sizeof(SpriteVertex), vertices.data());
					verticesDirty = false;
				}
				if (indicesDirty) {
					indexBuffer->setSubData(0, indices.size()*sizeof(unsigned int), indices.data());
					indicesDirty = false;
				}
				context->bindVertexArray(vertexArray);
				for (int i = 0; i < numDrawCalls; i++)
				{
					SpriteBatchDrawCall& drawCall = drawCalls[i];
					context->bindTexture(0, drawCall.texture);
					int end = 0;
					if (i + 1 < numDrawCalls)
						end = drawCalls[i + 1].offset;
					else
						end = indices.size();
					int count = end - drawCall.offset;
					if (drawCall.shader.valid())
					{
						if (drawCall.shader != curShaderPtr)
						{
							curShaderPtr = drawCall.shader.get();
							curShaderPtr->setMatrix(projMatrix);
							curShaderPtr->use();
						}
					}
					else
					{
						auto shaderPtr = shader.get();
						if (shaderPtr != curShaderPtr)
						{
							curShaderPtr = shaderPtr;
							shaderPtr->setMatrix(projMatrix);
							shaderPtr->use();
						}
					}
					context->drawElements(graphics::MeshTopology::TRIANGLES, drawCall.offset*sizeof(unsigned int), count);
				}
			}
		}

		size_t SpriteBatch::getNumVertices() const
		{
			return vertices.size();
		}
	}
}