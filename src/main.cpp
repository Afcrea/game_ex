#include "application.h"

int main() {
    // 시작을 알리는 로그
    SPDLOG_INFO("Start");

    auto app = Application::Create();
    if(!app) {
        SPDLOG_ERROR("application create error");
        return 0;
    }
    
    app->run();

    app.reset();
}