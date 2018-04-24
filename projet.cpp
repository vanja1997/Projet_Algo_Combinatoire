#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm> 
#include <random>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <SDL/SDL.h>
#include <math.h>
#include <unistd.h>

using std::vector;
using std::pair;
using std::for_each;
using std::cout;
using std::make_pair;

class arrete 
{
public :
	pair <float, float> coordonneespts1;
	pair <float, float> coordonneespts2;
	float dij;      
	float pij; 
	int nbr_passage; 
};


class fourmi
{
public :
	vector<int> point_visiter;
	int place_actuelle;
	int distance_parcour;
	bool plus_rapide;
};


typedef vector<fourmi> fourmisse ;
typedef vector<arrete> graph;
typedef vector<float> vecf;



SDL_Surface* screen;
void ligneHorizontale(int x, int y, int w, Uint32 coul)
{
  SDL_Rect r;
 
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = 1;
 
  SDL_FillRect(screen, &r, coul);
}

void cercle(int cx, int cy, int rayon)
{
  int d, y, x;
  Uint32 coul =  SDL_MapRGB(screen->format, 255, 0, 0);
  d = 3 - (2 * rayon);
  x = 0;
  y = rayon;
 
  while (y >= x) {
    ligneHorizontale(cx - x, cy - y, 2 * x + 1, coul);
    ligneHorizontale(cx - x, cy + y, 2 * x + 1, coul);
    ligneHorizontale(cx - y, cy - x, 2 * y + 1, coul);
    ligneHorizontale(cx - y, cy + x, 2 * y + 1, coul);
 
    if (d < 0)
      d = d + (4 * x) + 6;
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }
 
    x++;
  }
}

void echangerEntiers(int* x, int* y)
{
  int t = *x;
  *x = *y;
  *y = t;
}
 
void setPixel(int x, int y, Uint32 coul)
{
  *((Uint32*)(screen->pixels) + x + y * screen->w) = coul;
}


void setPixelVerif(int x, int y, Uint32 coul)
{
  if (x >= 0 && x < screen->w &&
      y >= 0 && y < screen->h)
    setPixel(x, y, coul);
}


void ligne(int x1, int y1, int x2, int y2, int color , int color1 , int color2)
{
  int d, dx, dy, aincr, bincr, xincr, yincr, x, y;
  Uint32 coul =  SDL_MapRGB(screen->format, color, color1, color2);
  if (abs(x2 - x1) < abs(y2 - y1)) {
    /* parcours par l'axe vertical */
 
    if (y1 > y2) {
      echangerEntiers(&x1, &x2);
      echangerEntiers(&y1, &y2);
    }
 
    xincr = x2 > x1 ? 1 : -1;
    dy = y2 - y1;
    dx = abs(x2 - x1);
    d = 2 * dx - dy;
    aincr = 2 * (dx - dy);
    bincr = 2 * dx;
    x = x1;
    y = y1;
 
    setPixelVerif(x, y, coul);
 
    for (y = y1+1; y <= y2; ++y) {
      if (d >= 0) {
	x += xincr;
	d += aincr;
      } else
	d += bincr;
 
      setPixelVerif(x, y, coul);
    }
 
  } else {
    /* parcours par l'axe horizontal */
    
    if (x1 > x2) {
      echangerEntiers(&x1, &x2);
      echangerEntiers(&y1, &y2);
    }
 
    yincr = y2 > y1 ? 1 : -1;
    dx = x2 - x1;
    dy = abs(y2 - y1);
    d = 2 * dy - dx;
    aincr = 2 * (dy - dx);
    bincr = 2 * dy;
    x = x1;
    y = y1;
 
    setPixelVerif(x, y, coul);
 
    for (x = x1+1; x <= x2; ++x) {
      if (d >= 0) {
	y += yincr;
	d += aincr;
      } else
	d += bincr;
 
      setPixelVerif(x, y, coul);
    }
  }    
}


vector<pair<float, float>> create_vertex(uint sommet) //génère des sommets aléatoires
{
	uint i;
	vector<pair<float, float>> v;
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution0(0.1);

	for(i = 0; i < sommet; i++) {

		v.push_back(make_pair(distribution0(generator),distribution0(generator)));
		cercle(v[i].first * 800, v[i].second * 600,10);
	}
	return v;
}

graph create_graph(uint sommet, vector<pair<float, float>> vertex)
{
	uint i , j;
	arrete a;
	graph g;
	pair<int,int> p;
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(0,1);
	for(i = 0; i < sommet; i++){
		for(j = 0; j < sommet; j++){
				a.coordonneespts1 = vertex[i];
				a.coordonneespts2 = vertex[j];
				ligne(vertex[i].first*800,vertex[i].second*600,vertex[j].first*800,vertex[j].second*600, 122,122,122);
				a.dij = fabs((a.coordonneespts2.first - a.coordonneespts1.first) + (a.coordonneespts2.second - a.coordonneespts1.second));
			if(j < i){ //Pour ne traiter que la moitié des sommets
				a.pij = distribution(generator);
				a.nbr_passage = 0;
			}
			else {
				a.pij = 0;
				a.nbr_passage = 0;
			}
			g.push_back(a);
		}
	}
	for(i = 0; i < sommet; i++){
		for(j = 0; j < i; j++){
			g[j * sommet + i].pij = g[i * sommet + j].pij;
		}
	}
	return g;
}

fourmisse fourmi_distributeur(uint sommet)
{	
	fourmi f;
	fourmisse g; //vecteur de fourmis
	for(uint i = 0; i < sommet; i++){
		for(uint j = 0; j <= 1; j++){
			f.place_actuelle = i;
			f.point_visiter.push_back(i);
			f.plus_rapide = false;
			f.distance_parcour = 0;
			g.push_back(f);
			f.point_visiter.pop_back();
		}
	}
	return g;
}



bool seen(vector<int> vec, int vu)
{
	for(uint i = 0; i < vec.size(); i++){
		if(vec[i] == vu)
			return true;
	}
	return false;
}

float somme_proba(uint sommet, fourmi f, graph g)
{
	float incre = 0.0;
	for(int i = 0 ; i < (int)sommet; i++){
		if(f.place_actuelle != i && seen(f.point_visiter, i) == false)
			incre += g[f.place_actuelle * sommet + i].pij / g[f.place_actuelle * sommet + i].dij;
	}
	return incre;
}

void parcours_fourmi(graph g, fourmisse f, uint sommet)
{
	float raound, proba;
	int ville_start = 0;
	int res;
	int meilleur;
	while(1){
		for(uint i = 0 ; i < f.size() ; i++) { //tant que toute les fourmis ne sont pas passer 
			while(f[i].point_visiter.size() != sommet){
				SDL_Flip(screen);
				proba = somme_proba(sommet,f[i], g);
				std::default_random_engine generator;
				std::uniform_real_distribution<float> distribution(0,proba);
				raound = distribution(generator);
				for(uint j= 0 ; j < sommet ; j++){ //tant que la fourmi n est pas passez sur tout les sommets
					int localisation = f[i].place_actuelle * sommet + j;
					int invlocalisation = j * sommet + f[i].place_actuelle;
					if(f[i].place_actuelle != (int)j && seen(f[i].point_visiter, j) == false){
						raound -= g[f[i].place_actuelle * sommet + j].pij / g[f[i].place_actuelle * sommet + j].dij;
						if(raound <= 0){ 
							g[localisation].pij = g[localisation].pij/2 + 100 * g[localisation].nbr_passage ;
							g[localisation].nbr_passage ++;
							g[invlocalisation].pij = g[localisation].pij;
							g[invlocalisation].nbr_passage ++;
							//ligne(g[f[p].place_actuelle * sommet + ville_start].coordonneespts1.first*800 , g[f[p].place_actuelle * sommet + ville_start].coordonneespts1.second*600 , g[f[p].place_actuelle * sommet + ville_start].coordonneespts2.first*800 , g[f[p].place_actuelle * sommet + ville_start].coordonneespts2.second*600, g[f[p].place_actuelle * sommet + ville_start].pij*50,g[f[p].place_actuelle * sommet + ville_start].pij*50,g[f[p].place_actuelle * sommet + ville_start].pij*50);
							f[i].place_actuelle = j;
							f[i].point_visiter.push_back(j);
							if(f[i].plus_rapide == true){
								ligne(g[localisation].coordonneespts1.first*800 , g[localisation].coordonneespts1.second*600 , g[localisation].coordonneespts2.first*800 , g[localisation].coordonneespts2.second*600 , 0,255,0);
								sleep(1);
							}
							j = sommet;
							f[i].distance_parcour += g[localisation].dij;
						}
					}
				}	
			}
		}
		for(uint p = 0 ; p < f.size() ; p++){
			ville_start = f[p].point_visiter.front();
			g[f[p].place_actuelle * sommet + ville_start].pij = g[f[p].place_actuelle * sommet + ville_start].pij /2 + 100* g[f[p].place_actuelle * sommet + ville_start].nbr_passage;
			g[f[p].place_actuelle * sommet + ville_start].nbr_passage++;
			g[ville_start * sommet + f[p].place_actuelle].pij = g[f[p].place_actuelle * sommet + ville_start].pij;
			g[ville_start * sommet + f[p].place_actuelle].nbr_passage++;
			//ligne(g[f[p].place_actuelle * sommet + ville_start].coordonneespts1.first*800 , g[f[p].place_actuelle * sommet + ville_start].coordonneespts1.second*600 , g[f[p].place_actuelle * sommet + ville_start].coordonneespts2.first*800 , g[f[p].place_actuelle * sommet + ville_start].coordonneespts2.second*600, g[f[p].place_actuelle * sommet + ville_start].pij*50,g[f[p].place_actuelle * sommet + ville_start].pij*50,g[f[p].place_actuelle * sommet + ville_start].pij*50);
			if(f[p].plus_rapide == true){
				ligne(g[f[p].place_actuelle * sommet + ville_start].coordonneespts1.first*800 , g[f[p].place_actuelle * sommet + ville_start].coordonneespts1.second*600 , g[f[p].place_actuelle * sommet + ville_start].coordonneespts2.first*800 , g[f[p].place_actuelle * sommet + ville_start].coordonneespts2.second*600 , 0,255,0);
				sleep(1);
			}
			f[p].place_actuelle = f[p].point_visiter.front();
			f[p].point_visiter.clear();
			f[p].point_visiter.push_back(f[p].place_actuelle);
		}
		res = f[0].distance_parcour;
		for(uint l= 0 ; l < f.size() ; l++){
			if(res >= f[l].distance_parcour){
				res = f[l].distance_parcour;
				meilleur = l;
			}
			f[l].plus_rapide = false;
		}
		f[meilleur].plus_rapide = true;
	}
}

void initSDL(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Erreur à l'initialisation de la SDL : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
 
  atexit(SDL_Quit);
  screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  if (screen == NULL) {
    fprintf(stderr, "Impossible d'activer le mode graphique : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  SDL_WM_SetCaption("Parcours des fourmis !!!", NULL);
}

int main()
{
	int sommet = 10;
	fourmisse f;
	graph g;
	initSDL();
	vector<pair<float, float>> v;
	v = create_vertex(sommet);
	g = create_graph(sommet, v);
	f = fourmi_distributeur(sommet);
	parcours_fourmi(g, f, sommet);
	
	return 0;
}

