/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

    This file is part of COLLADASaxFrameworkLoader.

    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/

#include "COLLADASaxFWLStableHeaders.h"
#include "COLLADASaxFWLMeshLoader14.h"
#include "COLLADASaxFWLMeshLoader15.h"
#include "COLLADASaxFWLGeometryLoader.h"

#include "COLLADAFWIWriter.h"


namespace COLLADASaxFWL
{

	GeometryLoader::GeometryLoader( IFilePartLoader* callingFilePartLoader)
		: FilePartLoader(callingFilePartLoader)
        , mMeshLoader(0)
	{
	}

    //------------------------------
	GeometryLoader::~GeometryLoader()
	{
	}

    //------------------------------
    COLLADAFW::ExtraData* GeometryLoader::getExtraData ()
    {
        return mMeshLoader->getMesh();
    }

	//------------------------------
	bool GeometryLoader::begin__mesh()
	{
        mMeshLoader = new MeshLoader(this, mGeometryId, mGeometryName);
        setPartLoader(mMeshLoader);
        switch (this->getParserImpl()->getCOLLADAVersion())
        {
        case COLLADA_14:
            {
            MeshLoader14* meshloader14 = new MeshLoader14( mMeshLoader );
            mMeshLoader->setParserImpl(meshloader14);
            setParser(meshloader14);
            break;
            }
        case COLLADA_15:
            {
            MeshLoader15* meshloader15 = new MeshLoader15( mMeshLoader );
            mMeshLoader->setParserImpl(meshloader15);
            setParser(meshloader15);
            break;
            }
        }
		return true;
	}

    //------------------------------
	bool GeometryLoader::end__geometry()
	{
		bool success = true;
		COLLADAFW::Mesh * mesh = mMeshLoader ? mMeshLoader->getMesh() : 0;
		if ( ((getObjectFlags() & Loader::GEOMETRY_FLAG) != 0) && mesh )
		{
			success = writer()->writeGeometry(mesh);
		}

		finish();
		moveUpInSidTree();
		return success;
	}

} // namespace COLLADAFW