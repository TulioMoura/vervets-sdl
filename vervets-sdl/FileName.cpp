#define ENV_W  15
#define ENV_H  15
#define SCREEN_W 480
#define SCREEN_H 480


#define monkey_count  6
#define predator_a_count  1
#define predator_b_count  1
#define predator_c_count 1

#define raioPercepcao  2
#define raioOuvir  3

#include <iostream>
#include<fstream>
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <SDL.h>
#include <ctime>

class animal
{

public:
    static int limitx;
    static int  limity;
    int posx, posy;


    static void setLimits(int x, int y) {
        limitx = x;
        limity = y;
    }
    animal(int x, int y)
    {
    }

    animal()
    {
    }

    float distanciaAte(int x, int y)
    {
        float distancia = sqrt(pow((posx - x), 2) + pow((posy - y), 2));
        return distancia;
    }

    void moveRandom()
    {
        int oldx = posx;
        int oldy = posy;
        do
        {
            posx = posx + ((rand() % 3) - 1);
            posy = posy + ((rand() % 3) - 1);

            if (posx == limitx)
            {
                posx = limitx-1;
            }
            if (posy == limity)
            {
                posy = limity-1;
            }

            if (posx < 0)
            {
                posx = 0;
            }
            if (posy < 0)
            {
                posy = 0;
            }
        } while (posx == oldx && posy == oldy);
    }
    void moveTo(int x, int y)
    {
        if ((posx < limitx && posx > 0) && (posy < limity && posy > 0))
        {
            posx = x;
            posy = y;
            return;
        }
        else
            return;
    }

    int getPosx()
    {
        return posx;
    }
    int getPosy()
    {
        return posy;
    }

    void setPosx(int x)
    {
        posx = x;
    }

    void setPosy(int y)
    {
        posy = y;
    }
};

class alerta
{
public:
    int posx, posy;
    int sinal;
    int getPosx()
    {
        return posx;
    }

    int getPosy()
    {
        return posy;
    }

    alerta(int x, int y, int sinal)
    {
        posx = x;
        posy = y;
        this->sinal = sinal;
    }
    alerta() {
    }
};

class predador : public animal
{
public:
    int tipo;
    int getTipo() const
    {
        return tipo;
    }
    void print() {
        std::cout << "tipo: " << tipo << " , x: " << posx << " y: " << posy << "\n";
    }
    predador(int tipo)
    {
        
        posx = rand() % limitx;
        posy = rand() % limity;
        this->tipo = tipo;
    }
    predador() {

    }
};

class vervet : public animal
{
public:
    std::vector<alerta>* arrayAlertas;
    std::vector<alerta>* arrayAlertasNext;
    int simboloOuvido = -1;
    int predadoRecente = -1;
    float pesosPredadores[10][3];

    void processaAlertas()
    {
        for (int i = 0; i < arrayAlertas->size(); i++)
        {
            int xalerta = arrayAlertas->at(i).getPosx();
            int yalerta = arrayAlertas->at(i).getPosy();
            float distToAlert = distanciaAte(xalerta, yalerta);
            if (distToAlert < raioOuvir)
            {
                simboloOuvido = arrayAlertas->at(i).sinal;
                //std::cout << "Alerta detectado a: " << distToAlert << " unidades de distancia" << std::endl;
                if (predadoRecente != -1) {
                    if (getPredador(simboloOuvido) != predadoRecente) {
                        pesosPredadores[simboloOuvido][predadoRecente] -= 0.1;
                    }
                    else {
                        pesosPredadores[simboloOuvido][predadoRecente] += 0.1;
                    }
                }

            }
        }
    }

    void print() {
        std::cout << " , x: " << posx << " y: " << posy << "\n";
    }

    std::string csv_dump() {
        std::string out;

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 3; j++) {
                out = out.append(std::to_string(pesosPredadores[i][j]));
                out = out.append(",");
            }
            out = out.append("\n");
        }
        return out;
    }

    void processaPredadores(std::vector<predador>* p) {
        for (int i = 0; i < p->size(); i++) {
            vePredador(p->at(i));
        }
    }


    void vePredador(predador p)
    {
        if (distanciaAte(p.getPosx(), p.getPosy()) < raioPercepcao)
        {
            int s = getSignal(p.getTipo());
            alerta a = alerta(getPosx(), getPosy(), s);
            arrayAlertasNext->push_back(a);
            predadoRecente = p.getTipo();
            if (simboloOuvido != -1) {
                if (simboloOuvido != s) {
                    pesosPredadores[simboloOuvido][p.getTipo()] -= 0.1;
                }
                else {
                    pesosPredadores[simboloOuvido][p.getTipo()] += 0.1;
                }
            }
        }
        moveRandom();
    }

    int getSignal(int p)
    {
        int temp = 0;
        for (int i = 0; i < 10; i++)
        {
            if (pesosPredadores[i][p] > pesosPredadores[temp][p])
            {
                temp = i;
            }
        }
        return temp;
    }

    int getPredador(int s) {
        int temp = 0;
        for (int i = 0; i < 3; i++)
        {
            if (pesosPredadores[i][s] > pesosPredadores[temp][s])
            {
                temp = i;
            }
        }
        return temp;
    }

    vervet(int x, int y, std::vector<alerta>* a, std::vector<alerta>* b)
    {
        int simboloOuvido = -1;
        arrayAlertas = a;
        arrayAlertasNext = b;
        
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                float peso = (rand() % 60);
                pesosPredadores[i][j] = peso/100 ;
            }
        }
        setPosx(x);
        setPosy(y);
    }
    vervet() {

    }
};


void renderAlerta(alerta a,SDL_Renderer * renderer) {
    int centerx = a.getPosx();
    int centery = a.getPosy();
    int realInitX =(centerx - raioOuvir) * ( SCREEN_W/ENV_W );
    int  realInitY =(centery - raioOuvir)*(SCREEN_H/ENV_H);
    int endx = (centerx + raioOuvir) * (SCREEN_W / ENV_W);
    int endy = (centery + raioOuvir) * (SCREEN_H / ENV_H);

    SDL_SetRenderDrawColor(renderer, 0xf9, 0xfc, 0xae, 0x80);

   // std::cout << " " << centerx << " " << centery << " " << realInitX << " " << realInitY << " "<<endx<<" " << endy << std::endl;

    for (int i = realInitX; i < endx; i++) {
        for (int j = realInitY; j < endy; j++) {

            float distancia = sqrt(pow(((centerx* (SCREEN_W / ENV_W)) - i), 2) + pow(((centery * (SCREEN_H / ENV_H)) - j), 2));
            //std::cout << " " << distancia;
            float pxDist = ((SCREEN_H / ENV_H) * raioPercepcao);
            if (distancia < pxDist ) {
                SDL_RenderDrawPoint(renderer, i, j);
                SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
            }
        }
    }
    

}

int animal::limitx = 15;
int animal::limity = 15;



int main(int argc, char* argv[])
{
    std::srand(time(0));
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cerr << "Error failed to create window!\n";
        return 1;
    }

    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

    SDL_Event event;
    bool quit = false;

    //initialize ant array and environment;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1 , SDL_RENDERER_ACCELERATED);


    std::vector<predador> arraypredadores;
    arraypredadores.resize(predator_a_count + predator_b_count + predator_c_count);
    std::vector<vervet> arrayvervets;
    arrayvervets.resize(monkey_count);
    std::vector<alerta> arrayAlertas;
    std::vector<alerta> arrayAlertasProx;

    for (int i = 0; i < predator_a_count; i++)
    {
        arraypredadores.at(i) = predador(0);
    }

    for (int i = predator_a_count; i < predator_a_count + predator_b_count; i++)
    {
        arraypredadores.at(i) = predador(1);
    }

    for (int i = predator_a_count + predator_b_count; i < predator_a_count + predator_b_count + predator_c_count; i++)
    {
        arraypredadores.at(i) = predador(2);
    }

    for (int i = 0; i < monkey_count; i++)
    {
        int posy = rand() % ENV_H;
        int posx = rand() % ENV_W;
        arrayvervets.at(i) = vervet(posx, posy, &arrayAlertas, &arrayAlertasProx);
    }


    int x = 0;

    std::string file_contents[monkey_count];

    //start code here

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(renderer);

        SDL_Rect* pixel = new SDL_Rect();
        pixel->h = SCREEN_H;
        pixel->w = SCREEN_W;
        pixel->x = 0;
        pixel->y = 0;
        SDL_FillRect(screenSurface, pixel, SDL_MapRGBA(screenSurface->format, 0xff, 0xff, 0xff, 0xff));
        SDL_RenderFillRect(renderer, pixel);
        delete pixel;


        for (int i = 0; i < arrayAlertas.size(); i++) {
            renderAlerta(arrayAlertas[i], renderer);
        }

        for (int i = 0; i < (predator_a_count + predator_b_count + predator_c_count); i++) {
           // arraypredadores[i].print();
            arraypredadores[i].moveRandom();
            SDL_Rect * pixel = new SDL_Rect();
            pixel->h = SCREEN_H/ENV_H;
            pixel->w = SCREEN_W/ENV_W;
            pixel->x = arraypredadores[i].getPosx() * pixel->w;
            pixel->y = arraypredadores[i].getPosy() * pixel->h;

            if (arraypredadores[i].getTipo() == 0) {
                SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x34, 0xff);

            }
            else if (arraypredadores[i].getTipo()==1) {
                SDL_SetRenderDrawColor(renderer,0xeb, 0x34, 0xde, 0xff);

            }
            else if (arraypredadores[i].getTipo()==2) {
                SDL_SetRenderDrawColor(renderer, 0x34, 0xeb, 0x5e, 0xff);
            }
            SDL_RenderFillRect(renderer, pixel);
            delete pixel;

        }

        for (int i = 0; i < arrayvervets.size(); i++) {
            arrayvervets.at(i).processaAlertas();
            arrayvervets.at(i).processaPredadores(&arraypredadores);
            SDL_Rect* pixel = new SDL_Rect();
            pixel->h = SCREEN_H / ENV_H;
            pixel->w = SCREEN_W / ENV_W;
            pixel->x = arrayvervets[i].getPosx() * pixel->w;
            pixel->y = arrayvervets[i].getPosy() * pixel->h;


            SDL_SetRenderDrawColor(renderer, 0x5e, 0x4d, 0x23, 0xff);
            SDL_RenderFillRect(renderer, pixel);
            
            delete pixel;

            file_contents[i] = file_contents[i].append(arrayvervets[i].csv_dump());
            file_contents[i] = file_contents[i].append("\n");


        }
        
     
        arrayAlertas = arrayAlertasProx;
        arrayAlertasProx.clear();
        SDL_RenderPresent(renderer);
     SDL_Delay(60);

    }

    for (int i = 0; i < monkey_count; i++) {


        std::ofstream arquivo;
        std::string filename = "monkey_weights";
        filename.append(std::to_string(i));
        filename.append(".csv");
        arquivo.open(filename);
        arquivo << file_contents[i];
        arquivo.close();

        std::cout << "Sinais Vervet " << i << std::endl;

        std::cout<<arrayvervets[i].getSignal(0)<<"  Sinal do predador 0\n";
        std::cout << arrayvervets[i].getSignal(1) << "  Sinal do predador 1\n";
        std::cout << arrayvervets[i].getSignal(2) << "  Sinal do predador 2\n\n";

    }

    SDL_Quit();


    return 0;
}