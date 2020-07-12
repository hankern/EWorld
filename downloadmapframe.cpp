#include "downloadmapframe.h"

DownLoadMapFrame::DownLoadMapFrame()
{

}

bool DownLoadMapFrame::populateHeightField(osg::ref_ptr<osg::HeightField> &hf, const TileKey &key, bool convertToHAE, ProgressCallback *progress) const
{
    osg::ref_ptr<const Map> map=_rex->getMap();

    if ( map )
    {
        ElevationInterpolation interp = map->getMapOptions().elevationInterpolation().get();

        return elevationLayers().populateHeightFieldAndNormalMap(
                    hf.get(),
                    0L,         // no normal map to populate
                    key,
                    convertToHAE ? map->getProfileNoVDatum() : 0L,
                    interp,
                    progress );
    }
    else
    {
        return false;
    }
}

osg::Vec3d getWorld( const GeoHeightField& geoHF, unsigned int c, unsigned int r)
{
    double x = geoHF.getExtent().xMin() + (double)c * geoHF.getXInterval();
    double y = geoHF.getExtent().yMin() + (double)r * geoHF.getYInterval();
    double h = geoHF.getHeightField()->getHeight(c,r);

    osg::Vec3d world;
    GeoPoint point(geoHF.getExtent().getSRS(), x, y, h );
    point.toWorld( world );
    return world;
}

osg::Node* renderHeightField(const GeoHeightField& geoHF)
{
    osg::MatrixTransform* mt = new osg::MatrixTransform;

    GeoPoint centroid;
    geoHF.getExtent().getCentroid(centroid);

    osg::Matrix world2local, local2world;
    centroid.createWorldToLocal( world2local );
    local2world.invert( world2local );

    mt->setMatrix( local2world );

    osg::Geometry* geometry = new osg::Geometry;
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( geometry );
    mt->addChild( geode );

    osg::Vec3Array* verts = new osg::Vec3Array;
    geometry->setVertexArray( verts );

    for (unsigned int c = 0; c < geoHF.getHeightField()->getNumColumns() - 1; c++)
    {
        for (unsigned int r = 0; r < geoHF.getHeightField()->getNumRows() - 1; r++)
        {
            // Add two triangles
            verts->push_back( getWorld( geoHF, c,     r    ) * world2local );
            verts->push_back( getWorld( geoHF, c + 1, r    ) * world2local );
            verts->push_back( getWorld( geoHF, c + 1, r + 1) * world2local );

            verts->push_back( getWorld( geoHF, c,     r    ) * world2local );
            verts->push_back( getWorld( geoHF, c + 1, r + 1) * world2local );
            verts->push_back( getWorld( geoHF, c,     r + 1) * world2local );
        }
    }
    geode->setCullingActive(false);
    mt->setCullingActive(false);

    geometry->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 0, verts->size()));

    osg::Vec4ubArray* colors = new osg::Vec4ubArray();
    colors->push_back(osg::Vec4ub(255,0,0,255));
    geometry->setColorArray(colors, osg::Array::BIND_OVERALL);
    mt->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    mt->getOrCreateStateSet()->setRenderBinDetails(99, "RenderBin");

    return mt;
}


osg::Node *DownLoadMapFrame::createTile(const TileKey &key)
{
    // Compute the sample size to use for the key's level of detail that will line up exactly with the tile size of the highest level of subdivision of the rex engine.
    unsigned int sampleSize = computeSampleSize( key.getLevelOfDetail() );


    TileKey sampleKey = key;

    GeoExtent extent(key.getExtent());
    GeoExtent result=extent.transform(osgEarth::SpatialReference::create("spherical-mercator"));
    unsigned iNumCols=result.width()/30;
    unsigned iNumRows=result.height()/30;
//    iNumCols>1000?1000:iNumCols<257?257:iNumCols;
    iNumCols=osg::clampBetween<unsigned>(iNumCols,257,500);
    iNumRows=osg::clampBetween<unsigned>(iNumRows,257,500);

    // ALWAYS use 257x257 b/c that is what rex always uses.
    osg::ref_ptr< osg::HeightField > out_hf = HeightFieldUtils::createReferenceHeightField(
                key.getExtent(), iNumCols, iNumRows, 0u, true );

    sampleKey = key;

    bool populated = false;
    while (!populated)
    {
        populated = populateHeightField(
                    out_hf,
                    sampleKey,
                    false, // convertToHAE
                    0 );

        if (!populated)
        {
            // Fallback on the parent
            sampleKey = sampleKey.createParentKey();
            if (!sampleKey.valid())
            {
                return 0;
            }
        }
    }

    // cannot happen (says coverity; see loop above), so commenting this out -gw
#if 0
    if (!populated)
    {
        // We have no heightfield so just create a reference heightfield.
        out_hf = HeightFieldUtils::createReferenceHeightField( key.getExtent(), 257, 257, 0u);
        sampleKey = key;
    }
#endif

    GeoHeightField geoHF( out_hf.get(), sampleKey.getExtent() );
    if (sampleKey != key)
    {
        geoHF = geoHF.createSubSample( key.getExtent(), sampleSize, sampleSize, osgEarth::INTERP_BILINEAR);
    }

    // We should now have a heightfield that matches up exactly with the requested key at the appropriate resolution.
    // Turn it into triangles.
    return renderHeightField( geoHF );
}

unsigned int DownLoadMapFrame::computeSampleSize(unsigned int levelOfDetail)
{
    unsigned maxLevel = 19u; // beyond LOD 19 or 20, morphing starts to lose precision.
    unsigned int meshSize = 17;

    unsigned int sampleSize = meshSize;
    int level = maxLevel; // make sure it's signed for the loop below to work

    while( level >= 0 && levelOfDetail != level)
    {
        sampleSize = sampleSize * 2 - 1;
        level--;
    }

    return sampleSize;
}


