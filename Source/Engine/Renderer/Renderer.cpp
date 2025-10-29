#include "Renderer.h"
#include "Texture.h"

namespace neu {
    /// <summary>
    /// Initializes the SDL video subsystem and the SDL_ttf font library.
    /// This must be called before creating windows or performing any rendering.
    /// </summary>
    /// <returns>True if both SDL and TTF were successfully initialized; otherwise, false</returns>
    bool Renderer::Initialize() {
        // Initialize SDL video subsystem
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            LOG_ERROR("SDL_Init Error: {}", SDL_GetError());
            return false;
        }

        // Initialize SDL_ttf for TrueType font rendering
        if (!TTF_Init()) {
            LOG_ERROR("TTF_Init Error: {}", SDL_GetError());
            return false;
        }

        return true;
    }

    /// <summary>
    /// Shuts down the renderer and releases associated resources.
    /// </summary>
    void Renderer::Shutdown() {
        TTF_Quit();                         // Shutdown SDL_ttf
        SDL_GL_DestroyContext(m_context);   // Destroy the OpenGL context
        SDL_DestroyWindow(m_window);        // Destroy the window
        SDL_Quit();                         // Shutdown SDL
    }

    /// <summary>
    /// Initializes and creates an OpenGL window using SDL with the specified parameters.
    /// </summary>
    /// <param name="name">The title of the window.</param>
    /// <param name="width">The width of the window in pixels.</param>
    /// <param name="height">The height of the window in pixels.</param>
    /// <param name="fullscreen">Whether the window should be created in fullscreen mode.</param>
    /// <returns>Returns true if the window and OpenGL context were successfully created; otherwise, returns false.</returns>
    bool Renderer::CreateWindow(const std::string& name, int width, int height, bool fullscreen) {
        // Store the logical dimensions
        m_width = width;
        m_height = height;

        // Create the SDL window
        m_window = SDL_CreateWindow(name.c_str(), width, height, SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0));
        if (m_window == nullptr) {
            LOG_ERROR("SDL_CreateWindow Error: {}", SDL_GetError());
            SDL_Quit();
            return false;
        }

        // OpenGL
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetSwapInterval(1);

        m_context = SDL_GL_CreateContext(m_window);
        if (m_context == nullptr) {
            LOG_ERROR("SDL_CreateWindow Error: {}", SDL_GetError());
            SDL_Quit();
            return false;
        }
        gladLoadGL();

        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        return true;
    }

    /// <summary>
    /// Clears the rendering buffers and sets the background color.
    /// </summary>
    void Renderer::Clear() {
        // Note: currently set to black
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /// <summary>
    /// Displays the rendered content by swapping the OpenGL buffers of the window.
    /// </summary>
    void Renderer::Present() {
        SDL_GL_SwapWindow(m_window);
    }
}
