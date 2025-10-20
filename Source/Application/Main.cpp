
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene
    float z = 0; // Use for screen
    // Vector things for OpenGL
    std::vector<neu::vec3> points{
        { -1, -1, z },
        { 1, -1, z },
        { 1, 1, z },
        { -1, 1, z }
    };
    std::vector<neu::vec3> colors{
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 },
        { 1, 1, 0 }
    };

    // Texture Coords
    std::vector<neu::vec2> texcoord{ { -1, -1 }, { 1, -1 }, { 1, 1 }, { -1, 1 } };
    
    // BUFFING TIME
    // Vertex
    GLuint vbo[3];
    glGenBuffers(3, vbo);
    
    // Points
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
    // Color
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);

    // TexCoord
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * texcoord.size(), texcoord.data(), GL_STATIC_DRAW);
    
    // Vertex Array
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Position
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // TexCoord
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    auto vs = neu::Resources().Get<neu::Shader>("shaders/basic.vert", GL_VERTEX_SHADER);
    auto fs = neu::Resources().Get<neu::Shader>("shaders/basic.frag", GL_FRAGMENT_SHADER);

    // LINK ME
    auto program = std::make_shared<neu::Program>();
    program->AttachShader(vs);
    program->AttachShader(fs);
    program->Link();
    program->Use();

    // Texture
    // UNIFORM (TEXCOORD)
    neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("textures/beast.png");
    program->SetUniform("u_texture", 0);

    //// UNIFORM (TIME)
    //program->SetUniform("u_time", 0);
    
    // GLM THINGS
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

    program->SetUniform("u_model", model);
    
    SDL_Event e;
    bool quit = false;

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();

        // Take User Input
        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        //glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());

        // draw
        neu::GetEngine().GetRenderer().Clear();

        glBindVertexArray(vao);
        glDrawArrays(GL_QUADS, 0, (GLsizei)points.size());

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
