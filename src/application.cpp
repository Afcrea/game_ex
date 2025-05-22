#include "application.h"
#include "scene_manager.h"
#include "input.h"

Application::~Application() {
    SceneManager::Shutdown();
    glfwDestroyWindow(mWindow);
    glfwTerminate();
    SPDLOG_INFO("system shutdown");
}

bool Application::Init() {
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
    glfwSetScrollCallback(mWindow, Input::scrollCallback);
    glfwSetCursorPosCallback(mWindow, Input::mousePositionCallback);

    // glad를 활용한 OpenGL 함수 로딩
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return false;
    }
    
    const char* glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    SPDLOG_INFO("OpenGL context version: {}", glVersion);

    Input::Initailize();

    SceneManager::SetScene(std::make_unique<LobbyScene>());
    //SceneManager::SetScene(new LobbySceneUPtr());

    return true;
}

void Application::run() {
    while (!glfwWindowShouldClose(mWindow)) {
        CalDeltaTime();

        glfwPollEvents();

        // OpenGL 렌더링 (배경 처리)
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f); // 파란빛 배경 (애니메이션 효과 대신 기본 색)
        glClear(GL_COLOR_BUFFER_BIT);
        
        Input::Update(mWindow);

        SceneManager::Update(mDeltaTime);
        SceneManager::Render();

        glfwSwapBuffers(mWindow);
    }
}

ApplicationUPtr Application::Create() {
    auto app = ApplicationUPtr(new Application());
    
    if(!app->Init()) {
        SPDLOG_ERROR("Application init error !");
        return nullptr;
    }
    return std::move(app);
}


void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::CalDeltaTime() {
    double currTime = glfwGetTime();
    static double prevTime = 0;
    mDeltaTime = currTime - prevTime;
    prevTime = currTime;
}