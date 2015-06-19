#pragma once
#ifndef __CPPFX_GUI_SPRITEBATCH_H
#define __CPPFX_GUI_SPRITEBATCH_H
#include <cppfx/Object.h>
#include <cppfx/vector.h>
#include <cppfx/graphics/Buffer.h>
#include <cppfx/ref_ptr.h>
#include <cppfx/graphics/Texture2D.h>
#include <cppfx/graphics/Context.h>
#include <cppfx/gui/SpriteBatchSprite.h>
#include <cppfx/gui/GuiShader.h>

namespace cppfx
{
	namespace gui
	{
		struct CPPFX_API SpriteBatchDrawCall
		{
			SpriteBatchDrawCall();
			SpriteBatchDrawCall(const SpriteBatchDrawCall& dc);
			SpriteBatchDrawCall& operator = (const SpriteBatchDrawCall& dc);

			ref_ptr<graphics::Texture2D> texture;
			ref_ptr<GuiShader> shader;
			int offset;
		};

		class CPPFX_API SpriteBatch : public Object
		{
		public:
			SpriteBatch(ref_ptr<graphics::Context> context, ref_ptr<GuiShader> shader);
			void setScreenSize(int width, int height);
			void clear();
			void updateVertices(size_t offset, const SpriteBatchSprite& sprites);
			void push(const SpriteBatchSprite& sprite, const ref_ptr<graphics::Texture2D>& texture);
			void push(const SpriteBatchSprite& sprite, const ref_ptr<graphics::Texture2D>& texture, const ref_ptr<GuiShader>& shader);
			void draw();

			size_t getNumVertices() const;
		private:
			SpriteBatch(const SpriteBatch&) = delete;
			SpriteBatch& operator = (const SpriteBatch&) = delete;

			vector2f screenSize;
			ref_ptr<graphics::Context> context;
			size_t capacity;			
			std::vector<SpriteVertex> vertices;
			bool verticesDirty;
			std::vector<unsigned int> indices;
			bool indicesDirty;
			ref_ptr<graphics::Buffer> vertexBuffer;
			ref_ptr<graphics::Buffer> indexBuffer;
			ref_ptr<graphics::VertexArray> vertexArray;
			int index;
			
			std::vector<SpriteBatchDrawCall> drawCalls;

			ref_ptr<GuiShader> shader;
		};
	}
}

#endif