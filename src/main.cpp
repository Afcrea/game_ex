#include "application.h"

int main() {
    // 시작을 알리는 로그
    SPDLOG_INFO("Start");

    auto app = Application::Create();
    
    app->run();
}