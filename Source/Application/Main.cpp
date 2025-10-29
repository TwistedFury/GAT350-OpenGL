
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // Program
    auto program = neu::Resources().Get<neu::Program>("shaders/basic_lit.prog");
    program->Use();

    // UNIFORM
    neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("textures/spot_diffuse.png");
    program->SetUniform("u_texture", 0);

    float aspect = neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
    program->SetUniform("u_projection", projection);

    program->SetUniform("u_ambient_light", glm::vec3{ 0.5f });
    neu::Transform light{ { 2, 4, 3 } };
    
    // Model Test
    auto model3d = std::make_shared<neu::Model>();
    model3d->Load("models/spot.obj");

    neu::Transform model{ { 0.5f, 0, 0 }, { 0, 0, 1 }, { 0.5f, 0.5f, 0.5f } };
    
    // Camera
    neu::Transform camera{ { 0, 0, 5.0f } };

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

        float dt = neu::GetEngine().GetTime().GetDeltaTime();

        // Take User Input
        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        camera.rotation.y += 90 * dt;
        program->SetUniform("u_model", model);

        float speed = 10.0f;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.y -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.y += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_Q)) camera.position.z -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_E)) camera.position.z += speed * dt;
        // fill in the rest of the controls (WS and QE)

        glm::mat4 view = glm::lookAt(camera.position, camera.position + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });
        program->SetUniform("u_view", view);
        program->SetUniform("u_light.position", (glm::vec3)(view * glm::vec4(light.position, 1)));
        program->SetUniform("u_light.color", glm::vec3( 0, 0, 1 ));

        // draw
        neu::GetEngine().GetRenderer().Clear();

        model3d->Draw(GL_TRIANGLES);

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
