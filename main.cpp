//#include <QCoreApplication>

#include <iostream>

#define GL_SILENCE_DEPRECATION

#include <osgViewer/Viewer>
#include <osgViewer/config/SingleWindow>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>

#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "OsgImGuiHandler.h"

//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);

//    return a.exec();
//}

#include "osgmanager.h"

class ImGuiInitOperation : public osg::Operation
{
public:
    ImGuiInitOperation()
        : osg::Operation("ImGuiInitOperation", false)
    {
    }

    void operator()(osg::Object* object) override
    {
        osg::GraphicsContext* context = dynamic_cast<osg::GraphicsContext*>(object);
        if (!context)
            return;

        if (!ImGui_ImplOpenGL3_Init("#version 410"))
        {
            std::cout << "ImGui_ImplOpenGL3_Init() failed\n";
        }
    }
};

class ImGuiDemo : public OsgImGuiHandler
{
protected:
    void drawUi() override
    {
        // ImGui code goes here...
        //ImGui::ShowDemoWindow();
        ImGui::Begin("Hello, world!");
        if (ImGui::Button("click button")) {
            OsgManager::getInstance()->show();
        }
        if (ImGui::Button("reconstruction")) {
            OsgManager::getInstance()->reconstruct("/Users/channyhuang/Documents/projects/PointCloudReconstruction/thermocolorlab");
        }
        if (ImGui::Button("reset button")) {
            OsgManager::getInstance()->reset();
        }

        ImGui::End();
    }
};

int main() {
    osgViewer::Viewer pviewer;

    const std::string version( "4.1" );
    osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
        traits->x = 20; traits->y = 30;
        traits->width = 800; traits->height = 450;
        traits->windowDecoration = true;
        traits->doubleBuffer = true;
        traits->glContextVersion = version;
        traits->readDISPLAY();
        traits->setUndefinedScreenDetailsToDefaultScreen();
    osg::ref_ptr< osg::GraphicsContext > gc = osg::GraphicsContext::createGraphicsContext( traits.get() );
    osg::Camera* cam = pviewer.getCamera();
        cam->setGraphicsContext( gc.get() );

    pviewer.apply(new osgViewer::SingleWindow(100, 100, 640, 480));
    pviewer.addEventHandler(new ImGuiDemo);
    pviewer.setRealizeOperation(new ImGuiInitOperation);

    OsgManager::getInstance()->setView(pviewer);

    pviewer.getCamera()->getGraphicsContext()->getState()->setUseModelViewAndProjectionUniforms(true);
    pviewer.getCamera()->getGraphicsContext()->getState()->setUseVertexAttributeAliasing(true);
//    osg::ref_ptr<osg::Geode> root = new osg::Geode;
//    root->addChild(createAxis());
//    pviewer.setSceneData(root);

    return pviewer.run();
}

