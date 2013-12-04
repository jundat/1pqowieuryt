#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

#include "Global.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("Fly And Fire");
    eglView->setFrameSize(DESIGN_WIDTH, DESIGN_HEIGHT);
    return CCApplication::sharedApplication()->run();
}
