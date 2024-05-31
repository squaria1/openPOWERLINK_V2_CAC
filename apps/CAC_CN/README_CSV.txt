Configuration :
- Nom des cartes, des capteurs et des vannes non significatif.

- Valeur : pour les vannes, 1 état haut et 0 état bas.
	   pour l'état général, hexadécimal en 16 bits (exemple : 0xA001)

- Dépendance : pour les vannes, la vanne en question se s'enclenchera qu'à condition 
               que toutes les vannes dont elle dépend sont enclenchées.
	       (exemple VALVE1 dépend de VALVE2 et 3 : 2/3)

- Timer dépendance : pour les vannes, après le declenchement de la dernière dépendance un timer démarre,
		     à la fin du timer, la vanne ce déclenche.

- Etat initial : pour les vannes, l'état initial peut être 1 état haut, 0 état bas et -1 pour non concerné.


		     