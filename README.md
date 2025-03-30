# 🕹️ Pong Multimode sur mbed LPC1768

## 📝 Description

Projet de jeu **Pong** développé en **C++ pour carte mbed LPC1768** avec affichage sur écran **C12832 LCD**.  
Le jeu propose **trois modes de jeu interactifs** et une interface physique utilisant **joystick**, **potentiomètres**, et un **haut-parleur**.

---

## 🚀 Fonctionnalités

- 🎮 **3 Modes de jeu** :
  - Pong Classique 🏓
  - Pong avec Obstacles 🚧
  - Pong avec 2 balles 🔥
- 📺 Affichage sur écran LCD `C12832`
- 🎚️ Contrôle des pads avec 2 potentiomètres
- 🎵 Effets sonores avec `PwmOut` (haut-parleur)
- 📌 Menu principal avec sélection via joystick (`Up`, `Down`, `Left`)
- 🧠 Gestion complète de collisions, rebonds, score, obstacles

---

## ⚙️ Matériel utilisé

- [mbed LPC1768](https://os.mbed.com/platforms/mbed-LPC1768/)

---

## 🛠️ Technologies & Bibliothèques
- [C++](https://fr.cppreference.com/w/) 
- [mbed OS 2 / mbed SDK](https://os.mbed.com/mbed-os/)


---
## 🧑‍💻 Comment compiler et flasher
1 - Ouvre le projet sur l’IDE en ligne mbed ou via Mbed CLI.

2 - Sélectionne la target LPC1768.

3 - Compile.

4 - Flash le .bin généré sur la carte mbed montée en USB.
