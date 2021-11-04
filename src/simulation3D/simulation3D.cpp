#include "simulation3D.hpp" 
#include "../simulation/simulation.hpp"

using namespace cgp;
using namespace std;
 
//Points des plans
vector<vec3> planesA3D = { {0,0,0}, {-H / 2, 0, -L / 2},{-H / 2, 0, -L / 2}, {H / 2, 0, L / 2}, {H / 2, 0, L / 2}, };
vector<vec3> normals3D = { {0,1,0}, {0,0,1}, {1,0,0}, {-1,0,0},{0,0,-1} };

void simulate3D(std::vector<boule_structure>& boules, float dt)
{
	vec3 const g = { 0,-9.81f,0 };
	size_t const N = boules.size();
	for (size_t k = 0; k < N; ++k)
	{
		boule_structure& boule = boules[k];

		vec3 const f = boule.m * g;

		boule.v = (1 - 0.9f * dt) * boule.v + dt * f;
		boule.p = boule.p + dt * boule.v;
	}

	//Collision
	for (size_t k = 0; k < N; ++k)
	{
		boule_structure& boule = boules[k];
		//Collision avec les faces du cube
		for (size_t j = 0; j < planesA3D.size(); j++)
		{
			float d = dot(boule.p - planesA3D[j], normals3D[j]);
			if (d < boule.r)
			{
				vec3 vpar, vperp;
				vperp = dot(boule.v, normals3D[j]) * normals3D[j];
				vpar = boule.v - vperp;

				boule.v = alpha * vpar - beta * vperp;
				boule.p += (boule.r - d) * normals3D[j];
			}
		}

		//Collision entre sphères
		for (size_t i = 0; i < N; i++)
		{
			if (i != k)
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
