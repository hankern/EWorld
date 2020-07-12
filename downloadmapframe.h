#ifndef DOWNLOADMAPFRAME_H
#define DOWNLOADMAPFRAME_H
#include <osgEarth/MapFrame>
#include <osgEarthDrivers/engine_rex/RexTerrainEngineNode>

class DownLoadMapFrame:public osgEarth::MapFrame
{
public:
    DownLoadMapFrame();

    bool populateHeightField(osg::ref_ptr<osg::HeightField>& hf,
                                  const TileKey&                  key,
                                  bool                            convertToHAE,
                                  ProgressCallback*               progress) const;
    osg::Node* createTile( const TileKey& key );
    unsigned int computeSampleSize(unsigned int levelOfDetail);
    void setRexEngine(osgEarth::Drivers::RexTerrainEngine::RexTerrainEngineNode* rex){
        _rex=rex;
    }

    osgEarth::Drivers::RexTerrainEngine::RexTerrainEngineNode* _rex;
};

#endif // DOWNLOADMAPFRAME_H
