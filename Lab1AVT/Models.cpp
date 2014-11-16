#include "Models.h"

coordinate::coordinate(float a,float b,float c)
{
        x=a;
        y=b;
        z=c;
}

face::face(int facen,int f1,int f2,int f3,int t1,int t2,int t3,int m){
        facenum=facen;
        faces[0]=f1;
        faces[1]=f2;
        faces[2]=f3;
        texcoord[0]=t1;
        texcoord[1]=t2;
        texcoord[2]=t3;
        mat=m;
        four=false;
}

face::face(int facen,int f1,int f2,int f3,int f4,int t1,int t2,int t3,int t4,int m){
        facenum=facen;
        faces[0]=f1;
        faces[1]=f2;
        faces[2]=f3;
        faces[3]=f4;
        texcoord[0]=t1;
        texcoord[1]=t2;
        texcoord[2]=t3;
        texcoord[3]=t4;
        mat=m;
        four=true;
}
       
material::material(const char* na,float al,float n,float ni2,float* d,float* a,float* s,int i,int t)
{
        name=na;
        alpha=al;
        ni=ni2;
        ns=n;
        dif[0]=d[0];
        dif[1]=d[1];
        dif[2]=d[2];
               
        amb[0]=a[0];
        amb[1]=a[1];
        amb[2]=a[2];
               
        spec[0]=s[0];
        spec[1]=s[1];
        spec[2]=s[2];
               
        illum=i;
        texture=t;
}

texcoord::texcoord(float a,float b)
{
        u=a;
        v=b;
}
 
void objLoader::load(const char* filename, float **vertices,
	float **normais, float **texCoords, unsigned int **faceIndex,
	int *verticeCount, int *faceCount)
{
        std::ifstream in(filename);     //open the model file
        if(!in.is_open())
        {
                std::cout << "Model not opened" << std::endl; //if it's not opened then error message
        }
        char buf[256];  //temp buffer
        int curmat=0;   //the current (default) material is 0, it's used, when we read the faces
        while(!in.eof())
        {
                in.getline(buf,256);    //while we are not in the end of the file, read everything as a string to the coord vector
                coord.push_back(new std::string(buf));
        }
        for(int i=0;i<coord.size();i++) //and then go through all line and decide what kind of line it is
        {
                if((*coord[i])[0]=='#') //if it's a comment
                        continue;       //we don't have to do anything with it
                else if((*coord[i])[0]=='v' && (*coord[i])[1]==' ')     //if a vertex
                {
                        float tmpx,tmpy,tmpz;
                        sscanf_s(coord[i]->c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz);       //read the 3 floats, which makes up the vertex
                        vertex.push_back(new coordinate(tmpx,tmpy,tmpz));       //and put it in the vertex vector
                }else if((*coord[i])[0]=='v' && (*coord[i])[1]=='n')    //if it's a normal vector
                {
                        float tmpx,tmpy,tmpz;
                        sscanf_s(coord[i]->c_str(),"vn %f %f %f",&tmpx,&tmpy,&tmpz);
                        normals.push_back(new coordinate(tmpx,tmpy,tmpz));      //basically do the same
                        isnormals=true;
                }else if((*coord[i])[0]=='f')   //if it's a face
                {
                        int a,b,c,d,e;
                        if(count(coord[i]->begin(),coord[i]->end(),' ')==4)     //if this is a quad
                        {
                                if(coord[i]->find("//")!=std::string::npos)     //if it's contain a normal vector, but not contain texture coorinate
                                {
                                        sscanf_s(coord[i]->c_str(),"f %d//%d %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b,&e,&b);      //read in this form
										faces.push_back(new face(b, a, c, d, 0, 0, 0, curmat));
										faces.push_back(new face(b, d, e, a, 0, 0, 0, curmat));
                                        //faces.push_back(new face(b,a,c,d,e,0,0,0,0,curmat));    //and put to the faces, we don't care about the texture coorinate in this case
                                                                                                                                                                                                                                                                //and if there is no material, it doesn't matter, what is curmat
                                }else if(coord[i]->find("/")!=std::string::npos)        //if we have texture coorinate and normal vectors
                                {
                                        int t[4];       //texture coorinates
                                        //read in this form, and put to the end of the vector
                                        sscanf_s(coord[i]->c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",&a,&t[0],&b,&c,&t[1],&b,&d,&t[2],&b,&e,&t[3],&b);
										faces.push_back(new face(b, a, c, d, t[0], t[1], t[2], curmat));
										faces.push_back(new face(b, d, e, a, t[2], t[3], t[0], curmat));
                                        //faces.push_back(new face(b,a,c,d,e,t[0],t[1],t[2],t[3],curmat));
                                }else{
                                        //else we don't have normal vectors nor texture coorinate
                                        sscanf_s(coord[i]->c_str(),"f %d %d %d %d",&a,&b,&c,&d);
										faces.push_back(new face(-1, a, b, c, 0, 0, 0, curmat));
										faces.push_back(new face(-1, d, e, a, 0, 0, 0, curmat));
                                        //faces.push_back(new face(-1,a,b,c,d,0,0,0,0,curmat));          
                                }
                        }else{  //if it's a triangle
                                                        //do the same, except we use one less vertex/texture coorinate/face number
                                        if(coord[i]->find("//")!=std::string::npos)
                                        {
                                                sscanf_s(coord[i]->c_str(),"f %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b);
                                                faces.push_back(new face(b,a,c,d,0,0,0,curmat));
                                        }else if(coord[i]->find("/")!=std::string::npos)
                                        {
                                                int t[3];
                                                sscanf_s(coord[i]->c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d",&a,&t[0],&b,&c,&t[1],&b,&d,&t[2],&b);
                                                faces.push_back(new face(b,a,c,d,t[0],t[1],t[2],curmat));
                                        }else{
                                                sscanf_s(coord[i]->c_str(),"f %d %d %d",&a,&b,&c);
                                                faces.push_back(new face(-1,a,b,c,0,0,0,curmat));                                      
                                        }
                        }
        }else if((*coord[i])[0]=='u' && (*coord[i])[1]=='s' && (*coord[i])[2]=='e')     //use material material_name
        {
                char tmp[200];
                sscanf_s(coord[i]->c_str(),"usemtl %s",tmp, _countof(tmp));      //read the name of the material to tmp
                for(int i=0;i<materials.size();i++)     //go through all of the materials
                {
                        if(strcmp(materials[i]->name.c_str(),tmp)==0)   //and compare the tmp with the name of the material
                        {
                                curmat=i;       //if it's equal then set the current material to that
                                break;
                        }
                }
        }else if((*coord[i])[0]=='m' && (*coord[i])[1]=='t' && (*coord[i])[2]=='l' && (*coord[i])[3]=='l')      //material library, a file, which contain
                                                                                                                                                                                                                                                                                                                                                                                                                        //all of the materials
        {
                char filen[200];

                sscanf_s(coord[i]->c_str(),"mtllib %s",filen, _countof(filen));    //read the filename
                std::ifstream mtlin(filen);     //open the file
                if(!mtlin.is_open())    //if not opened error message, clean all memory, return with -1
                {
                        std::cout << "cannot open the material file" << std::endl;
                        clean();
                }
                ismaterial=true;        //we use materials
                std::vector<std::string> tmp;//contain all of the line of the file
                char c[200];
                while(!mtlin.eof())
                {
                        mtlin.getline(c,200);   //read all lines to tmp
                        tmp.push_back(c);
                }
                char name[200]; //name of the material
                char filename[200];     //filename of the texture
                float amb[3],dif[3],spec[3],alpha,ns,ni;        //colors, shininess, and something else
                int illum;
                unsigned int texture;
                bool ismat=false;       //do we already have a material read in to these variables?
				strcpy_s(filename, 2, "\0");
                //strcpy(filename,"\0");  //set filename to nullbyte character
                for(int i=0;i<tmp.size();i++) //go through all lines of the mtllib file
                {
                        if(tmp[i][0]=='#')      //we don't care about comments
                                continue;
                        if(tmp[i][0]=='n' && tmp[i][1]=='e' && tmp[i][2]=='w')  //new material
                        {
                                if(ismat)       //if we have a material
                                {
                                        if(strcmp(filename,"\0")!=0)    //if we have a texture
                                        {
                                                materials.push_back(new material(name,alpha,ns,ni,dif,amb,spec,illum,texture)); //push back
												strcpy_s(filename, 2, "\0");
												//strcpy(filename,"\0");
                                        }else{
                                                        materials.push_back(new material(name,alpha,ns,ni,dif,amb,spec,illum,-1));              //push back, but use -1 to texture             
                                        }
                                }
                                ismat=false;    //we start from a fresh material
                                sscanf_s(tmp[i].c_str(),"newmtl %s",name, _countof(name));        //read in the name
                        }else if(tmp[i][0]=='N' && tmp[i][1]=='s')      //the shininess
                        {
                                sscanf_s(tmp[i].c_str(),"Ns %f",&ns);
                                ismat=true;
                        }else if(tmp[i][0]=='K' && tmp[i][1]=='a')      //the ambient
                        {
                                sscanf_s(tmp[i].c_str(),"Ka %f %f %f",&amb[0],&amb[1],&amb[2]);
                                ismat=true;
                        }else if(tmp[i][0]=='K' && tmp[i][1]=='d')      //the diffuse
                        {
                                sscanf_s(tmp[i].c_str(),"Kd %f %f %f",&dif[0],&dif[1],&dif[2]);
                                ismat=true;
                        }else if(tmp[i][0]=='K' && tmp[i][1]=='s')      //the specular
                        {
                                sscanf_s(tmp[i].c_str(),"Ks %f %f %f",&spec[0],&spec[1],&spec[2]);
                                ismat=true;
                        }else if(tmp[i][0]=='N' && tmp[i][1]=='i')      //the I don't know what is this
                        {
                                sscanf_s(tmp[i].c_str(),"Ni %f",&ni);
                                ismat=true;
                        }else if(tmp[i][0]=='d' && tmp[i][1]==' ')      //the alpha
                        {
                                sscanf_s(tmp[i].c_str(),"d %f",&alpha);
                                ismat=true;
                        }else if(tmp[i][0]=='i' && tmp[i][1]=='l')      //the illum (don't ask)
                        {
                                sscanf_s(tmp[i].c_str(),"illum %d",&illum);
                                ismat=true;
                        }else if(tmp[i][0]=='m' && tmp[i][1]=='a')      //and the texture
                        {
                                sscanf_s(tmp[i].c_str(),"map_Kd %s",filename, _countof(filename));
                                texture=loadTexture(filename);  //read the filename, and use the loadTexture function to load it, and get the id.
                                ismat=true;
                        }
                }
                if(ismat)       //there is no newmat after the last newmat, so we have to put the last material 'manually'
                {
                        if(strcmp(filename,"\0")!=0)
                        {
                                materials.push_back(new material(name,alpha,ns,ni,dif,amb,spec,illum,texture));
                        }else{
                                        materials.push_back(new material(name,alpha,ns,ni,dif,amb,spec,illum,-1));                             
                        }
                }
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 't')    //back to the obj file, texture coorinate
		{
			float u, v;
			sscanf_s(coord[i]->c_str(), "vt %f %f", &u, &v);     //read the uv coordinate
			texturecoordinate.push_back(new texcoord(u, 1 - v));       //I push back 1-v instead of normal v, because obj file use the upper left corner as 0,0 coorinate
			//but OpenGL use bottom left corner as 0,0, so I convert it
			istexture = true;
		}
}
        if(materials.size()==0) //if some reason the material file doesn't contain any material, we don't have material
                ismaterial=false;
        else    //else we have
                ismaterial=true;
        std::cout << vertex.size() << " " << normals.size() << " " << faces.size() << " " << materials.size() << std::endl;     //test purposes
        
		// Put the vertexes in the object structures
		int numVerts = vertex.size();
		*verticeCount = numVerts;
		*vertices = new float[4 * numVerts];
		for (int i = 0; i < numVerts; i++)
		{
			(*vertices)[i*4] = vertex[i]->x;
			(*vertices)[i*4 + 1] = vertex[i]->y;
			(*vertices)[i*4 + 2] = vertex[i]->z;
			(*vertices)[i*4 + 3] = 1.0f;
		}

		// Do the same for the normals
		int numNormals = normals.size();
		*normais = new float[3 * numNormals];
		for (int i = 0; i < numNormals; i++)
		{
			(*normais)[i*3] = normals[i]->x;
			(*normais)[i*3 + 1] = normals[i]->y;
			(*normais)[i*3 + 2] = normals[i]->z;
		}

		// For the tex coordinates
		int numTexCoords = texturecoordinate.size();
		*texCoords = new float[2 * numTexCoords];
		for (int i = 0; i < numTexCoords; i++)
		{
			(*texCoords)[i*2] = texturecoordinate[i]->u;
			(*texCoords)[i*2 + 1] = texturecoordinate[i]->v;
		}

		// And the face index
		int numFaces = faces.size();
		*faceCount = numFaces;
		*faceIndex = new unsigned int[3 * numFaces];
		for (int i = 0; i < numFaces; i++)
		{
			(*faceIndex)[i*3] = faces[i]->faces[0]-1;
			(*faceIndex)[i*3 + 1] = faces[i]->faces[1]-1;
			(*faceIndex)[i*3 + 2] = faces[i]->faces[2]-1;
		}

		/*
		for (int i = 0; i < numVerts; i ++)
		{
			std::cout << (*vertices)[i*4] << " " <<
				(*vertices)[i*4 + 1] << " " <<
				(*vertices)[i*4 + 2] << " " <<
				(*vertices)[i*4 + 3] << std::endl;
		}

		for (int i = 0; i < numNormals; i ++)
		{
			std::cout << (*normais)[i*3] << " " <<
				(*normais)[i*3 + 1] << " " <<
				(*normais)[i*3 + 2] << std::endl;
		}

		for (int i = 0; i < numTexCoords; i ++)
		{
			std::cout << (*texCoords)[i*2] << " " <<
				(*texCoords)[i*2 + 1] << std::endl;
		}

		for (int i = 0; i < numFaces; i ++)
		{
			std::cout << (*faceIndex)[i*3] << " " <<
				(*faceIndex)[i*3 + 1] << " " <<
				(*faceIndex)[i*3 + 2] << std::endl;
		}*/

        clean();
}
 
void objLoader::clean()
{
        //delete all the dynamically allocated memory
        for(int i=0;i<coord.size();i++)
                delete coord[i];
        for(int i=0;i<faces.size();i++)
                delete faces[i];
        for(int i=0;i<normals.size();i++)
                delete normals[i];
        for(int i=0;i<vertex.size();i++)
                delete vertex[i];
        for(int i=0;i<materials.size();i++)
                delete materials[i];
        for(int i=0;i<texturecoordinate.size();i++)
                delete texturecoordinate[i];
        //and all elements from the vector
        coord.clear();
        faces.clear();
        normals.clear();
        vertex.clear();
        materials.clear();
        texturecoordinate.clear();
}
 
objLoader::~objLoader()
{
        //delete textures
        for(std::vector<unsigned int>::const_iterator it=texture.begin();it!=texture.end();it++)
        {
                glDeleteTextures(1,&(*it));
        }
}
 
unsigned int objLoader::loadTexture(const char* filename)
{
        //nothing new in here
        unsigned int num;
        glGenTextures(1,&num);
        Image* image=loadBMP(filename);
        glBindTexture(GL_TEXTURE_2D,num);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image->width, image->height,0,GL_RGB,GL_UNSIGNED_BYTE,image->pixels);
        glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);       //maybe just this
        delete image;
        texture.push_back(num);
        return num;
}

objLoader::objLoader()
{
        //at default we set all booleans to false, so we don't use anything
        ismaterial=false;
        isnormals=false;
        istexture=false;
}

unsigned int objLoader::loadTextureSky(const char* filename)
{
	
        //nothing new in here
        unsigned int num;
        glGenTextures(1,&num);
        Image* image=loadBMP(filename);
        glBindTexture(GL_TEXTURE_2D,num);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);      //we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);      //we do it for vertically and horizontally (previous line)
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image->width, image->height,0,GL_RGB,GL_UNSIGNED_BYTE,image->pixels);
        glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);       
        //texture.push_back(num);
		delete image;

		//num=-1;
        return num;
}