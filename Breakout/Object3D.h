#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "linearalgebra.h"

namespace Logic
{
	class Object3D
	{

	public:
		Object3D(){}
		virtual void update(double _dt) = 0;

		int getModelID()	{ return modelID;	};
		int getTextureID()	{ return textureID;	};
		Vec3 getPosition()	{ return position;	};
		Vec3 getRotation()	{ return rotation;	};
		Vec3 getScale()		{ return scale;		};
		Matrix getWorld()	{ return world;		};

		void setModelID(int _modelID) { modelID = _modelID; };
		void setTextureID(int _textureID) { textureID = _textureID; };
		void setPosition(Vec3 _pos) { position = _pos; };
		void setRotation(Vec3 _rot) { rotation = _rot; };
		void setScale(Vec3 _scale)	{ scale = _scale; };

	protected:
		int		modelID, textureID;
		Vec3	position; 
		Vec3	rotation; 
		Vec3	scale;    
		Matrix	world;
	};
}

#endif // ! _OBJECT_H_