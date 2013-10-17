#ifndef _LINALGEBRA_H_
#define _LINALGEBRA_H_

#include <math.h>
#include <vector>

#pragma region vector2
	struct Vec2
	{
		float x, y;
		Vec2()					: x(0), y(0) {}
		Vec2(float x, float y)	: x(x), y(y) {}
		float &operator[](int a)
		{
			switch(a)
			{
				case 0 :	return x;
				case 1 :	return y;
				default :	return y;
			}
		}
		Vec2 operator+(Vec2 v)
		{
			return Vec2(x + v.x, y + v.y);
		}
		Vec2 operator-(Vec2 v)
		{
			return Vec2(x - v.x, y - v.y);
		}
		Vec2 operator*(float a)
		{
			return Vec2(a * x, a * y);
		}
		Vec2 operator*(Vec2 v)
		{
			return Vec2(x * v.x, y * v.y);
		}
		void operator*=(float a)
		{
			x *= a;
			y *= a;
		}
		void operator*=(Vec2 v)
		{
			x *= v.x;
			y *= v.y;
		}
		Vec2 operator-()
		{
			return Vec2(-x, -y);
		}
		bool operator==(Vec2 v)
		{
			return x == v.x && y == v.y;
		}
		bool operator!=(Vec2 v)
		{
			return x != v.x || y != v.y;
		}
		float dot(Vec2 v)
		{
			return (x * v.x + y * v.y);
		}
		void normalize()
		{
			float len = 1 / length();
			x *= len;
			y *= len;
		}
		float length()
		{
			return sqrt(x*x + y*y);
		}
	};
#pragma endregion

#pragma region vector3
	struct Vec3
	{
		float x, y, z;
		Vec3()							: x(0), y(0), z(0) {}
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
		float& operator[](int a)
        {
            switch(a)
            {
                case 0:		return x;
                case 1:		return y;
                case 2:		return z;
				default:	return z;
            }
        }
		Vec3 operator+(Vec3 v)
		{
			return Vec3(x + v.x, y + v.y, z + v.z);
		}
		Vec3 operator-(Vec3 v)
		{
			return Vec3(x - v.x, y - v.y, z - v.z);
		}
		Vec3 operator*(float a)
		{
			return Vec3(a * x, a * y, a * z);
		}
        Vec3 operator*(Vec3 v)
        {
            return Vec3(x * v.x, y * v.y, z * v.z);
        }
		void operator+=(Vec3 v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
		}
		void operator-=(Vec3 v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}
		void operator*=(float a)
		{
			x *= a;
			y *= a;
			z *= a;
		}
		void operator*=(Vec3 v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
		}
		Vec3 operator-()
		{
			return Vec3(-x, -y, -z);
		}
		bool operator==(Vec3 v)
		{
			return x == v.x && y == v.y && z == v.z;
		}
		bool operator!=(Vec3 v)
		{
			return x != v.x || y != v.y || z != v.z;
		}
		void cross(Vec3 v, Vec3 u)
		{
			x = v.y * u.z - u.y * v.z;
			y = u.x * v.z - v.x * u.z;
			z = v.x * u.y - u.x * v.y;
		}
		float dot(Vec3 v)
		{
			return (x * v.x + y * v.y + z * v.z);
		}
		void normalize()
		{
			float len = 1 / length();
			x *= len;
			y *= len;
			z *= len;
		}
		float length()
		{
			return sqrt(x * x + y * y + z * z);
		}
	};
#pragma endregion

#pragma region vector4
	struct Vec4
	{
		float x, y, z, w;
		Vec4()										: x(0), y(0), z(0), w(0) {}
		Vec4(float x, float y, float z, float w)	: x(x), y(y), z(z), w(w) {}
		Vec4(Vec3 v, float w)							: x(v.x), y(v.y), z(v.z), w(w) {}
        float& operator[](int a)
        {
            switch(a)
            {
                case 0:		return x;
                case 1:		return y;
                case 2:		return z;
                case 3:		return w;
				default:	return w;
            }
        }
		Vec4 operator+(Vec4 v)
		{
			return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
		}
		Vec4 operator-(Vec4 v)
		{
			return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
		}
		Vec4 operator*(float a)
		{
			return Vec4(a * x, a * y, a * z, a * w);
		}
		Vec4 operator*(Vec4 v)
		{
			return Vec4(x * v.x, y * v.y, z * v.z, w * v.w);
		}
		void operator+=(Vec4 v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
		}
		void operator-=(Vec4 v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
		}
		void operator*=(float a)
		{
			x *= a; y *= a; z *= a; w *= a;
		}
		void operator*=(Vec4 v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;
		}
		Vec4 operator-()
		{
			return Vec4(-x, -y, -z, -w);
		}
		bool operator==(Vec4 v)
		{
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}
		bool operator!=(Vec4 v)
		{
			return x != v.x || y != v.y || z != v.z || w != v.w;
		}
		float dot(Vec4 v)
		{
			return (x * v.x + y * v.y + z * v.z + w * v.w);
		}
		void normalize()
		{
			float len = 1 / length();
			x *= len;
			y *= len;
			z *= len;
            w *= len;
		}
		float length()
		{
			return sqrt(x*x + y*y + z*z + w*w);
		}
	};
#pragma endregion

#pragma region matrix
	struct Matrix
	{
		Vec4 r[4];
		Matrix()
		{
			r[0] = Vec4(1, 0, 0, 0);
			r[1] = Vec4(0, 1, 0, 0);
			r[2] = Vec4(0, 0, 1, 0);
			r[3] = Vec4(0, 0, 0, 1);
		}
		Matrix(	float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24,
				float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44)
		{
			r[0] = Vec4(_11, _12, _13, _14);
			r[1] = Vec4(_21, _22, _23, _24);
			r[2] = Vec4(_31, _32, _33, _34);
			r[3] = Vec4(_41, _42, _43, _44);
		}
		Vec3 operator*(Vec3 v)
		{
			 return Vec3(r[0].x * v.x + r[1].x * v.y + r[2].x * v.z + r[3].x,
						 r[0].y * v.x + r[1].y * v.y + r[2].y * v.z + r[3].y,
						 r[0].z * v.x + r[1].z * v.y + r[2].z * v.z + r[3].z);
		}
		Matrix operator*(Matrix m)
		{
			Matrix out;
			out[0].x = r[0].x * m.r[0].x + r[0].y * m.r[1].x + r[0].z * m.r[2].x + r[0].w * m.r[3].x;
			out[0].y = r[0].x * m.r[0].y + r[0].y * m.r[1].y + r[0].z * m.r[2].y + r[0].w * m.r[3].y;
			out[0].z = r[0].x * m.r[0].z + r[0].y * m.r[1].z + r[0].z * m.r[2].z + r[0].w * m.r[3].z;
			out[0].w = r[0].x * m.r[0].w + r[0].y * m.r[1].w + r[0].z * m.r[2].w + r[0].w * m.r[3].w;

			out[1].x = r[1].x * m.r[0].x + r[1].y * m.r[1].x + r[1].z * m.r[2].x + r[1].w * m.r[3].x;
			out[1].y = r[1].x * m.r[0].y + r[1].y * m.r[1].y + r[1].z * m.r[2].y + r[1].w * m.r[3].y;
			out[1].z = r[1].x * m.r[0].z + r[1].y * m.r[1].z + r[1].z * m.r[2].z + r[1].w * m.r[3].z;
			out[1].w = r[1].x * m.r[0].w + r[1].y * m.r[1].w + r[1].z * m.r[2].w + r[1].w * m.r[3].w;

			out[2].x = r[2].x * m.r[0].x + r[2].y * m.r[1].x + r[2].z * m.r[2].x + r[2].w * m.r[3].x;
			out[2].y = r[2].x * m.r[0].y + r[2].y * m.r[1].y + r[2].z * m.r[2].y + r[2].w * m.r[3].y;
			out[2].z = r[2].x * m.r[0].z + r[2].y * m.r[1].z + r[2].z * m.r[2].z + r[2].w * m.r[3].z;
			out[2].w = r[2].x * m.r[0].w + r[2].y * m.r[1].w + r[2].z * m.r[2].w + r[2].w * m.r[3].w;

			out[3].x = r[3].x * m.r[0].x + r[3].y * m.r[1].x + r[3].z * m.r[2].x + r[3].w * m.r[3].x;
			out[3].y = r[3].x * m.r[0].y + r[3].y * m.r[1].y + r[3].z * m.r[2].y + r[3].w * m.r[3].y;
			out[3].z = r[3].x * m.r[0].z + r[3].y * m.r[1].z + r[3].z * m.r[2].z + r[3].w * m.r[3].z;
			out[3].w = r[3].x * m.r[0].w + r[3].y * m.r[1].w + r[3].z * m.r[2].w + r[3].w * m.r[3].w;

			return out;
		}
		Matrix operator*(float a)
		{
			Matrix out;

			out[0] = r[0] * a;
			out[1] = r[1] * a;
			out[2] = r[2] * a;
			out[3] = r[3] * a;

			return out;
		}
		void operator*=(Matrix m)
		{
			Vec4 v;
			v.x = r[0].x * m.r[0].x + r[0].y * m.r[1].x + r[0].z * m.r[2].x + r[0].w * m.r[3].x;
			v.y = r[0].x * m.r[0].y + r[0].y * m.r[1].y + r[0].z * m.r[2].y + r[0].w * m.r[3].y;
			v.z = r[0].x * m.r[0].z + r[0].y * m.r[1].z + r[0].z * m.r[2].z + r[0].w * m.r[3].z;
			v.w = r[0].x * m.r[0].w + r[0].y * m.r[1].w + r[0].z * m.r[2].w + r[0].w * m.r[3].w;
			r[0] = v;

			v.x = r[1].x * m.r[0].x + r[1].y * m.r[1].x + r[1].z * m.r[2].x + r[1].w * m.r[3].x;
			v.y = r[1].x * m.r[0].y + r[1].y * m.r[1].y + r[1].z * m.r[2].y + r[1].w * m.r[3].y;
			v.z = r[1].x * m.r[0].z + r[1].y * m.r[1].z + r[1].z * m.r[2].z + r[1].w * m.r[3].z;
			v.w = r[1].x * m.r[0].w + r[1].y * m.r[1].w + r[1].z * m.r[2].w + r[1].w * m.r[3].w;
			r[1] = v;

			v.x = r[2].x * m.r[0].x + r[2].y * m.r[1].x + r[2].z * m.r[2].x + r[2].w * m.r[3].x;
			v.y = r[2].x * m.r[0].y + r[2].y * m.r[1].y + r[2].z * m.r[2].y + r[2].w * m.r[3].y;
			v.z = r[2].x * m.r[0].z + r[2].y * m.r[1].z + r[2].z * m.r[2].z + r[2].w * m.r[3].z;
			v.w = r[2].x * m.r[0].w + r[2].y * m.r[1].w + r[2].z * m.r[2].w + r[2].w * m.r[3].w;
			r[2] = v;

			v.x = r[3].x * m.r[0].x + r[3].y * m.r[1].x + r[3].z * m.r[2].x + r[3].w * m.r[3].x;
			v.y = r[3].x * m.r[0].y + r[3].y * m.r[1].y + r[3].z * m.r[2].y + r[3].w * m.r[3].y;
			v.z = r[3].x * m.r[0].z + r[3].y * m.r[1].z + r[3].z * m.r[2].z + r[3].w * m.r[3].z;
			v.w = r[3].x * m.r[0].w + r[3].y * m.r[1].w + r[3].z * m.r[2].w + r[3].w * m.r[3].w;
			r[3] = v;
		}
		void operator*=(float a)
		{
			r[0] *= a;
			r[1] *= a;
			r[2] *= a;
			r[3] *= a;
		}
		Matrix operator+(Matrix m)
		{
			Matrix out;

			out[0] = r[0] + m[0];
			out[1] = r[1] + m[1];
			out[2] = r[2] + m[2];
			out[3] = r[3] + m[3];

			return out;
		}
		Matrix operator-(Matrix m)
		{
			Matrix out;

			out[0] = r[0] - m[0];
			out[1] = r[1] - m[1];
			out[2] = r[2] - m[2];
			out[3] = r[3] - m[3];

			return out;
		}
		void operator+=(Matrix m)
		{
			r[0] += m[0];
			r[1] += m[1];
			r[2] += m[2];
			r[3] += m[3];
		}
		void operator-=(Matrix m)
		{
			r[0] -= m[0];
			r[1] -= m[1];
			r[2] -= m[2];
			r[3] -= m[3];
		}
		Vec4& operator[](int i)
		{
			return r[i];
		}
		void identity()
		{
			r[0] = Vec4(1, 0, 0, 0);
			r[1] = Vec4(0, 1, 0, 0);
			r[2] = Vec4(0, 0, 1, 0);
			r[3] = Vec4(0, 0, 0, 1);
		}
		void transpose()
		{
			float temp;
			temp = r[0].y; r[0].y = r[1].x; r[1].x = temp;
			temp = r[2].x; r[2].x = r[0].z; r[0].z = temp;
			temp = r[3].x; r[3].x = r[0].w; r[0].w = temp;
			temp = r[2].y; r[2].y = r[1].z; r[1].z = temp;
			temp = r[3].y; r[3].y = r[1].w; r[1].w = temp;
			temp = r[3].z; r[3].z = r[2].w; r[2].w = temp;
		}
	};
#pragma endregion

#pragma region inline functions

	inline void transformCoord(Vec3 &vOut, Vec3 vec, Matrix mat)
	{
		vOut.x = mat[0][0] * vec.x + mat[1][0] * vec.y + mat[2][0] * vec.z;
		vOut.y = mat[0][1] * vec.x + mat[1][1] * vec.y + mat[2][1] * vec.z;
		vOut.z = mat[0][2] * vec.x + mat[1][2] * vec.y + mat[2][2] * vec.z;
	}

	inline Vec3 cross(Vec3 v, Vec3 u)
	{
		return Vec3(v.y * u.z - u.y * v.z, u.x * v.z - v.x * u.z, v.x * u.y - u.x * v.y);
	};

	inline float vec3Length(Vec3 v)
	{
		return v.length();
	};

	/* Returns transposed matrix */
    inline Matrix transpose(Matrix in)
	{
		Matrix mOut;
		mOut[0][0] = in[0][0]; mOut[0][1] = in[1][0]; mOut[0][2] = in[2][0]; mOut[0][3] = in[3][0];
		mOut[1][0] = in[0][1]; mOut[1][1] = in[1][1]; mOut[1][2] = in[2][1]; mOut[1][3] = in[3][1];
		mOut[2][0] = in[0][2]; mOut[2][1] = in[1][2]; mOut[2][2] = in[2][2]; mOut[2][3] = in[3][2];
		mOut[3][0] = in[0][3]; mOut[3][1] = in[1][3]; mOut[3][2] = in[2][3]; mOut[3][3] = in[3][3];
		return mOut;
	}

	/* Creates translation matrix */
    inline void translate(Matrix &mOut, float x, float y, float z)
    {
        mOut.identity();
        mOut[3][0] = x; mOut[3][1] = y; mOut[3][2] = z;
    }

	inline void translate(Matrix &mOut, Vec3 v)
    {
        mOut.identity();
        mOut[3][0] = v.x; mOut[3][1] = v.y; mOut[3][2] = v.z;
    }

	/* Returns a rotation matrix along all axis */
	inline Matrix yawPitchRoll(Vec3 rot)
	{
		float sinx = sin(rot.x), siny = sin(rot.y), sinz = sin(rot.z);
		float cosx = cos(rot.x), cosy = cos(rot.y), cosz = cos(rot.z);
		Matrix mOut;
		mOut[0][0] = cosz * cosy + sinz * sinx * siny;
		mOut[0][1] = sinz * cosx;
		mOut[0][2] = cosz * -siny + sinz * sinx * cosy;

		mOut[1][0] = -sinz * cosy + cosz * sinx * siny;
		mOut[1][1] = cosz * cosx;
		mOut[1][2] = sinz * siny + cosz * sinx * cosy;

		mOut[2][0] = cosx * siny;
		mOut[2][1] = -sinx;
		mOut[2][2] = cosx * cosy;

		return mOut;
	}

	/* Returns vector of length 1.0 */
	inline Vec3 normalize(Vec3 v)
	{
		float len = 1 / v.length();
		return Vec3(v.x * len, v.y * len, v.z * len);
	}

	/* Creates and reaturns a scaing matrix */
	inline Matrix scalingMatrix(float x, float y, float z)
	{
		Matrix out;
		out[0][0] = x;
		out[1][1] = y;
		out[2][2] = z;
		return out;
	}

	/* Creates and reaturns a scaing matrix */
	inline Matrix scalingMatrix(Vec3 scale)
	{
		Matrix out;
		out[0][0] = scale.x;
		out[1][1] = scale.y;
		out[2][2] = scale.z;
		return out;
	}

	/* Creates and returns a translation matrix */
	inline Matrix translationMatrix(float x, float y, float z)
	{
		Matrix out;
		out[3][0] = x;
		out[3][1] = y;
		out[3][2] = z;
		return out;
	}

	/* Creates and returns a translation matrix */
	inline Matrix translationMatrix(Vec3 v)
	{
		Matrix out;
		out[3][0] = v.x;
		out[3][1] = v.y;
		out[3][2] = v.z;
		return out;
	}

	inline float determinant(Matrix a)
	{
		if(a[0][0] == 0 || a[1][1] == 0 || a[2][2] == 0 || a[3][3] == 0)
		{
			int arr[24][4] = {	{0,1,2,3}, {0,1,3,2}, {0,2,1,3}, {0,2,3,1}, {0,3,1,2}, {0,3,2,1},
								{1,0,2,3}, {1,0,3,2}, {1,2,0,3}, {1,2,3,0}, {1,3,0,2}, {1,3,2,0},
								{2,0,1,3}, {2,0,3,1}, {2,1,0,3}, {2,1,3,0}, {2,3,0,1}, {2,3,1,0},
								{3,0,1,2}, {3,0,2,1}, {3,1,0,2}, {3,1,2,0}, {3,2,0,1}, {3,2,1,0}};

			float det = 0;
			int sigma = 1;
			for(int c = 0; c < 24; c++)
			{
				if((c + 1) % 2 == 0)
					sigma *= -1;

				det += (a[0][arr[c][0]] * a[1][arr[c][1]] * a[2][arr[c][2]] * a[3][arr[c][3]]) * sigma;
			}

			return det;
		}

		// Faster, but will not work if any of the main diagonal values are 0
		for(int c = 0; c < 4; c++)
		{
			float div = 1 / a[c][c];
			for(int d = c + 1; d < 4; d++)
				a[c][d] *= div;
			for(int e = c + 1; e < 4; e++)
			{
				for(int d = c + 1; d < 4; d++)
					a[e][d] -= a[e][c] * a[c][d];

				a[e][c] = 0;
			}
		}

		return a[0][0] * a[1][1] * a[2][2] * a[3][3];
	}

	/* TODO: test if can replace mOut with A completely */
	inline void MatrixInversion(Matrix &out, Matrix in)
	{
		int colCount, rowCount;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				Matrix B;
				rowCount = -1;
				for(int k = 0; k < 4; k++)
				{
					rowCount++;

					colCount = -1;
					for(int m = 0; m < 4; m++)
					{
						colCount++;
						if(m == j)
						{
							if(k == i || m == k)
								B[rowCount][colCount] = (i + j) % 2 == 1 ? -1.f : 1.f;
							else
								B[rowCount][colCount] = 0;
							continue;
						}
						else if(i == k)
						{
							B[rowCount][colCount] = 0;
							continue;
						}
						else
							B[rowCount][colCount] = in[k][m];
					}
				}
				out[i][j] = determinant(B);
				if((i + j) % 2 == 1)
					out[i][j] = -out[i][j];
			}
		}
		out *= 1 / determinant(in);
		out.transpose();
	}

	/** Creates projection matrix for left hand coordinate systems.
     *  [out] the output perspective matrix *
     *  [in] field of view                  *
     *  [in] aspect ratio                   *
     *  [in] near clipping plane            *
     *  [in] far clipping plane             **/
	inline void perspectiveFovLH(Matrix &pOut, float fov, float a, float zn, float zf)
	{
		float yscale = cos(fov * .5f) / sin(fov * .5f);

		pOut[0][0] = yscale / a;        pOut[0][1] = 0;         pOut[0][2] = 0;                         pOut[0][3] = 0;
		pOut[1][0] = 0;                 pOut[1][1] = yscale;    pOut[1][2] = 0;                         pOut[1][3] = 0;
		pOut[2][0] = 0;                 pOut[2][1] = 0;         pOut[2][2] = zf / (zf - zn);		    pOut[2][3] = 1;
		pOut[3][0] = 0;                 pOut[3][1] = 0;         pOut[3][2] = -(zn * zf) / (zf - zn);	pOut[3][3] = 0;
	}
	/** Creates projection matrix for right hand coordinate systems.
     *  [out] the output perspective matrix *
     *  [in] field of view                  *
     *  [in] aspect ratio                   *
     *  [in] near clipping plane            *
     *  [in] far clipping plane             **/
	inline void perspectiveFovRH(Matrix &pOut, float fov, float a, float zn, float zf)
	{
		float yscale = cos(fov * .5f) / sin(fov * .5f);

		pOut[0][0] = yscale / a;        pOut[0][1] = 0;         pOut[0][2] = 0;                             pOut[0][3] = 0;
		pOut[1][0] = 0;                 pOut[1][1] = yscale;    pOut[1][2] = 0;                             pOut[1][3] = 0;
		pOut[2][0] = 0;                 pOut[2][1] = 0;         pOut[2][2] = (zf + zn) / (zn - zf);         pOut[2][3] = -1;
		pOut[3][0] = 0;                 pOut[3][1] = 0;         pOut[3][2] = (2 * zn * zf) / (zn - zf);     pOut[3][3] = 0;
	}

	/*inline void perspectiveLH(Matrix &pOut, float w, float h, float zn, float zf)
	{
		float height = cos(w * .5f) / sin(w * .5f);
		pOut[0][0] = height / h;
		pOut[1][1] = height;
		pOut[2][2] = zf / (zf - zn);		pOut[3][2] = 1;
		pOut[2][3] = zn * zf / (zn - zf);	pOut[3][3] = 0;
	}*/

	/* Creates a view matrix for left hand coordinate systems */
	inline void lookAtLH(Matrix &pOut, Vec3 look, Vec3 up, Vec3 eye)
	{
		Vec3 right = cross(up, look);
		pOut[0][0] = right.x;	        pOut[0][1] = up.x;	        pOut[0][2] = look.x;	        pOut[0][3] = 0;
		pOut[1][0] = right.y;	        pOut[1][1] = up.y;	        pOut[1][2] = look.y;	        pOut[1][3] = 0;
		pOut[2][0] = right.z;	        pOut[2][1] = up.z;	        pOut[2][2] = look.z;	        pOut[2][3] = 0;
        pOut[3][0] = right.dot(-eye);   pOut[3][1] = up.dot(-eye);  pOut[3][2] = look.dot(-eye);    pOut[3][3] = 1;
	}

	/* Creates a view matrix for left hand coordinate systems */
	inline void lookAtLHP(Matrix &pOut, Vec3 lookatTarget, Vec3 up, Vec3 eye)
	{
		Vec3 look = lookatTarget - eye;
		look.normalize();
		Vec3 right = cross(up, look);
		right.normalize();
		Vec3 yaxis = cross(look, right);
		pOut[0][0] = right.x;	        pOut[0][1] = yaxis.x;	        pOut[0][2] = look.x;	        pOut[0][3] = 0;
		pOut[1][0] = right.y;	        pOut[1][1] = yaxis.y;	        pOut[1][2] = look.y;	        pOut[1][3] = 0;
		pOut[2][0] = right.z;	        pOut[2][1] = yaxis.z;	        pOut[2][2] = look.z;	        pOut[2][3] = 0;
        pOut[3][0] = -right.dot(eye);   pOut[3][1] = -yaxis.dot(eye);	pOut[3][2] = -look.dot(eye);    pOut[3][3] = 1;
	}

    inline void lookAtRH(Matrix &pOut, Vec3 look, Vec3 up, Vec3 eye)
    {
        Vec3 left = cross(look, up);
        pOut[0][0] = left.x;	pOut[0][1] = up.x;	    pOut[0][2] = -look.x;	pOut[0][3] = 0;
		pOut[1][0] = left.y;	pOut[1][1] = up.y;	    pOut[1][2] = -look.y;	pOut[1][3] = 0;
		pOut[2][0] = left.z;	pOut[2][1] = -up.z;	    pOut[2][2] = -look.z;	pOut[2][3] = 0;
		pOut[3][0] = -eye.x;    pOut[3][1] = -eye.y;    pOut[3][2] = eye.z;    pOut[3][3] = 1;
    }
	/*inline void lookAtLH(Matrix &pOut, Vec3 look, Vec3 up, Vec3 eye)
	{
		Vec3 right = cross(up, look);
		pOut[0][0] = right.x;	pOut[1][0] = up.x;	pOut[2][0] = look.x;	pOut[3][0] = 0;
		pOut[0][1] = right.y;	pOut[1][1] = up.y;	pOut[2][1] = look.y;	pOut[3][1] = 0;
		pOut[0][2] = right.z;	pOut[1][2] = up.z;	pOut[2][2] = look.z;	pOut[3][2] = 0;

		pOut[0][3] = right.dot(-eye);
		pOut[1][3] = up.dot(-eye);
		pOut[2][3] = look.dot(-eye);
		pOut[3][3] = 1;
	}*/

	/* Creates rotation matrix for rotation around a given axis */
	inline void rotationAxis(Matrix &mOut, Vec3 axis, float angle)
	{
		mOut[0][0] = axis.x * axis.x * (1 - cos(angle)) + cos(angle);
		mOut[0][1] = axis.x * axis.y * (1 - cos(angle)) + axis.z * sin(angle);
		mOut[0][2] = axis.x * axis.z * (1 - cos(angle)) - axis.y * sin(angle);

		mOut[1][0] = axis.x * axis.y * (1 - cos(angle)) - axis.z * sin(angle);
		mOut[1][1] = axis.y * axis.y * (1 - cos(angle)) + cos(angle);
		mOut[1][2] = axis.y * axis.z * (1 - cos(angle)) + axis.x * sin(angle);

		mOut[2][0] = axis.x * axis.z * (1 - cos(angle)) + axis.y * sin(angle);
		mOut[2][1] = axis.y * axis.z * (1 - cos(angle)) - axis.x * sin(angle);
		mOut[2][2] = axis.z * axis.z * (1 - cos(angle)) + cos(angle);
	}

	/* Returns the resulting angle of a wall collission. Should hopefully work. */
	inline Vec3 planeReflection(Vec3 _v, Vec3 _n)
	{
		return _v - _n * (_v.dot(_n)) * 2;
		//return -( (_n * _v) *  2.0f * _n - _v);
	}

	//inline std::vector<Vec4> buildViewFrustum(Matrix view, Matrix proj)
	//{
	//	Matrix viewProj = view * proj;
	//	std::vector<Vec4> frustum(6);

	//	frustum[0].x = viewProj[0][3] + viewProj[0][0];
	//	frustum[0].y = viewProj[1][3] + viewProj[1][0];
	//	frustum[0].z = viewProj[2][3] + viewProj[2][0];
	//	frustum[0].w = viewProj[3][3] + viewProj[3][0];

	//	frustum[1].x = viewProj[0][3] - viewProj[0][0];
	//	frustum[1].y = viewProj[1][3] - viewProj[1][0];
	//	frustum[1].z = viewProj[2][3] - viewProj[2][0];
	//	frustum[1].w = viewProj[3][3] - viewProj[3][0];

	//	frustum[2].x = viewProj[0][3] - viewProj[0][1];
	//	frustum[2].y = viewProj[1][3] - viewProj[1][1];
	//	frustum[2].z = viewProj[2][3] - viewProj[2][1];
	//	frustum[2].w = viewProj[3][3] - viewProj[3][1];

	//	frustum[3].x = viewProj[0][3] + viewProj[0][1];
	//	frustum[3].y = viewProj[1][3] + viewProj[1][1];
	//	frustum[3].z = viewProj[2][3] + viewProj[2][1];
	//	frustum[3].w = viewProj[3][3] + viewProj[3][1];

	//	frustum[4].x = viewProj[0][2];
	//	frustum[4].y = viewProj[1][2];
	//	frustum[4].z = viewProj[2][2];
	//	frustum[4].w = viewProj[3][2];

	//	frustum[5].x = viewProj[0][3] - viewProj[0][2];
	//	frustum[5].y = viewProj[1][3] - viewProj[1][2];
	//	frustum[5].z = viewProj[2][3] - viewProj[2][2];
	//	frustum[5].w = viewProj[3][3] - viewProj[3][2];

	//	for(int i = 0; i < 6; i++)
	//		frustum[i].normalize();
	//	return frustum;
	//}

#define _max(a,b)            (((a) > (b)) ? (a) : (b))

#define _min(a,b)            (((a) < (b)) ? (a) : (b))

#pragma endregion
#endif
