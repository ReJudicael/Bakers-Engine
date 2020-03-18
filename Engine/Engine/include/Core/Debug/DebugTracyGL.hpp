#pragma once

#include <Tracy.hpp>
#include <glad/glad.h>
#include <TracyOpenGL.hpp>
#include <vector>

#ifndef TRACY_ENABLE

#define TRACY_GL_IMAGE
#define INIT_TRACY_GL_IMAGE(width, height) 
#define TRACY_GL_IMAGE_SEND(width, height) 

#else

#define TRACY_GL_IMAGE	Core::Debug::TracyGL*	__TracyGLImageSender;
#define INIT_TRACY_GL_IMAGE(width, height) __TracyGLImageSender = new Core::Debug::TracyGL(width, height);
#define TRACY_GL_IMAGE_SEND(width, height) __TracyGLImageSender->SendCapture(width, height);

namespace Core::Debug
{
	/**
	 *Sends debug opengl data to Tracy
	 */
	class TracyGL
	{
	private:
		GLuint	m_fiTexture[4];
		GLuint	m_fiFramebuffer[4]; 
		GLuint m_fiPbo[4];
		GLsync m_fiFence[4];
		int m_fiIdx = 0;
		std::vector <int> m_fiQueue;

		int m_width, m_height;
	public:
		inline TracyGL(long captureWidth = 320, long captureHeight = 180) noexcept;

		inline void	SendCapture(int screenWidth, int screenHeight);
	};

	inline	TracyGL::TracyGL(long captureWidth, long captureHeight) noexcept : m_fiFence(), m_width {captureWidth}, m_height {captureHeight}
	{
		glGenTextures(4, m_fiTexture);
		glGenFramebuffers(4, m_fiFramebuffer);
		glGenBuffers(4, m_fiPbo);
		for (int i = 0; i < 4; i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_fiTexture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glBindFramebuffer(GL_FRAMEBUFFER, m_fiFramebuffer[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
				m_fiTexture[i], 0);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, m_fiPbo[i]);
			glBufferData(GL_PIXEL_PACK_BUFFER, m_width* m_height * 4, nullptr, GL_STREAM_READ);
		}
	}

	inline void TracyGL::SendCapture(int screenWidth, int screenHeight)
	{
		ZoneScoped
			ZoneText("Sending data to tracy server", 29)
		while (!m_fiQueue.empty())
		{
			const auto fiIdx = m_fiQueue.front();
			if (glClientWaitSync(m_fiFence[fiIdx], 0, 0) == GL_TIMEOUT_EXPIRED) break;
			glDeleteSync(m_fiFence[fiIdx]);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, m_fiPbo[fiIdx]);
			auto ptr = glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, 320 * 180 * 4, GL_MAP_READ_BIT);
			FrameImage(ptr, m_width, m_height, m_fiQueue.size(), true);
			glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
			m_fiQueue.erase(m_fiQueue.begin());
		}
		TracyGpuZone("Resizing frame data for capture sending")
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fiFramebuffer[m_fiIdx]);
		glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fiFramebuffer[m_fiIdx]);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, m_fiPbo[m_fiIdx]);
		glReadPixels(0, 0, 320, 180, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		m_fiFence[m_fiIdx] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		m_fiQueue.emplace_back(m_fiIdx);
		m_fiIdx = (m_fiIdx + 1) % 4;
	}
}
#endif
