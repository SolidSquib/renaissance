#pragma once

#include "Renaissance/Graphics/Texture.h"

#include <glad/glad.h>

namespace Renaissance::Graphics
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const String& path);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(uint32_t width, uint32_t height, void* data);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return mWidth; }
		virtual uint32_t GetHeight() const override { return mHeight; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return mRendererId == ((OpenGLTexture2D&)other).mRendererId;
		}

	private:
		uint32_t mWidth = 0, mHeight = 0;
		uint32_t mRendererId = 0;
		String mPath = "";
		GLenum mInternalFormat = GL_RGB8;
		GLenum mDataFormat = GL_RGB;
	};

	class OpenGLTexture3D : public Texture3D
	{
	public:
		OpenGLTexture3D(const String& path);
		virtual ~OpenGLTexture3D();

		virtual uint32_t GetWidth() const override { return mWidth; }
		virtual uint32_t GetHeight() const override { return mHeight; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return mRendererId == ((OpenGLTexture3D&)other).mRendererId;
		}

	private:
		uint32_t mWidth = 0, mHeight = 0;
		uint32_t mRendererId = 0;
		String mPath = "";
	};
}