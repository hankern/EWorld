#ifndef TILEADDCALLBACK_H
#define TILEADDCALLBACK_H

#include <osgEarth/Terrain>
class TileNodeSurfaceCallback;
class TileAddCallback:public osgEarth::TerrainCallback
{
public:
    TileAddCallback();
public:
    TileNodeSurfaceCallback* _tileNodeSurfaceCallback;
    // TerrainCallback interface
public:
    void onTileAdded(const osgEarth::TileKey &key, osg::Node *graph, osgEarth::TerrainCallbackContext &context);
};

#endif // TILEADDCALLBACK_H
