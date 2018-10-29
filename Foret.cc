using namespace std;
#include <iostream>
#include "Foret.h"
#include <cmath>

Foret::Foret() {
    //this->l = new list();
}

Foret::~Foret()  {cout << "liste detruite" << endl;}

bool Foret::collision(Obstacle& O) {
    for (Obstacle o : this->l){
        double l; int dx, dy; // l = distance entre les deux obstacles
        dx = o.getCentre()->getx() - O.getCentre()->getx();
        dy = o.getCentre()->gety() - O.getCentre()->gety();
        l = sqrt(dx*dx + dy*dy);
        double l2 = (O.getDiametre()/2) + (o.getDiametre()/2);
        if (l < l2) return true;
    }
    return false;
}

void Foret::ajoute(Obstacle& O) {
    if(this->collision(O)) cout << "collision, creation de l'obstacle impossible" << endl;
    else {
        this->l.push_back(O);
    }
}

void Foret::supprime(Obstacle& O) {

}
