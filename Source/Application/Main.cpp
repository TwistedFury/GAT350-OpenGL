
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene
    float z = 0; // Use for screen
    // Vector things for OpenGL
    std::vector<neu::vec3> triangle_points{
        {  -2.0f/3, 0.0f, z },
        {     0.0f, 1.0f, z },
        { (2/3.0f), 0.0f, z }
    };
    std::vector<neu::vec3> triangle_colors{
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 }
    };

    std::vector<neu::vec3> strip_points{
        { -0.6f,  -0.6f,  z },
        { -0.7f,  -0.3f,  z },
        { -0.45f, -0.45f, z },
        { -0.5f,   0.0f,  z },
        { -0.1f,  -0.1f,  z },
        { -0.2f,   0.1f,  z }
    };
    std::vector<neu::vec3> strip_colors{
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 },
        { 1, 1, 1 },
        { 0, 0, 1 },
        { 0, 1, 0 }
    };

    std::vector<neu::vec3> quad_points{
        { -0.75f, -0.75f, z },
        { -0.75f,  0.75f, z },
        {  0.75f,  0.75f, z },
        {  0.75f, -0.75f, z }
    };
    std::vector<neu::vec3> quad_colors{
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 },
        { 1, 1, 1 }
    };
    /*
    // BUFFING TIME
    // Vertex
    GLuint vbo[2];
    glGenBuffers(2, vbo);
    
    // Points
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * triangle_points.size(), triangle_points.data(), GL_STATIC_DRAW);
    
    // Color
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * triangle_colors.size(), triangle_colors.data(), GL_STATIC_DRAW);

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

    // SHADE ME
    std::string vs_source;
    neu::file::ReadTextFile("Shaders/basic.vert", vs_source);
    const char* vs_cstr = vs_source.c_str();

    GLuint vs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_cstr, NULL);
    glCompileShader(vs);

    int success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(vs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

    // Fragment
    // SHADE ME
    std::string fs_source;
    neu::file::ReadTextFile("Shaders/basic.frag", fs_source);
    const char* fs_cstr = fs_source.c_str();

    GLuint fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_cstr, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(fs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

    // LINK ME
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glUseProgram(program);

    // UNIFORM (TIME)
    GLint uniform = glGetUniformLocation(program, "u_time");
    ASSERT(uniform != -1);
    */
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

        // ORDER:: SCALE -> ROTATE -> TRANSLATE
        
        // Define angle and scale
        float angle = neu::GetEngine().GetTime().GetTime() * 90.0f;
        float scale = neu::math::Remap(-1.0f, 1.0f, 0.3f, 1.2f, neu::math::sin(neu::GetEngine().GetTime().GetTime()));

        // draw
        neu::vec3 color{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();

        //glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, (GLsizei)triangle_points.size());

        
        // TRIANGLE_STRIP !!!!!!! THIS IS THE GOODBYE WORLD ASSIGNMENT
        glLoadIdentity();
        glPushMatrix();
        // Do things
        glScalef(scale, scale, scale);
        glRotatef(angle / 10, 0, 0, 1);
        glTranslatef(sin(neu::GetEngine().GetTime().GetTime()), cos(neu::GetEngine().GetTime().GetTime()), 0);

        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i < strip_points.size() && i < strip_colors.size(); i++)
        {
            glVertex3f(strip_points[i].x, strip_points[i].y, strip_points[i].z);
            glColor3f(strip_colors[i].r, strip_colors[i].g, strip_colors[i].b);
        }
        glEnd();

        // Remove layer with transformations (prepare for next shape)
        glPopMatrix();

        // Ensure that matrix is good, then push a layer out (copy) and prepare for transformations
        glLoadIdentity();
        glPushMatrix();
        // ...
        // TRANSFORM
        //
        /*
        // TRIANGLE
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < triangle_points.size() && i < triangle_colors.size(); i++)
        {
            glVertex3f(triangle_points[i].x, triangle_points[i].y, triangle_points[i].z);
            glColor3f(triangle_colors[i].r, triangle_colors[i].g, triangle_colors[i].b);
        }
        glPopMatrix();

        glEnd();

        // QUAD
        glBegin(GL_QUADS);
        for (int i = 0; i < quad_points.size() && i < quad_colors.size(); i++)
        {
            glVertex3f(quad_points[i].x, quad_points[i].y, quad_points[i].z);
            glColor3f(quad_points[i].r, quad_points[i].g, quad_points[i].b);
        }
        glEnd();
        */

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
