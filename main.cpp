#include <iostream>
#include <deque>
#include <cmath>
#include <vector>

// Structure Point
struct Points
{
    // Coordonnee du point
    int x;
    int y;
    // Contructeur d'un point vide
    Points()
    {
        x=0;
        y=0;
    }
    // Constructeur d'un point avec des valeurs en parametre
    Points(int _x, int _y)
    {
        x=_x;
        y=_y;
    }
};

// structure noeud qui va etre utilisé dans l'algorithme A*
struct Noeud
{
    int x;
    int y;
    float fCost;
    // Constructeur d'un point avec des valeurs en parametre
    Noeud(int _x, int _y)
    {
        x=_x;
        y=_y;
        fCost=0;
    }
};


float distance(Points point1, Points point2)
{
    //distance euclidienne
    return sqrt((point1.x-point2.x)*(point1.x-point2.x) + (point1.y-point2.y)*(point1.y-point2.y));
}

class TableauBidimentionnel
{
public :
    // Constructeur de la classe
    TableauBidimentionnel()
    {
        tableau.resize(10);
        for(auto it = tableau.begin(); it != tableau.end(); ++it)
        {
           (*it).resize(10);
        }
        origin.x = 0;
        origin.y = 0;
    }
    // Destruction de la classe
    ~TableauBidimentionnel()
    {
        std::cout << "Destruction du tableau bidimentionnel" << std::endl;
    }
    // Ajout d'une point
    void addPoint(Points points)
    {
        if(points.y < 0)
        {
            if(points.y < -origin.y)
            {
                for(int i=0; i<abs(points.y)-origin.y; i++)
                {
                    tableau.push_front(tableau[0]);
                }
                //changer la valeur de l'origine à y
                origin.y = origin.y + abs(points.y)-origin.y;
            }
        }
        else
        {
            if(points.y >=tableau.size())
            {
                for(int i = 0; i<tableau.size()-points.y;i++)
                {
                    tableau.push_back(tableau[0]);
                }
            }
        }
        if(points.x < 0)
        {
            if(points.x < -origin.x)
            {
                for(int i=0; i<abs(points.x)-origin.x; i++)
                {
                    std::deque<std::deque<int>>::iterator it;
                    for (it = tableau.begin(); it != tableau.end(); ++it)
                    {
                        (*it).push_front(0);
                    }
                }
                //changer la valeur de l'origine à x
                origin.x = origin.x + abs(points.x)-origin.x;
            }
        }
        else
        {
            if(points.x >=tableau.front().size())
            {
                for(int i = 0; i<tableau.front().size()-points.x;i++)
                {
                    std::deque<std::deque<int>>::iterator it;
                    for (it = tableau.begin(); it != tableau.end(); ++it)
                    {
                        (*it).resize(points.x+1);
                    }
                }
            }
        }
        // Rajoute le point dans le deque de point
        deques.push_back(points);
    }
    // Retourne le tableau
    std::deque<std::deque<int>> getTableau()
    {
        return tableau;
    }
    // Retourne le deque de points
    std::deque<Points> getDeques()
    {
        return deques;
    }
    // Afficher les coordonnees du point a l'origine
    void afficheOrigin()
    {
        std::cout << "Les coordonnees a l'origine est ["<<origin.x <<";"<<origin.y<<"]"<<std::endl;
    }
    // Affichage du tableau
    void affichage()
    {
        //va affihcer le tableau bidimentionel avec les points qui se trouve dans deques
        std::deque<std::deque<int>>::iterator it;
        for (it = tableau.begin(); it != tableau.end(); ++it)
        {
            std::deque<int>::iterator it2;
            for(it2 = (*it).begin(); it2 != (*it).end();it2++)
            {
                std::cout << *it2 << " " ;
            }
            std::cout << "" << std::endl;
        }
    }
    // Ajouter un obstacle
    void addObstacle(Points point)
    {
        if(point.y < -origin.y || point.y >=tableau.size() || point.x < -origin.x || point.x >=tableau.front().size())
        {
            std::cout<< "Le point n'est pas dans le tableau" <<std::endl;
        }
        else
        {
            tableau[point.x+origin.x][point.y+origin.y]=1;
        }

    }
    // Teste si il y a un obstacle
    bool isObstacle(int x, int y)
    {
        return tableau[x+origin.x][y+origin.y]==1;
    }
    //Recherche d'un chemin simple avec la distance de manhanttan
    void rechercheCheminSimple(Points point1, Points point2)
    {
        // Teste si le premier point est dans le tableau
        if((point1.y < -origin.y)|| (point1.y >=tableau.size()-origin.y) || (point1.x < -origin.x) ||(point1.x >= int(tableau.front().size()-origin.x)))
        {
            std::cout<< "Le premier point n'est pas dans le tableau" <<std::endl;
            std::cout<< point1.x << " " << point1.y<<std::endl;
            std::cout<< -origin.x << " " << -origin.y<<std::endl;
            std::cout<< tableau.front().size() << " " << tableau.size()<<std::endl;
            exit(1);
        }
        // teste si le deuxieme point est dans le tableau
        if(point2.y < -origin.y || point2.y >=tableau.size()-origin.y || point2.x < -origin.x || point2.x >=int(tableau.front().size()-origin.x))
        {
            std::cout<< "Le deuxieme point n'est pas dans le tableau" <<std::endl;
            exit(1);
        }
        // initilisation des coordonnees du point de debut
        int x = point1.x;
        int y = point1.y;
        // distance de manhattan
        int dist = abs(point2.x-point1.x)+abs(point2.y-point1.y);
        int i = 0;
        while(dist>0)
        {
            //Test aller a droite en regardant si il y a un obstacle et la nouvelle distance est inferieur a la nouvelle
            if(!(this->isObstacle(x+1,y)) && dist >= (abs(point2.x-(x+1))+abs(point2.y-y)))
            {
                i++;
                x=x+1;
                dist = abs(point2.x-(x+1))+abs(point2.y-point1.y);
                std::cout << i << " ->  x = " << x << " y = " << y <<std::endl;
                //std::cout << dist <<std::endl;
            }
            // Teste aller en haut en regardant si il y a un obstacle et la nouvelle distance est inferieur a la nouvelle
            if(!(this->isObstacle(x,y+1)) && dist >= (abs(point2.x-x)+abs(point2.y-(y+1))))
            {
                i++;
                y=y+1;
                dist = abs(point2.x-x)+abs(point2.y-(y+1));
                std::cout << i << " ->  x = " << x << " y = " << y <<std::endl;
                //std::cout << dist <<std::endl;
            }
            // Teste aller a gauche en regardant si il y a un obstacle et la nouvelle distance est inferieur a la nouvelle
            if(!(this->isObstacle(x-1,y)) && dist >= (abs(point2.x-(x-1))+abs(point2.y-y)))
            {
                i++;
                x=x-1;
                dist = abs(point2.x-(x-1))+abs(point2.y-y);
                std::cout << i << " ->  x = " << x << " y = " << y <<std::endl;
                //std::cout << dist <<std::endl;
            }
            // Teste aller en bas en regardant si il y a un obstacle et la nouvelle distance est inferieur a la nouvelle
            if(!(this->isObstacle(x,y-1)) && dist >= (abs(point2.x-x)+abs(point2.y-(y-1))))
            {
                i++;
                y=y-1;
                dist = abs(point2.x-x)+abs(point2.y-(y-1));
                std::cout << i << " ->  x = " << x << " y = " << y <<std::endl;
                //std::cout << dist <<std::endl;
            }
        }
    }
    // creation du tableau avec les voisins d'un noeud qui va être utiliser dans A*
    std::deque<std::deque<int>> voisins(Noeud noeud)
    {
        // directions possibles
        std::vector<std::vector<int>> dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        // creation de la variable result
        std::deque<std::deque<int>> result;
        for (auto dir : dirs)
        {
            std::deque<int> voisin = {noeud.x + dir[0], noeud.y + dir[1]};
            // Teste si les voisin font partie du tableau
            if (-origin.x <= voisin[0] && voisin[0] < int(tableau.front().size()-origin.x) && -origin.y <= voisin[1] && voisin[1] < tableau.size()-origin.y)
            {
                result.push_back(voisin);
            }
        }
        return result;
    }
    // Recherche de chemin complexe avec pondération de vitesse de progression
    // Pas finis
    void rechercheCheminComplexe(Points point1, Points point2)
    {
        std::vector<Noeud> listOuvert;
        std::vector<Noeud> listFerme;

    }
protected :
    std::deque<std::deque<int>> tableau;
    std::deque<Points> deques;
    // coordonne du point a l'origine dans le tableau car il peux avoir des points negatives
    Points origin;
};

void affichageVoisins(std::deque<std::deque<int>> tableauVoisin)
    {
        //va affihcer le tableau bidimentionel avec les points qui se trouve dans deques
        std::deque<std::deque<int>>::iterator it;
        for (it = tableauVoisin.begin(); it != tableauVoisin.end(); ++it)
        {
            std::deque<int>::iterator it2;
            for(it2 = (*it).begin(); it2 != (*it).end();it2++)
            {
                std::cout << *it2 << " " ;
            }
            std::cout << "" << std::endl;
        }
    }


int main()
{
    TableauBidimentionnel tableDim;
    std::cout << tableDim.getTableau().size() << std::endl;
    Points point1(15,10);
    Points point2(-5,1);
    tableDim.addPoint(point1);
    tableDim.afficheOrigin();
    tableDim.addPoint(point2);
    tableDim.afficheOrigin();
    Points point3(3,9);
    tableDim.addObstacle(point3);
    std::cout << "Taille du tableau en abscisse : "<<tableDim.getTableau().front().size() << std::endl;
    std::cout << "Taille du tableau en ordonnee : "<< tableDim.getTableau().size() << std::endl;
    tableDim.affichage();
    tableDim.rechercheCheminSimple(point2,point3);
    //Noeud noeud(-5,5);
    //std::deque<std::deque<int>> tableauVoisin = tableDim.voisins(noeud);
    //qaffichageVoisins(tableauVoisin);
    return 0;
}
