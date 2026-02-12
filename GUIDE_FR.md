# Guide de Démarrage Rapide - HEM (Home Electrical Measurement)

## 🇫🇷 Guide en Français

Ce guide vous aide à démarrer rapidement avec votre projet de mesure électrique domestique.

---

## ⚠️ IMPORTANT - Sécurité Avant Tout

### Le code contient des identifiants d'exemple à remplacer !

Avant d'utiliser ce projet, vous **DEVEZ** remplacer les identifiants par défaut :

1. **Mot de passe du réseau mesh** : `"somethingSneaky"` → Utilisez un mot de passe fort
2. **SSID WiFi** : `"Freebox_BSHOQE"` → Votre réseau WiFi
3. **Mot de passe WiFi** : `"?"` → Votre mot de passe WiFi
4. **Identifiants MQTT** : username `"admin"` et mots de passe `"147258"`, `"14258"` → Vos identifiants
5. **Adresse IP du broker MQTT** : `192.168.1.42` → Votre adresse IP

### 📋 Consultez les fichiers de sécurité :
- **SECURITY.md** : Liste complète des problèmes de sécurité trouvés
- **config.h.example** : Modèle pour créer votre fichier de configuration

---

## 📖 Documentation Disponible

Ce projet contient maintenant une documentation complète :

### README.md
Documentation principale en anglais contenant :
- Description du projet
- Liste du matériel nécessaire
- Instructions d'installation détaillées
- Guide d'utilisation
- Sujets MQTT
- Structure du projet

### SECURITY.md
Rapport d'audit de sécurité complet :
- Tous les identifiants trouvés dans le code
- Emplacements précis (fichiers et lignes)
- Recommandations de sécurisation
- Checklist de sécurité

### LICENSE
Licence MIT - Ce projet est personnel et sous licence libre.

### .gitignore
Fichier de configuration Git pour :
- Protéger vos fichiers de configuration (config.h)
- Exclure les fichiers de compilation
- Ignorer les fichiers temporaires

### config.h.example
Modèle de configuration à copier et remplir avec vos propres identifiants.

---

## 🚀 Démarrage en 5 Étapes

### Étape 1 : Installer l'Environnement
- Installez **Arduino IDE** ou **PlatformIO**
- Ajoutez le support ESP8266
- Installez les bibliothèques : painlessMesh, ArduinoJson, PubSubClient

### Étape 2 : Configurer vos Identifiants
1. Copiez `config.h.example` vers `config.h` dans chaque dossier de projet
2. Éditez `config.h` avec vos identifiants :
   ```cpp
   #define MESH_PASSWORD   "VotreMotDePasseFort123!"
   #define STATION_SSID    "VotreReseauWiFi"
   #define STATION_PASSWORD "VotreMotDePasseWiFi"
   #define MQTT_USER       "votre_utilisateur"
   #define MQTT_PASSWORD   "VotreMotDePasseMQTT123!"
   ```
3. **NE PAS commiter config.h** (déjà dans .gitignore)

### Étape 3 : Préparer le Matériel
- **Carte ESP8266** (NodeMCU v3 ou Witty Cloud)
- **Capteur LDR** connecté sur la broche A0
- **LEDs** (optionnel, sur les broches 15, 12, 13 pour Witty Cloud)

### Étape 4 : Téléverser le Code
- Pour le **serveur** : Ouvrez `HEM_Server_v0.3/HEM_Server_v0.3.ino`
- Pour un **client** : Ouvrez `HEM_Client_v0.2/HEM_Client_v0.2.ino`
- Sélectionnez votre carte ESP8266 et le port COM
- Téléversez le code

### Étape 5 : Installer et Tester
1. Positionnez le capteur LDR sur la LED de votre compteur électrique (ex: Linky)
2. Alimentez votre ESP8266
3. Ouvrez le moniteur série (115200 bauds)
4. Vérifiez que les données s'affichent

---

## 📊 Comprendre les Données

### Valeurs Mesurées
- **ConsoInstVA** : Consommation instantanée en VA (Volt-Ampère)
- **WhPer15Minute** : Wh consommés sur 15 minutes
- **WhPerHour** : Wh consommés par heure
- **maxConsoInstVA** : Pic de consommation enregistré

### Sujets MQTT (pour HEM_Server_v0.3)
```
linky/ServerAlive      # État du serveur (0/1)
linky/LDR              # Valeur brute du capteur LDR
linky/ConsoInstVA      # Consommation instantanée
linky/maxConsoInstVA   # Consommation maximale
linky/WhPer15Minute    # Consommation sur 15 min
linky/WhPerHour        # Consommation horaire
```

---

## 🔍 Versions Disponibles

### Serveurs
- **v0.1** : Version de base avec réseau mesh simple
- **v0.2** : Améliorations du serveur
- **v0.3** : Version complète avec pont MQTT et WiFi

### Clients
- **v0.1** : Client de base
- **v0.2** : Client amélioré avec meilleure gestion

### Utilitaires
- **HEM_MACFinder** : Trouve l'adresse MAC de votre ESP8266
- **Test_LDR** : Teste et calibre votre capteur LDR
- **HEM_BlinkSimulator** : Simule les clignotements du compteur

---

## 💡 Conseils

### Sécurité
✅ Changez TOUS les mots de passe par défaut
✅ Utilisez des mots de passe forts (12+ caractères)
✅ Ne commitez JAMAIS config.h dans Git
✅ Gardez votre broker MQTT sur le réseau local
✅ Utilisez WPA2/WPA3 sur votre WiFi

### Performance
- Positionnez le capteur LDR bien en face de la LED du compteur
- Utilisez du ruban adhésif noir pour isoler de la lumière ambiante
- Vérifiez la qualité du signal mesh entre les nœuds
- Pour de meilleures connexions, utilisez le canal WiFi 11

### Dépannage
- **Pas de connexion mesh** : Vérifiez que tous les nœuds ont le même MESH_PREFIX et MESH_PASSWORD
- **Pas de données LDR** : Testez avec Test_LDR.ino pour calibrer
- **MQTT ne se connecte pas** : Vérifiez l'IP du broker et les identifiants
- **WiFi instable** : Essayez de changer le canal WiFi (paramètre dans mesh.init())

---

## 📚 Ressources Additionnelles

### Documentation Technique
- **README.md** : Documentation complète en anglais
- **SECURITY.md** : Audit de sécurité détaillé
- **config.h.example** : Modèle de configuration

### Liens Externes
- [painlessMesh Library](https://gitlab.com/painlessMesh/painlessMesh)
- [ArduinoJson](https://arduinojson.org/)
- [PubSubClient (MQTT)](https://pubsubclient.knolleary.net/)
- [ESP8266 Arduino Core](https://arduino-esp8266.readthedocs.io/)

---

## 🆘 Besoin d'Aide ?

1. Consultez **README.md** pour les instructions détaillées
2. Lisez **SECURITY.md** pour les problèmes de sécurité
3. Vérifiez que vous avez bien configuré **config.h**
4. Ouvrez une issue sur GitHub si le problème persiste

---

## ✅ Checklist Avant Déploiement

Avant de déployer en production, vérifiez :

- [ ] J'ai créé un fichier `config.h` dans chaque dossier de projet
- [ ] J'ai remplacé TOUS les mots de passe par défaut
- [ ] J'ai testé la connexion du capteur LDR
- [ ] Le fichier `config.h` est bien dans `.gitignore`
- [ ] Mon réseau WiFi utilise WPA2 ou WPA3
- [ ] Mon broker MQTT est sécurisé et sur le réseau local
- [ ] J'ai testé la connexion MQTT
- [ ] Les nœuds mesh se connectent correctement
- [ ] Je peux voir les données dans le moniteur série
- [ ] Les sujets MQTT reçoivent bien les données (si applicable)

---

**🏠 Bonne mesure de votre consommation électrique ! ⚡**

---

*Dernière mise à jour : 2026-02-12*  
*Projet personnel sous licence MIT*
