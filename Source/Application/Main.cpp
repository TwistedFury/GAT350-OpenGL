
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texcoord;
    };

    // initialize scene
    // Use for screen
    // Vector things for OpenGL
    //std::vector<neu::vec3> points{
    //    { -1, -1, z },
    //    { 1, -1, z },
    //    { 1, 1, z },
    //    { -1, 1, z }
    //};
    //std::vector<neu::vec3> colors{
    //    { 1, 0, 0 },
    //    { 0, 1, 0 },
    //    { 0, 0, 1 },
    //    { 1, 1, 0 }
    //};
    //// Texture Coords
    //std::vector<neu::vec2> texcoord{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 } };

    // Vertices
    std::vector<Vertex> vertices
    {
        { { -0.5f, -0.5f, 0 }, { 0, 0, 1 }, { 0, 0 } },
        { { 0.5f, -0.5f, 0}, { 0, 0, 1 }, { 0, 1 } },
        { { 0.5f, 0.5f, 0}, { 0, 0, 1 }, { 1, 1 } },
        { { -0.5f, 0.5f, 0}, { 0, 0, 1 }, { 1, 0 } }
    };

    // Indices -> switch to GLushort
    std::vector<GLushort> indices{ 0, 1, 2, 2, 3, 0 };

    // Vertex Buffer
    neu::res_t<neu::VertexBuffer> vb = std::make_shared<neu::VertexBuffer>();
    // Casting "bullshittery"
    vb->CreateVertexBuffer((GLsizei)(sizeof(Vertex) * vertices.size()), (GLsizei)vertices.size(), vertices.data());
    vb->CreateIndexBuffer(GL_UNSIGNED_SHORT, (GLsizei)indices.size(), indices.data());
    vb->SetAttribute(0, 3, sizeof(Vertex), offsetof(Vertex, position));
    vb->SetAttribute(1, 3, sizeof(Vertex), offsetof(Vertex, color));
    vb->SetAttribute(2, 2, sizeof(Vertex), offsetof(Vertex, texcoord));

    // Shaders
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

    // Model Class Test
    auto model3d = std::make_shared<neu::Model>();
    model3d->Load("models/sphere.obj");

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

        //vb->Draw(GL_TRIANGLES);
        model3d->Draw(GL_TRIANGLES);

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
