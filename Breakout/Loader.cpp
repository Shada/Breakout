#include "Loader.h"


Loader::Loader(void)
{
}


Loader::~Loader(void)
{
}


void Loader::LoadObject(char file[256],float scale,Model *_model,float invertX = 1,float invertY = 1,float invertZ = 1)
{
	Vertex pData;

	char buffer[256]="";
	bool last = false;

	std::fstream ObjFile;
	ObjFile.open(file,std::fstream::in | std::fstream::out | std::fstream::app);

	std::vector<Vec3> Position;
	std::vector<Vec3> Normal;
	std::vector<Vec2> TextureCoord;


	float x,y,z;
	float u,v;
	int FaceIndex=NULL;
	int Vertexsize=NULL;

	while(!ObjFile.eof())
	{
		pData.normal = Vec3(0,0,0);
		pData.pos = Vec3(0,0,0);
		pData.texCoord = Vec2(0,0);

		ObjFile >> buffer;

		if(0==strcmp(buffer,"v"))
		{
			last = false;
			ObjFile >>x>>y>>z;
			
			Position.push_back(Vec3((x*(scale/10)*invertX),(y*(scale/10)*invertY),(-z*(scale/10)*invertZ)));
			
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
			FaceIndex = atof(temp);
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
				FaceIndex = atof(temp);
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
					FaceIndex = atof(temp);
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
	//check the file signature and deduce its format
	_texture->setFif(FreeImage_GetFileType(file, 0));
	//if still unknown, try to guess the file format from the file extension
	if(*_texture->getFif() == FIF_UNKNOWN) 
		_texture->setFif(FreeImage_GetFIFFromFilename(file));
	//if still unkown, return failure
	if(*_texture->getFif() == FIF_UNKNOWN)
		return;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(*_texture->getFif()))
		_texture->setDib(FreeImage_Load(*_texture->getFif(), file));
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
}


