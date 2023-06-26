#include "osgmanager.h"

OsgManager* OsgManager::instance = nullptr;
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "commonMath/triangle.h"
#include "commonMath/plane.h"
#include "commonGeometry/delaunay.h"

OsgManager::OsgManager()
{
   root = new osg::Geode;
   root->addChild(createAxis());
}

osg::ref_ptr<osg::Geometry> OsgManager::createAxis() {
    osg::ref_ptr<osg::Geometry> geometry (new osg::Geometry());

    osg::ref_ptr<osg::Vec3Array> vertices (new osg::Vec3Array());
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 10.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 10.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 10.0));
    geometry->setVertexArray (vertices.get());

    osg::ref_ptr<osg::Vec4Array> colors (new osg::Vec4Array());
    colors->push_back (osg::Vec4 (1.0f, 0.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (1.0f, 0.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 1.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 1.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 0.0f, 1.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 0.0f, 1.0f, 1.0f));
    geometry->setColorArray (colors.get(), osg::Array::BIND_PER_VERTEX);
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,6));
    return geometry.release();
}

osg::ref_ptr<osg::Geometry> OsgManager::createPoints()
{
    // create Geometry object to store all the vertices and lines primitive.
    osg::Geometry* polyGeom = new osg::Geometry();

    // note, first coord at top, second at bottom, reverse to that buggy OpenGL image..
    osg::Vec3 myCoords[] =
    {
        // TRIANGLES 6 vertices, v0..v5
        // note in anticlockwise order.
        osg::Vec3(-1.12056, -2.15188e-09, -0.840418),
        osg::Vec3(-0.95165, -2.15188e-09, -0.840418),
        osg::Vec3(-1.11644, 9.18133e-09, -0.716827),

        // note in anticlockwise order.
        osg::Vec3(-0.840418, 9.18133e-09, -0.778623),
        osg::Vec3(-0.622074, 9.18133e-09, -0.613835),
        osg::Vec3(-1.067, 9.18133e-09, -0.609715),

        // TRIANGLE STRIP 6 vertices, v6..v11
        // note defined top point first,
        // then anticlockwise for the next two points,
        // then alternating to bottom there after.
        osg::Vec3(-0.160668, -2.15188e-09, -0.531441),
        osg::Vec3(-0.160668, -2.15188e-09, -0.749785),
        osg::Vec3(0.0617955, 9.18133e-09, -0.531441),
        osg::Vec3(0.168908, -2.15188e-09, -0.753905),
        osg::Vec3(0.238942, -2.15188e-09, -0.531441),
        osg::Vec3(0.280139, -2.15188e-09, -0.823939),

        // TRIANGLE FAN 5 vertices, v12..v16
        // note defined in anticlockwise order.
        osg::Vec3(0.844538, 9.18133e-09, -0.712708),
        osg::Vec3(1.0258, 9.18133e-09, -0.799221),
        osg::Vec3(1.03816, -2.15188e-09, -0.692109),
        osg::Vec3(0.988727, 9.18133e-09, -0.568518),
        osg::Vec3(0.840418, -2.15188e-09, -0.506723),

    };

    int numCoords = sizeof(myCoords)/sizeof(osg::Vec3);

    osg::Vec3Array* vertices = new osg::Vec3Array(numCoords,myCoords);

    polyGeom->setVertexArray(vertices);

    polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,6));
    polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP,6,6));
    polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN,12,5));

    return polyGeom;
}

void setShader(osg::ref_ptr<osg::Geometry> geom) {
    osg::StateSet* stateset = geom->getOrCreateStateSet();
    osg::Program *program = new osg::Program;
    program->addShader(osgDB::readRefShaderFile(osg::Shader::VERTEX, "./../vertex.glsl"));
    program->addShader(osgDB::readRefShaderFile(osg::Shader::FRAGMENT, "./../fragment.glsl"));
    program->addBindAttribLocation("in_vertex", 1);
    program->addBindAttribLocation("in_normal", 2);
    stateset->setAttribute(program);

    osg::Vec3Array *vertex = (osg::Vec3Array *)(geom->getVertexArray());
    geom->setVertexAttribArray(1, vertex);
    osg::Vec3Array *normal = (osg::Vec3Array *)(geom->getNormalArray());
    if (normal == nullptr) {
        normal = new osg::Vec3Array;
        for (int i = 0; i < vertex->size(); ++i)
            normal->push_back(osg::Vec3(0.f, 0.f, 1.f));
    }
    geom->setVertexAttribArray(2, normal);

    //osg::Vec3f lightDir( 0., 0.5, 1. );
    osg::Vec3f lightDir( 0., 0, 5.f );
    lightDir.normalize();
    stateset->addUniform( new osg::Uniform( "lightDir", lightDir ) );
}

void OsgManager::show() {
    osg::ref_ptr<osg::Geometry> geom = createPoints();

    setShader(geom);

    root->addChild(geom);
}

osg::ref_ptr<osg::Geometry> loader(std::string &sObjFileName) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                sObjFileName.c_str(),
                                "./", /*triangulate*/ false);

    if (!warn.empty()) {
      std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
      std::cerr << "ERR: " << err << std::endl;
    }

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array;
    for (int i = 0; i < attrib.vertices.size() / 3; ++i) {
        vertex->push_back(osg::Vec3(attrib.vertices[3 * i + 0], attrib.vertices[3 * i + 1], attrib.vertices[3 * i + 2]));
    }
    geom->setVertexArray(vertex);

    return geom;
}

std::vector<std::string> splitString(std::string str, char c = '\t') {
    std::size_t pos = 0;
    std::size_t next = 0;
    std::vector<std::string> res;
    while (next != std::string::npos) {
        next = str.find_first_of(c, pos);
        if (next - pos > 0) {
            res.push_back(str.substr(pos, next - pos));
        }
        pos = next + 1;
    }
    return res;
}

void OsgManager::reconstruct(const std::string& sFilePath) {
    char buff[10] = {0};
    char line[128] = {0};
    for (int i = 0; i < 9; ++i) {
        std::sprintf(buff, "%03d", i);
        std::string sFileName = sFilePath + "/scan" + std::string(buff) + ".obj";
        osg::ref_ptr<osg::Geometry> geom = loader(sFileName);

        std::string sPoseName = sFilePath + "/scan" + std::string(buff) + ".pose";
        std::ifstream ifs(sPoseName.c_str());
        ifs.getline(line, 128);
        std::vector<std::string> res = splitString(line, ' ');
        osg::Vec3 position = osg::Vec3(std::atof(res[0].c_str()), std::atof(res[1].c_str()), std::atof(res[2].c_str()));
        ifs.getline(line, 128);
        res = splitString(line, ' ');
        osg::Vec3 rotation = osg::Vec3(std::atof(res[0].c_str()), std::atof(res[1].c_str()), std::atof(res[2].c_str()));

        osg::Matrix trans;
        trans.rotate(rotation.x(), osg::Vec3(1, 0, 0), rotation.y(), osg::Vec3(0, 1, 0), rotation.z(), osg::Vec3(0, 0, 1));
        trans.translate(position);

        osg::ref_ptr<osg::Geometry> pTargetGeom = new osg::Geometry;
        osg::Vec3Array *vTargetVertex = new osg::Vec3Array;
        osg::Vec3Array *vTargetNormal = new osg::Vec3Array;

        std::vector<Vector2> points;
        osg::Vec3Array *vertex = (osg::Vec3Array *)geom->getVertexArray();
        float angle = 20.f;

        Vector3 vNormal = Vector3(-std::sin(angle * Math::PI / 180.f), 0.f, std::cos(angle * Math::PI / 180.f));
        vNormal.normalize();
        Plane plane(vNormal, vNormal * 100.f);

        for (int j = 0; j < vertex->size(); ++j) {
            vTargetVertex->push_back(vertex->at(j));
            vTargetNormal->push_back(osg::Vec3(0, 0, 0));

            //ofs << "v " << vertex->at(j).x() << " " << vertex->at(j).y() << " " << vertex->at(j).z() << std::endl;

            Vector3 proj = plane.interate(Vector3(0), Vector3(vertex->at(j).x(), vertex->at(j).y(), vertex->at(j).z()));

            points.push_back(Vector2(proj.y, proj.z));
        }
        pTargetGeom->setVertexArray(vTargetVertex);

        Graph_Geometry::Delaunay T;
        Graph_Geometry::GraphGeometry V = T.triangulate(points);
        std::vector<unsigned int> indices;

        for (int j = 0; j < V.faces.size(); ++j) {
            HalfEdge h = V.outerComponent(V.faces[j]);
            int p0 = V.origin(h).id;
            int pi = V.origin(V.next(h)).id;
            int pj = V.origin(V.prev(h)).id;

            int i0 = T.mapIndex[p0];
            int i1 = T.mapIndex[pi];
            int i2 = T.mapIndex[pj];
            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i1);

            Triangle tri(Vector3(vertex->at(i0).x(), vertex->at(i0).y(), vertex->at(i0).z()),
                         Vector3(vertex->at(i1).x(), vertex->at(i1).y(), vertex->at(i1).z()),
                         Vector3(vertex->at(i2).x(), vertex->at(i2).y(), vertex->at(i2).z()));
            Vector3 triNormal = tri.getNormal();
]
            vTargetNormal->at(i0) += osg::Vec3(triNormal.x, triNormal.y, triNormal.z);
            vTargetNormal->at(i1) += osg::Vec3(triNormal.x, triNormal.y, triNormal.z);
            vTargetNormal->at(i2) += osg::Vec3(triNormal.x, triNormal.y, triNormal.z);

        }
        pTargetGeom->setNormalArray(vTargetNormal);
        pTargetGeom->addPrimitiveSet(new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, indices.size(), indices.data()));

        setShader(pTargetGeom);
        root->addChild(pTargetGeom);
    }
}
