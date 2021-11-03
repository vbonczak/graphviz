#include "simulation.hpp" 

using namespace cgp;
using namespace std;

float alpha, beta, mu, vepsilon;

void simulate(std::vector<particle_structure>& particles, float dt)
{
	vec3 const g = { 0,-9.81f,0 };
	size_t const N = particles.size();
	for (size_t k = 0; k < N; ++k)
	{
		particle_structure& particle = particles[k];

		vec3 const f = particle.m * g;

		particle.v = (1 - 0.9f * dt) * particle.v + dt * f;
		particle.p = particle.p + dt * particle.v;
	}

	// To do :
	//  Handle collision ...

	//Points des plans
	vector<vec3> planesA = { {0,0,0}  };
	vector<vec3> normals = { {0,1,0}  };

	for (size_t k = 0; k < N; ++k)
	{
		particle_structure& particle = particles[k];
		//Collision avec les faces du cube
		for (size_t j = 0; j < planesA.size(); j++)
		{
			float d = dot(particle.p - planesA[j], normals[j]);
			if (d < particle.r)
			{
				vec3 vpar, vperp;
				vperp = dot(particle.v, normals[j]) * normals[j];
				vpar = particle.v - vperp;

				particle.v = alpha * vpar - beta * vperp;
				particle.p += (particle.r - d) * normals[j];
			}
		}

		//Collision entre sphères
		for (size_t i = 0; i < N; i++)
		{
			if (i != k)
			{
				particle_structure& particle2 = particles[i];
				float dist = norm(particle.p - particle2.p);
				if (dist <= particle2.r + particle.r)
				{
					//Ces deux sphères sont en collision
					//Formule générale avec des masses différentes
					vec3 u = (particle.p - particle2.p) / dist;
					if (norm(particle.v) > vepsilon || norm(particle2.v) > vepsilon)
					{
						float j = 2 * dot(particle2.v - particle.v, u) * (particle.m * particle2.m) / (particle.m + particle2.m);

						particle.v = alpha * particle.v + u * beta * j / particle.m;
						particle2.v = alpha * particle2.v - u * beta * j / particle2.m;
					}
					else
					{
						particle.v = mu * particle.v;
						particle2.v = mu * particle2.v;
					}

					//Position corrigée
					float d = particle.r + particle2.r - dist;
					particle.p += (d / 2) * u;
					particle2.p -= (d / 2) * u;

					//Formule avec m1=m2

				}
			}
		}
	}
}
