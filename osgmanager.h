#ifndef OSGMANAGER_H
#define OSGMANAGER_H

#include <osgViewer/Viewer>
#include <osgViewer/config/SingleWindow>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>

class OsgManager
{
public:
    static OsgManager* getInstance() {
        if (instance == nullptr) {
            instance = new OsgManager();
        }
        return instance;
    }

    osg::ref_ptr<osg::Geometry> createPoints();
    osg::ref_ptr<osg::Geometry> createAxis();

    void setView(osgViewer::Viewer &pviewer) {
        m_pViewer = &pviewer;
        m_pViewer->setSceneData(root);
    }

    void reset() { root->removeChildren(0, root->getNumChildren()); }

    osg::ref_ptr<osg::Geode> getRoot() { return root; }

    void show();

    void reconstruct(const std::string& sFilePath);

private:
    OsgManager();
    ~OsgManager();

private:
    static OsgManager* instance;

    osg::ref_ptr<osgViewer::Viewer> m_pViewer;
    osg::ref_ptr<osg::Geode> root;
};

#endif // OSGMANAGER_H
