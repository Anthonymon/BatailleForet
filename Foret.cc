using namespace std;
#include <iostream>
#include "Foret.h"

using namespace sf;

Foret::Foret() {
    this->Centre=new Point(TAILLE_FORET/2,TAILLE_FORET/2);
    for (int i = 0; i<NB_CASES; i++){
        for (int j = 0; j<NB_CASES; j++){
            T[i][j]=0;
        }
    }
}

Foret::~Foret()  {/*cout << "liste detruite" << endl;*/}

bool Foret::collision(Obstacle& O) {
    for (Obstacle o : this->l){
        // double l; int dx, dy; // l = distance entre les deux obstacles
        // dx = o.getCentre()->getx() - O.getCentre()->getx();
        // dy = o.getCentre()->gety() - O.getCentre()->gety();
        // l = sqrt(dx*dx + dy*dy);
        // double l2 = (O.getDiametre()/2.0) + (o.getDiametre()/2.0);
        // if (l < l2) return true;
        if ((o.getCentre()->getx()==O.getCentre()->getx())&&(o.getCentre()->gety()==O.getCentre()->gety())) return true;
    }
    return false;
}

void Foret::ajoute(Obstacle& O) {
    if(this->collision(O)) cout << "collision, creation de l'obstacle impossible" << endl;
    else {
        this->l.push_back(O);
    }
}

void Foret::afficher(){
    for(Obstacle o : this->l) {
        o.print();
    }
}

void Foret::supprime(int x, int y) {
    this->l.remove_if([x, y](Obstacle o){return ((o.getCentre()->getx()==x) && (o.getCentre()->gety()==y));});
}

void Foret::editerNiveau() {
    int i = 0;
    string s;
    cout << "Tapez c pour creer un obstacle, (ou s pour supprimer) des obstacles, q sinon ";
    cin >> s;
    while(s != "q") {
        if(s == "c") { // bloc creation d'un obstacle
        Obstacle* o;
            int h, x, y, PV;
            string nom;
            cout << "Quel obstacle voulez-vous creer? {Arbre, Rocher, Buisson, Lac} ";
            cin >> nom;
            cout << "Quelle est sa hauteur? ";
            cin >> h;
            cout << "Quelles sont ses coordonnees?" << endl << "x = ";
            cin >> x;
            x=(x/TAILLE_CASE)*TAILLE_CASE;
            cout << "y = ";
            cin >> y;
            y=(y/TAILLE_CASE)*TAILLE_CASE;
            cout << "Quel est son nombre de PV? ";
            cin >> PV;
            if(nom == "Arbre") {
                o = new Arbre(h, x, y, PV);
                this->ajoute(*o);
            }
            else if(nom == "Rocher") {
                o = new Rocher(h, x, y, PV);
                this->ajoute(*o);
            }
            else if(nom == "Buisson") {
                o = new Buisson(h, x, y, PV);
                this->ajoute(*o);
            }
            else if(nom == "Lac") {
                o = new Lac(h, x, y, PV);
                this->ajoute(*o);
            }
            else cout << "Cet obstacle n'existe pas" << endl;
        }
        if(s == "s") { // bloc de suppression d'un obstacle
            this->afficher();
            int x, y;
            cout << "Donnez les coordonnees de l'obstacle a supprimer" << endl << "x = ";
            cin >> x;
            cout << "y = ";
            cin >> y;
            this->supprime(x, y);
        }
        cout << "Tapez c pour continuer a creer des obstacles, s pour en supprimer un, q sinon ";
        cin >> s;
        cout << endl;
    }
    this->afficher();
    string fichier;
    cout << "Donner un nom pour le fichier de sauvegarde (finir par .txt) : ";
    cin >> fichier;
    this->sauvegarde(fichier);
    this->remplirTableau();
}

void Foret::remplirTableau(){
    for(Obstacle o : this->l) {
        if (o.getNom()=="Arbre"){
            T[o.getCentre()->getx()/TAILLE_CASE][o.getCentre()->gety()/TAILLE_CASE]=1;
        }
        else if (o.getNom()=="Buisson"){
            T[o.getCentre()->getx()/TAILLE_CASE][o.getCentre()->gety()/TAILLE_CASE]=2;
        }
        else if (o.getNom()=="Rocher"){
            T[o.getCentre()->getx()/TAILLE_CASE][o.getCentre()->gety()/TAILLE_CASE]=3;
        }
        else if (o.getNom()=="Lac"){
            T[o.getCentre()->getx()/TAILLE_CASE][o.getCentre()->gety()/TAILLE_CASE]=4;
        }

    }
    for (int i = 0; i<NB_CASES; i++){
        for (int j = 0; j<NB_CASES; j++){
            cout << T[i][j];
        }
        cout << endl;
    }


}

void Foret::sauvegarde(string nom) {
    ofstream ofs(nom);
    boost::archive::text_oarchive oa(ofs);
    oa << *this;
}

void Foret::lecture(string nom) {
    ifstream ifs(nom);
    boost::archive::text_iarchive ia(ifs);
    ia >> *this;
}

void Foret::Edition_Nouvelle_Foret(){
    RenderWindow window(VideoMode(TAILLE_FORET,TAILLE_FORET),
     "Edition Nouvelle Foret");

    vector<RectangleShape> v;
    VertexArray lines(Lines,4*NB_CASES);
    for (int i = 0; i<NB_CASES; i++){
        lines[2*i].position = sf::Vector2f(i*TAILLE_CASE, 0);
        lines[2*i+1].position = sf::Vector2f(i*TAILLE_CASE, TAILLE_FORET);
        lines[2*i].color = sf::Color::White;
        lines[2*i+1].color = sf::Color::White;
    }
    for (int i = 0; i <NB_CASES; i++){
        lines[2*NB_CASES+2*i].position = sf::Vector2f(0,i*TAILLE_CASE);
        lines[2*NB_CASES+2*i+1].position = sf::Vector2f(TAILLE_FORET,i*TAILLE_CASE);
        lines[2*NB_CASES+2*i].color = sf::Color::White;
        lines[2*NB_CASES+2*i+1].color = sf::Color::White;
    }
    RectangleShape fond(Vector2f(TAILLE_FORET,TAILLE_FORET));
    fond.setOrigin(TAILLE_FORET/2.f,TAILLE_FORET/2.f);
    fond.setPosition(TAILLE_FORET/2.f,TAILLE_FORET/2.f);
    Texture textureF;
    textureF.loadFromFile("foret4.png");
    fond.setTexture(&textureF);
    textureF.setSmooth(true);
    Texture arbre;
    arbre.loadFromFile("./sans_fond/arbre.png");
    //joueur.setTexture(&joueurH);
    arbre.setSmooth(true);
    Texture buisson;
    buisson.loadFromFile("./sans_fond/buisson.png");
    buisson.setSmooth(true);
    Texture rocher;
    rocher.loadFromFile("./sans_fond/rocher.png");
    rocher.setSmooth(true);
    Texture lac;
    lac.loadFromFile("./sans_fond/lac.png");
    lac.setSmooth(true);
    while(window.isOpen()){
        Event event;
        while (window.pollEvent(event)){
            switch (event.type) {
                case Event::Closed:
                    Obstacle* o;
                    for(int i = 0; i<NB_CASES; i++){
                        for(int j = 0; j<NB_CASES; j++){
                            switch (this->T[i][j]) {
                                case 1:
                                o = new Arbre(5, i*TAILLE_CASE, j*TAILLE_CASE, 20);
                                this->ajoute(*o);
                                break;
                                case 2:
                                o = new Buisson(5, i*TAILLE_CASE, j*TAILLE_CASE, 20);
                                this->ajoute(*o);
                                break;
                                case 3:
                                o = new Rocher(5, i*TAILLE_CASE, j*TAILLE_CASE, 20);
                                this->ajoute(*o);
                                break;
                                case 4:
                                o = new Lac(5, i*TAILLE_CASE, j*TAILLE_CASE, 20);
                                this->ajoute(*o);
                                break;
                                default:
                                //case vide
                                break;
                            }
                        }
                    }
                    if (!(this->l.empty())) {
                        string fichier;
                        //this->afficher();
                        cin >> fichier;
                        if (!cin) window.close();
                        this->sauvegarde(fichier);
                    }
                    window.close();
                    break;
                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left) {
                        bool existe = false;
                    //sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    //cout << localPosition.x << " "<< localPosition.y << endl;
                        for(int i = 0; i<v.size();i++){
                            if (v[i].getPosition()==
                            Vector2f(event.mouseButton.x/TAILLE_CASE*TAILLE_CASE,
                                event.mouseButton.y/TAILLE_CASE*TAILLE_CASE)){
                                    Vector2f pos = v[i].getPosition();
                                    this->T[(int)pos.y/TAILLE_CASE][(int)pos.x/TAILLE_CASE]++;
                                    switch (T[(int)pos.y/TAILLE_CASE][(int)pos.x/TAILLE_CASE]) {
                                        case 1:
                                        v[i].setTexture(&arbre);
                                        break;
                                        case 2:
                                        v[i].setTexture(&buisson);
                                        break;
                                        case 3:
                                        v[i].setTexture(&rocher);
                                        break;
                                        case 4:
                                        v[i].setTexture(&lac);
                                        break;
                                        default:
                                        T[(int)pos.y/TAILLE_CASE][(int)pos.x/TAILLE_CASE]=1;
                                        v[i].setTexture(&arbre);
                                        break;
                                    }
                                    existe=true;
                                }
                            }
                            if (!existe){
                                RectangleShape r2(Vector2f(TAILLE_CASE,TAILLE_CASE));
                                r2.setPosition((event.mouseButton.x/TAILLE_CASE)*TAILLE_CASE,(event.mouseButton.y/TAILLE_CASE)*TAILLE_CASE);
                                r2.setTexture(&arbre);
                                v.push_back(r2);
                                this->T[event.mouseButton.y/TAILLE_CASE][event.mouseButton.x/TAILLE_CASE]=1;
                            }

                        }
                    if (event.mouseButton.button == Mouse::Middle){
                        v.clear();
                        for (int i = 0; i<NB_CASES; i++){
                            for (int j = 0; j<NB_CASES; j++){
                                T[i][j]=0;
                            }
                        }
                    }
                    if (event.mouseButton.button == Mouse::Right) {
                            for(int i = 0; i<v.size();i++){
                                if (v[i].getPosition()==
                                Vector2f(event.mouseButton.x/TAILLE_CASE*TAILLE_CASE,
                                    event.mouseButton.y/TAILLE_CASE*TAILLE_CASE)){
                                        Vector2f pos = v[i].getPosition();
                                        this->T[(int)pos.y/TAILLE_CASE][(int)pos.x/TAILLE_CASE]=0;
                                        v.erase(v.begin()+i);
                                        break;
                                    }
                                }
                            }
                    break;
            }
        }
        window.draw(fond);
        for(auto o : v) window.draw(o);
        window.draw(lines);
        window.display();
        window.clear();
    }
}

void Foret::Edition_Foret_Existante(){
    RenderWindow window(VideoMode(TAILLE_FORET,TAILLE_FORET),
     "Edition Foret Existante");
    string fichier;
    cin >> fichier;
    this->lecture(fichier);
    this->l.clear();
    vector<RectangleShape> v;
    RectangleShape fond(Vector2f(TAILLE_FORET,TAILLE_FORET));
    fond.setOrigin(TAILLE_FORET/2.f,TAILLE_FORET/2.f);
    fond.setPosition(TAILLE_FORET/2.f,TAILLE_FORET/2.f);
    Texture textureF;
    textureF.loadFromFile("foret4.png");
    fond.setTexture(&textureF);
    textureF.setSmooth(true);
    Texture arbre;
    arbre.loadFromFile("./sans_fond/arbre.png");
    arbre.setSmooth(true);
    Texture buisson;
    buisson.loadFromFile("./sans_fond/buisson.png");
    buisson.setSmooth(true);
    Texture rocher;
    rocher.loadFromFile("./sans_fond/rocher.png");
    rocher.setSmooth(true);
    Texture lac;
    lac.loadFromFile("./sans_fond/lac.png");
    lac.setSmooth(true);
    VertexArray lines(Lines,4*NB_CASES);
    for (int i = 0; i<NB_CASES; i++){
        lines[2*i].position = sf::Vector2f(i*TAILLE_CASE, 0);
        lines[2*i+1].position = sf::Vector2f(i*TAILLE_CASE, TAILLE_FORET);
        lines[2*i].color = sf::Color::White;
        lines[2*i+1].color = sf::Color::White;
    }
    for (int i = 0; i <NB_CASES; i++){
        lines[2*NB_CASES+2*i].position = sf::Vector2f(0,i*TAILLE_CASE);
        lines[2*NB_CASES+2*i+1].position = sf::Vector2f(TAILLE_FORET,i*TAILLE_CASE);
        lines[2*NB_CASES+2*i].color = sf::Color::White;
        lines[2*NB_CASES+2*i+1].color = sf::Color::White;
    }
    //charger le terrain
    RectangleShape r2(Vector2f(TAILLE_CASE,TAILLE_CASE));
    for(int i = 0; i<NB_CASES; i++){
        for(int j = 0; j<NB_CASES; j++){
            switch (this->T[i][j]) {
                case 1:
                r2.setPosition(i*TAILLE_CASE, j*TAILLE_CASE);
                r2.setTexture(&arbre);
                v.push_back(r2);
                break;
                case 2:
                //RectangleShape r2(Vector2f(TAILLE_CASE,TAILLE_CASE));
                r2.setPosition(i*TAILLE_CASE, j*TAILLE_CASE);
                r2.setTexture(&buisson);
                v.push_back(r2);
                break;
                case 3:
                //RectangleShape r2(Vector2f(TAILLE_CASE,TAILLE_CASE));
                r2.setPosition(i*TAILLE_CASE, j*TAILLE_CASE);
                r2.setTexture(&rocher);
                v.push_back(r2);
                break;
                case 4:
                //RectangleShape r2(Vector2f(TAILLE_CASE,TAILLE_CASE));
                r2.setPosition(i*TAILLE_CASE, j*TAILLE_CASE);
                r2.setTexture(&lac);
                v.push_back(r2);
                break;
                default:
                //case vide
                break;
            }
        }
    }
    while(window.isOpen()){
        Event event;
        while (window.pollEvent(event)){
            switch (event.type) {
                case Event::Closed:
                Obstacle* o;
                for(int i = 0; i<NB_CASES; i++){
                    for(int j = 0; j<NB_CASES; j++){
                        switch (this->T[i][j]) {
                            case 1:
                            o = new Arbre(5, i*TAILLE_CASE, j*TAILLE_CASE, 20);
                            this->ajoute(*o);
                            break;
                            case 2:
                            o = new Buisson(5, i*TAILLE_CASE, j*TAILLE_CASE, 20);
                            this->ajoute(*o);
                            break;
                            case 3:
                            o = new Rocher(5, i*TAILLE_CASE, j*TAILLE_CASE, 20);
                            this->ajoute(*o);
                            break;
                            case 4:
                            o = new Lac(5, i*TAILLE_CASE, j*TAILLE_CASE, 20);
                            this->ajoute(*o);
                            break;
                            case 5:
                            //Personnage
                            break;
                            default:
                            //case vide
                            break;
                        }
                    }
                }
                if (!(this->l.empty())) {
                    this->sauvegarde(fichier);
                }
                window.close();
                break;
                case Event::MouseButtonPressed:
                if (event.mouseButton.button == Mouse::Left) {
                    bool existe = false;
                    //sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    //cout << localPosition.x << " "<< localPosition.y << endl;
                            for(int i = 0; i<v.size();i++){
                                if (v[i].getPosition()==
                                Vector2f(event.mouseButton.x/TAILLE_CASE*TAILLE_CASE,
                                    event.mouseButton.y/TAILLE_CASE*TAILLE_CASE)){
                                        Vector2f pos = v[i].getPosition();
                                        this->T[(int)pos.y/TAILLE_CASE][(int)pos.x/TAILLE_CASE]++;
                                        switch (T[(int)pos.y/TAILLE_CASE][(int)pos.x/TAILLE_CASE]) {
                                            case 1:
                                            v[i].setTexture(&arbre);
                                            break;
                                            case 2:
                                            v[i].setTexture(&buisson);
                                            break;
                                            case 3:
                                            v[i].setTexture(&rocher);
                                            break;
                                            case 4:
                                            v[i].setTexture(&lac);
                                            break;
                                            default:
                                            T[(int)pos.y/TAILLE_CASE][(int)pos.x/TAILLE_CASE]=1;
                                            v[i].setTexture(&arbre);
                                            break;
                                        }
                                        existe=true;
                                    }
                                }
                                if (!existe){
                                    RectangleShape r2(Vector2f(TAILLE_CASE,TAILLE_CASE));
                                    r2.setPosition((event.mouseButton.x/TAILLE_CASE)*TAILLE_CASE,(event.mouseButton.y/TAILLE_CASE)*TAILLE_CASE);
                                    r2.setTexture(&arbre);
                                    v.push_back(r2);
                                    this->T[event.mouseButton.y/TAILLE_CASE][event.mouseButton.x/TAILLE_CASE]=1;
                                }

                        }
                        if (event.mouseButton.button == Mouse::Middle){
                            v.clear();
                            for (int i = 0; i<NB_CASES; i++){
                                for (int j = 0; j<NB_CASES; j++){
                                    T[i][j]=0;
                                }
                            }
                            // sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                            // this->T[localPosition.x/TAILLE_CASE][localPosition.y/TAILLE_CASE]=0;
                        }
                        if (event.mouseButton.button == Mouse::Right) {
                                    for(int i = 0; i<v.size();i++){
                                        if (v[i].getPosition()==
                                        Vector2f(event.mouseButton.x/TAILLE_CASE*TAILLE_CASE,
                                            event.mouseButton.y/TAILLE_CASE*TAILLE_CASE)){
                                                Vector2f pos = v[i].getPosition();
                                                this->T[(int)pos.y/TAILLE_CASE][(int)pos.x/TAILLE_CASE]=0;
                                                v.erase(v.begin()+i);
                                                break;
                                            }
                                        }

                                }
            }
        }
        window.draw(fond);
        for(auto o : v) window.draw(o);
        window.draw(lines);
        window.display();
        window.clear();
    }


}
