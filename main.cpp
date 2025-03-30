#include "mbed.h"
#include "C12832.h"


#define DO  262.0
#define SI  494.0

//écran
C12832 lcd(p5, p7, p6, p8, p11);
//joystick
DigitalIn Up(p15);     
DigitalIn Down(p12); 
DigitalIn Left(p13);
//pot
AnalogIn pot1(p19);
AnalogIn pot2(p20);


PwmOut speaker(p26);
//position balle1 et balle2
int ball_x2 = lcd.width() / 2;
int ball_y2 = (lcd.height() / 2) -2;
int ball_x = lcd.width() / 2;
int ball_y = (lcd.height() / 2) +2;
//taille de l'écran
int height = lcd.height() - 5;
int width = lcd.width() - 2;
//position des pads
int pad_width = 3;
int pad_height = 10;
float pad_gauche = height / 2 - pad_height / 2;
float pad_droite = height / 2 - pad_height / 2;
//directions des balles
int direction_x = 1;
int direction_y = 1;
int direction_x2 = -1;
int direction_y2 = -1;
//vitesse
float vitesse = 1;
float vitesse2 = 1;
//score
int scoreG = 0;
int scoreD = 0;

//tableau de position et nombre d'obstacles
int obstacle_x[2];
int obstacle_y[2];
int obstacle_width[2];
int obstacle_height[2];
int nombreObstacles = 0;

//jouer un son
void jouerSon(float frequence) {
    speaker.write(0.5);               
    speaker.period(1.0 / frequence);     
    wait(0.06);
    speaker.write(0);
}

//affiche le menu principale
void afficherMenu() {
    lcd.cls();
    lcd.locate(0, 0);
    lcd.printf("UP : Pong Classique");
    lcd.locate(0, 10);
    lcd.printf("LEFT : Pong Obstacles");
    lcd.locate(0, 20);
    lcd.printf("DOWN : Pong 2 balles");
}

//dessine la ligne du milieu
void dessinerLigneMilieu() {
    int centreX = lcd.width() / 2; 
    int segmentHauteur = 4;       
    int espace = 2;               

    for (int y = 0; y < lcd.height(); y += segmentHauteur + espace) {
        lcd.line(centreX, y, centreX, y + segmentHauteur, 1); 
    }
}

//genere les obstacles
void genererObstacles() {
    nombreObstacles = rand() % 2 + 1; 
    for (int i = 0; i < nombreObstacles; i++) {
        do {
            obstacle_x[i] = rand() % (width - 20) + 10; 
            obstacle_y[i] = rand() % (height - 10) + 5; 

            
            if (rand() % 2 == 0) {
                obstacle_width[i] = 2;  
                obstacle_height[i] = 6;
            } else {
                obstacle_width[i] = 6;  
                obstacle_height[i] = 2;
            }
        } while (
            (obstacle_x[i] > width / 2 - 15 && obstacle_x[i] < width / 2 + 15 && 
             obstacle_y[i] > height / 2 - 10 && obstacle_y[i] < height / 2 + 10) || 
            (obstacle_x[i] < 30 && obstacle_y[i] + obstacle_height[i] > pad_gauche - 10 && obstacle_y[i] < pad_gauche + pad_height + 10) || 
            (obstacle_x[i] > width - 40 && obstacle_y[i] + obstacle_height[i] > pad_droite - 10 && obstacle_y[i] < pad_droite + pad_height + 10) 
        );
    }
}

//dessine les obstacles
void dessinerObstacles() {
    for (int i = 0; i < nombreObstacles; i++) {
        lcd.fillrect(obstacle_x[i], obstacle_y[i], obstacle_x[i] + obstacle_width[i], obstacle_y[i] + obstacle_height[i], 1);
    }
}

//verifie les collisions avec les obstacles
void verifierCollisionObstacles() {
    for (int i = 0; i < nombreObstacles; i++) {
        if (ball_x + 2 >= obstacle_x[i] && ball_x - 2 <= obstacle_x[i] + obstacle_width[i] &&
            ball_y + 2 >= obstacle_y[i] && ball_y - 2 <= obstacle_y[i] + obstacle_height[i]) {
            direction_x = -direction_x;
            direction_y = -direction_y;
            jouerSon(DO);
        }
    }
}



// reset la balle
void resetBall() {
    ball_x = width / 2;
    ball_y = height / 2;
    genererObstacles(); 

    lcd.cls();
    lcd.fillcircle(ball_x, ball_y, 2, 1);
    lcd.fillrect(7, pad_gauche, pad_width + 7, pad_gauche + pad_height, 1); 
    lcd.fillrect(width - pad_width - 7, pad_droite, width - 7, pad_droite + pad_height, 1);
    wait(0.1);

    direction_x = (rand() % 2 == 0) ? 1 : -1;
    direction_y = (rand() % 2 == 0) ? 1 : -1;
    vitesse = 1;
    wait(0.1);
}

//reset les 2 balles
void resetBall2() {
    ball_x = width / 2;
    ball_y = (height / 2) + 2;
    ball_x2 = width / 2;
    ball_y2 = (height / 2) -2 ;
    genererObstacles(); 

    lcd.cls();
    lcd.fillcircle(ball_x, ball_y, 2, 1);
    lcd.fillcircle(ball_x2, ball_y2, 2, 1);
    lcd.fillrect(7, pad_gauche, pad_width + 7, pad_gauche + pad_height, 1);
    lcd.fillrect(width - pad_width - 7, pad_droite, width - 7, pad_droite + pad_height, 1);
    wait(0.1);

    direction_x = (rand() % 2 == 0) ? 1 : -1;
    direction_y = (rand() % 2 == 0) ? 1 : -1;
    direction_x2 = (rand() % 2 == 0) ? 1 : -1;
    direction_y2 = (rand() % 2 == 0) ? 1 : -1;
    vitesse = 1;
    vitesse2 = 1;
    wait(0.1);
}

// met à jour le score
void score() {
    if (ball_x <= 0) {
        scoreD += 1;
    }
    if (ball_x >= width) {
        scoreG += 1;
    }
    if (ball_x2 <= 0) {
        scoreD += 1;
    }
    if (ball_x2 >= width) {
        scoreG += 1;
    }
}

//met à jour la position des pads
void MajPads(){
        pad_gauche = pot1.read() * (height - pad_height);
        pad_droite = pot2.read() * (height - pad_height);
}

//deplace la balle, vérifie les rebondissement et ajuste la vitesse 
void DeplacerBalle(){
        ball_x += direction_x * vitesse;
        ball_y += direction_y * vitesse;
        

        
        if (ball_y <= 0 || ball_y >= height){  
             direction_y = -direction_y;
             jouerSon(DO);
        }
        
        if (ball_x <= pad_width + 7 && ball_y >= pad_gauche-5 && ball_y <= pad_gauche + pad_height +5) {
            direction_x = -direction_x;
            vitesse += 0.2;
            jouerSon(DO);
        }
        if (ball_x >= width - pad_width - 7 && ball_y >= pad_droite-5 && ball_y <= pad_droite + pad_height+5) {
            direction_x = -direction_x;
            vitesse += 0.2;
            jouerSon(DO);
        }

}

//deplace les 2 balles, vérifie les rebondissement et ajuste la vitesse 
void Deplacer2Balles(){
    
        ball_x += direction_x * vitesse;
        ball_y += direction_y * vitesse;
        ball_x2 += direction_x2 * vitesse2;
        ball_y2 += direction_y2 * vitesse2;

        
        if (ball_y <= 0 || ball_y >= height) {
            direction_y = -direction_y;
               
        }
        if (ball_y2 <= 0 || ball_y2 >= height) {
            direction_y2 = -direction_y2;
              
        }

        
        if (ball_x <= pad_width + 7 && ball_y >= pad_gauche -5 && ball_y <= pad_gauche + pad_height +5) {
            direction_x = -direction_x;
            vitesse += 0.2;
               
        }
        if (ball_x2 <= pad_width + 7 && ball_y2 >= pad_gauche -5 && ball_y2 <= pad_gauche + pad_height +5) {
            direction_x2 = -direction_x2;
            vitesse2 += 0.2;
               
        }
        if (ball_x >= width - pad_width - 7 && ball_y >= pad_droite -5 && ball_y <= pad_droite + pad_height+5) {
            direction_x = -direction_x;
            vitesse += 0.2;
               
        }
        if (ball_x2 >= width - pad_width - 7 && ball_y2 >= pad_droite -5 && ball_y2 <= pad_droite + pad_height+5) {
            direction_x2 = -direction_x2;
            vitesse2 += 0.2;
              
        }

        int distanceX = ball_x - ball_x2;
        int distanceY = ball_y - ball_y2;
        int distancecarre = distanceX * distanceX + distanceY * distanceY;
        int collision = 4; 

        if (distancecarre <= collision * collision) {
            direction_x = -direction_x;
            direction_y = -direction_y;
            direction_x2 = -direction_x2;
            direction_y2 = -direction_y2;
        }
        
}

void jeuPongClassique() {
    while (1) {
        MajPads();


        DeplacerBalle();

        
        if (ball_x <= 0 || ball_x >= width) { // verifie si la balle sort
            score();
            jouerSon(SI);
            resetBall(); 
        }

       
        lcd.cls();
        dessinerLigneMilieu();
        lcd.fillcircle(ball_x, ball_y, 2, 1); //dessine la balle
        lcd.fillrect(7, pad_gauche, pad_width + 7, pad_gauche + pad_height, 1); //dessine le pad droit
        lcd.fillrect(width - pad_width - 7, pad_droite, width - 7, pad_droite + pad_height, 1); //dessine le pad gauche
        //afficher le score
        lcd.locate(30, 0);
        lcd.printf("%d", scoreG);
        lcd.locate(90, 0);
        lcd.printf("%d", scoreD);       
        wait(0.05);
    }
}


void jeuPongObstacles() {
    while (1) {
        
        MajPads();

        DeplacerBalle();

        
        verifierCollisionObstacles();

        
        if (ball_x <= 0 || ball_x >= width) { // verifie si la balle sort
            score();
            jouerSon(SI);
            resetBall(); 
            dessinerObstacles();
        }

        
        lcd.cls();
        dessinerLigneMilieu();
        lcd.fillcircle(ball_x, ball_y, 2, 1); //dessine la balle
        lcd.fillrect(7, pad_gauche, pad_width + 7, pad_gauche + pad_height, 1); //dessine le pad droit
        lcd.fillrect(width - pad_width - 7, pad_droite, width - 7, pad_droite + pad_height, 1); //dessine le pad gauche
        dessinerObstacles(); 
        //afficher le score
        lcd.locate(30, 0);
        lcd.printf("%d", scoreG);
        lcd.locate(90, 0);
        lcd.printf("%d", scoreD);
        wait(0.05);
    }
}

void pong2balles(){
     while (1) {
         MajPads();

        Deplacer2Balles();

        
        if (ball_x <= 0 || ball_x >= width) { // verifie si la balle 1 sort
            score();
            jouerSon(SI);
            resetBall2(); 
        }
        if (ball_x2 <= 0 || ball_x2 >= width) { // verifie si la balle 2 sort
            score();
            jouerSon(SI);
            resetBall2(); 
        }

        
        lcd.cls();
        dessinerLigneMilieu();
        lcd.fillcircle(ball_x, ball_y, 2, 1); //dessine la balle 1
        lcd.fillcircle(ball_x2, ball_y2, 2, 1);//dessine la balle 2
        lcd.fillrect(7, pad_gauche, pad_width + 7, pad_gauche + pad_height, 1);//dessine le pad droit
        lcd.fillrect(width - pad_width - 7, pad_droite, width - 7, pad_droite + pad_height, 1);//dessine le pad gauche
        //afficher le score
        lcd.locate(30, 0);
        lcd.printf("%d", scoreG);
        lcd.locate(90, 0);
        lcd.printf("%d", scoreD);       
        wait(0.05);
    }
}

int main() {
    afficherMenu();
    while (1) {
        if (Up) {
            lcd.cls();
            jeuPongClassique();  // lance le jeu pong classique
        }
        if (Left){
            lcd.cls();
            jeuPongObstacles(); // lance le jeu pong avec obstacles
        }
        if (Down) {
            lcd.cls();
            pong2balles(); // lance le jeu pong avec 2 balles
        }
    }
}