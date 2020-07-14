/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
* Copyright 2016 Pelican Mapping
* http://osgearth.org
*
* osgEarth is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include <QGuiApplication>
#include <QPainter>
#include <QImage>
#include <QPen>
#include <QFont>
#include <QDir>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/View>
#include <osg/StateSet>
#include <osgGA/StateSetManipulator>
#include <osgEarth/Notify>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>
#include <osgEarth/MapNode>
#include <osgEarth/OverlayDecorator>
#include <osgEarth/ThreadingUtils>
#include <osgEarth/Metrics>
#include <osgEarth/TileRasterizer>
#include <osgEarth/ResourceReleaser>
#include <osgEarth/VirtualProgram>
#include <osgEarth/ShaderLoader>
#include <osgEarthDrivers/engine_rex/RexTerrainEngineNode>
#include <osgEarthDrivers/engine_rex/RenderBindings>
#include <osgEarthDrivers/engine_rex/GeometryPool>
#include <osgEarthUtil/Controls>
#include <osgEarth/NodeUtils>
#include <osgEarthAnnotation/ImageOverlay>
#include <iostream>
#include "downloadmapframe.h"
#include <osg/ref_ptr>
#include <osg/observer_ptr>
#include <osg/Observer>
#include <osg/DisplaySettings>
#include <Windows.h>
#include "tileaddcallback.h"

#define LC "[viewer] "

using namespace osgEarth;
using namespace osgEarth::Util;

int
usage(const char* name)
{
    OE_NOTICE
        << "\nUsage: " << name << " file.earth" << std::endl
        << MapNodeHelper().usage() << std::endl;

    getchar();
    return 0;
}

class TTT:public osg::Referenced{
public:
    TTT(){
        _i=0;
    }
    TTT(int i){
        _i=i;
    }
    void operator = (int i)
    {
        _i=i;
    }
    void printI(){
        std::cout<<_i<<std::endl;
    }
    int _i;
};
typedef std::list< int > TSet;
TSet      tset;

int& ff(){
    int i=1;
    return i;
//    return 1;
}
//重定向osg日志输出
class RoadbedNotifyHandler :public osg::NotifyHandler
{
public:
    virtual void notify(osg::NotifySeverity severity, const char *message)
    {
        int i;
        if (severity <= osg::WARN){
            //i=1;
            //std::cout<<message;
            std::cout<<"llll1";
            //app_print_info(message_type::MT_INFO,message);
        }else
        {
            //i=2;
            //std::cout<<message;
            std::cout<<"llll2";
//            std::string logFileName= string_tools::CString_to_string(file_tools::get_app_path_with_xg());
//            logFileName += "3D_Error.log";

//            FILE* fp=nullptr;
//            fopen_s(&fp, logFileName.c_str(), "w");
//            if (!fp)
//            {
//                app_print_info(message_type::MT_ERROR, _T("3DLog file"));
//                return;
//            }
//            else
//            {
//                fprintf_s(fp, message);
//                fputchar('\n');
//                fclose(fp);
//            }
        }
    }
};
struct TRE
{
    TRE(){
        i=456;
    }
    operator int () const
    {
        return i;
    }
    int i;
};
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <QDebug>



class UseEventHandler :public osgGA::GUIEventHandler
{
public:
    UseEventHandler(osg::Group* root,osgEarth::Drivers::RexTerrainEngine::RexTerrainEngineNode* rex){
        _root=root;
        _rex=rex;
    }
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if (!viewer)return false;

        switch (ea.getEventType())
        {
        case(osgGA::GUIEventAdapter::KEYDOWN):
        {

//            if (ea.getKey()==osgGA::GUIEventAdapter::KEY_K)
//            {
//            }
            if (ea.getKey()==107 || ea.getKey()==229)
            {
                qDebug()<<"hello:"<<ea.getKey();
                return false;
                //获取高层层
                osg::ref_ptr<osg::HeightField> result;

                //从高层层获取指定范围的高程场



//                Profile profile=Profile::create(
//                                "epsg:4326",
//                                -180.0, -90.0, 180.0, 90.0,
//                                "",
//                                1, 1 );

                const Profile* profile=Profile::create(
                                "epsg:4326",
                                110.9, 32.9, 111.1, 33.1,
                                "",
                                1, 1 );




                osgEarth::TileKey key(0,0,0,profile);


                DownLoadMapFrame mapFrame;
                mapFrame.setMap(_rex->getMap());
                mapFrame.setRexEngine(_rex);
                osg::Node* node=mapFrame.createTile(key);
                osg::MatrixTransform* mm=findTopMostNodeOfType<osg::MatrixTransform>( node);
                osg::Matrix local2world;
                local2world=mm->getMatrix();
                osg::Vec3d up=osg::Vec3d(0,0,1)*local2world-local2world.getTrans();
                osg::Vec3d trans=up*1000;
                osg::MatrixTransform* mm1=new osg::MatrixTransform;
                mm1->setMatrix(osg::Matrix::translate(trans));
                mm1->addChild(node);
                _root->addChild(mm1);
            }
            break;
        }
        default:
            break;
        }
        return false;
    }
    osgEarth::Drivers::RexTerrainEngine::RexTerrainEngineNode* _rex;
    osg::Group* _root;
};



int main(int argc, char** argv)
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    if (engine.rootObjects().isEmpty())
//        return -1;

    QDir dir("./elevation.frag.glsl");
    dir.path();

    qDebug()<<dir.path();
    qDebug()<<dir.dirName();
    qDebug()<<dir.homePath();
    qDebug()<<dir.rootPath();
    qDebug()<<dir.currentPath();
    qDebug()<<dir.absolutePath();
    qDebug()<<dir.canonicalPath();
    bool bb=dir.exists();


    QImage imag;




    //HMODULE  hh=LoadLibrary(L"D:\\OSG_OAGEARTH\\bin\\osgPlugins-3.6.3\\osgdb_osgearth_xyzd.dll");
    //HMODULE  hh=LoadLibrary(L"D:\\OSG_OAGEARTH\\bin\\osgPlugins-3.6.3\\osgdb_osgearth_gdald.dll");
    //DWORD res111=GetLastError();

    int red=TRE();
    Stringify() << std::hex << std::setw(8) << std::setfill('0') << 123;
    std::string temp = Stringify() << std::hex << std::setw(8) << std::setfill('0') << 123;
    std::stringstream strStream;
    printf("size=%d\n", strStream.str().capacity());
    strStream << "abcdef";
    for (int i = 1; i < 10; ++i)
    {
        strStream.clear();
        strStream << i;
        std::string numStr;
        strStream >> numStr;
        std::cout<<numStr<<" ";
    }
    std::cout<<std::endl;
    printf("size=%d\n", strStream.str().capacity());
    std::cout<<"aaaa";
    osg::setNotifyLevel(osg::ALWAYS);
    //osg::setNotifyLevel(osg::DEBUG_FP);
    //osg::setNotifyHandler(new RoadbedNotifyHandler);
    OSG_INFO <<"hhhhh"<<std::endl;
    OSG_INFO <<"aaaaa"<<std::endl;

    osg::ArgumentParser arguments(&argc,argv);

    //int& iii=2;
    int& iii=ff();

    tset.push_back(1);
    tset.push_back(2);
    tset.push_back(3);
    tset.push_back(4);
    bool found = false;
    for (typename TSet::iterator i = tset.begin(); !found && i != tset.end(); )
    {
        typename TSet::iterator j = i++;
        tset.erase( j );
    }

    TTT* tt;
    osg::ref_ptr<TTT> st;
    {
        tt=new TTT;
        st=tt;
    }

    delete tt;
    tt=0;
    if(st.valid()){
        std::cout<<"valid";
    }else{
        std::cout<<"invalid";
    }



//    TTT* tt=new TTT;
//    *tt=1;
//    tt->printI();
//    {
//        osg::ref_ptr<TTT> st=tt;
//        *st=2;
//        tt->printI();
//    }
//    tt->printI();
//    {
//        //osg::ref_ptr<TTT> st=tt;
//        osg::ref_ptr<TTT> st=new TTT;
//        osg::observer_ptr<TTT> nst=st;
//        osg::observer_ptr<TTT> nst1=st;
//        {
//            osg::ref_ptr<TTT> st1=st;
//        }
//        osg::ref_ptr<TTT> st2=st;
//    }



    // help?
    if ( arguments.read("--help") )
        return usage(argv[0]);

    float vfov = -1.0f;
    arguments.read("--vfov", vfov);







    // create a viewer:

    osgViewer::Viewer viewer(arguments);
    //osgViewer::Viewer viewer("D:\OSG_OAGEARTH(x86)\tests\mb_tiles_test.earth");
    osg::DisplaySettings::instance()->setStereo(false);
    osg::DisplaySettings::instance()->setStereoMode(osg::DisplaySettings::CHECKERBOARD);

    // Tell the database pager to not modify the unref settings
    viewer.getDatabasePager()->setUnrefImageDataAfterApplyPolicy( true, false );

    // thread-safe initialization of the OSG wrapper manager. Calling this here
    // prevents the "unsupported wrapper" messages from OSG
    osgDB::Registry::instance()->getObjectWrapperManager()->findWrapper("osg::Image");

    // install our default manipulator (do this before calling load)
    viewer.setCameraManipulator( new EarthManipulator(arguments) );

    // disable the small-feature culling
    viewer.getCamera()->setSmallFeatureCullingPixelSize(-1.0f);

    // set a near/far ratio that is smaller than the default. This allows us to get
    // closer to the ground without near clipping. If you need more, use --logdepth
    viewer.getCamera()->setNearFarRatio(0.0001);

    if ( vfov > 0.0 )
    {
        double fov, ar, n, f;
        viewer.getCamera()->getProjectionMatrixAsPerspective(fov, ar, n, f);
        viewer.getCamera()->setProjectionMatrixAsPerspective(vfov, ar, n, f);
    }

//    //已经添加过，不用再次添加
//    //viewer.addEventHandler( new osgViewer::StatsHandler() );

//    // load an earth file, and support all or our example command-line options
//    // and earth file <external> tags
//    osgEarth::Util::Controls::Container* mainContainer = new VBox();
//    //osg::Group* gp=mainContainer

//    //osg::Node* node = MapNodeHelper().load(arguments, &viewer);
//    osg::Node* node = MapNodeHelper().load(arguments, &viewer,mainContainer);
//    //mainContainer->removeChild(mainContainer->getChild(0));
//    osg::Group* ctrlCanvas=dynamic_cast<osg::Group*>(mainContainer->getParent(0));
//    osg::Group* root=dynamic_cast<osg::Group*>(ctrlCanvas->getParent(0));
//    osgEarth::MapNode* mapNode = osgEarth::MapNode::get(node);







//    MapNode* mapNode = MapNode::load( arguments );
//    if ( !mapNode )
//        return usage( "Failed to load a map from the .earth file" );
//    // root scene graph:
//    osg::Group* root = new osg::Group();
//    root->addChild( mapNode );








    // load an earth file and parse demo arguments
    osg::Group* node = MapNodeHelper().load(arguments, &viewer);
    if ( !node )
        return usage( "Failed to load a map from the .earth file" );;
    osg::Group* root = new osg::Group();
    root->addChild( node );
    MapNode* mapNode = MapNode::get(node);




    //osg::Group* container=dynamic_cast<osg::Group*>(node->getChild(1));
    osg::Group* container=dynamic_cast<osg::Group*>(mapNode->getChild(1));
    //mapNode->removeChild(mapNode->getChild(0));//resourcereleaser
    //mapNode->removeChild(mapNode->getChild(1));//overlaydecorator
    osgEarth::OverlayDecorator* deco= dynamic_cast<osgEarth::OverlayDecorator*>(container->getChild(0));
    //osgEarth::TerrainEngineNode* ter=dynamic_cast<osgEarth::TerrainEngineNode*>(deco->getChild(0));
    osgEarth::Drivers::RexTerrainEngine::RexTerrainEngineNode* Rexter=dynamic_cast<osgEarth::Drivers::RexTerrainEngine::RexTerrainEngineNode*>(deco->getChild(0));



    // finalize setup and run.
    viewer.setSceneData( root );
    viewer.addEventHandler(new osgViewer::StatsHandler());
    viewer.addEventHandler(new osgViewer::WindowSizeHandler());
//    viewer.addEventHandler(new osgViewer::ThreadingHandler());
//    viewer.addEventHandler(new osgViewer::LODScaleHandler());
    viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
    viewer.addEventHandler(new UseEventHandler(root,Rexter));
//    viewer.addEventHandler(new osgViewer::RecordCameraPathHandler());
//    viewer.addEventHandler(new osgViewer::ScreenCaptureHandler());
    int i=root->getNumChildren();
//    i=mapNode->getNumChildren();

    //osgEarth::Drivers::RexTerrainEngine::RexTerrainEngineOptions op=    (osgEarth::Drivers::RexTerrainEngine::RexTerrainEngineOptions)Rexter->getTerrainOptions();
    //op.morphImagery()=false;
    //禁用颜色渐变
    //Rexter->getEngineContext()->getRenderBindings();
    //Rexter->getEngineContext()->getRenderBindings()[osgEarth::Drivers::RexTerrainEngine::SamplerBinding::COLOR_PARENT].unit()=-1;


    //监听瓦片添加
//    TileAddCallback* tileAddCB=new TileAddCallback;
//    Rexter->getTerrain()->addTerrainCallback(tileAddCB);

    //在地球上叠加一张图片
//    osgEarth::Annotation::ImageOverlay* imageOverlay = 0L;
//    osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile( "fj.jpg" );
//    if (image.valid())
//    {
//        imageOverlay = new osgEarth::Annotation::ImageOverlay(mapNode, image.get());
//        imageOverlay->setBounds( Bounds( 100.0, 30.0, 120.0, 40.0) );
//        osg::Group* annoGroup = new osg::Group();
//        MapNode::get(node)->addChild( annoGroup );
//        annoGroup->addChild( imageOverlay );
//    }

    //加入自定义的着色器
    osg::StateSet* surfaceStateSet = Rexter->getSurfaceStateSet();
    osgEarth::VirtualProgram* surfaceVP = osgEarth::VirtualProgram::getOrCreate(surfaceStateSet);
    osgEarth::ShaderPackage package;
    //package.load(surfaceVP, package.ENGINE_VERT_VIEW);
    //package.load(surfaceVP, "E:\\Projects\\QT5.12\\EWorld\\elevation.frag.glsl");//高程着色、瓦片边界、瓦片编号
    package.load(surfaceVP, "./elevation.frag.glsl");//高程着色、瓦片边界、瓦片编号
    //package.load(surfaceVP, "E:/Projects/QT5.12/build-EWorld-Desktop_Qt_5_12_0_MSVC2017_32bit-Debug/elevation.frag.glsl");//高程着色、瓦片边界、瓦片编号

    osg::ref_ptr<osg::Texture2D> numTexture = new osg::Texture2D;
    numTexture->setDataVariance(osg::Object::DYNAMIC);
    osg::Image* numImage = new osg::Image;
    GLenum pixelFormat = GL_RGBA;
    numImage->allocateImage(256, 256, 1, pixelFormat, GL_UNSIGNED_BYTE);
    memset(numImage->data(), 0, numImage->getImageSizeInBytes());
    //osg::ref_ptr<osg::Image> numImage = osgDB::readImageFile("E:\\Projects\\QT5.8\\Test_OsgEarth\\hello.png");
    //numImage = osgDB::readImageFile("E:\\Projects\\QT5.8\\Test_OsgEarth\\hello.jpg");
    if (!numImage)
    {
        std::cout<<"load texture failed !"<<std::endl;
        return -1;
    }
    //assign the texture from the image file
    numTexture->setImage(numImage);

    //生成照片
    QSize size(256,256); //指定图片大小;
    QImage image1(size,QImage::Format_ARGB32); //以ARGB32格式构造一个QImage,
    //第一个对应osg::image的b，第二个对应g，第三个对r，第四个对应a
    //qimage的bit顺为 b g r a
    image1.fill(qRgba(0,255,0,0));//填充图片背景,120/250为透明度
    QPainter painter(&image1); //为这个QImage构造一个QPainter
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    //设置画刷的组合e799bee5baa6e997aee7ad94e4b893e5b19e31333330323935模式CompositionMode_SourceOut这个模式为目标图像在上。
    //改变组合模式和上面的填充方式可以画出透明的图片。
    //改变画笔和字体
    QPen pen = painter.pen();
    pen.setColor(Qt::red);
    //QFont font = painter.font();
    QFont font("宋体",25,QFont::Bold,true);
    font.setBold(false);//加粗
    font.setItalic(false);
    font.setPixelSize(25);//改变字体大小
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(image1.rect(),Qt::AlignCenter,"0123456789/");
    int byteCount=image1.byteCount();
    int byteLine=image1.bytesPerLine();
    int height=256,width=256;
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            int r,g,b,a;

            r=image1.bits()[i*byteLine+j*4+2];
            if(r!=1){
                int jjj=1;
                jjj+=1;
            }
            *(numImage->data(j, height-i-1) + 0) = r;
            g=image1.bits()[i*byteLine+j*4+1];
            *(numImage->data(j,  height-i-1) + 1) = g;
            b=image1.bits()[i*byteLine+j*4];
            *(numImage->data(j,  height-i-1) + 2) = b;
            a=image1.bits()[i*byteLine+j*4+3];
            *(numImage->data(j,  height-i-1) + 3) = a;
        }
    }
    //image1.save("hello.jpg");
    //image1.save("hello.png");



    //set the stateset for decal texture
    osg::ref_ptr<osg::TexEnv> texenv = new osg::TexEnv;
    texenv->setMode(osg::TexEnv::DECAL);

    //与osgearth用到的纹理单元保持一致，不要随意分配
    int unit;
    Rexter->getResources()->reserveTextureImageUnit( unit, "Tile XYZ" );
    osg::Uniform* tileXYZUniform=new osg::Uniform("tile_xyz", unit);
    surfaceStateSet->addUniform(tileXYZUniform);
    //surfaceStateSet->setTextureAttribute(unit, numTexture.get());
    surfaceStateSet->setTextureAttributeAndModes(unit,numTexture.get(),osg::StateAttribute::ON);
    //surfaceStateSet->setTextureAttributeAndModes(0,numTexture.get(),osg::StateAttribute::ON);
    //surfaceStateSet->setTextureAttribute(0,texenv.get());


    osg::Node* nd;




//    osgEarth::TileRasterizer* ras=dynamic_cast<osgEarth::TileRasterizer*>(Rexter->getChild(5));
//    ras->setStateSet(new osg::StateSet);
//    ras->setClearColor(osg::Vec4(1,0,0,0.5));
//    int count=Rexter->getNumChildren();
//    for(int i=0;i<count;i++){
//        nd = Rexter->getChild(i);
//    }
//    osg::StateSet* ss;
//    osg::Group* terrain=dynamic_cast<osg::Group*>(Rexter->getChild(0));
//    ss=terrain->getStateSet();
//    terrain->setStateSet(new osg::StateSet);

//    Rexter->removeChild(Rexter->getChild(0));//terrain
//    Rexter->removeChild(Rexter->getChild(1));//geometrypool
//    Rexter->removeChild(Rexter->getChild(1));//loader
//    Rexter->removeChild(Rexter->getChild(1));//unloader
//    Rexter->removeChild(Rexter->getChild(0));//releaser
//    //ter->removeChild(ter->getChild(0));//raster

//    ss = Rexter->getStateSet();
//    //ter->setStateSet(NULL);
//    Rexter->setStateSet(new osg::StateSet);

//    ss=Rexter->getSurfaceStateSet();
//    osg::StateAttribute* sa=ss->getAttribute(osg::StateAttribute::PROGRAM);
//    ss->removeAttribute(osg::StateAttribute::PROGRAM);
//    sa=ss->getAttribute(osg::StateAttribute::PROGRAM);
//    ss->removeAttribute(osg::StateAttribute::CULLFACE);
//    ss->removeAttribute(osg::StateAttribute::DEPTH);
//    osg::StateSet::ModeList& ml=ss->getModeList();
//    ml.clear();
//    osg::StateSet::UniformList& ul=ss->getUniformList();
//    //ul.clear();
//    ul.erase(ul.begin());
//    ul.erase(ul.begin());
//    //osg::StateSet::UniformList::iterator iter=ul.begin();//定义一个迭代指针iter
//    //iter++;
//    //ul.erase(iter);
//    osg::StateSet::DefineList& dl = ss->getDefineList();
//    dl.erase(dl.begin());
//    dl.erase(dl.begin());
//    dl.erase(dl.begin());
//    dl.erase(dl.begin());
//    dl.erase(dl.begin());
//    dl.erase(dl.begin());
////    for(iter=ul.begin(); iter!=ul.end(); iter++)
////        ul.erase(ul.begin());
//    //ss->clear();



//    for(int i=0;i<mapNode->getNumChildren();i++){
//        nd = mapNode->getChild(i);
//    }
//    ss=mapNode->getStateSet();
//    mapNode->setStateSet(NULL);
//    //osg::StateSet* ss = ter->getOrCreateStateSet();
//    //ss=NULL;

//    //ter->removeChildren(0,count);
//    //mapNode->removeChild(container);
//    //deco->removeChild(ter);

//    osg::Group* layerNode=dynamic_cast<osg::Group*>(mapNode->getChild(2));
//    mapNode->removeChild(layerNode);
//    osgEarth::ResourceReleaser* resReleaser=dynamic_cast<osgEarth::ResourceReleaser*>(mapNode->getChild(0));

//    //resReleaser->setStateSet(new osg::StateSet);//原来没有stateset，没必要增加
//    //mapNode->removeChild(resReleaser);//报错


    viewer.setRunFrameScheme(osgViewer::ViewerBase::ON_DEMAND);
    return viewer.run();



//    if ( node )
//    {
//        viewer.setSceneData( node );
//        Metrics::run(viewer);
//    }
//    else
//    {
//        return usage(argv[0]);
//    }

    return app.exec();
}
