#pragma once
#ifndef __CPPFX_GUI_GUISHADER_H
#define __CPPFX_GUI_GUISHADER_H
#include <cppfx/Object.h>
#include <cppfx/graphics/Context.h>
#include <cppfx/graphics/ShaderProgram.h>
#include <cppfx/graphics/Texture2D.h>

namespace cppfx
{
	namespace gui
	{
		class CPPFX_API GuiShader : public Object
		{
		public:
			GuiShader(const ref_ptr<graphics::Context>& context, const string& vertexShader, const string& fragmentShader);

			void setMatrix(const matrix4f& mvp);
			void use();
		private:
			GuiShader(const GuiShader&) = delete;
			GuiShader& operator = (const GuiShader&) = delete;

			virtual ~GuiShader();
			ref_ptr<graphics::Context> context;
			ref_ptr<graphics::ShaderProgram> shaderProgram;
			ref_ptr<graphics::ShaderProgramParameter> shaderParamMvp;
			matrix4f shaderParamMvpValue;
			bool shaderParamMvpDirty;
			ref_ptr<graphics::ShaderProgramParameter> shaderParamTexture;
			bool shaderParamTextureDirty;
		};
	}
}

#endif