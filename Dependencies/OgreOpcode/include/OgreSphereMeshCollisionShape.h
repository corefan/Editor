///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionShape.h
///  @brief <TODO: insert file description here>
///
///  @author The OgreOpcode Team
///
///////////////////////////////////////////////////////////////////////////////
///
///  This file is part of OgreOpcode.
///
///  A lot of the code is based on the Nebula Opcode Collision module, see docs/Nebula_license.txt
///  
///  OgreOpcode is free software; you can redistribute it and/or
///  modify it under the terms of the GNU Lesser General Public
///  License as published by the Free Software Foundation; either
///  version 2.1 of the License, or (at your option) any later version.
///
///  OgreOpcode is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
///  Lesser General Public License for more details.
///
///  You should have received a copy of the GNU Lesser General Public
///  License along with OgreOpcode; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __OgreSphereMeshCollisionShape_h__
# define __OgreSphereMeshCollisionShape_h__

#include "OgreOpcodeExports.h"
# include <Ogre.h>

#include "IOgreCollisionShape.h"
#include "OgreCollisionTypes.h"
#include "OgreOpcodeDebugObject.h"
#include "Opcode/Opcode.h"

namespace OgreOpcode
{
	class CollisionPair;

	/// Describes shapes for collision system.
	/// Holds a triangle list describing a collision shape.
	/// One SphereMeshCollisionShape object may be shared between several
	/// CollisionObject%s. 2 SphereMeshCollisionShape objects may also
	/// be queried directly whether they intersect.
	///
	/// SphereMeshCollisionShape objects are also able to load themselves
	/// from a mesh file.
	class _OgreOpcode_Export SphereMeshCollisionShape : public ICollisionShape
	{
	public:
		/// Constructs a SphereMeshCollisionShape
		SphereMeshCollisionShape(const Ogre::String& name);
		virtual ~SphereMeshCollisionShape();

		/// load collide geometry from mesh, and build a collision tree
		virtual bool load(const Ogre::String& name, Ogre::SceneNode* scnNode, const float r, const int nRings = 16, const int nSegments = 16);

		/// Retrieve current vertex data from mesh and refit collision tree.
		/// This is an O(n) operation in the number of vertices in the mesh.
		virtual bool refit();

	protected:
		
		/// Reload the collision geometry from mesh, rebuild collision tree from scratch. 
		/// Potentially very slow. Only necessary if the mesh has drastically changed,
		/// like topology changing deformations, or a change in the number of tris.
		/// In most cases RefitToMesh() is sufficient, and much faster.
		/// Under usual circumstances there is no need to call this method.
		virtual bool rebuild();
		/// Refits the collision tree to the currently cached vertex data.
		/// This is an O(n) operation in the number of vertices in the mesh.
		/// This is an advanced method.  It assumes that the user is manually 
		/// updating both the SphereMeshCollisionShape's cached data and the actual mesh
		/// hardware buffers.  Mostly useful for implementing something like 
		/// deformable body physics.
		virtual bool _refitToCachedData();
		/// rebuild collision tree from scratch using currently cached vertex data
		/// This is potentially quite slow.  Only necessary if the mesh has drastically changed,
		/// like topology changing deformations, or a change in the number of tris.
		/// In most cases _RefitToCachedGeometry() is sufficient, and much faster.
		/// This is an advanced method.  It assumes that the user is manually 
		/// updating both the SphereMeshCollisionShape's cached data and the actual mesh
		/// hardware buffers.  Mostly useful for implementing something like
		/// deformable body physics.
		virtual bool _rebuildFromCachedData();

	private:
		Ogre::Entity* mEntity;

		void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16);
		/// Count up the total number of vertices and indices in the Ogre mesh
		void countIndicesAndVertices(Ogre::Entity * entity, size_t & index_count, size_t & vertex_count);
		/// Convert ogre Mesh to simple float and int arrays
		void convertMeshData(Ogre::Entity * entity, float * vertexData, size_t vertex_count, size_t * faceData=0, size_t index_count=0);

		/// prevent default construction
		SphereMeshCollisionShape();

	};

}; // namespace OgreOpcode

#endif // __OgreSphereMeshCollisionShape_h__
