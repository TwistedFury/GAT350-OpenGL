
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


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

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        // draw
        neu::vec3 color{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();

        float z = 0; // Use for screen

        // TRIANGLE_STRIP
        glBegin(GL_TRIANGLE_STRIP);

        glColor3f(1, 0, 0);
        glVertex3f(-0.6f, -0.6f, z);
        glColor3f(0, 1, 0);
        glVertex3f(-0.7f, -0.3f, z);
        glColor3f(0, 0, 1);
        glVertex3f(-0.45f, -0.45f, z);
        glColor3f(1, 1, 1);
        glVertex3f(-0.5f, 0, z);
        glColor3f(0, 0, 1);
        glVertex3f(-0.1f, -0.1f, z);
        glColor3f(1, 1, 1);
        glVertex3f(-0.2f, 0.1f, z);

        glEnd();

        // TRIANGLE
        glBegin(GL_TRIANGLES);

        glColor3f(1, 0, 0);
        glVertex3f(-0.5f, -0.5f, z);
        glColor3f(0, 1, 0);
        glVertex3f(0, 1, z);
        glColor3f(0, 0, 1);
        glVertex3f(0.5, 0.5, z);

        glEnd();


        // QUAD
        glBegin(GL_QUADS);

        glColor3f(1, 0, 0);
        glVertex3f(-0.75f, -0.75f, z);
        glColor3f(0, 1, 0);
        glVertex3f(-0.75f, 0.75f, z);
        glColor3f(0, 0, 1);
        glVertex3f(0.75f, 0.75f, z);
        glColor3f(1, 1, 1);
        glVertex3f(0.75f, -0.75f, z);

        glEnd();


        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
