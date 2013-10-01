#include "Loader.h"
#include <string.h>
namespace Resources
{

	Loader::Loader(void)
	{
	}


	Loader::~Loader(void)
	{
	}

// TODO: Throw away all lines starting with character: #
	void Loader::LoadObject(char file[256],float scale,Model *_model,float invertX,float invertY,float invertZ)
	{
		Vertex pData;

		char buffer[256]="";
		bool last = false;

        std::string filename = "";
#ifndef _WIN32
		filename = "/home/torrebjorne/Documents/GitHub/Breakout/Breakout/";
#endif // !_WIN32
		filename += file;

		std::fstream ObjFile;
		ObjFile.open(filename,std::fstream::in | std::fstream::out | std::fstream::app);

		std::vector<Vec3> Position;
		std::vector<Vec3> Normal;
		std::vector<Vec2> TextureCoord;


		float x,y,z;
		float u,v;
		int FaceIndex=NULL;
		int Vertexsize=NULL;

		while(ObjFile >> buffer)
		{
			pData.normal = Vec3(0,0,0);
			pData.pos = Vec3(0,0,0);
			pData.texCoord = Vec2(0,0);

			if(0==strcmp(buffer,"v"))
			{
				last = false;
				ObjFile >> x >> y >> z;
			
				Position.push_back(Vec3(x * invertX, y * invertY, z * invertZ));
			}
			else if(0==strcmp(buffer,"vt"))
			{
				last = false;
				ObjFile >>u>>v;

				TextureCoord.push_back(Vec2(u,1-v));
			}
			else if(0==strcmp(buffer,"vn"))
			{
				last = false;
				ObjFile >>x>>y>>z;

				Normal.push_back(Vec3(x,y,z));
			}

			else if(0==strcmp(buffer,"f"))
			{
				last = true;

				for(int i = 0; i < 3;i ++ )
				{
					ObjFile >>FaceIndex;
					if(FaceIndex < 0)
						FaceIndex*=-1;
					pData.pos = Position.at(FaceIndex-1);

					if('/'==ObjFile.peek())         /////  '/'  Ignorieren
					{
						ObjFile.ignore();
						if('/'!=ObjFile.peek())
						{

							ObjFile >>FaceIndex;
							if(FaceIndex < 0)
								FaceIndex*=-1;
							pData.texCoord = TextureCoord.at(FaceIndex-1);
						}
					}
					if('/'==ObjFile.peek())
					{
						ObjFile.ignore();

						if('/'!=ObjFile.peek())
						{
							ObjFile >>FaceIndex;
							if(FaceIndex < 0)
								FaceIndex*=-1;
							pData.normal = Normal.at(FaceIndex-1);
						}

					}
					//Data->push_back(pData);
					_model->addData(pData);
				}
			}
			else if(0==strcmp(buffer,"s"))
			{
				last = false;
			}
			else if(0==strcmp(buffer,"g"))
			{
				last = false;
			}
			else if(0==strcmp(buffer,"#"))
			{
				last = false;
			}
			else if(buffer[0] == '#')
			{
				last = false;
			}
			else if(0==strcmp(buffer,"usemtl"))
			{
				last = false;
			}
			else if(last == true && !ObjFile.eof())
			{

				_model->lastFace();
				//Data->push_back(Data->at(Data->size()-3));
				//objekt->addData(Data->at(Data->size()-2));
				//Data->push_back(Data->at(Data->size()-2));

				char temp[256] = "";
				int i = 0;
				int j = 0;

				while(buffer[i] != '/' && buffer[i] != 0)
				{
					temp[j] = buffer[i];
					i++;
					j++;
				}
				i++;
				j = 0;
				FaceIndex = atoi(temp);
				if(FaceIndex < 0)
					FaceIndex*=-1;
				pData.pos = Position.at(FaceIndex-1);

				if(buffer[i-1] != 0)
				{

					for(int l = 0;l < 256;l++)
					{
						temp[l] = NULL;
					}


					while(buffer[i] != '/' && buffer[i] != 0)
					{
						temp[j] = buffer[i];
						i++;
						j++;
					}
					i++;
					j = 0;
					FaceIndex = atoi(temp);
					if(FaceIndex < 0)
						FaceIndex*=-1;
					pData.texCoord = TextureCoord.at(FaceIndex-1);

					if(buffer[i-1] != 0)
					{

						for(int l = 0;l < 256;l++)
						{
							temp[l] = NULL;
						}

						while(buffer[i] != 0)
						{
							temp[j] = buffer[i];
							i++;
							j++;
						}
						FaceIndex = atoi(temp);
						if(FaceIndex < 0)
							FaceIndex*=-1;
						pData.normal = Normal.at(FaceIndex-1);

					}

				}
				//Data->push_back(pData);
				_model->addData(pData);
			}
		}


		ObjFile.close();

	}

	void Loader::loadTexture(char file[256],Texture *_texture)
	{
        std::string filename = "";
#ifndef _WIN32
        filename = "/home/torrebjorne/Documents/GitHub/Breakout/Breakout/";
#endif // ! _WIN32

		filename += file;
		//check the file signature and deduce its format
		_texture->setFif(FreeImage_GetFileType(filename.c_str(), 0));
		//if still unknown, try to guess the file format from the file extension
		if(*_texture->getFif() == FIF_UNKNOWN)
			_texture->setFif(FreeImage_GetFIFFromFilename(filename.c_str()));
		//if still unkown, return failure
		if(*_texture->getFif() == FIF_UNKNOWN)
			return;

		//check that the plugin has reading c_WIN32bilities and load the file
		if(FreeImage_FIFSupportsReading(*_texture->getFif()))
			_texture->setDib(FreeImage_Load(*_texture->getFif(), filename.c_str()));
		//if the image failed to load, return failure
		if(!_texture->getDib())
			return;

		//retrieve the image data
		_texture->setBits(FreeImage_GetBits(_texture->getDib()));
		//get the image width and height
		_texture->setWidth(FreeImage_GetWidth(_texture->getDib()));
		_texture->setHeight(FreeImage_GetHeight(_texture->getDib()));
		//if this somehow one of these failed (they shouldn't), return failure
		if((_texture->getBits() == 0) || (_texture->getWidth() == 0) || (_texture->getHeight() == 0))
			return;

		_texture->setFilePath(file);
	}


}
