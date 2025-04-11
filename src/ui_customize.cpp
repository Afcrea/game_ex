#include "ui_customize.h"

Ui_customize::~Ui_customize() {
    // 정리
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Ui_customize::Initialization() {
    // ImGui 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // 스타일을 기본 다크로 시작

    // ImGui 백엔드 초기화
    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // 스타일 커스터마이징
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.FrameRounding = 2.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(1, 1, 1, 1); // 검은 배경
    style.Colors[ImGuiCol_Button] = ImVec4(0, 0, 1, 1);   // 흰색 버튼
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0, 1, 0.1f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.6f, 0.6f, 0.6f, 1);
}

bool Ui_customize::MakeFrame() {
    // ImGui UI 오버레이
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 전체 화면에 맞춘 투명 창
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Button", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoSavedSettings);

    return true;
}

bool Ui_customize::EndFrame() {
    ImGui::End();

    // 렌더링
    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return true;
}

bool Ui_customize::MakeButton(const char* label, float x, float y, float s, glm::vec2 pos, std::function<void()> onClick) {
    // 버튼 크기와 위치 조정
    ImVec2 button_size(x, y);
    float spacing = s;
    float total_height = button_size.y * 3 + spacing * 2;

    ImVec2 center = ImVec2(pos.x, pos.y);
    ImGui::SetCursorPos(center);

    if (ImGui::Button(label, button_size)) {
        SPDLOG_INFO("{} 클릭됨\n", label);
        if(onClick != nullptr)
            onClick();
    }

    return true;
}