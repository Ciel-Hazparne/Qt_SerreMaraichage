# Projet 2022 Serre de maraîchage connectée
**Jauste, développement Qt**
## Mission
Mon but dans ce projet est de réaliser sur un PC Window une IHM de secours qui va permettre aux maraîchers de modifier les valeurs des capteurs en cas de pannes:
- première étape: la carte Arduino envoie les valeurs d’Humidité et de Température des deux champs au broker par MQTT, qui les envoie sur le PC pour affichage
- deuxième étape: si les données n’arrivent plus (panne) l’utilisateur saisit des valeurs qui sont envoyées par requêtes RESTPOST au site web pour une historisation des données,
- troisième étape: L’utilisateur peut gérer à distance la vitesse de la ventilation en utilisant une connexion MODBUS TCP/IP.
