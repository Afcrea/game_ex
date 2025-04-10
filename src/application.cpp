#include "application.h"
#include "scene_manager.h"

Application::~Application() {
    glfwDestroyWindow(mWindow);
    glfwTerminate();
    SPDLOG_INFO("system shutdown");
}

void Application::run() {
    while (!glfwWindowShouldClose(mWindow)) {
        glfwPollEvents();

        // OpenGL 렌더링 (배경 처리)
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f); // 파란빛 배경 (애니메이션 효과 대신 기본 색)
        glClear(GL_COLOR_BUFFER_BIT);

        //SceneManager::Update(deltaTime); 추후에 델타 타임 구현하면 주석 해제
        SceneManager::Render();

        glfwSwapBuffers(mWindow);
    }
}

ApplicationUPtr Application::Create() {
    auto app = ApplicationUPtr(new Application());
    
    if(!app->Inititialization()) {
        SPDLOG_ERROR("Application init error !");
    }
    return std::move(app);
}

bool Application::Inititialization() {
    // glfw 라이브러리 초기화, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit()) {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw 윈도우 생성, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Create glfw window");
    mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!mWindow) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(mWindow);
    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);

    // glad를 활용한 OpenGL 함수 로딩
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return false;
    }
    
    const char* glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    SPDLOG_INFO("OpenGL context version: {}", glVersion);

    SceneManager::SetScene(std::make_unique<LobbyScene>());

    return true;
}

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}