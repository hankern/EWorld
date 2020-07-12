#include "tileaddcallback.h"
#include <QDebug>
#include <QPainter>
#include <QImage>

#include <QPen>
#include <QFont>
#include <QVector>
#include <osg/Texture2D>
#include <osgEarthDrivers/engine_rex/TileNode>
#include <osgEarthDrivers/engine_rex/SurfaceNode>
#include <osgEarthDrivers/engine_rex/TerrainCuller>
#include <osgEarthDrivers/engine_rex/LayerDrawable>
using namespace osgEarth::Drivers::RexTerrainEngine;

class TileNodeSurfaceCallback:public osg::Callback
{


    // Callback interface
public:
    bool run(osg::Object *object, osg::Object *data)
    {
        //qDebug()<<"Surface Callback";
        //修改瓦片的纹理图片
        SurfaceNode* sfNode=dynamic_cast<SurfaceNode*>(object);
        TerrainCuller* tCuller=dynamic_cast<TerrainCuller*>(data);
        if(tCuller){
            int lnum=tCuller->_terrain.layers().size();
            for(int q=0;q<lnum;q++){
                LayerDrawable* ld=tCuller->_terrain.layers().at(q);
                if(ld->_tiles.size()<1) continue;
                DrawTileCommand* tileCMD = &ld->_tiles.back();
                if(!tileCMD->_colorSamplers){
                    ld->getOrCreateStateSet()->setDefine("OE_TERRAIN_RENDER_IMAGERY", osg::StateAttribute::ON);


                    osg::Image* image = new osg::Image;
                    GLenum pixelFormat = GL_RGBA;
                    image->allocateImage(256, 256, 1, pixelFormat, GL_UNSIGNED_BYTE);
                    memset(image->data(), 100, image->getImageSizeInBytes());
                    osg::Texture2D* tex = new osg::Texture2D( image );
                    tex->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
                    tex->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );
                    tex->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR );
                    tex->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR );
                    Samplers* samplers=new Samplers;
                    samplers->resize(SamplerBinding::COLOR_PARENT);
                    (*samplers)[SamplerBinding::COLOR]._texture = tex;
                    (*samplers)[SamplerBinding::COLOR]._matrix = osg::Matrixf();
                    (*samplers)[SamplerBinding::COLOR_PARENT]._texture = tex;
                    (*samplers)[SamplerBinding::COLOR_PARENT]._matrix = osg::Matrixf();
                    tileCMD->_colorSamplers=samplers;

                    continue;
                }
                Samplers samplers=*tileCMD->_colorSamplers;
                for(int t=0;t<samplers.size();t++){
                    Sampler sampler=samplers[t];
                    //osg::Texture tex=s._texture;
                    osg::Texture2D* tex=dynamic_cast<osg::Texture2D*>(sampler._texture.get());




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
                    QFont font("宋体",15,QFont::Bold,true);
                    font.setBold(true);//加粗
                    font.setPixelSize(50);//改变字体大小
                    painter.setPen(pen);
                    painter.setFont(font);
                    painter.drawText(image1.rect(),Qt::AlignCenter,"Hello");
                    int byteCount=image1.byteCount();
                    int byteLine=image1.bytesPerLine();
                    //image1.save("hello.jpg");
//                    unsigned char* data1 = new unsigned char[byteCount];
//                    for (int i = 0; i < byteCount; i++)
//                    {
//                        data1[i] = image1.bits()[i];
//                    }



                    //修改照片
                    osg::Image* image=tex->getImage();
                    unsigned char* data = image->data();
                    int len=sizeof(data);
                    int width=image->s();
                    int height=image->t();
                    if(width>10){
                        int len1=sizeof(data);
                        //int strLen = strlen(data);
                        for(int i=0;i<height;i++){
                            for(int j=0;j<width;j++){
//                                if(0==i%3){
//                                    *(image->data(i, j) + 0) = 10;
//                                    *(image->data(i, j) + 1) = 10;
//                                    *(image->data(i, j) + 2) = 10;
//                                    *(image->data(i, j) + 3) = 10;
//                                }

                                int r,g,b,a;

                                r=image1.bits()[i*byteLine+j*4+2];
                                if(r!=1){
                                    int jjj=1;
                                    jjj+=1;
                                }
                                *(image->data(j, height-i-1) + 0) = r;
                                g=image1.bits()[i*byteLine+j*4+1];
                                *(image->data(j,  height-i-1) + 1) = g;
                                b=image1.bits()[i*byteLine+j*4];
                                *(image->data(j,  height-i-1) + 2) = b;
                                a=image1.bits()[i*byteLine+j*4+3];
                                *(image->data(j,  height-i-1) + 3) = a;



//                                value=image1.bits()[i+j*4+2];
//                                *(image->data(i, j) + 0) = 0;
//                                value=image1.bits()[i+j*4+1];
//                                *(image->data(i, j) + 1) = 255;
//                                value=image1.bits()[i+j*4];
//                                *(image->data(i, j) + 2) = 0;
//                                value=image1.bits()[i+j*4+3];
//                                *(image->data(i, j) + 3) = 255;


                            }
                        }
                        //tex->setImage(image);
                    }
                }

            }
        }
        return true;
    }
};


TileAddCallback::TileAddCallback()
{
    _tileNodeSurfaceCallback=new TileNodeSurfaceCallback();
}

void TileAddCallback::onTileAdded(const osgEarth::TileKey &key, osg::Node *graph, osgEarth::TerrainCallbackContext &context)
{

    qDebug()<<"Tile added:"<<QString::fromStdString(key.str());
    osgEarth::Drivers::RexTerrainEngine::TileNode* tn=dynamic_cast<osgEarth::Drivers::RexTerrainEngine::TileNode*>(graph);
    SurfaceNode* sfNode=tn->getSurfaceNode();
    //sfNode->setCullCallback(new TileNodeSurfaceCallback());
    sfNode->setCullCallback(_tileNodeSurfaceCallback);



//    //在瓦片添加瓦片编号,在最后一个通道上添加
//    if(tn){

//        if(tn->renderModel()._passes.size()>0){
//            int i=0;
//            for(int k=0;k<tn->renderModel()._passes.size();k++){
//                //RenderingPass pass=tn->renderModel()._passes.at(tn->renderModel()._passes.size()-1);
//                RenderingPass pass=tn->renderModel()._passes.at(k);
//                Samplers ss=pass.samplers();
//                for(int t=0;t<ss.size();t++){
//                    Sampler s=ss[t];
//                    //osg::Texture tex=s._texture;
//                    osg::Texture2D* tex=dynamic_cast<osg::Texture2D*>(s._texture.get());
//                    osg::Image* image=tex->getImage();
//                    unsigned char* data = image->data();
//                    int len=sizeof(data);
//                    int width=image->s();
//                    int height=image->t();
//                    if(width>10){
//                        int len1=sizeof(data);
//                        //int strLen = strlen(data);
//                        for(int i=0;i<10;i++){
//                            for(int j=0;j<height;j++){
//                                *(image->data(i, j) + 0) = 0;
//                                *(image->data(i, j) + 1) = 0;
//                                *(image->data(i, j) + 2) = 0;
//                                *(image->data(i, j) + 3) = 0;
//                            }
//                        }
//                        tex->setImage(image);
//                    }
//                }
//            }

//        }
//    }
}
