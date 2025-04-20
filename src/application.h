#include "common.h"

CLASS_PTR(Application)

class Application 
{
private:
    Application() {};
    bool Init();

    GLFWwindow* mWindow;
    
    double mDeltaTime = 0;
    void CalDeltaTime();
    
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
public:
    ~Application();
    static ApplicationUPtr Create();
    void run();

    GLFWwindow* getWindow() { return mWindow; };
};