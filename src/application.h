#include "common.h"

CLASS_PTR(Application)

class Application 
{
private:
    Application() {};
    bool Inititialization();
    GLFWwindow* mWindow;
    
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
public:
    ~Application();
    static ApplicationUPtr Create();
    void run();

    GLFWwindow* getWindow() { return mWindow; };
};