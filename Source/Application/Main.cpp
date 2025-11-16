
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    auto scene = std::make_unique<neu::Scene>();
    scene->Load("scenes/scene01.json");
    scene->Start();

    auto editor = std::make_unique<neu::Editor>();

    SDL_Event e;
    bool quit = false;

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            ImGui_ImplSDL3_ProcessEvent(&e);
        }

        // update
        neu::GetEngine().Update();
        float dt = neu::GetEngine().GetTime().GetDeltaTime();

        // UPDATE SCENE
        scene->Update(dt);

        // Take User Input
        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        // draw
        neu::GetEngine().GetRenderer().Clear();

        // DRAW SCENE
        scene->Draw(neu::GetEngine().GetRenderer());

        // draw ImGui
        editor->Begin();
        editor->UpdateGui(*scene);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
