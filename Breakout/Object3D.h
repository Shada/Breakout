#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "linearalgebra.h"

namespace Logic
{
	class Object3D
	{

	public:
		Object3D()
		{
		}
		virtual void update(float _dt) = 0;

		int getModelID()		{ return modelID;	};
		int getTextureID()		{ return textureID;	};
		Vec3 getPosition()		{ return position;	};
		Vec3 getRotation()		{ return rotation;	};
		Vec3 getScale()			{ return scale;		};
		Matrix getWorld()		{ return world;		};
		Matrix getWorldInv()	{ return worldInv;	};

		void setModelID(int _modelID) { modelID = _modelID; };
		void setTextureID(int _textureID) { textureID = _textureID; };
		virtual void setPosition(Vec3 _pos) { position = _pos; };
		void setRotation(Vec3 _rot) { rotation = _rot; };
		void setScale(Vec3 _scale)	{ scale = _scale; };
		void updateWorld()
		{
			world		= scalingMatrix(scale) * yawPitchRoll(rotation) * translationMatrix(position);
			MatrixInversion(worldInv, world);
			Matrix inv = world * worldInv;
		}

	protected:
		int		modelID, textureID;
		Vec3	position; 
		Vec3	rotation; 
		Vec3	scale;    
		Matrix	world, worldInv;
	};
}

#endif // ! _OBJECT_H_