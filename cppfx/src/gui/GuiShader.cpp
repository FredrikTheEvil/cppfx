#include <cppfx/gui/GuiShader.h>

namespace cppfx
{
	namespace gui
	{
		GuiShader::GuiShader(const ref_ptr<graphics::Context>& context, const string& vertexShader, const string& fragmentShader) :
			context(context),
			shaderProgram(context->createShaderProgram(vertexShader, fragmentShader)),
			shaderParamMvp(shaderProgram->getParameter("ModelViewProjectionMatrix")),
			shaderParamMvpValue(),
			shaderParamMvpDirty(true),
			shaderParamTexture(shaderProgram->getParameter("Texture")),
			shaderParamTextureDirty(true)
		{
		}

		void GuiShader::setMatrix(const matrix4f& mvp)
		{
			if (shaderParamMvpValue == mvp)
				return;
			shaderParamMvpValue = mvp;
			shaderParamMvpDirty = true;
		}

		void GuiShader::use()
		{
			context->useShaderProgram(shaderProgram);
			if (shaderParamMvpDirty && shaderParamMvp.valid())
			{
				shaderParamMvp->setMatrix4(shaderParamMvpValue);
				shaderParamMvpDirty = false;
			}
			if (shaderParamTextureDirty && shaderParamTexture.valid())
			{
				shaderParamTexture->setInt(0);
				shaderParamTextureDirty = false;
			}
		}

		GuiShader::~GuiShader()
		{
		}
	}
}