#include "simulation.hpp" 

using namespace cgp;
using namespace std;

float alpha, beta, beta_feutre, mu, vepsilon;

float H = 1.27f;
float L = 2.54f;
float h = 0.04f;

#ifdef TROIS_DIMENSION
vector<vec3> holes = { {-H / 2,-H/2,-L / 2},{H / 2, -H/2, L / 2},{-H / 2, -H/2, L / 2},{H / 2, -H/2, -L / 2},{H / 2, -H/2, 0},{-H / 2, -H/2, 0} ,
 {-H / 2,H/2,-L / 2},{H / 2, H/2, L / 2},{-H / 2, H/2, L / 2},{H / 2, H/2, -L / 2},{H / 2, H/2, 0},{-H / 2, H/2, 0} };

//Points des plans
vector<vec3> planesA = { {-H / 2, 0, 0},{0,-H / 2,0}, {0, 0, -L / 2}, {H / 2, 0, 0}, {0, H / 2, 0}, {0,0,L / 2} };
vector<vec3> normals = { {1,0,0}, {0,1,0}, {0,0,1}, {-1,0,0},{0,-1,0} ,{0,0,-1} };
#else 
vector<vec3> holes = { {-H / 2,0,-L / 2},{H / 2, 0, L / 2},{-H / 2, 0, L / 2},{H / 2, 0, -L / 2},{H / 2, 0, 0},{-H / 2, 0, 0} };

//Points des plans
vector<vec3> planesA = { {0,0,0}, {-H / 2, 0, -L / 2},{-H / 2, 0, -L / 2}, {H / 2, 0, L / 2}, {H / 2, 0, L / 2}, {0,-1,0} };
vector<vec3> normals = { {0,1,0}, {0,0,1}, {1,0,0}, {-1,0,0},{0,0,-1} , {0,1,0} };


#endif

void simulate(std::vector<boule_structure>& boules, float dt)
{
	vec3 const g = { 0,-9.81f,0 };
	size_t const N = boules.size();
	for (size_t k = 0; k < N; ++k)
	{
		boule_structure& boule = boules[k];

		boule.v = boule.v + dt * g;
		boule.p = boule.p + dt * boule.v;
	}

	//Collision
	for (size_t k = 0; k < N; ++k)
	{
		boule_structure& boule = boules[k];
		//Collision avec les faces du cube, face de la table traitée séparément pour les trous

		float d = dot(boule.p - planesA[0], normals[0]);
		if (d < boule.r && boule.in_play && !over_hole(boule))
		{
			vec3 vpar, vperp;
			vperp = dot(boule.v, normals[0]) * normals[0];
			vpar = boule.v - vperp;

			boule.v = alpha * vpar - beta_feutre * vperp;
			boule.p += (boule.r - d) * normals[0];
		}

		for (size_t j = 1; j < planesA.size(); j++)
		{
			float d = dot(boule.p - planesA[j], normals[j]);
			if (d < boule.r)
			{
				vec3 vpar, vperp;
				vperp = dot(boule.v, normals[j]) * normals[j];
				vpar = boule.v - vperp;

				boule.v = alpha * vpar - beta * vperp;
				boule.p += (boule.r - d) * normals[j];
			}
		}

		//Collision entre sphères
		for (size_t i = 0; i < k; i++)
		{

			boule_structure& boule2 = boules[i];
			float dist = norm(boule.p - boule2.p);
			if (dist <= boule2.r + boule.r)
			{
				//Ces deux sphères sont en collision
				//Formule générale avec des masses différentes
				vec3 u = (boule.p - boule2.p) / dist;
				if (norm(boule.v) > vepsilon || norm(boule2.v) > vepsilon)
				{
					float j = 2 * dot(boule2.v - boule.v, u) * (boule.m * boule2.m) / (boule.m + boule2.m);

					boule.v = alpha * boule.v + u * beta * j / boule.m;
					boule2.v = alpha * boule2.v - u * beta * j / boule2.m;
				}
				else
				{
					boule.v = mu * boule.v;
					boule2.v = mu * boule2.v;
				}

				//Position corrigée
				float d = boule.r + boule2.r - dist;
				boule.p += (d / 2) * u;
				boule2.p -= (d / 2) * u;

				//Formule avec m1=m2

			}

		}
	}
}

bool over_hole(boule_structure& boule)
{
	for (size_t j = 0; j < holes.size(); j++)
	{
		if (norm(boule.p - holes[j]) < 2 * boule.r)
		{
			boule.in_play = false;
#ifndef TROIS_DIMENSION
			boule.v = { 0,0,0 };
#endif
			return true;
		}
	}
	return false;
}


void simulate3d(std::vector<boule_structure>& boules, float dt)
{
	size_t const N = boules.size();
	for (size_t k = 0; k < N; ++k)
	{
		boule_structure& boule = boules[k];

		boule.v = (1 - 1.5f * dt) * boule.v;
		boule.p = boule.p + dt * boule.v;
	}

	//Collision
	for (size_t k = 0; k < N; ++k)
	{
		boule_structure& boule = boules[k];
		//Collision avec les faces du cube
		if (boule.in_play && !over_hole(boule))
		{
			for (size_t j = 0; j < planesA.size(); j++)
			{
				float d = dot(boule.p - planesA[j], normals[j]);
				if (d < boule.r)
				{
					vec3 vpar, vperp;
					vperp = dot(boule.v, normals[j]) * normals[j];
					vpar = boule.v - vperp;

					boule.v = alpha * vpar - beta * vperp;
					boule.p += (boule.r - d) * normals[j];
				}
			}

			//Collision entre sphères
			for (size_t i = 0; i < k; i++)
			{

				boule_structure& boule2 = boules[i];
				float dist = norm(boule.p - boule2.p);
				if (dist <= boule2.r + boule.r)
				{
					//Ces deux sphères sont en collision
					//Formule générale avec des masses différentes
					vec3 u = (boule.p - boule2.p) / dist;
					if (norm(boule.v) > vepsilon || norm(boule2.v) > vepsilon)
					{
						float j = 2 * dot(boule2.v - boule.v, u) * (boule.m * boule2.m) / (boule.m + boule2.m);

						boule.v = alpha * boule.v + u * beta * j / boule.m;
						boule2.v = alpha * boule2.v - u * beta * j / boule2.m;
					}
					else
					{
						boule.v = mu * boule.v;
						boule2.v = mu * boule2.v;
					}

					//Position corrigée
					float d = boule.r + boule2.r - dist;
					boule.p += (d / 2) * u;
					boule2.p -= (d / 2) * u;

					//Formule avec m1=m2

				}

			}
		}
	}
}