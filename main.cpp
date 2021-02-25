#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include<fstream>

extern "C" {
#include"../template-project-2/SDL2-2.0.10/include/SDL.h"
#include"../template-project-2/SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define ILOSC_SAMOCHODOW 10
#define WIELKOSC_OBIEKTOW 32
#define ILOSC_3_ZOLWI 5
#define ILOSC_2_ZOLWI 5
#define ILOSC_MALYCH_KLOD 6
#define ILOSC_DUZYCH_KLOD 5
#define ILOSC_SREDNICH_KLOD 8
#define POLE_DODATKOWE 50



typedef struct objekt {
	int x;
	int y;
	int speed;
	int distance;
	SDL_Surface* bitmapa;

}objekt;

typedef struct high_score
{
	int score;
	char name[20];
} high_score;

int my_compare(const void* a, const void* b)
{
	int _a = *(int*)a;
	int _b = *(int*)b;
	if (_a < _b) return 1;
	else if (_a == _b) return 0;
	else return -1;
}

	

bool kolizja(int zabax, int przeszkodax, int wielkosc_przeszkody) {
	for (int i = przeszkodax - (0.5 * wielkosc_przeszkody); i <= przeszkodax + (0.5 * wielkosc_przeszkody); i++)
		if (zabax == i) return true;
	return false;
}

bool kolizja_s(int zabax, int przeszkodax, int wielkosc_przeszkody) {
	for (int i = przeszkodax - (wielkosc_przeszkody); i <= przeszkodax + (wielkosc_przeszkody); i++)
		if (zabax == i) return true;
	return false;
}

bool kolizja_3_zolwi(int* zabax,int wielkosc_przeszkody, objekt przeszkoda[], int distance) {
	for (int i = 0; i < ILOSC_3_ZOLWI; i++) {
		if (kolizja(*zabax, przeszkoda[i].x-distance, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x - distance;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x-distance + WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x - distance + WIELKOSC_OBIEKTOW;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x - distance - WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x - distance - WIELKOSC_OBIEKTOW;
			return true;
		}
	}
	return false;
}

bool kolizja_2_zolwi(int* zabax, int wielkosc_przeszkody, objekt przeszkoda[], int distance) {
	for (int i = 0; i < ILOSC_2_ZOLWI; i++) {
		if (kolizja(*zabax, przeszkoda[i].x - distance, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x - distance;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x - distance - WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x - distance - WIELKOSC_OBIEKTOW;
			return true;
		}
	}
	return false;
}

bool kolizja_krotkie_klody(int* zabax, int wielkosc_przeszkody, objekt przeszkoda[], int distance, int ilosc_przeszkod) {
	for (int i = 0; i < ilosc_przeszkod; i++) {
		if (kolizja(*zabax, przeszkoda[i].x + distance, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x + distance + WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance + WIELKOSC_OBIEKTOW;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x + distance - WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance - WIELKOSC_OBIEKTOW;
			return true;
		}
	}
	return false;
}

bool kolizja_srednie_klody(int* zabax, int wielkosc_przeszkody, objekt przeszkoda[], int distance, int ilosc_przeszkod) {
	for (int i = 0; i < ilosc_przeszkod; i++) {
		if (kolizja(*zabax, przeszkoda[i].x + distance, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x + distance + WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance + WIELKOSC_OBIEKTOW;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x + distance - WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance - WIELKOSC_OBIEKTOW;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x + distance + 2*WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance + 2*WIELKOSC_OBIEKTOW;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x + distance - 2 * WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance - 2*WIELKOSC_OBIEKTOW;
			return true;
		}
	}
	return false;
}

bool kolizja_dlugie_klody(int* zabax, int wielkosc_przeszkody, objekt przeszkoda[], int distance, int ilosc_przeszkod) {
	for (int i = 0; i < ilosc_przeszkod; i++) {
		if (kolizja(*zabax, przeszkoda[i].x + distance, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x + distance + WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance + WIELKOSC_OBIEKTOW;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x + distance - WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance - WIELKOSC_OBIEKTOW;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x + distance + 2 * WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance + 2*WIELKOSC_OBIEKTOW;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x + distance - 2 * WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance - 2*WIELKOSC_OBIEKTOW;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x + distance + 3 * WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance + 3*WIELKOSC_OBIEKTOW;
			return true;
		}
		else if (kolizja(*zabax, przeszkoda[i].x + distance - 3 * WIELKOSC_OBIEKTOW, wielkosc_przeszkody)) {
			*zabax = przeszkoda[i].x + distance - 3*WIELKOSC_OBIEKTOW;
			return true;
		}
	}
	return false;
}




// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
				SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt œrodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
	};


// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
	};


// rysowanie linii o d³ugoœci l w pionie (gdy dx = 0, dy = 1) 
// b¹dŸ poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
	};


// rysowanie prostok¹ta o d³ugoœci boków l i k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
				   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};


// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	int t1, t2, quit, frames, rc;
	double delta, worldTime, fpsTimer, fps, time = 40, distance, etiSpeed;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *eti;
	SDL_Surface *plansza;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	bool menu = true;
	bool pause = false;
	bool quit_game = false;
	bool game_over = false;
	bool wpisywanie = false;
	bool score_tab = false, block=false;
	int score_position_x = SCREEN_WIDTH/2 - 15;
	int score_position_y[10];
	for (int i = 0; i < 10; i++) {
		if (i == 0) score_position_y[i] = 150;
		else score_position_y[i] = score_position_y[i - 1] + 10;
	}
	char show_score[20] = { 0 };
	int score = 0;
	int najwyzsza_pozycja = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;


	objekt samochod_1[ILOSC_SAMOCHODOW], samochod_2[ILOSC_SAMOCHODOW], samochod_3[ILOSC_SAMOCHODOW], samochod_4[ILOSC_SAMOCHODOW], autobus[ILOSC_SAMOCHODOW], zaba, zolwie1[ILOSC_3_ZOLWI], zolwie2[ILOSC_2_ZOLWI], male_klody[ILOSC_MALYCH_KLOD],
		srednie_klody[ILOSC_SREDNICH_KLOD], duze_klody[ILOSC_DUZYCH_KLOD], odwrocone_zaby[5], zycia[5];

	int ilosc_zyc = 5;
	char scoreText[13];
	char name[20];

	for (int i = 0; i < 20; i++) {
		if (i == 0) name[i] = '|';
		else name[i] = '\0';
	}
	int dlugosc_l = strlen(name);
	int f = 0;

	FILE* plik;
	plik = fopen("hs.txt", "r");
	high_score tab[10];
	/*fstream plik;
	plik.open("hs.txt", ios::in);*/
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			tab[i].name[j] = '0';
		}
		tab[i].score = 0;
	}

	for (int i = 0; i < 10; i++) {

		fscanf(plik, "%s", tab[i].name);
		fscanf(plik, "%d", &tab[i].score);
	}

	fclose(plik);
	plik = fopen("hs.txt", "w");

	// okno konsoli nie jest widoczne, je¿eli chcemy zobaczyæ
	// komunikaty wypisywane printf-em trzeba w opcjach:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// zmieniæ na "Console"
	printf("wyjscie printfa trafia do tego okienka\n");
	printf("printf output goes here\n");

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 2;
		}

//	 tryb pe³noekranowy / fullscreen mode
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT+POLE_DODATKOWE, 0,
									 &window, &renderer);
	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 3;
		};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Szablon do zdania drugiego 2017");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT+POLE_DODATKOWE, 32,
								  0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
							   SDL_TEXTUREACCESS_STREAMING,
							   SCREEN_WIDTH, SCREEN_HEIGHT+POLE_DODATKOWE/2);


	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("../template-project-2/cs8x8.bmp");
	if(charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 4;
		};
	SDL_SetColorKey(charset, true, 0x000000);

	/*eti = SDL_LoadBMP("../template-project-2/eti.bmp");
	if(eti == NULL) {
		printf("SDL_LoadBMP(eti.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 5;
		};*/

	for (int i = 0; i < ilosc_zyc; i++) {
		zycia[i].bitmapa = SDL_LoadBMP("../template-project-2/mala_zaba.bmp");
		zycia[i].y = SCREEN_HEIGHT + 13;
		if (i == 0) zycia[i].x = 66;
		else zycia[i].x = zycia[i - 1].x + 18;
	}


	for (int i = 0; i < ILOSC_SAMOCHODOW; i++)
		autobus[i].bitmapa = SDL_LoadBMP("../template-project-2/autobus.bmp");

	plansza = SDL_LoadBMP("../template-project-2/plansza.bmp");
	zaba.bitmapa = SDL_LoadBMP("../template-project-2/zaba.bmp");
	for (int i = 0; i < ILOSC_SAMOCHODOW; i++) {
		samochod_1[i].bitmapa = SDL_LoadBMP("../template-project-2/samochod.bmp");
		samochod_2[i].bitmapa = SDL_LoadBMP("../template-project-2/samochod.bmp");
		samochod_3[i].bitmapa = SDL_LoadBMP("../template-project-2/samochod.bmp");
		samochod_4[i].bitmapa = SDL_LoadBMP("../template-project-2/samochod.bmp");
	}

	for (int i = 0; i < ILOSC_3_ZOLWI; i++) {
		zolwie1[i].bitmapa = SDL_LoadBMP("../template-project-2/zolw.bmp");
		if (i == 0) zolwie1[i].x = SCREEN_WIDTH;
		else zolwie1[i].x = zolwie1[i - 1].x - 9 * WIELKOSC_OBIEKTOW;
		zolwie1[i].y = 220;
		
	}

	for (int i = 0; i < ILOSC_2_ZOLWI; i++) {
		zolwie2[i].bitmapa = SDL_LoadBMP("../template-project-2/zolw.bmp");
		if (i == 0) zolwie2[i].x = SCREEN_WIDTH-WIELKOSC_OBIEKTOW;
		else zolwie2[i].x = zolwie2[i - 1].x - 9 * WIELKOSC_OBIEKTOW;
		zolwie2[i].y = 115;
	}

	for (int i = 0; i < ILOSC_MALYCH_KLOD; i++) {
		male_klody[i].bitmapa = SDL_LoadBMP("../template-project-2/krotka_kloda.bmp");
		if (i == 0) male_klody[i].x = 0;
		else male_klody[i].x = male_klody[i - 1].x + 9 * WIELKOSC_OBIEKTOW;
		male_klody[i].y = 185;
	}

	for (int i = 0; i < ILOSC_SREDNICH_KLOD; i++) {
		srednie_klody[i].bitmapa = SDL_LoadBMP("../template-project-2/srednia_kloda.bmp");
		if (i == 0) srednie_klody[i].x = 0;
		else srednie_klody[i].x = srednie_klody[i - 1].x + 8* WIELKOSC_OBIEKTOW;
		srednie_klody[i].y = 80;
	}

	for (int i = 0; i < ILOSC_DUZYCH_KLOD; i++) {
		duze_klody[i].bitmapa = SDL_LoadBMP("../template-project-2/dluga_kloda.bmp");
		if (i == 0) duze_klody[i].x = 0;
		else duze_klody[i].x = duze_klody[i - 1].x + 9 * WIELKOSC_OBIEKTOW;
		duze_klody[i].y = 150;
	}

	for (int i = 0; i < 5; i++) {
		odwrocone_zaby[i].bitmapa = SDL_LoadBMP("../template-project-2/odwr_zaba.bmp");
		odwrocone_zaby[i].y = 44;
	}
	odwrocone_zaby[0].x = 40;
	odwrocone_zaby[1].x = 180;
	odwrocone_zaby[2].x = 320;
	odwrocone_zaby[3].x = 460;
	odwrocone_zaby[4].x = 600;

	zaba.x = SCREEN_WIDTH/2 ;
	zaba.y = SCREEN_HEIGHT-WIELKOSC_OBIEKTOW/2 /*254*/;

	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	distance = 0;
	etiSpeed = 100;
	for (int i = 0; i < ILOSC_SAMOCHODOW; i++) {
		if (i == 0) autobus[i].x = SCREEN_WIDTH;
		else {
			autobus[i].x = autobus[i-1].x-8*WIELKOSC_OBIEKTOW;
		}
		autobus[i].y = 290;
	}
	for (int i = 0; i < ILOSC_SAMOCHODOW; i++) {
		if (i == 0) {
			samochod_2[i].x = SCREEN_WIDTH;
			samochod_1[i].x = 0;
			samochod_3[i].x = 2*WIELKOSC_OBIEKTOW;
			samochod_4[i].x = SCREEN_WIDTH;
		}
		else {
			samochod_1[i].x = samochod_1[i-1].x+4*WIELKOSC_OBIEKTOW;
			samochod_3[i].x = samochod_3[i-1].x +4*WIELKOSC_OBIEKTOW;
			samochod_2[i].x = samochod_2[i-1].x -4*WIELKOSC_OBIEKTOW;
			samochod_4[i].x = samochod_4[i-1].x -4*WIELKOSC_OBIEKTOW;
		}
		
		samochod_1[i].y = 435;
		samochod_3[i].y = 360;
		samochod_2[i].y = 395;
		samochod_4[i].y = 325;
	}

	int flagi[5];
	for (int i = 0; i < 5; i++)
		flagi[i] = 0;




	while(!quit) {
		/////////////////////////////////////////////////KOLIZJE////////////////////////////////////////////////////
		if (zaba.y == 429) {
			for (int i = 0; i < ILOSC_SAMOCHODOW; i++) {
				if (kolizja_s(zaba.x, samochod_1[i].x+distance, WIELKOSC_OBIEKTOW) == true) {
					zaba.x = SCREEN_WIDTH / 2;
					zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
					zycia[ilosc_zyc - 1].bitmapa = SDL_LoadBMP("../template-project-2/czarne_pole.bmp");
					ilosc_zyc--;
					break;
				}
			}
		}
		else if (zaba.y == 394) {
			for (int i = 0; i < ILOSC_SAMOCHODOW; i++) {
				if (kolizja_s(zaba.x, samochod_2[i].x - distance, WIELKOSC_OBIEKTOW) == true) {
					zaba.x = SCREEN_WIDTH / 2;
					zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
					zycia[ilosc_zyc - 1].bitmapa = SDL_LoadBMP("../template-project-2/czarne_pole.bmp");
					ilosc_zyc--;
					break;
				}
			}
		}

		else if(zaba.y==359) {
			for (int i = 0; i < ILOSC_SAMOCHODOW; i++) {
				if (kolizja_s(zaba.x, samochod_3[i].x + distance, WIELKOSC_OBIEKTOW) == true) {
					zaba.x = SCREEN_WIDTH / 2;
					zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
					zycia[ilosc_zyc - 1].bitmapa = SDL_LoadBMP("../template-project-2/czarne_pole.bmp");
					ilosc_zyc--;
					break;
				}
			}
		}

		else if (zaba.y == 324) {
			for (int i = 0; i < ILOSC_SAMOCHODOW; i++) {
				if (kolizja_s(zaba.x, samochod_4[i].x - distance, WIELKOSC_OBIEKTOW) == true) {
					zaba.x = SCREEN_WIDTH / 2;
					zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
					zycia[ilosc_zyc - 1].bitmapa = SDL_LoadBMP("../template-project-2/czarne_pole.bmp");
					ilosc_zyc--;
					break;
				}
			}
		}

		else if (zaba.y == 289) {
			for (int i = 0; i < ILOSC_SAMOCHODOW; i++) {
				if (kolizja_s(zaba.x, autobus[i].x + distance, 2*WIELKOSC_OBIEKTOW) == true) {
					zaba.x = SCREEN_WIDTH / 2;
					zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
					zycia[ilosc_zyc - 1].bitmapa = SDL_LoadBMP("../template-project-2/czarne_pole.bmp");
					ilosc_zyc--;
					break;
				}
			}
		}

		else if (zaba.y == 219) {
			if (kolizja_3_zolwi(&zaba.x, WIELKOSC_OBIEKTOW, zolwie1, distance));
			else {
				zaba.x = SCREEN_WIDTH / 2;
				zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
				zycia[ilosc_zyc - 1].bitmapa = SDL_LoadBMP("../template-project-2/czarne_pole.bmp");
				ilosc_zyc--;
			}
		}

		else if (zaba.y == 184) {
			if (kolizja_krotkie_klody(&zaba.x, WIELKOSC_OBIEKTOW, male_klody, distance, ILOSC_MALYCH_KLOD));
			else {
				zaba.x = SCREEN_WIDTH / 2;
				zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
				zycia[ilosc_zyc - 1].bitmapa = SDL_LoadBMP("../template-project-2/czarne_pole.bmp");
				ilosc_zyc--;
			}
		}

		else if (zaba.y == 149) {
			if (kolizja_dlugie_klody(&zaba.x, WIELKOSC_OBIEKTOW, duze_klody, distance, ILOSC_DUZYCH_KLOD));
			else {
				zaba.x = SCREEN_WIDTH / 2;
				zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
				zycia[ilosc_zyc - 1].bitmapa = SDL_LoadBMP("../template-project-2/czarne_pole.bmp");
				ilosc_zyc--;
			}
		}

		else if (zaba.y == 114) {
			if (kolizja_2_zolwi(&zaba.x, WIELKOSC_OBIEKTOW, zolwie2, distance));
			else {
				zaba.x = SCREEN_WIDTH / 2;
				zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
				zycia[ilosc_zyc - 1].bitmapa = SDL_LoadBMP("../template-project-2/czarne_pole.bmp");
				ilosc_zyc--;
			}
		}

		else if (zaba.y == 79) {
			if (kolizja_srednie_klody(&zaba.x, WIELKOSC_OBIEKTOW, srednie_klody, distance, ILOSC_SREDNICH_KLOD));
			else {
				zaba.x = SCREEN_WIDTH / 2;
				zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
				zycia[ilosc_zyc - 1].bitmapa = SDL_LoadBMP("../template-project-2/czarne_pole.bmp");
				ilosc_zyc--;
			}
		}

		if (zaba.x<0 || zaba.x>SCREEN_WIDTH || time<=0) {
			zaba.x = SCREEN_WIDTH / 2;
			zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
			zycia[ilosc_zyc - 1].bitmapa = SDL_LoadBMP("../template-project-2/czarne_pole.bmp");
			ilosc_zyc--;
			//time = 40;
			//break;
		}

		//////WYGRANA?/////////
		else if (zaba.y == 44) {
			if ((zaba.x + 16 >= 40 && zaba.x - 16 <= 40) &&flagi[0] == 0) {
				flagi[0] = 1;
				zaba.x = SCREEN_WIDTH / 2;
				zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
				score += 50 + time * 10;
				time = 40;
				najwyzsza_pozycja = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;

			}
			else if ((zaba.x + 16 >= 180 && zaba.x - 16 <= 180) && flagi[1] == 0) {
				flagi[1] = 1;
				zaba.x = SCREEN_WIDTH / 2;
				zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
				score += 50 + time * 10;
				time = 40;
				najwyzsza_pozycja = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
			}
			else if ((zaba.x + 16 >= 320 && zaba.x - 16 <= 320) && flagi[2] == 0) {
				flagi[2] = 1; zaba.x = SCREEN_WIDTH / 2;
				zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
				score += 50 + time * 10;
				time = 40;
				najwyzsza_pozycja = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
			}
			else if ((zaba.x + 16 >= 460 && zaba.x - 16 <= 460 )&& flagi[3] == 0) {
				flagi[3] = 1;
				zaba.x = SCREEN_WIDTH / 2;
				zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
				score += 50 + time * 10;
				time = 40;
				najwyzsza_pozycja = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
			}
			else if ((zaba.x + 16 >= 600 && zaba.x - 16 <= 600) && flagi[4] == 0) {
				flagi[4] = 1;
				zaba.x = SCREEN_WIDTH / 2;
				zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
				score += 50 + time * 10;
				time = 40;
				najwyzsza_pozycja = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
			}
			else {
				zaba.x = SCREEN_WIDTH / 2;
				zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
				zycia[ilosc_zyc - 1].bitmapa = SDL_LoadBMP("../template-project-2/czarne_pole.bmp");
				ilosc_zyc--;
		
			}
		}


		DrawString(screen, 10, SCREEN_HEIGHT + 10, "zycia:", charset);
		for (int i = 0; i < 5; i++) {
			DrawSurface(screen, zycia[i].bitmapa, zycia[i].x, zycia[i].y);
		}
		




		
		t2 = SDL_GetTicks();

		// w tym momencie t2-t1 to czas w milisekundach,
		// jaki uplyna³ od ostatniego narysowania ekranu
		// delta to ten sam czas w sekundach
		delta = (t2 - t1) * 0.001;
		t1 = t2;

		worldTime += delta;

		distance += etiSpeed * delta;







		if(pause == false && menu==false) time -= delta;

		if(distance == 2*SCREEN_WIDTH)

		SDL_FillRect(screen, NULL, czarny);

		DrawSurface(screen, plansza, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
		
		for (int i = 0; i < ILOSC_SAMOCHODOW; i++)
		{
			if (samochod_1[i].x + distance >= 2 * SCREEN_WIDTH) samochod_1[i].x = -distance;
			if (samochod_3[i].x + distance >= 2 * SCREEN_WIDTH) samochod_3[i].x = -distance;
			if (samochod_2[i].x - distance <= -SCREEN_WIDTH) samochod_2[i].x = distance+SCREEN_WIDTH;
			if (samochod_4[i].x - distance <= -SCREEN_WIDTH) samochod_4[i].x = distance+SCREEN_WIDTH;
			if (autobus[i].x + distance >=  2*SCREEN_WIDTH) autobus[i].x = - distance;
			DrawSurface(screen, samochod_1[i].bitmapa, samochod_1[i].x+distance, samochod_1[i].y);
			DrawSurface(screen, samochod_3[i].bitmapa, samochod_3[i].x+distance, samochod_3[i].y);
			DrawSurface(screen, samochod_2[i].bitmapa, samochod_2[i].x-distance, samochod_2[i].y);
			DrawSurface(screen, samochod_4[i].bitmapa, samochod_4[i].x-distance, samochod_4[i].y);
			DrawSurface(screen, autobus[i].bitmapa, autobus[i].x + distance, autobus[i].y);
		}

		for (int i = 0; i < ILOSC_3_ZOLWI; i++) {
			if ((zolwie1[i].x - distance) <= (-SCREEN_WIDTH)) zolwie1[i].x = (SCREEN_WIDTH +WIELKOSC_OBIEKTOW + distance);
			
				DrawSurface(screen, zolwie1[i].bitmapa, zolwie1[i].x - distance, zolwie1[i].y);
				DrawSurface(screen, zolwie1[i].bitmapa, zolwie1[i].x+(WIELKOSC_OBIEKTOW) - distance, zolwie1[i].y);
				DrawSurface(screen, zolwie1[i].bitmapa, zolwie1[i].x-WIELKOSC_OBIEKTOW - distance, zolwie1[i].y);
		}

		for (int i = 0; i < ILOSC_2_ZOLWI; i++) {
			if ((zolwie2[i].x - distance) <= (-SCREEN_WIDTH)) zolwie2[i].x = (SCREEN_WIDTH + WIELKOSC_OBIEKTOW + distance);
			DrawSurface(screen, zolwie2[i].bitmapa, zolwie2[i].x - distance, zolwie2[i].y);
			DrawSurface(screen, zolwie2[i].bitmapa, zolwie2[i].x - distance-WIELKOSC_OBIEKTOW, zolwie2[i].y);
		}


		for (int i = 0; i < ILOSC_MALYCH_KLOD; i++) {
			if (male_klody[i].x + distance >= 2 * SCREEN_WIDTH) male_klody[i].x = -SCREEN_WIDTH-distance ;
			DrawSurface(screen, male_klody[i].bitmapa, male_klody[i].x+distance, male_klody[i].y);
		}

		for (int i = 0; i < ILOSC_SREDNICH_KLOD; i++) {
			if (srednie_klody[i].x + distance >= 2 * SCREEN_WIDTH) srednie_klody[i].x = -SCREEN_WIDTH - distance;
			DrawSurface(screen, srednie_klody[i].bitmapa, srednie_klody[i].x+distance, srednie_klody[i].y);
		}



		for (int i = 0; i < ILOSC_DUZYCH_KLOD; i++) {
			if (duze_klody[i].x + distance >= 2 * SCREEN_WIDTH) duze_klody[i].x = -SCREEN_WIDTH - distance;
			DrawSurface(screen, duze_klody[i].bitmapa, duze_klody[i].x+distance, duze_klody[i].y);
		}

		DrawSurface(screen, zaba.bitmapa, zaba.x, zaba.y);

		for (int i = 0; i < 5; i++) {
			if (flagi[i] > 0) DrawSurface(screen, odwrocone_zaby[i].bitmapa, odwrocone_zaby[i].x, odwrocone_zaby[i].y);
		}
		/*DrawSurface(screen, eti,
					SCREEN_WIDTH / 2 + sin(distance) * SCREEN_HEIGHT / 3,
				SCREEN_HEIGHT / 2 + cos(distance) * SCREEN_HEIGHT / 3);*/

		fpsTimer += delta;
		if(fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};


		if (ilosc_zyc <= 0) {
			DrawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, czarny, czarny);
			//game_over = true;
			//SDL_sscanf("%c", name);
			DrawString(screen, SCREEN_WIDTH / 2 - strlen(name) * 8 / 2, SCREEN_HEIGHT / 2 + 100, name, charset);
			if(f==0) wpisywanie = true;
			//score = 0;
			//break;
		}

		if (game_over) {
			if (!wpisywanie) {
				DrawString(screen, SCREEN_WIDTH / 2 - strlen("GAME OVER") * 8 / 2, SCREEN_HEIGHT / 2, "GAME OVER", charset);
				DrawString(screen, SCREEN_WIDTH / 2 - strlen("CHCESZ WYJSC Z GRY?(Y/N)") * 8 / 2, SCREEN_HEIGHT / 2 + 50, "CHCESZ WYJSC Z GRY(Y/N)?", charset);
			}
		}

		if (menu == true) {		
				
				if (!score_tab) {
					DrawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, czarny, czarny);
					DrawString(screen, SCREEN_WIDTH / 2 - strlen("Aby rozpoczac nacisnij spacje") * 8 / 2, SCREEN_HEIGHT / 2, "Aby rozpoczac nacisnij spacje", charset);
					DrawString(screen, SCREEN_WIDTH / 2 - strlen("Aby zobaczyc najlepsze wyniki nacisnij H") * 8 / 2, SCREEN_HEIGHT / 2 + 15, "Aby zobaczyc najlepsze wyniki nacisnij H", charset);
				}
		}

		if(time>10)
			DrawRectangle(screen, SCREEN_WIDTH - 200, SCREEN_HEIGHT+12, 2*time, 12, niebieski, zielony);
		else if(time >=0)
			DrawRectangle(screen, SCREEN_WIDTH - 200, SCREEN_HEIGHT + 12, 2*time, 12, niebieski, czerwony);



		if (pause == true) {
			DrawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, czarny, czarny);
			DrawString(screen, SCREEN_WIDTH / 2 - strlen("pauza") * 8 / 2, SCREEN_HEIGHT / 2, "pauza", charset);
		}

		if (quit_game == true) {
			DrawString(screen, SCREEN_WIDTH / 2 - strlen("Quit game? Y/N") * 8 / 2, SCREEN_HEIGHT / 2 + 20, "Quit game? Y/N", charset);
		}
		if (wpisywanie) {
			DrawString(screen, SCREEN_WIDTH / 2 - strlen("PODAJ NICK") * 8 / 2, SCREEN_HEIGHT / 2, "PODAJ NICK", charset);
		}



		if (zaba.y < najwyzsza_pozycja) {
			score += 10;
			najwyzsza_pozycja = zaba.y;
		}



		if (score_tab) {
			DrawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, czarny, czarny);
			//if(!block){
				
				for (int i = 0; i < 10; i++) {
					
					DrawString(screen, score_position_x, score_position_y[i], tab[i].name, charset);
					sprintf(show_score, "%d", tab[i].score);
					DrawString(screen, score_position_x + 50, score_position_y[i], show_score, charset);
				}
			//}
			block = true;
		}


		sprintf(scoreText, "SCORE = %.1d", score);
		DrawString(screen, 12, 8, scoreText, charset);

		//DrawString(screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT + 10,scoreText , charset);

		/*// tekst informacyjny 
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
		sprintf(text, "Szablon drugiego zadania, czas trwania = %.1lf s  %.0lf klatek / s", worldTime, fps);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		//	      "Esc - exit, \030 - faster, \031 - slower"
		sprintf(text, "Esc - wyjscie, \030 - przyspieszenie, \031 - zwolnienie");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);*/

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		// obs³uga zdarzeñ (o ile jakieœ zasz³y)
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					
					if (wpisywanie) {
						switch (event.key.keysym.sym)
						{
						case SDLK_a: name[dlugosc_l - 1] = 'a'; name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_b: name[dlugosc_l - 1] = 'b';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_c: name[dlugosc_l - 1] = 'c';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_d: name[dlugosc_l - 1] = 'd';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_e: name[dlugosc_l - 1] = 'e';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_f: name[dlugosc_l - 1] = 'f';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_g: name[dlugosc_l - 1] = 'g'; name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_h: name[dlugosc_l - 1] = 'h';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_i: name[dlugosc_l - 1] = 'i';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_j: name[dlugosc_l - 1] = 'j';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_k: name[dlugosc_l - 1] = 'k'; name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_l: name[dlugosc_l - 1] = 'l';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_m: name[dlugosc_l - 1] = 'm'; name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_n: name[dlugosc_l - 1] = 'n';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_o: name[dlugosc_l - 1] = 'o'; name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_p: name[dlugosc_l - 1] = 'p'; name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_q: name[dlugosc_l - 1] = 'q';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_r: name[dlugosc_l - 1] = 'r';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_s: name[dlugosc_l - 1] = 's';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_t: name[dlugosc_l - 1] = 't';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_u: name[dlugosc_l - 1] = 'u'; name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_v: name[dlugosc_l - 1] = 'v'; name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_w: name[dlugosc_l - 1] = 'w';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_x: name[dlugosc_l - 1] = 'x';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_y: name[dlugosc_l - 1] = 'y'; name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_z: name[dlugosc_l - 1] = 'z';  name[dlugosc_l] = '|'; dlugosc_l++; break;
						case SDLK_TAB:
							game_over = true; f = 1;
							wpisywanie = false;
							

							if (score > tab[9].score) {
								tab[9].score = score;
								for (int i = 0; i < strlen(name); i++) {
									if (i == strlen(name) - 1) tab[9].name[i] = '\0';
									else  tab[9].name[i] = name[i];
								}
							}
							
							break;

						default:
							break;
						}

					}
					
					if (!wpisywanie) {
						if (!game_over) {
							if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) && zaba.y > 16) zaba.y -= WIELKOSC_OBIEKTOW + 3;
							else if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) && zaba.y < SCREEN_HEIGHT - 16) zaba.y += WIELKOSC_OBIEKTOW + 3;
							else if ((event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) && zaba.x < SCREEN_WIDTH - 16) zaba.x += WIELKOSC_OBIEKTOW + 3;
							else if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) && zaba.x > 16) zaba.x -= WIELKOSC_OBIEKTOW + 3;
						}
						if ((event.key.keysym.sym == SDLK_SPACE && menu == true)) {
							menu = false;
							game_over = false;
							ilosc_zyc = 5;
							for (int i = 0; i < ilosc_zyc; i++)
								zycia[i].bitmapa = SDL_LoadBMP("../template-project-2/mala_zaba.bmp");
							time = 40;
							score = 0;
							najwyzsza_pozycja = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
							for (int i = 0; i < 5; i ++ )
								flagi[i] = 0;
							f = 0;
							for (int i = 0; i < 20; i++)
							{
								if (i == 0) name[i] = '|';
								else name[i] = '\0';
							}
							dlugosc_l = strlen(name);
							//name[0] = '\0';
						}
						else if ((event.key.keysym.sym == SDLK_h && score_tab ==false)&&menu) {
							score_tab = true;
							
						}
						else if ((event.key.keysym.sym == SDLK_h && score_tab == true)&&menu) {
							score_tab = false;
							block = false;
							
						}
						else if (((event.key.keysym.sym == SDLK_p && pause == false) && menu == false) && !wpisywanie) {
							etiSpeed = 0;
							pause = true;
						}
						else if (event.key.keysym.sym == SDLK_p && pause == true) {
							etiSpeed = 100;
							pause = false;
						}
						else if (event.key.keysym.sym == SDLK_q && quit_game == false) {
							quit_game = true;
							game_over = false;
						}
						else if (event.key.keysym.sym == SDLK_y && quit_game == true) {
							pause = false;
							quit_game = false;
							menu = true;
							zaba.x = SCREEN_WIDTH / 2;
							zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
							etiSpeed = 100;
							time = 40;
						}
						else if ((event.key.keysym.sym == SDLK_n && quit_game == true)) {
							;
							quit_game = false;
							game_over = false;
						}
						else if ((event.key.keysym.sym == SDLK_n && game_over == true)) {
							/*pause = false;
							quit_game = false;
							menu = true;
							zaba.x = SCREEN_WIDTH / 2;
							zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
							etiSpeed = 100;
							time = 40;*/
							menu = true;
							
						}
						else if (event.key.keysym.sym == SDLK_y && game_over == true) {
							/*pause = false;
							quit_game = false;
							menu = true;
							zaba.x = SCREEN_WIDTH / 2;
							zaba.y = SCREEN_HEIGHT - WIELKOSC_OBIEKTOW / 2;
							etiSpeed = 100;
							time = 40;*/
							quit = 1;
						}
					}
					break;
				case SDL_QUIT:
					quit = 1;
					break;
				};
			};
		frames++;
		};

	// zwolnienie powierzchni
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	for (int i = 0; i < 5; i++) {
		SDL_FreeSurface(zycia[i].bitmapa);
	}

	SDL_Quit();
	qsort(tab, 10, sizeof(high_score), my_compare);
	for (int i = 0; i < 10; i++) {
		//for (int j = 0; j < strlen(tab[i].name); i++)
		fprintf(plik, "%s", &tab[i].name);
		fprintf(plik, "%c", '\t');
		fprintf(plik, "%d", tab[i].score);
		fprintf(plik, "%c", '\n');
	}

	return 0;
	};
